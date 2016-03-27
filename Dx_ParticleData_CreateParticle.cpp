//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　         ParticleDataクラス                                 **//
//**                                  CreateParticle関数                                 **//
//*****************************************************************************************//

#include "Dx11Process.h"

ParticleData::ParticleData(){
	dx = Dx11Process::GetInstance();
	pGeometryShaderSO = NULL;
	pVertexShaderSO = NULL;
	pGeometryShader = NULL;
	pVertexLayout = NULL;
	pVertexShader = NULL;
	pPixelShader = NULL;
	pConstantBuffer = NULL;
	ver = 0;
	pMyVB = NULL;
	pMyVB_SO = NULL;
	Drawfirst = TRUE;
	P_pos = NULL;
}

ParticleData::~ParticleData(){
	if (pConstantBuffer != NULL){
		pConstantBuffer->Release();
		pConstantBuffer = NULL;
	}
	if (pMyVB != NULL){
		pMyVB->Release();
		pMyVB = NULL;
	}
	if (pMyVB_SO != NULL){
		pMyVB_SO->Release();
		pMyVB_SO = NULL;
	}
	if (P_pos != NULL){
		free(P_pos);
		P_pos = NULL;
	}
}

void ParticleData::GetShaderPointer(){
	pGeometryShaderSO = dx->pGeometryShader_PSO;
	pGeometryShader = dx->pGeometryShader_P;
	pVertexLayout = dx->pVertexLayout_P;
	pVertexShaderSO = dx->pVertexShader_PSO;
	pVertexShader = dx->pVertexShader_P;
	pPixelShader = dx->pPixelShader_P;
}

void ParticleData::MatrixMap(float x, float y, float z, float theta, float size, bool init, float speed){
	MATRIX mov;
	MATRIX rot;

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_P cb;
	dx->pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	dx->MatrixRotationZ(&rot, theta);
	dx->MatrixTranslation(&mov, x, y, z);
	dx->MatrixMultiply(&dx->World, &rot, &mov);
	dx->MatrixMultiply(&cb.WV, &dx->World, &dx->View);
	cb.Proj = dx->Proj;
	dx->MatrixTranspose(&cb.WV);
	dx->MatrixTranspose(&cb.Proj);
	cb.size.x = size;
	if (init)cb.size.y = 1.0f; else cb.size.y = 0.0f;
	cb.size.z = speed;
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	dx->pDeviceContext->Unmap(pConstantBuffer, 0);
}

