//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	      DxTextクラス (unicode対応,マルチバイトだと×)      **//
//**                                  DrawText関数                                       **//
//*****************************************************************************************//

#include "DxText.h"
#include <string.h>
#include <tchar.h>

DxText *DxText::textobj = NULL;

void DxText::InstanceCreate(){

	if (textobj == NULL)textobj = new DxText();
}

DxText *DxText::GetInstance(){

	if (textobj != NULL)return textobj;
	return NULL;
}

void DxText::DeleteInstance(){

	if (textobj != NULL){
		delete textobj;
		textobj = NULL;
	}
}

DxText::DxText(const DxText &obj) {}      // コピーコンストラクタ禁止
void DxText::operator=(const DxText& obj) {} // 代入演算子禁止

DxText::DxText(){

	dx = Dx11Process::GetInstance();

	//文字列用バッファ初期化
	for (int i = 0; i < STRTEX_MAX_PCS; i++){
		text[i].GetVBarray2D(1);
		_tcscpy_s(str[i], STR_MAX_LENGTH * sizeof(TCHAR), _T("***************************************"));//直接代入
		f_size[i] = 0;
	}

	//可変用
	for (int i = 0; i < VAL_PCS; i++){
		value[i].GetVBarray2D(1);
		CreateText(value, CreateTextValue(i), i, 15.0f);
	}

	CreateTextNo = 0;
}

DxText::~DxText(){

}

