//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          MeshDataクラス                                    **//
//**                                   GetVBarray関数                                    **//
//*****************************************************************************************//

#include "Dx11Process.h"

MeshData::MeshData(){
	dx = Dx11Process::GetInstance();
	pHullShader = NULL;
	pDomainShader = NULL;
	pVertexLayout = NULL;
	pVertexShader = NULL;
	pPixelShader = NULL;
	pConstantBuffer = NULL;
	pConstantBuffer_MESH = NULL;
	pMyVB = NULL;
	pMaterial = NULL;
	VerCount = 0;
	FaceCount = 0;
}

MeshData::~MeshData(){

	if (pConstantBuffer_MESH != NULL){
		pConstantBuffer_MESH->Release();
		pConstantBuffer_MESH = NULL;
	}
	if (pMyVB != NULL){
		pMyVB->Release();
		pMyVB = NULL;
		for (DWORD i = 0; i < MaterialCount; i++){
			if (pMyVBI[i] == NULL)continue;
			pMyVBI[i]->Release();
			pMyVBI[i] = NULL;
		}
		delete pMyVBI;
		pMyVBI = NULL;
	}
	if (pMaterial != NULL){
		delete[] pMaterial;
		pMaterial = NULL;
	}
}

void MeshData::GetShaderPointer(bool disp){

	pConstantBuffer = dx->pConstantBuffer;

	if (disp == TRUE){
		pHullShader = dx->pHullShader_MESH_D;
		pDomainShader = dx->pDomainShader_MESH_D;
		pVertexLayout = dx->pVertexLayout_MESH_D;
		pVertexShader = dx->pVertexShader_MESH_D;
		pPixelShader = dx->pPixelShader_MESH_D;
	}
	else{
		pHullShader = NULL;
		pDomainShader = NULL;
		pVertexLayout = dx->pVertexLayout_MESH;
		pVertexShader = dx->pVertexShader_MESH;
		pPixelShader = dx->pPixelShader_MESH;
	}
}

HRESULT MeshData::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL** ppMaterial){

	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");
	char key[110] = { 0 };
	VECTOR4 v = { 0, 0, 0, 1 };

	//マテリアル数を調べる
	MaterialCount = 0;
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			MaterialCount++;
		}
	}
	MY_MATERIAL* pMaterial = new MY_MATERIAL[MaterialCount];

	//本読み込み	
	fseek(fp, SEEK_SET, 0);
	INT iMCount = -1;

	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(pMaterial[iMCount].MaterialName, key);
		}
		//Kd　ディフューズ
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].Kd = v;
		}
		//map_Kd　テクスチャー
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &pMaterial[iMCount].TextureName, sizeof(pMaterial[iMCount].TextureName));
			//ワイド文字に変換
			wchar_t wname[110];
			mbstowcs(wname, pMaterial[iMCount].TextureName, 110);
			//テクスチャーを作成
			if (FAILED(DirectX::CreateWICTextureFromFile(dx->pDevice, wname, NULL, &pMaterial[iMCount].pTexture)))
			{
				return E_FAIL;
			}
		}
	}
	fclose(fp);

	*ppMaterial = pMaterial;

	return S_OK;
}

