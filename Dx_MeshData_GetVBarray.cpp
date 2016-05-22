//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          MeshDataクラス                                    **//
//**                                   GetVBarray関数                                    **//
//*****************************************************************************************//

#include "Dx11Process.h"

HANDLE *MeshData::MeshObj_H = NULL;
MeshData *MeshData::MeshObj = NULL;
char **MeshData::MeshPass = NULL;

void MeshData::GetVBarrayThreadArray(MeshData *meshObj, char **Mpass, int pcs){
	//objファイルパス配列受け取り
	MeshPass = Mpass;
	//生成済objポインタ受け取り
	MeshObj = meshObj;
	//ハンドル生成
	MeshObj_H = (HANDLE*)malloc(sizeof(HANDLE) * pcs);

	//スレッド生成
	if (pcs > 0)MeshObj_H[0] = (HANDLE)_beginthreadex(NULL, 0, GetVB0, NULL, 0, NULL);
	if (pcs > 1)MeshObj_H[1] = (HANDLE)_beginthreadex(NULL, 0, GetVB1, NULL, 0, NULL);
	if (pcs > 2)MeshObj_H[2] = (HANDLE)_beginthreadex(NULL, 0, GetVB2, NULL, 0, NULL);
	if (pcs > 3)MeshObj_H[3] = (HANDLE)_beginthreadex(NULL, 0, GetVB3, NULL, 0, NULL);
	if (pcs > 4)MeshObj_H[4] = (HANDLE)_beginthreadex(NULL, 0, GetVB4, NULL, 0, NULL);
	if (pcs > 5)MeshObj_H[5] = (HANDLE)_beginthreadex(NULL, 0, GetVB5, NULL, 0, NULL);
	if (pcs > 6)MeshObj_H[6] = (HANDLE)_beginthreadex(NULL, 0, GetVB6, NULL, 0, NULL);
	if (pcs > 7)MeshObj_H[7] = (HANDLE)_beginthreadex(NULL, 0, GetVB7, NULL, 0, NULL);
	if (pcs > 8)MeshObj_H[8] = (HANDLE)_beginthreadex(NULL, 0, GetVB8, NULL, 0, NULL);
	if (pcs > 9)MeshObj_H[9] = (HANDLE)_beginthreadex(NULL, 0, GetVB9, NULL, 0, NULL);
	if (pcs > 10)MeshObj_H[10] = (HANDLE)_beginthreadex(NULL, 0, GetVB10, NULL, 0, NULL);
	if (pcs > 11)MeshObj_H[11] = (HANDLE)_beginthreadex(NULL, 0, GetVB11, NULL, 0, NULL);
	if (pcs > 12)MeshObj_H[12] = (HANDLE)_beginthreadex(NULL, 0, GetVB12, NULL, 0, NULL);
	if (pcs > 13)MeshObj_H[13] = (HANDLE)_beginthreadex(NULL, 0, GetVB13, NULL, 0, NULL);
	if (pcs > 14)MeshObj_H[14] = (HANDLE)_beginthreadex(NULL, 0, GetVB14, NULL, 0, NULL);
	if (pcs > 15)MeshObj_H[15] = (HANDLE)_beginthreadex(NULL, 0, GetVB15, NULL, 0, NULL);
	if (pcs > 16)MeshObj_H[16] = (HANDLE)_beginthreadex(NULL, 0, GetVB16, NULL, 0, NULL);
	if (pcs > 17)MeshObj_H[17] = (HANDLE)_beginthreadex(NULL, 0, GetVB17, NULL, 0, NULL);
	if (pcs > 18)MeshObj_H[18] = (HANDLE)_beginthreadex(NULL, 0, GetVB18, NULL, 0, NULL);
	if (pcs > 19)MeshObj_H[19] = (HANDLE)_beginthreadex(NULL, 0, GetVB19, NULL, 0, NULL);
	if (pcs > 20)MeshObj_H[20] = (HANDLE)_beginthreadex(NULL, 0, GetVB20, NULL, 0, NULL);
	if (pcs > 21)MeshObj_H[21] = (HANDLE)_beginthreadex(NULL, 0, GetVB21, NULL, 0, NULL);
	if (pcs > 22)MeshObj_H[22] = (HANDLE)_beginthreadex(NULL, 0, GetVB22, NULL, 0, NULL);
	if (pcs > 23)MeshObj_H[23] = (HANDLE)_beginthreadex(NULL, 0, GetVB23, NULL, 0, NULL);
	if (pcs > 24)MeshObj_H[24] = (HANDLE)_beginthreadex(NULL, 0, GetVB24, NULL, 0, NULL);
	if (pcs > 25)MeshObj_H[25] = (HANDLE)_beginthreadex(NULL, 0, GetVB25, NULL, 0, NULL);
	if (pcs > 26)MeshObj_H[26] = (HANDLE)_beginthreadex(NULL, 0, GetVB26, NULL, 0, NULL);
	if (pcs > 27)MeshObj_H[27] = (HANDLE)_beginthreadex(NULL, 0, GetVB27, NULL, 0, NULL);
	if (pcs > 28)MeshObj_H[28] = (HANDLE)_beginthreadex(NULL, 0, GetVB28, NULL, 0, NULL);
	if (pcs > 29)MeshObj_H[29] = (HANDLE)_beginthreadex(NULL, 0, GetVB29, NULL, 0, NULL);
	if (pcs > 30)MeshObj_H[30] = (HANDLE)_beginthreadex(NULL, 0, GetVB30, NULL, 0, NULL);
	if (pcs > 31)MeshObj_H[31] = (HANDLE)_beginthreadex(NULL, 0, GetVB31, NULL, 0, NULL);
	if (pcs > 32)MeshObj_H[32] = (HANDLE)_beginthreadex(NULL, 0, GetVB32, NULL, 0, NULL);
	if (pcs > 33)MeshObj_H[33] = (HANDLE)_beginthreadex(NULL, 0, GetVB33, NULL, 0, NULL);
	if (pcs > 34)MeshObj_H[34] = (HANDLE)_beginthreadex(NULL, 0, GetVB34, NULL, 0, NULL);
	if (pcs > 35)MeshObj_H[35] = (HANDLE)_beginthreadex(NULL, 0, GetVB35, NULL, 0, NULL);

	//全スレッドが終了するまでループ
	int cnt = 0;
	while (cnt != pcs){
		cnt = 0;
		for (int i = 0; i < pcs; i++){
			DWORD th_end;
			GetExitCodeThread(MeshObj_H[i], &th_end);//ハンドルの状態
			if (th_end != STILL_ACTIVE)cnt++;//停止してればカウント
		}
	}
	for (int i = 0; i < pcs; i++){
		WaitForSingleObject(MeshObj_H[i], INFINITE);//スレッドが終了するまで待つ
		CloseHandle(MeshObj_H[i]);                 //ハンドルを閉じる
		MeshObj_H[i] = NULL;
	}
	free(MeshObj_H);//ハンドル配列解放
	MeshObj_H = NULL;
	MeshObj = NULL;
	MeshPass = NULL;
}

