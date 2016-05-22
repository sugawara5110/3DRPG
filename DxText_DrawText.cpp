//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	      DxText�N���X (unicode�Ή�,�}���`�o�C�g���Ɓ~)      **//
//**                                  DrawText�֐�                                       **//
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

DxText::DxText(const DxText &obj) {}      // �R�s�[�R���X�g���N�^�֎~
void DxText::operator=(const DxText& obj) {} // ������Z�q�֎~

DxText::DxText(){

	dx = Dx11Process::GetInstance();

	//������p�o�b�t�@������
	for (int i = 0; i < STRTEX_MAX_PCS; i++){
		text[i].GetVBarray2D(1);
		_tcscpy_s(str[i], STR_MAX_LENGTH * sizeof(TCHAR), _T("***************************************"));//���ڑ��
		f_size[i] = 0;
	}

	//�ϗp
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
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, (L"�l�r �S�V�b�N") };
	hFont = CreateFontIndirect(&lf);

	//�f�o�C�X�R���e�L�X�g�擾�BGetGlyphOutline�֐����G���[�ɂȂ��
	hdc = GetDC(NULL);
	oldFont = (HFONT)SelectObject(hdc, hFont);
	int count = 0;//�������J�E���g
	UINT code = 0;//�����R�[�h
	do{
#if _UNICODE
		// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ��ł�
		code = (UINT)c[count++];
#else
		// �}���`�o�C�g�����̏ꍇ�A
		// 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
		// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]
		if (IsDBCSLeadByte(*c))//�w�肳�ꂽ��������s�o�C�g���ǂ����𒲂ׂ�
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
	allsize = new DWORD[count];//�e�v�f�܂ł̍��v�T�C�Y

	for (int cnt = 0; cnt < count; cnt++){
		code = 0;
#if _UNICODE
		// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ��ł�
		code = (UINT)c[cnt];
#else
		// �}���`�o�C�g�����̏ꍇ�A
		// 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
		// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]
		if (IsDBCSLeadByte(*c))//�w�肳�ꂽ��������s�o�C�g���ǂ����𒲂ׂ�
			code = (BYTE)c[0] << 8 | (BYTE)c[1];
		else
			code = c[0];
#endif
		// �t�H���g�r�b�g�}�b�v�擾
		GetTextMetrics(hdc, &TM[cnt]);
		DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM[cnt], 0, NULL, &Mat);

		//�e�v�f�܂ł̍��v�T�C�Y�i�[
		if (cnt == 0)allsize[0] = size; else{
			allsize[cnt] = allsize[cnt - 1] + size;
		}
	}
	ptr = new BYTE[allsize[count - 1]];

	for (int cnt = 0; cnt < count; cnt++){
		code = 0;
#if _UNICODE
		// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ��ł�
		code = (UINT)c[cnt];
#else
		// �}���`�o�C�g�����̏ꍇ�A
		// 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
		// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]
		if (IsDBCSLeadByte(*c))//�w�肳�ꂽ��������s�o�C�g���ǂ����𒲂ׂ�
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
	ID3D11Texture2D *pTex; //�e�N�X�`���`��(�����`�悷�閈�ɏ�����������̂ł��̓s�x���)
	pTex = NULL;
	// �e�N�X�`�����쐬
	D3D11_TEXTURE2D_DESC desc;               //�e�N�X�`�����
	memset(&desc, 0, sizeof(desc));
	desc.Width = (int)(w * 1.2);  //�]���܂݂̑S��������, �␳�l1.2
	desc.Height = TM[0].tmHeight;//�]���܂݂̏c��
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dx->pDevice->CreateTexture2D(&desc, NULL, &pTex);//�e�N�X�`������

	//���������O���
	RELEASE(p2[texNo].pTexview);
	//�V�F�[�_���\�[�X�r���[�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srdesc;//�V�F�[�_���\�[�X�r���[���
	srdesc.Format = desc.Format;
	srdesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srdesc.Texture2D.MostDetailedMip = 0;
	srdesc.Texture2D.MipLevels = 1;
	dx->pDevice->CreateShaderResourceView(pTex, &srdesc, &p2[texNo].pTexview);

	//�e�N�X�`����������
	D3D11_MAPPED_SUBRESOURCE pData;

	dx->pDeviceContext->Map(pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	BYTE* pBits = (BYTE*)pData.pData;
	memset(pBits, 0, pData.RowPitch * TM[0].tmHeight);//0����
	for (int cnt = 0; cnt < count; cnt++){
		UINT offset1 = pData.RowPitch / count * cnt;

		// �t�H���g���̏�������
		// iOfs_x, iOfs_y : �����o���ʒu(����)
		// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
		// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
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

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	//�e�N�X�`��,�r�b�g�}�b�v�z����
	RELEASE(pTex);

	return count;
}

TCHAR *DxText::CreateTextValue(int val){

	int cnt = 0;
	int va = val;
	static TCHAR c[STR_MAX_LENGTH];
	_tcscpy_s(c, L"");//������

	//�����J�E���g
	for (int i = 1;; i *= 10){
		if (val / i == 0)break;
		cnt++;
	}
	if (cnt == 0)_tcscpy_s(c, L"�O");

	for (int i = cnt - 1; i >= 0; i--){

		int s = va / (int)pow(10.0, i);
		switch (s){
		case 0:
			_tcscat(c, L"�O");
			break;
		case 1:
			_tcscat(c, L"�P");
			break;
		case 2:
			_tcscat(c, L"�Q");
			break;
		case 3:
			_tcscat(c, L"�R");
			break;
		case 4:
			_tcscat(c, L"�S");
			break;
		case 5:
			_tcscat(c, L"�T");
			break;
		case 6:
			_tcscat(c, L"�U");
			break;
		case 7:
			_tcscat(c, L"�V");
			break;
		case 8:
			_tcscat(c, L"�W");
			break;
		case 9:
			_tcscat(c, L"�X");
			break;
		}
		va = va - (int)pow(10.0, i) * s;
	}
	return c;
}

void DxText::Drawtext(TCHAR *c, float x, float y, float fontsize, VECTOR4 cl){//�e�L�X�g�`��

	bool match = FALSE;
	int texNo = -1;

	//�o�^�ς݃e�L�X�g����
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
		_tcscpy_s(str[texNo], c);//�e�L�X�g�o�^
		f_size[texNo] = fontsize;//�t�H���g�T�C�Y�o�^
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

	//�����J�E���g
	int cnt = 0;
	for (int i = 1;; i *= 10){
		if (val / i == 0)break;
		cnt++;
	}
	if (val == 0)cnt = 1;

	float xx = x + (pcs - cnt) * fontsize;//�`��J�n�ʒu(x����)

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