int DxText::CreateText(PolygonData2D *p2, TCHAR *c, int texNo, float fontsize){

	MAT2 Mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
	int fsize = 100;
	LOGFONT lf = { fsize, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, (L"ＭＳ ゴシック") };
	hFont = CreateFontIndirect(&lf);

	//デバイスコンテキスト取得。GetGlyphOutline関数がエラーになる為
	hdc = GetDC(NULL);
	oldFont = (HFONT)SelectObject(hdc, hFont);
	int count = 0;//文字数カウント
	UINT code = 0;//文字コード
	do{
#if _UNICODE
		// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
		code = (UINT)c[count++];
#else
		// マルチバイト文字の場合、
		// 1バイト文字のコードは1バイト目のUINT変換、
		// 2バイト文字のコードは[先導コード]*256 + [文字コード]
		if (IsDBCSLeadByte(*c))//指定された文字が先行バイトかどうかを調べる
			code = (BYTE)c[0] << 8 | (BYTE)c[1];
		else
			code = c[0];
#endif
	} while (code != 0);
	count--;
	if (count == 0)return 0;

	ptr = NULL;
	TM = new TEXTMETRIC[count];
	GM = new GLYPHMETRICS[count];
	allsize = new DWORD[count];//各要素までの合計サイズ

	for (int cnt = 0; cnt < count; cnt++){
		code = 0;
#if _UNICODE
		// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
		code = (UINT)c[cnt];
#else
		// マルチバイト文字の場合、
		// 1バイト文字のコードは1バイト目のUINT変換、
		// 2バイト文字のコードは[先導コード]*256 + [文字コード]
		if (IsDBCSLeadByte(*c))//指定された文字が先行バイトかどうかを調べる
			code = (BYTE)c[0] << 8 | (BYTE)c[1];
		else
			code = c[0];
#endif
		// フォントビットマップ取得
		GetTextMetrics(hdc, &TM[cnt]);
		DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM[cnt], 0, NULL, &Mat);

		//各要素までの合計サイズ格納
		if (cnt == 0)allsize[0] = size; else{
			allsize[cnt] = allsize[cnt - 1] + size;
		}
	}
	ptr = new BYTE[allsize[count - 1]];

	for (int cnt = 0; cnt < count; cnt++){
		code = 0;
#if _UNICODE
		// unicodeの場合、文字コードは単純にワイド文字のUINT変換です
		code = (UINT)c[cnt];
#else
		// マルチバイト文字の場合、
		// 1バイト文字のコードは1バイト目のUINT変換、
		// 2バイト文字のコードは[先導コード]*256 + [文字コード]
		if (IsDBCSLeadByte(*c))//指定された文字が先行バイトかどうかを調べる
			code = (BYTE)c[0] << 8 | (BYTE)c[1];
		else
			code = c[0];
#endif
		DWORD size;
		BYTE *p;
		if (cnt == 0){
			size = allsize[0];
			p = &ptr[0];
		}
		else{
			size = allsize[cnt] - allsize[cnt - 1];
			p = &ptr[allsize[cnt - 1]];
		}
		GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM[cnt], size, p, &Mat);
	}

	UINT w = 0;
	for (int cnt = 0; cnt < count; cnt++){
		w += GM[cnt].gmCellIncX;
	}
	ID3D11Texture2D *pTex; //テクスチャ描画(文字描画する毎に書き換えられるのでその都度解放)
	pTex = NULL;
	// テクスチャ情報作成
	D3D11_TEXTURE2D_DESC desc;               //テクスチャ情報
	memset(&desc, 0, sizeof(desc));
	desc.Width = (int)(w * 1.2);  //余白含みの全文字横幅, 補正値1.2
	desc.Height = TM[0].tmHeight;//余白含みの縦幅
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dx->pDevice->CreateTexture2D(&desc, NULL, &pTex);//テクスチャ生成

	//書き換え前解放
	RELEASE(p2[texNo].pTexview);
	//シェーダリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srdesc;//シェーダリソースビュー情報
	srdesc.Format = desc.Format;
	srdesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srdesc.Texture2D.MostDetailedMip = 0;
	srdesc.Texture2D.MipLevels = 1;
	dx->pDevice->CreateShaderResourceView(pTex, &srdesc, &p2[texNo].pTexview);

	//テクスチャ書き込み
	D3D11_MAPPED_SUBRESOURCE pData;

	dx->pDeviceContext->Map(pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	BYTE* pBits = (BYTE*)pData.pData;
	memset(pBits, 0, pData.RowPitch * TM[0].tmHeight);//0埋め
	for (int cnt = 0; cnt < count; cnt++){
		UINT offset1 = pData.RowPitch / count * cnt;

		// フォント情報の書き込み
		// iOfs_x, iOfs_y : 書き出し位置(左上)
		// iBmp_w, iBmp_h : フォントビットマップの幅高
		// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
		int iOfs_x = GM[cnt].gmptGlyphOrigin.x;
		int iOfs_y = TM[cnt].tmAscent - GM[cnt].gmptGlyphOrigin.y;
		int iBmp_w = GM[cnt].gmBlackBoxX + (4 - (GM[cnt].gmBlackBoxX % 4)) % 4;
		int iBmp_h = GM[cnt].gmBlackBoxY;
		int Level = 17;
		int x, y;
		DWORD Alpha, Color;

		for (y = iOfs_y; y < iOfs_y + iBmp_h; y++){
			for (x = iOfs_x; x < iOfs_x + iBmp_w; x++) {
				int offset2;
				if (cnt == 0)offset2 = 0; else {
					offset2 = allsize[cnt - 1];
				}
				Alpha = (255 * ptr[(x - iOfs_x + iBmp_w * (y - iOfs_y)) + offset2]) / (Level - 1);
				Color = 0x00ffffff | (Alpha << 24);
				memcpy((BYTE*)pBits + pData.RowPitch * y + 4 * x + offset1, &Color, sizeof(DWORD));
			}
		}
	}
	dx->pDeviceContext->Unmap(pTex, 0);
	delete TM;
	TM = NULL;
	delete GM;
	GM = NULL;
	delete allsize;
	allsize = NULL;
	delete[] ptr;
	ptr = NULL;

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	//テクスチャ,ビットマップ配列解放
	RELEASE(pTex);

	return count;
}

TCHAR *DxText::CreateTextValue(int val){

	int cnt = 0;
	int va = val;
	static TCHAR c[STR_MAX_LENGTH];
	_tcscpy_s(c, L"");//初期化

	//桁数カウント
	for (int i = 1;; i *= 10){
		if (val / i == 0)break;
		cnt++;
	}
	if (cnt == 0)_tcscpy_s(c, L"０");

	for (int i = cnt - 1; i >= 0; i--){

		int s = va / (int)pow(10.0, i);
		switch (s){
		case 0:
			_tcscat(c, L"０");
			break;
		case 1:
			_tcscat(c, L"１");
			break;
		case 2:
			_tcscat(c, L"２");
			break;
		case 3:
			_tcscat(c, L"３");
			break;
		case 4:
			_tcscat(c, L"４");
			break;
		case 5:
			_tcscat(c, L"５");
			break;
		case 6:
			_tcscat(c, L"６");
			break;
		case 7:
			_tcscat(c, L"７");
			break;
		case 8:
			_tcscat(c, L"８");
			break;
		case 9:
			_tcscat(c, L"９");
			break;
		}
		va = va - (int)pow(10.0, i) * s;
	}
	return c;
}