//GetVBarrayThreadArray内で使用
unsigned __stdcall GetVB0(void *){
	MeshData::MeshObj[0].GetVBarray(MeshData::MeshPass[0], FALSE);
	return 0;
}
unsigned __stdcall GetVB1(void *){
	MeshData::MeshObj[1].GetVBarray(MeshData::MeshPass[1], FALSE);
	return 0;
}
unsigned __stdcall GetVB2(void *){
	MeshData::MeshObj[2].GetVBarray(MeshData::MeshPass[2], FALSE);
	return 0;
}
unsigned __stdcall GetVB3(void *){
	MeshData::MeshObj[3].GetVBarray(MeshData::MeshPass[3], FALSE);
	return 0;
}
unsigned __stdcall GetVB4(void *){
	MeshData::MeshObj[4].GetVBarray(MeshData::MeshPass[4], FALSE);
	return 0;
}
unsigned __stdcall GetVB5(void *){
	MeshData::MeshObj[5].GetVBarray(MeshData::MeshPass[5], FALSE);
	return 0;
}
unsigned __stdcall GetVB6(void *){
	MeshData::MeshObj[6].GetVBarray(MeshData::MeshPass[6], FALSE);
	return 0;
}
unsigned __stdcall GetVB7(void *){
	MeshData::MeshObj[7].GetVBarray(MeshData::MeshPass[7], FALSE);
	return 0;
}
unsigned __stdcall GetVB8(void *){
	MeshData::MeshObj[8].GetVBarray(MeshData::MeshPass[8], FALSE);
	return 0;
}
unsigned __stdcall GetVB9(void *){
	MeshData::MeshObj[9].GetVBarray(MeshData::MeshPass[9], FALSE);
	return 0;
}
unsigned __stdcall GetVB10(void *){
	MeshData::MeshObj[10].GetVBarray(MeshData::MeshPass[10], FALSE);
	return 0;
}
unsigned __stdcall GetVB11(void *){
	MeshData::MeshObj[11].GetVBarray(MeshData::MeshPass[11], FALSE);
	return 0;
}
unsigned __stdcall GetVB12(void *){
	MeshData::MeshObj[12].GetVBarray(MeshData::MeshPass[12], FALSE);
	return 0;
}
unsigned __stdcall GetVB13(void *){
	MeshData::MeshObj[13].GetVBarray(MeshData::MeshPass[13], FALSE);
	return 0;
}
unsigned __stdcall GetVB14(void *){
	MeshData::MeshObj[14].GetVBarray(MeshData::MeshPass[14], FALSE);
	return 0;
}
unsigned __stdcall GetVB15(void *){
	MeshData::MeshObj[15].GetVBarray(MeshData::MeshPass[15], FALSE);
	return 0;
}
unsigned __stdcall GetVB16(void *){
	MeshData::MeshObj[16].GetVBarray(MeshData::MeshPass[16], FALSE);
	return 0;
}
unsigned __stdcall GetVB17(void *){
	MeshData::MeshObj[17].GetVBarray(MeshData::MeshPass[17], FALSE);
	return 0;
}
unsigned __stdcall GetVB18(void *){
	MeshData::MeshObj[18].GetVBarray(MeshData::MeshPass[18], FALSE);
	return 0;
}
unsigned __stdcall GetVB19(void *){
	MeshData::MeshObj[19].GetVBarray(MeshData::MeshPass[19], FALSE);
	return 0;
}
unsigned __stdcall GetVB20(void *){
	MeshData::MeshObj[20].GetVBarray(MeshData::MeshPass[20], FALSE);
	return 0;
}
unsigned __stdcall GetVB21(void *){
	MeshData::MeshObj[21].GetVBarray(MeshData::MeshPass[21], FALSE);
	return 0;
}
unsigned __stdcall GetVB22(void *){
	MeshData::MeshObj[22].GetVBarray(MeshData::MeshPass[22], FALSE);
	return 0;
}
unsigned __stdcall GetVB23(void *){
	MeshData::MeshObj[23].GetVBarray(MeshData::MeshPass[23], FALSE);
	return 0;
}
unsigned __stdcall GetVB24(void *){
	MeshData::MeshObj[24].GetVBarray(MeshData::MeshPass[24], FALSE);
	return 0;
}
unsigned __stdcall GetVB25(void *){
	MeshData::MeshObj[25].GetVBarray(MeshData::MeshPass[25], FALSE);
	return 0;
}
unsigned __stdcall GetVB26(void *){
	MeshData::MeshObj[26].GetVBarray(MeshData::MeshPass[26], FALSE);
	return 0;
}
unsigned __stdcall GetVB27(void *){
	MeshData::MeshObj[27].GetVBarray(MeshData::MeshPass[27], FALSE);
	return 0;
}
unsigned __stdcall GetVB28(void *){
	MeshData::MeshObj[28].GetVBarray(MeshData::MeshPass[28], FALSE);
	return 0;
}
unsigned __stdcall GetVB29(void *){
	MeshData::MeshObj[29].GetVBarray(MeshData::MeshPass[29], FALSE);
	return 0;
}
unsigned __stdcall GetVB30(void *){
	MeshData::MeshObj[30].GetVBarray(MeshData::MeshPass[30], FALSE);
	return 0;
}
unsigned __stdcall GetVB31(void *){
	MeshData::MeshObj[31].GetVBarray(MeshData::MeshPass[31], FALSE);
	return 0;
}
unsigned __stdcall GetVB32(void *){
	MeshData::MeshObj[32].GetVBarray(MeshData::MeshPass[32], FALSE);
	return 0;
}
unsigned __stdcall GetVB33(void *){
	MeshData::MeshObj[33].GetVBarray(MeshData::MeshPass[33], FALSE);
	return 0;
}
unsigned __stdcall GetVB34(void *){
	MeshData::MeshObj[34].GetVBarray(MeshData::MeshPass[34], FALSE);
	return 0;
}
unsigned __stdcall GetVB35(void *){
	MeshData::MeshObj[35].GetVBarray(MeshData::MeshPass[35], FALSE);
	return 0;
}

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

	RELEASE(pConstantBuffer_MESH);

	if (pMyVB != NULL){
		RELEASE(pMyVB);
		for (DWORD i = 0; i < MaterialCount; i++){
			RELEASE(pMyVBI[i]);
		}
		S_DELETE(pMyVBI);
	}
	ARR_DELETE(pMaterial);
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