HRESULT MeshData::GetVBarray(LPSTR FileName, bool disp){

	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD VCount = 0;//読み込みカウンター
	DWORD VNCount = 0;//読み込みカウンター
	DWORD VTCount = 0;//読み込みカウンター
	DWORD FCount = 0;//読み込みカウンター

	char key[200] = { 0 };
	//OBJファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//事前に頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		//マテリアル読み込み
		if (strcmp(key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			LoadMaterialFromFile(key, &pMaterial);
		}
		//頂点
		if (strcmp(key, "v") == 0)
		{
			VerCount++;
		}
		//法線
		if (strcmp(key, "vn") == 0)
		{
			VNCount++;
		}
		//テクスチャー座標
		if (strcmp(key, "vt") == 0)
		{
			VTCount++;
		}
		//フェイス（ポリゴン）
		if (strcmp(key, "f") == 0)
		{
			FaceCount++;
		}
	}

	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	MY_VERTEX_MESH* pvVertexBuffer = new MY_VERTEX_MESH[FaceCount * 3];
	VECTOR3* pvCoord = new VECTOR3[VerCount];
	VECTOR3* pvNormal = new VECTOR3[VNCount];
	VECTOR2* pvTexture = new VECTOR2[VTCount];
	
	//本読み込み	
	fseek(fp, SEEK_SET, 0);
	VCount = 0;
	VNCount = 0;
	VTCount = 0;
	FCount = 0;

	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s", key, sizeof(key));

		//頂点 読み込み
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[VCount].x = -x;
			pvCoord[VCount].y = z;
			pvCoord[VCount].z = y;
			VCount++;
		}

		//法線 読み込み
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[VNCount].x = x;
			pvNormal[VNCount].y = -z;
			pvNormal[VNCount].z = -y;
			VNCount++;
		}

		//テクスチャー座標 読み込み
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
			pvTexture[VTCount].x = x;
			pvTexture[VTCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
			VTCount++;
		}
	}

	//マテリアルの数だけインデックスバッファーを作成
	pMyVBI = new ID3D11Buffer*[MaterialCount];

	//フェイス　読み込み　バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
	bool boFlag = false;
	int* piFaceBuffer = new int[FaceCount * 3];//3頂点なので3インデックス
	FCount = 0;
	DWORD dwPartFCount = 0;
	for (DWORD i = 0; i < MaterialCount; i++)
	{
		dwPartFCount = 0;
		fseek(fp, SEEK_SET, 0);

		while (!feof(fp))
		{
			//キーワード 読み込み
			ZeroMemory(key, sizeof(key));
			fscanf_s(fp, "%s ", key, sizeof(key));

			//フェイス 読み込み→頂点インデックスに
			if (strcmp(key, "usemtl") == 0)
			{
				fscanf_s(fp, "%s ", key, sizeof(key));
				if (strcmp(key, pMaterial[i].MaterialName) == 0)
				{
					boFlag = true;
				}
				else
				{
					boFlag = false;
				}
			}
			if (strcmp(key, "f") == 0 && boFlag == true)
			{
				if (pMaterial[i].pTexture != NULL)//テクスチャーありサーフェイス
				{
					fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else//テクスチャー無しサーフェイス
				{
					fscanf_s(fp, "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
					vt1 = vt2 = vt3 = 1;//↓エラー防止
				}

				//インデックスバッファー
				piFaceBuffer[dwPartFCount * 3] = FCount * 3;
				piFaceBuffer[dwPartFCount * 3 + 1] = FCount * 3 + 1;
				piFaceBuffer[dwPartFCount * 3 + 2] = FCount * 3 + 2;
				//頂点構造体に代入
				pvVertexBuffer[FCount * 3].Pos = pvCoord[v1 - 1];
				pvVertexBuffer[FCount * 3].normal = pvNormal[vn1 - 1];
				pvVertexBuffer[FCount * 3].tex = pvTexture[vt1 - 1];

				pvVertexBuffer[FCount * 3 + 1].Pos = pvCoord[v2 - 1];
				pvVertexBuffer[FCount * 3 + 1].normal = pvNormal[vn2 - 1];
				pvVertexBuffer[FCount * 3 + 1].tex = pvTexture[vt2 - 1];

				pvVertexBuffer[FCount * 3 + 2].Pos = pvCoord[v3 - 1];
				pvVertexBuffer[FCount * 3 + 2].normal = pvNormal[vn3 - 1];
				pvVertexBuffer[FCount * 3 + 2].tex = pvTexture[vt3 - 1];

				dwPartFCount++;
				FCount++;
			}
		}

		if (dwPartFCount == 0)//使用されていないマテリアル対策
		{
			pMyVBI[i] = NULL;
			continue;
		}

		//インデックスバッファーを作成
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * dwPartFCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = piFaceBuffer;
		if (FAILED(dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVBI[i])))
			return FALSE;
		pMaterial[i].FaceCnt = dwPartFCount;
	}
	delete[] piFaceBuffer;
	fclose(fp);

	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_VERTEX_MESH) * FaceCount * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;

	if (FAILED(dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVB)))
		return FALSE;

	//一時的変数解放
	delete[] pvCoord;
	delete[] pvNormal;
	delete[] pvTexture;
	delete[] pvVertexBuffer;

	//コンスタントバッファー作成　マテリアル渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_MESH);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(dx->pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer_MESH)))
	{
		return E_FAIL;
	}

	GetShaderPointer(disp);

	return S_OK;
}

