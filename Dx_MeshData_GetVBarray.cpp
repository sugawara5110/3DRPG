//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          MeshData�N���X                                    **//
//**                                   GetVBarray�֐�                                    **//
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

	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");
	char key[110] = { 0 };
	VECTOR4 v = { 0, 0, 0, 1 };

	//�}�e���A�����𒲂ׂ�
	MaterialCount = 0;
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			MaterialCount++;
		}
	}
	MY_MATERIAL* pMaterial = new MY_MATERIAL[MaterialCount];

	//�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);
	INT iMCount = -1;

	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(pMaterial[iMCount].MaterialName, key);
		}
		//Kd�@�f�B�t���[�Y
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial[iMCount].Kd = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &pMaterial[iMCount].TextureName, sizeof(pMaterial[iMCount].TextureName));
			//���C�h�����ɕϊ�
			wchar_t wname[110];
			mbstowcs(wname, pMaterial[iMCount].TextureName, 110);
			//�e�N�X�`���[���쐬
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
	DWORD VCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD VNCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD VTCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD FCount = 0;//�ǂݍ��݃J�E���^�[

	char key[200] = { 0 };
	//OBJ�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A���ǂݍ���
		if (strcmp(key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			LoadMaterialFromFile(key, &pMaterial);
		}
		//���_
		if (strcmp(key, "v") == 0)
		{
			VerCount++;
		}
		//�@��
		if (strcmp(key, "vn") == 0)
		{
			VNCount++;
		}
		//�e�N�X�`���[���W
		if (strcmp(key, "vt") == 0)
		{
			VTCount++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
		{
			FaceCount++;
		}
	}

	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	MY_VERTEX_MESH* pvVertexBuffer = new MY_VERTEX_MESH[FaceCount * 3];
	VECTOR3* pvCoord = new VECTOR3[VerCount];
	VECTOR3* pvNormal = new VECTOR3[VNCount];
	VECTOR2* pvTexture = new VECTOR2[VTCount];
	
	//�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);
	VCount = 0;
	VNCount = 0;
	VTCount = 0;
	FCount = 0;

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s", key, sizeof(key));

		//���_ �ǂݍ���
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[VCount].x = -x;
			pvCoord[VCount].y = z;
			pvCoord[VCount].z = y;
			VCount++;
		}

		//�@�� �ǂݍ���
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[VNCount].x = x;
			pvNormal[VNCount].y = -z;
			pvNormal[VNCount].z = -y;
			VNCount++;
		}

		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
			pvTexture[VTCount].x = x;
			pvTexture[VTCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
			VTCount++;
		}
	}

	//�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
	pMyVBI = new ID3D11Buffer*[MaterialCount];

	//�t�F�C�X�@�ǂݍ��݁@�o���o���Ɏ��^����Ă���\��������̂ŁA�}�e���A�����𗊂�ɂȂ����킹��
	bool boFlag = false;
	int* piFaceBuffer = new int[FaceCount * 3];//3���_�Ȃ̂�3�C���f�b�N�X
	FCount = 0;
	DWORD dwPartFCount = 0;
	for (DWORD i = 0; i < MaterialCount; i++)
	{
		dwPartFCount = 0;
		fseek(fp, SEEK_SET, 0);

		while (!feof(fp))
		{
			//�L�[���[�h �ǂݍ���
			ZeroMemory(key, sizeof(key));
			fscanf_s(fp, "%s ", key, sizeof(key));

			//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
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
				if (pMaterial[i].pTexture != NULL)//�e�N�X�`���[����T�[�t�F�C�X
				{
					fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else//�e�N�X�`���[�����T�[�t�F�C�X
				{
					fscanf_s(fp, "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
					vt1 = vt2 = vt3 = 1;//���G���[�h�~
				}

				//�C���f�b�N�X�o�b�t�@�[
				piFaceBuffer[dwPartFCount * 3] = FCount * 3;
				piFaceBuffer[dwPartFCount * 3 + 1] = FCount * 3 + 1;
				piFaceBuffer[dwPartFCount * 3 + 2] = FCount * 3 + 2;
				//���_�\���̂ɑ��
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

		if (dwPartFCount == 0)//�g�p����Ă��Ȃ��}�e���A���΍�
		{
			pMyVBI[i] = NULL;
			continue;
		}

		//�C���f�b�N�X�o�b�t�@�[���쐬
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

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
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

	//�ꎞ�I�ϐ����
	delete[] pvCoord;
	delete[] pvNormal;
	delete[] pvTexture;
	delete[] pvVertexBuffer;

	//�R���X�^���g�o�b�t�@�[�쐬�@�}�e���A���n���p
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

	//�g�p����V�F�[�_�[�̃Z�b�g
	dx->pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	dx->pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	if (pHullShader != NULL)dx->pDeviceContext->HSSetShader(pHullShader, NULL, 0);
	if (pDomainShader != NULL)dx->pDeviceContext->DSSetShader(pDomainShader, NULL, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	dx->MatrixMap(pConstantBuffer, x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, size, disp);

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	dx->pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	dx->pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	if (pHullShader != NULL)dx->pDeviceContext->HSSetConstantBuffers(0, 1, &pConstantBuffer);
	if (pDomainShader != NULL)dx->pDeviceContext->DSSetConstantBuffers(0, 1, &pConstantBuffer);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(MY_VERTEX_MESH);
	UINT offset = 0;
	dx->pDeviceContext->IASetVertexBuffers(0, 1, &pMyVB, &stride, &offset);

	//�g�p���钸�_�C���v�b�g���C�A�E�g���Z�b�g
	dx->pDeviceContext->IASetInputLayout(pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	if (pDomainShader != NULL){ dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST); }
	else{
		dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (DWORD i = 0; i < MaterialCount; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (pMaterial[i].FaceCnt == 0)
		{
			continue;
		}
		//�A���t�@�u�����h,�A���t�@�e�X�g�؂�ւ�
		bool a = FALSE;
		if (pMaterial[i].TextureName[0] != NULL)a = TRUE;
		dx->ChangeBlendState(a, a);

		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		dx->pDeviceContext->IASetIndexBuffer(pMyVBI[i], DXGI_FORMAT_R32_UINT, 0);
		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(dx->pDeviceContext->Map(pConstantBuffer_MESH, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			CONSTANT_BUFFER_MESH sg;
			sg.vDiffuse = pMaterial[i].Kd;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
			//�e�N�X�`���L���t���O
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
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
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
		//�v���~�e�B�u�������_�����O
		dx->pDeviceContext->DrawIndexed(pMaterial[i].FaceCnt * 3, 0, 0);
	}
	//�n��,�h���C���V�F�[�_�[����
	dx->pDeviceContext->HSSetShader(NULL, NULL, 0);
	dx->pDeviceContext->DSSetShader(NULL, NULL, 0);
}