void MeshData::LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL** ppMaterial){

	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");
	char line[200] = { 0 };//1行読み込み用
	char key[110] = { 0 };//1単語読み込み用
	VECTOR4 v = { 0, 0, 0, 1 };

	//マテリアル数を調べる
	MaterialCount = 0;
	while (!feof(fp))
	{
		//キーワード読み込み
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s ", key, sizeof(key));
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			MaterialCount++;
		}
	}
	MY_MATERIAL* pMaterial = new MY_MATERIAL[MaterialCount];

	//本読み込み	
	fseek(fp, 0, SEEK_SET);
	INT iMCount = -1;

	while (!feof(fp))
	{
		//キーワード読み込み
		fgets(line, sizeof(line), fp);//1行読み込みlineに格納,FILEポインタ1行進む
		sscanf_s(line, "%s ", key, sizeof(key));//読み込んだ1行から"%s"最初の文字列1個読み込み
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			sscanf_s(&line[7], "%s ", key, sizeof(key));//lineの7要素目(newmtl)の直後から1個目の文字列をkeyに格納
			strcpy_s(pMaterial[iMCount].MaterialName, key);
		}
		//Kd　ディフューズ
		if (strcmp(key, "Kd") == 0)
		{
			sscanf_s(&line[3], "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].Kd = v;
		}
		//map_Kd　テクスチャー
		if (strcmp(key, "map_Kd") == 0)
		{
			sscanf_s(&line[7], "%s", &pMaterial[iMCount].TextureName, sizeof(pMaterial[iMCount].TextureName));
			//Dx11クラス内から使用するテクスチャーナンバー取得(atoi 数値文字列をint型変換)
			pMaterial[iMCount].tex_no = atoi(pMaterial[iMCount].TextureName);
		}
	}
	fclose(fp);

	*ppMaterial = pMaterial;
}