void MeshData::D3primitive(float x, float y, float z, float theta, float size, float disp){

	//使用するシェーダーのセット
	dx->pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	dx->pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	if (pHullShader != NULL)dx->pDeviceContext->HSSetShader(pHullShader, NULL, 0);
	if (pDomainShader != NULL)dx->pDeviceContext->DSSetShader(pDomainShader, NULL, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す
	dx->MatrixMap(pConstantBuffer, x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, size, disp);

	//このコンスタントバッファーをどのシェーダーで使うか
	dx->pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	dx->pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	if (pHullShader != NULL)dx->pDeviceContext->HSSetConstantBuffers(0, 1, &pConstantBuffer);
	if (pDomainShader != NULL)dx->pDeviceContext->DSSetConstantBuffers(0, 1, &pConstantBuffer);

	//バーテックスバッファーをセット
	UINT stride = sizeof(MY_VERTEX_MESH);
	UINT offset = 0;
	dx->pDeviceContext->IASetVertexBuffers(0, 1, &pMyVB, &stride, &offset);

	//使用する頂点インプットレイアウトをセット
	dx->pDeviceContext->IASetInputLayout(pVertexLayout);

	//プリミティブ・トポロジーをセット
	if (pDomainShader != NULL){ dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST); }
	else{
		dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (DWORD i = 0; i < MaterialCount; i++)
	{
		//使用されていないマテリアル対策
		if (pMaterial[i].FaceCnt == 0)
		{
			continue;
		}
		//アルファブレンド,アルファテスト切り替え
		bool a = FALSE;
		if (pMaterial[i].TextureName[0] != NULL)a = TRUE;
		dx->ChangeBlendState(a, a);

		//インデックスバッファーをセット
		dx->pDeviceContext->IASetIndexBuffer(pMyVBI[i], DXGI_FORMAT_R32_UINT, 0);
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(dx->pDeviceContext->Map(pConstantBuffer_MESH, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			CONSTANT_BUFFER_MESH sg;
			sg.vDiffuse = pMaterial[i].Kd;//ディフューズカラーをシェーダーに渡す
			//テクスチャ有無フラグ
			if (pMaterial[i].TextureName[0] != NULL){
				sg.tex_f = { 1.0f, 0.0f, 0.0f, 0.0f };
			}
			else{
				sg.tex_f = { 0.0f, 0.0f, 0.0f, 0.0f };
			}
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CONSTANT_BUFFER_MESH));
			dx->pDeviceContext->Unmap(pConstantBuffer_MESH, 0);
		}
		dx->pDeviceContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_MESH);
		dx->pDeviceContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_MESH);
		if (pHullShader != NULL)dx->pDeviceContext->HSSetConstantBuffers(1, 1, &pConstantBuffer_MESH);
		if (pDomainShader != NULL)dx->pDeviceContext->DSSetConstantBuffers(1, 1, &pConstantBuffer_MESH);
		//テクスチャーをシェーダーに渡す
		if (pMaterial[i].TextureName[0] != NULL)
		{
			dx->pDeviceContext->PSSetSamplers(0, 1, &dx->pSampleLinear);
			dx->pDeviceContext->PSSetShaderResources(0, 1, &pMaterial[i].pTexture);
			if (pDomainShader != NULL){
				dx->pDeviceContext->DSSetSamplers(0, 1, &dx->pSampleLinear);
				dx->pDeviceContext->DSSetShaderResources(0, 1, &pMaterial[i].pTexture);
			}
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			dx->pDeviceContext->PSSetShaderResources(0, 1, Nothing);
			if (pDomainShader != NULL)dx->pDeviceContext->DSSetShaderResources(0, 1, Nothing);
		}
		//プリミティブをレンダリング
		dx->pDeviceContext->DrawIndexed(pMaterial[i].FaceCnt * 3, 0, 0);
	}
	//ハル,ドメインシェーダー無効
	dx->pDeviceContext->HSSetShader(NULL, NULL, 0);
	dx->pDeviceContext->DSSetShader(NULL, NULL, 0);
}