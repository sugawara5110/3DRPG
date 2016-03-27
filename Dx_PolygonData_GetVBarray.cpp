//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          PolygonData�N���X                                 **//
//**                                    GetVBarray�֐�                                   **//
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
	lockV = FALSE;  //���b�N,���_�ݒ�O
	lockI = FALSE;//���b�N,�C���f�b�N�X�ݒ�O
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

HRESULT PolygonData::GetVBarray(PrimitiveType type, int pieces){//���_�o�b�t�@,�z��m��

	if (type == SQUARE){
		topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ver = pieces * 4;//pieces==�l�p�`�̌�
		verI = pieces * 6;
	}
	if (type == POINt){
		topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		ver = pieces;//pieces==�_�̌�
		verI = pieces;
	}
	if (type == LINE_L){
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		ver = pieces * 2;//pieces==���̌�
		verI = pieces * 2;
	}
	if (type == LINE_S){
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		ver = pieces * 2;//pieces==���̌�
		verI = pieces * 2;
	}

	//���_�o�b�t�@�擾
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(MY_VERTEX) * ver;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	//���_�z��擾
	d3varray = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * ver);
	//���_�o�b�t�@�ݒ�
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = d3varray;
	dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVB);

	//�C���f�b�N�X�o�b�t�@�擾
	bdI.Usage = D3D11_USAGE_DYNAMIC;
	bdI.ByteWidth = sizeof(UINT) * verI;
	bdI.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdI.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bdI.MiscFlags = 0;
	bdI.StructureByteStride = 0;
	//�C���f�b�N�X�z��擾
	d3varrayI = (UINT*)malloc(sizeof(UINT) * verI);
	//�C���f�b�N�X�o�b�t�@�ݒ�
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = d3varrayI;
	dx->pDevice->CreateBuffer(&bdI, &IndexData, &pMyVBI);

	CPUAccess = TRUE;
	return S_OK;
}

void PolygonData::GetVBarrayCPUNotAccess(int pieces){

	ver = verI = pieces * 4;//pieces==�l�p�`�̌�

	d3varray = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * ver);
	d3varrayI = (UINT*)malloc(sizeof(UINT) * ver);
	CPUAccess = FALSE;
}

void PolygonData::TextureInit(int width, int height){

	//����e�N�X�`���g�p���J���e�N�X�`������
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

	//�V�F�[�_���\�[�X�r���[�쐬
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

	//�e�N�X�`���̉��T�C�Y�擾
	int width = desc2D.Width;
	//�e�N�X�`���̏c�T�C�Y�擾
	int height = desc2D.Height;

	//���\�[�X�̃}�b�v
	D3D11_MAPPED_SUBRESOURCE pData;
	dx->pDeviceContext->Map(pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	UCHAR *ptex = (UCHAR*)pData.pData;

	for (int j = 0; j < height; j++){
		UINT j1 = j * pData.RowPitch;//RowPitch�f�[�^�̍s�s�b�`�A�s���A�܂��͕����T�C�Y (�o�C�g�P��)
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

	//�A���t�@�u�����h,�A���t�@�e�X�g�؂�ւ�
	bool at = a;
	if (d3varray[0].color.w != 1.0f)at = FALSE;//�A���t�@�l1.0�ȊO�̏ꍇ�͔������ɂ���
	dx->ChangeBlendState(at, a);

	//�g�p����V�F�[�_�[�̃Z�b�g
	dx->pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	dx->pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	if (pHullShader != NULL)dx->pDeviceContext->HSSetShader(pHullShader, NULL, 0);
	if (pDomainShader != NULL)dx->pDeviceContext->DSSetShader(pDomainShader, NULL, 0);

	//�R���g���[���|�C���g�Ńo�[�e�b�N�X�o�b�t�@�[�쐬
	if (CPUAccess == FALSE && lockV == FALSE){
		//���_�o�b�t�@�擾
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) * ver;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		//���_�o�b�t�@�ݒ�
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = d3varray;
		dx->pDevice->CreateBuffer(&bd, &InitData, &pMyVB);

		//�C���f�b�N�X�o�b�t�@�擾
		bdI.Usage = D3D11_USAGE_DEFAULT;
		bdI.ByteWidth = sizeof(UINT) * verI;
		bdI.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bdI.CPUAccessFlags = 0;
		bdI.MiscFlags = 0;
		bdI.StructureByteStride = 0;

		//�C���f�b�N�X�o�b�t�@�ݒ�
		D3D11_SUBRESOURCE_DATA IndexData;
		IndexData.pSysMem = d3varrayI;
		dx->pDevice->CreateBuffer(&bdI, &IndexData, &pMyVBI);
		lockV = TRUE;
	}

	D3D11_MAPPED_SUBRESOURCE pData, pData1;
	//�C���f�b�N�X�z�񏑂������̏ꍇ����(��{�I�ɍŏ��̈��̂�)
	if (CPUAccess == TRUE && lockI == FALSE){
		dx->pDeviceContext->Map(pMyVBI, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		memcpy_s(pData.pData, pData.RowPitch, (void*)d3varrayI, sizeof(UINT) * verI);
		dx->pDeviceContext->Unmap(pMyVBI, 0);
		lockI = TRUE;
	}

	//���_�z�񏑂������̏ꍇ����(�ŏ���1��͕K�����s�����,���̌�͔C��)
	if (CPUAccess == TRUE && (lockV == FALSE || lock == TRUE)){
		dx->pDeviceContext->Map(pMyVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData1);
		memcpy_s(pData1.pData, pData1.RowPitch, (void*)d3varray, sizeof(MY_VERTEX) * ver);
		dx->pDeviceContext->Unmap(pMyVB, 0);
		lockV = TRUE;
	}

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	dx->MatrixMap(pConstantBuffer, x, y, z, r, g, b, theta, 1.0f, disp);

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
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

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
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

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	dx->pDeviceContext->IASetVertexBuffers(0, 1, &pMyVB, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	dx->pDeviceContext->IASetIndexBuffer(pMyVBI, DXGI_FORMAT_R32_UINT, 0);

	//�g�p���钸�_�C���v�b�g���C�A�E�g���Z�b�g
	dx->pDeviceContext->IASetInputLayout(pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	if (CPUAccess == TRUE){
		dx->pDeviceContext->IASetPrimitiveTopology(topology);
	}
	else{
		dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	}
	//�v���~�e�B�u�������_�����O
	dx->pDeviceContext->DrawIndexed(verI, 0, 0);

	//�n��,�h���C���V�F�[�_�[����
	dx->pDeviceContext->HSSetShader(NULL, NULL, 0);
	dx->pDeviceContext->DSSetShader(NULL, NULL, 0);
}