void MeshData::GetVBarray(LPSTR FileName, bool disp){

	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD VCount = 0;//読み込みカウンター
	DWORD VNCount = 0;//読み込みカウンター
	DWORD VTCount = 0;//読み込みカウンター
	DWORD FCount = 0;//読み込みカウンター

	char line[200] = { 0 };
	char key[200] = { 0 };
	//OBJファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//事前に頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s ", key, sizeof(key));
		//マテリアル読み込み
		if (strcmp(key, "mtllib") == 0)
		{
			sscanf_s(&line[7], "%s ", key, sizeof(key));
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
	fseek(fp, 0, SEEK_SET);
	VCount = 0;
	VNCount = 0;
	VTCount = 0;
	FCount = 0;

	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fgets(line, sizeof(line), fp);
		sscanf_s(line, "%s", key, sizeof(key));

		//頂点 読み込み
		if (strcmp(key, "v") == 0)
		{
			sscanf_s(&line[2], "%f %f %f", &x, &y, &z);
			pvCoord[VCount].x = -x;
			pvCoord[VCount].y = z;
			pvCoord[VCount].z = y;
			VCount++;
		}

		//法線 読み込み
		if (strcmp(key, "vn") == 0)
		{
			sscanf_s(&line[3], "%f %f %f", &x, &y, &z);
			pvNormal[VNCount].x = x;
			pvNormal[VNCount].y = -z;
			pvNormal[VNCount].z = -y;
			VNCount++;
		}

		//テクスチャー座標 読み込み
		if (strcmp(key, "vt") == 0)
		{
			sscanf_s(&line[3], "%f %f", &x, &y);
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
		fseek(fp, 0, SEEK_SET);

		while (!feof(fp))
		{
			//キーワード 読み込み
			ZeroMemory(key, sizeof(key));
			fgets(line, sizeof(line), fp);
			sscanf_s(line, "%s ", key, sizeof(key));

			//フェイス 読み込み→頂点インデックスに
			if (strcmp(key, "usemtl") == 0)
			{
				sscanf_s(&line[7], "%s ", key, sizeof(key));
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
				if (pMaterial[i].tex_no != -1)//テクスチャーありサーフェイス
				{
					sscanf_s(&line[2], "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else//テクスチャー無しサーフェイス
				{
					sscanf_s(&line[2], "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
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
		dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVBI[i]);

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

	dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVB);

	//一時的変数解放
	ARR_DELETE(pvCoord);
	ARR_DELETE(pvNormal);
	ARR_DELETE(pvTexture);
	ARR_DELETE(pvVertexBuffer);

	//コンスタントバッファー作成　マテリアル渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_MESH);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	dx->pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer_MESH);

	GetShaderPointer(disp);
}

void MeshData::Draw(float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size, float disp){

	//使用するシェーダーのセット
	dx->pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	dx->pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	if (pHullShader != NULL)dx->pDeviceContext->HSSetShader(pHullShader, NULL, 0);
	if (pDomainShader != NULL)dx->pDeviceContext->DSSetShader(pDomainShader, NULL, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す
	dx->MatrixMap(pConstantBuffer, x, y, z, r, g, b, thetaZ, thetaY, thetaX, size, disp);

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
			dx->pDeviceContext->PSSetShaderResources(0, 1, &dx->pTexture[pMaterial[i].tex_no]);
			if (pDomainShader != NULL){
				dx->pDeviceContext->DSSetSamplers(0, 1, &dx->pSampleLinear);
				dx->pDeviceContext->DSSetShaderResources(0, 1, &dx->pTexture[pMaterial[i].tex_no]);
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