//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          PolygonDataクラス                                 **//
//**                                    GetVBarray関数                                   **//
//*****************************************************************************************//

#include "Dx11Process.h"

PolygonData::PolygonData(){
	dx = Dx11Process::GetInstance();
	pHullShader = NULL;
	pDomainShader = NULL;
	pVertexLayout = NULL;
	pVertexShader = NULL;
	pPixelShader = NULL;
	pConstantBuffer = NULL;

	tex_no = 0;
	load_tex_no[0] = -1;
	pTexview = NULL;
	pTex = NULL;
	pMyVB = NULL;
	lockV = FALSE;  //ロック,頂点設定前
	lockI = FALSE;//ロック,インデックス設定前
	CPUAccess = TRUE;
	lighteffect = FALSE;
}

PolygonData::~PolygonData(){

	if (pMyVB != NULL){
		pMyVB->Release();
		pMyVB = NULL;
		pMyVBI->Release();
		pMyVBI = NULL;
		free(d3varray);
		d3varray = NULL;
		free(d3varrayI);
		d3varrayI = NULL;
	}
	if (pTexview != NULL){
		pTexview->Release();
		pTexview = NULL;
	}
	if (pTex != NULL){
		pTex->Release();
		pTex = NULL;
	}
}

void PolygonData::SetVertex(int I1, int I2, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float r, float g, float b, float a,
	float u, float v){
	d3varrayI[I1] = i;
	d3varrayI[I2] = i;
	d3varray[i].Pos.as(vx, vy, vz);
	d3varray[i].normal.as(nx, ny, nz);
	d3varray[i].color.as((FLOAT)r, (FLOAT)g, (FLOAT)b, (FLOAT)a);
	d3varray[i].tex.as(u, v);
}

void PolygonData::SetVertex(int I1, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float r, float g, float b, float a,
	float u, float v){
	d3varrayI[I1] = i;
	d3varray[i].Pos.as(vx, vy, vz);
	d3varray[i].normal.as(nx, ny, nz);
	d3varray[i].color.as((FLOAT)r, (FLOAT)g, (FLOAT)b, (FLOAT)a);
	d3varray[i].tex.as(u, v);
}

void PolygonData::Light(bool f){
	lighteffect = f;
}

HRESULT PolygonData::GetVBarray(PrimitiveType type, int pieces){//頂点バッファ,配列確保

	if (type == SQUARE){
		topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ver = pieces * 4;//pieces==四角形の個数
		verI = pieces * 6;
	}
	if (type == POINt){
		topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		ver = pieces;//pieces==点の個数
		verI = pieces;
	}
	if (type == LINE_L){
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		ver = pieces * 2;//pieces==線の個数
		verI = pieces * 2;
	}
	if (type == LINE_S){
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		ver = pieces * 2;//pieces==線の個数
		verI = pieces * 2;
	}

	//頂点バッファ取得
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(MY_VERTEX) * ver;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	//頂点配列取得
	d3varray = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * ver);
	//頂点バッファ設定
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = d3varray;
	dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVB);

	//インデックスバッファ取得
	bdI.Usage = D3D11_USAGE_DYNAMIC;
	bdI.ByteWidth = sizeof(UINT) * verI;
	bdI.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdI.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bdI.MiscFlags = 0;
	bdI.StructureByteStride = 0;
	//インデックス配列取得
	d3varrayI = (UINT*)malloc(sizeof(UINT) * verI);
	//インデックスバッファ設定
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = d3varrayI;
	dx->pDevice->CreateBuffer(&bdI, &IndexData, &pMyVBI);

	CPUAccess = TRUE;
	return S_OK;
}

void PolygonData::GetVBarrayCPUNotAccess(int pieces){

	ver = verI = pieces * 4;//pieces==四角形の個数

	d3varray = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * ver);
	d3varrayI = (UINT*)malloc(sizeof(UINT) * ver);
	CPUAccess = FALSE;
}

void PolygonData::TextureInit(int width, int height){

	//動画テクスチャ使用時カラテクスチャ生成
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dx->pDevice->CreateTexture2D(&desc, NULL, &pTex);

	//シェーダリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srdesc;
	srdesc.Format = desc.Format;
	srdesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srdesc.Texture2D.MostDetailedMip = 0;
	srdesc.Texture2D.MipLevels = 1;
	dx->pDevice->CreateShaderResourceView(pTex, &srdesc, &pTexview);
}