void ParticleData::CreateParticle(int texture_no, float size, float density){
	//パーティクル用テクスチャ情報取得
	D3D11_TEXTURE2D_DESC desc2D;
	dx->pTexCPUAcc[texture_no]->GetDesc(&desc2D);

	//テクスチャの横サイズ取得
	int width = desc2D.Width;
	//テクスチャの縦サイズ取得
	int height = desc2D.Height;

	//ステップ数
	float step = 1 / size / density;

	//頂点個数カウント
	D3D11_MAPPED_SUBRESOURCE pData;
	dx->pDeviceContext->Map(dx->pTexCPUAcc[texture_no], 0, D3D11_MAP_READ, 0, &pData);
	UCHAR *ptex = (UCHAR*)pData.pData;
	for (float j = 0; j < height; j += step){
		UINT j1 = (UINT)j * pData.RowPitch;//RowPitchデータの行ピッチ、行幅、または物理サイズ (バイト単位)
		for (float i = 0; i < width; i += step){
			UINT ptexI = (UINT)i * 4 + j1;
			if (ptex[ptexI + 3] > 0)ver++;//アルファ値0より高い場合カウント
		}
	}
	dx->pDeviceContext->Unmap(dx->pTexCPUAcc[texture_no], 0);

	//パーティクル配列確保
	P_pos = (PartPos*)malloc(sizeof(PartPos) * ver);

	//ピクセルデータ読み込み
	dx->pDeviceContext->Map(dx->pTexCPUAcc[texture_no], 0, D3D11_MAP_READ, 0, &pData);
	ptex = (UCHAR*)pData.pData;
	int P_no = 0;
	float ws = width * size / 2;//中心を0,0,0にする為
	float hs = height * size / 2;
	for (float j = 0; j < height; j += step){
		UINT j1 = (UINT)j * pData.RowPitch;//RowPitchデータの行ピッチ、行幅、または物理サイズ (バイト単位)
		for (float i = 0; i < width; i += step){
			UINT ptexI = (UINT)i * 4 + j1;
			float yp = (float)(j * size - hs);
			if (ptex[ptexI + 3] > 0){
				float xp = (float)(i * size - ws);
				P_pos[P_no].Col.as(ptex[ptexI + 0], ptex[ptexI + 1], ptex[ptexI + 2], ptex[ptexI + 3]);//色
				float xst = xp + ((rand() % 500) * size) - ((rand() % 500) * size);
				float yst = yp + ((rand() % 500) * size) - ((rand() % 500) * size);
				float zst = (rand() % 500) * size;
				P_pos[P_no].CurrentPos.as(xst, yst, zst);
				P_pos[P_no].PosSt.as(xst, yst, zst);
				P_pos[P_no].PosEnd.as(xp, yp, 0.0f);//0,0,0を中心にする
				P_no++;
			}
		}
	}
	dx->pDeviceContext->Unmap(dx->pTexCPUAcc[texture_no], 0);

	//コンスタントバッファー作成　
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_P);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	dx->pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);

	//頂点バッファ, ストリーム出力先バッファ取得
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PartPos) * ver;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	//頂点バッファ設定
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = P_pos;
	dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVB);
	//ストリーム出力先
	dx->pDevice->CreateBuffer(&bd, NULL, &pMyVB_SO);
}

void ParticleData::Draw(float x, float y, float z, float theta, float size, bool init, float speed){
	GetShaderPointer();

	//このコンスタントバッファーをどのシェーダーで使うか
	dx->pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	dx->pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	dx->pDeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);

	//バーテックスバッファーをセット(計算前)
	ID3D11Buffer* buffer[1] = { pMyVB };
	UINT stride = sizeof(PartPos);
	UINT offset = 0;
	dx->pDeviceContext->IASetVertexBuffers(0, 1, buffer, &stride, &offset);
	//使用する頂点インプットレイアウトをセット
	dx->pDeviceContext->IASetInputLayout(pVertexLayout);
	//プリミティブ・トポロジーをセット
	dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//使用するシェーダーのセット
	dx->pDeviceContext->VSSetShader(pVertexShaderSO, NULL, 0);
	dx->pDeviceContext->GSSetShader(pGeometryShaderSO, NULL, 0);
	//ストリーム出力先セット
	buffer[0] = pMyVB_SO;
	dx->pDeviceContext->SOSetTargets(1, buffer, &offset);
	//ピクセルシェーダー無効
	dx->pDeviceContext->PSSetShader(NULL, NULL, 0);

	//ストリーム出力
	if (Drawfirst){
		dx->pDeviceContext->Draw(ver, 0);
		Drawfirst = false;
	}
	else
		dx->pDeviceContext->DrawAuto();

	buffer[0] = NULL;
	dx->pDeviceContext->SOSetTargets(1, buffer, &offset);

	//新しく計算した頂点データを描画対象にする
	ID3D11Buffer *tmp = pMyVB;
	pMyVB = pMyVB_SO;
	pMyVB_SO = tmp;

	//バーテックスバッファーをセット(計算後)
	dx->pDeviceContext->IASetVertexBuffers(0, 1, &pMyVB, &stride, &offset);
	//使用する頂点インプットレイアウトをセット
	dx->pDeviceContext->IASetInputLayout(pVertexLayout);
	//プリミティブ・トポロジーをセット
	dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	dx->pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	dx->pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	dx->pDeviceContext->GSSetShader(pGeometryShader, NULL, 0);

	MatrixMap(x, y, z, theta, size, init, speed);

	//描画
	dx->pDeviceContext->DrawAuto();

	//ジオメトリシェーダー無効
	dx->pDeviceContext->GSSetShader(NULL, NULL, 0);
}