void DxText::Drawtext(TCHAR *c, float x, float y, float fontsize, VECTOR4 cl){//テキスト描画

	bool match = FALSE;
	int texNo = -1;

	//登録済みテキスト検索
	for (int i = 0; i < STRTEX_MAX_PCS; i++){
		if (_tcscmp(c, str[i]) == 0 && f_size[i] == fontsize){
			match = TRUE;
			texNo = i;
		}
	}

	if (texNo == -1){
		if (CreateTextNo == STRTEX_MAX_PCS)CreateTextNo = 0;
		texNo = CreateTextNo++;
	}

	if (match == FALSE){
		strcnt[texNo] = CreateText(text, c, texNo, fontsize);
		_tcscpy_s(str[texNo], c);//テキスト登録
		f_size[texNo] = fontsize;//フォントサイズ登録
	}
	text[texNo].d3varray[0].x = x;
	text[texNo].d3varray[0].y = y;
	text[texNo].d3varray[0].z = 0.0f;
	text[texNo].d3varray[0].color = cl;
	text[texNo].d3varray[0].tex.as(0.0f, 0.0f);

	text[texNo].d3varray[1].x = x + f_size[texNo] * strcnt[texNo];
	text[texNo].d3varray[1].y = y;
	text[texNo].d3varray[1].z = 0.0f;
	text[texNo].d3varray[1].color = cl;
	text[texNo].d3varray[1].tex.as(1.0f, 0.0f);

	text[texNo].d3varray[2].x = x;
	text[texNo].d3varray[2].y = y + f_size[texNo];
	text[texNo].d3varray[2].z = 0.0f;
	text[texNo].d3varray[2].color = cl;
	text[texNo].d3varray[2].tex.as(0.0f, 1.0f);

	text[texNo].d3varray[3].x = x + f_size[texNo] * strcnt[texNo];
	text[texNo].d3varray[3].y = y + f_size[texNo];
	text[texNo].d3varray[3].z = 0.0f;
	text[texNo].d3varray[3].color = cl;
	text[texNo].d3varray[3].tex.as(1.0f, 1.0f);

	text[texNo].Draw(TRUE, TRUE);
}

void DxText::DrawValue(int val, float x, float y, float fontsize, int pcs, VECTOR4 cl){

	//桁数カウント
	int cnt = 0;
	for (int i = 1;; i *= 10){
		if (val / i == 0)break;
		cnt++;
	}
	if (val == 0)cnt = 1;

	float xx = x + (pcs - cnt) * fontsize;//描画開始位置(x方向)

	for (int i = cnt - 1; i >= 0; i--){
		int s = val / (int)pow(10.0, i);
		value[s].d3varray[0].x = xx;
		value[s].d3varray[0].y = y;
		value[s].d3varray[0].z = 0.0f;
		value[s].d3varray[0].color = cl;
		value[s].d3varray[0].tex.as(0.0f, 0.0f);

		value[s].d3varray[1].x = xx + fontsize;
		value[s].d3varray[1].y = y;
		value[s].d3varray[1].z = 0.0f;
		value[s].d3varray[1].color = cl;
		value[s].d3varray[1].tex.as(1.0f, 0.0f);

		value[s].d3varray[2].x = xx;
		value[s].d3varray[2].y = y + fontsize;
		value[s].d3varray[2].z = 0.0f;
		value[s].d3varray[2].color = cl;
		value[s].d3varray[2].tex.as(0.0f, 1.0f);

		value[s].d3varray[3].x = xx + fontsize;
		value[s].d3varray[3].y = y + fontsize;
		value[s].d3varray[3].z = 0.0f;
		value[s].d3varray[3].color = cl;
		value[s].d3varray[3].tex.as(1.0f, 1.0f);

		value[s].Draw(TRUE, TRUE);
		val = val - (int)pow(10.0, i) * s;
		xx += fontsize;
	}
}