void PolygonData::GetTexture(int no){
	load_tex_no[tex_no] = no;
}

void PolygonData::SetTextureMPixel(int **m_pix, BYTE r, BYTE g, BYTE b, int a){

	D3D11_TEXTURE2D_DESC desc2D;
	pTex->GetDesc(&desc2D);

	//テクスチャの横サイズ取得
	int width = desc2D.Width;
	//テクスチャの縦サイズ取得
	int height = desc2D.Height;

	//リソースのマップ
	D3D11_MAPPED_SUBRESOURCE pData;
	dx->pDeviceContext->Map(pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	UCHAR *ptex = (UCHAR*)pData.pData;

	for (int j = 0; j < height; j++){
		UINT j1 = j * pData.RowPitch;//RowPitchデータの行ピッチ、行幅、または物理サイズ (バイト単位)
		for (int i = 0; i < width; i++){
			UINT ptexI = i * 4 + j1;
			ptex[ptexI + 0] = m_pix[j][i] >> 16 & r;
			ptex[ptexI + 1] = m_pix[j][i] >> 8 & g;
			ptex[ptexI + 2] = m_pix[j][i] & b;

			if ((m_pix[j][i] >> 16 & b) < 50 && (m_pix[j][i] >> 8 & g) < 50 && (m_pix[j][i] & r) < 50){
				ptex[ptexI + 3] = 0;
			}
			else{
				ptex[ptexI + 3] = a;
			}
		}
	}
	dx->pDeviceContext->Unmap(pTex, 0);
}

void PolygonData::GetShaderPointer(){

	pConstantBuffer = dx->pConstantBuffer;

	if ((load_tex_no[0] != -1 || pTexview != NULL) && lighteffect == TRUE){
		if (CPUAccess == TRUE){
			pVertexShader = dx->pVertexShader_TCL;
			pPixelShader = dx->pPixelShader_TCL;
			pHullShader = NULL;
			pDomainShader = NULL;
			pVertexLayout = dx->pVertexLayout_TCL;
		}
		else{
			pVertexShader = dx->pVertexShader_DISPL;
			pPixelShader = dx->pPixelShader_DISPL;
			pHullShader = dx->pHullShader_DISPL;
			pDomainShader = dx->pDomainShader_DISPL;
			pVertexLayout = dx->pVertexLayout_DISPL;
		}
	}
	if ((load_tex_no[0] != -1 || pTexview != NULL) && lighteffect == FALSE){
		if (CPUAccess == TRUE){
			pVertexShader = dx->pVertexShader_TC;
			pPixelShader = dx->pPixelShader_TC;
			pHullShader = NULL;
			pDomainShader = NULL;
			pVertexLayout = dx->pVertexLayout_TC;
		}
		else{
			pVertexShader = dx->pVertexShader_DISP;
			pPixelShader = dx->pPixelShader_DISP;
			pHullShader = dx->pHullShader_DISP;
			pDomainShader = dx->pDomainShader_DISP;
			pVertexLayout = dx->pVertexLayout_DISP;
		}
	}
	if (load_tex_no[0] == -1 && pTexview == NULL){
		pVertexShader = dx->pVertexShader_BC;
		pPixelShader = dx->pPixelShader_BC;
		pHullShader = NULL;
		pDomainShader = NULL;
		pVertexLayout = dx->pVertexLayout_BC;
	}
}

void PolygonData::D3primitive(float x, float y, float z, float r, float g, float b, float theta, bool a, bool lock, float disp){

	GetShaderPointer();

	//アルファブレンド,アルファテスト切り替え
	bool at = a;
	if (d3varray[0].color.w != 1.0f)at = FALSE;//アルファ値1.0以外の場合は半透明にする
	dx->ChangeBlendState(at, a);

	//使用するシェーダーのセット
	dx->pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	dx->pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	if (pHullShader != NULL)dx->pDeviceContext->HSSetShader(pHullShader, NULL, 0);
	if (pDomainShader != NULL)dx->pDeviceContext->DSSetShader(pDomainShader, NULL, 0);

	//コントロールポイントでバーテックスバッファー作成
	if (CPUAccess == FALSE && lockV == FALSE){
		//頂点バッファ取得
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) * ver;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		//頂点バッファ設定
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = d3varray;
		dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVB);

		//インデックスバッファ取得
		bdI.Usage = D3D11_USAGE_DEFAULT;
		bdI.ByteWidth = sizeof(UINT) * verI;
		bdI.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bdI.CPUAccessFlags = 0;
		bdI.MiscFlags = 0;
		bdI.StructureByteStride = 0;

		//インデックスバッファ設定
		D3D11_SUBRESOURCE_DATA IndexData;
		IndexData.pSysMem = d3varrayI;
		dx->pDevice->CreateBuffer(&bdI, &IndexData, &pMyVBI);
		lockV = TRUE;
	}

	D3D11_MAPPED_SUBRESOURCE pData, pData1;
	//インデックス配列書き換えの場合処理(基本的に最初の一回のみ)
	if (CPUAccess == TRUE && lockI == FALSE){
		dx->pDeviceContext->Map(pMyVBI, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		memcpy_s(pData.pData, pData.RowPitch, (void*)d3varrayI, sizeof(UINT) * verI);
		dx->pDeviceContext->Unmap(pMyVBI, 0);
		lockI = TRUE;
	}

	//頂点配列書き換えの場合処理(最初の1回は必ず実行される,その後は任意)
	if (CPUAccess == TRUE && (lockV == FALSE || lock == TRUE)){
		dx->pDeviceContext->Map(pMyVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData1);
		memcpy_s(pData1.pData, pData1.RowPitch, (void*)d3varray, sizeof(MY_VERTEX) * ver);
		dx->pDeviceContext->Unmap(pMyVB, 0);
		lockV = TRUE;
	}

	//シェーダーのコンスタントバッファーに各種データを渡す
	dx->MatrixMap(pConstantBuffer, x, y, z, r, g, b, theta, 1.0f, disp);

	//このコンスタントバッファーをどのシェーダーで使うか
	if (CPUAccess == TRUE){
		dx->pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		dx->pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	}
	else{
		dx->pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		dx->pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		dx->pDeviceContext->HSSetConstantBuffers(0, 1, &pConstantBuffer);
		dx->pDeviceContext->DSSetConstantBuffers(0, 1, &pConstantBuffer);
	}

	//テクスチャーをシェーダーに渡す
	if (load_tex_no[0] != -1){
		dx->pDeviceContext->PSSetSamplers(0, 1, &dx->pSampleLinear);
		dx->pDeviceContext->PSSetShaderResources(0, 1, &dx->pTexture[load_tex_no[tex_no]]);
		if (CPUAccess == FALSE){
			dx->pDeviceContext->DSSetSamplers(0, 1, &dx->pSampleLinear);
			dx->pDeviceContext->DSSetShaderResources(0, 1, &dx->pTexture[load_tex_no[tex_no]]);
		}
	}
	if (pTexview != NULL){
		dx->pDeviceContext->PSSetSamplers(0, 1, &dx->pSampleLinear);
		dx->pDeviceContext->PSSetShaderResources(0, 1, &pTexview);
		if (CPUAccess == FALSE){
			dx->pDeviceContext->DSSetSamplers(0, 1, &dx->pSampleLinear);
			dx->pDeviceContext->DSSetShaderResources(0, 1, &pTexview);
		}
	}

	//バーテックスバッファーをセット
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	dx->pDeviceContext->IASetVertexBuffers(0, 1, &pMyVB, &stride, &offset);
	//インデックスバッファーをセット
	dx->pDeviceContext->IASetIndexBuffer(pMyVBI, DXGI_FORMAT_R32_UINT, 0);

	//使用する頂点インプットレイアウトをセット
	dx->pDeviceContext->IASetInputLayout(pVertexLayout);

	//プリミティブ・トポロジーをセット
	if (CPUAccess == TRUE){
		dx->pDeviceContext->IASetPrimitiveTopology(topology);
	}
	else{
		dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	}
	//プリミティブをレンダリング
	dx->pDeviceContext->DrawIndexed(verI, 0, 0);

	//ハル,ドメインシェーダー無効
	dx->pDeviceContext->HSSetShader(NULL, NULL, 0);
	dx->pDeviceContext->DSSetShader(NULL, NULL, 0);
}
