//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          PolygonData2D�N���X                               **//
//**                                    GetVBarray2D�֐�                                 **//
//*****************************************************************************************//

#include "Dx11Process.h"

void PolygonData2D::Pos2DCompute(VECTOR3 *p){
	MATRIX VP, VP_VP;
	//����3D���W����ϊ��s��擾
	MatrixMultiply(&VP, &Dx11Process::GetInstance()->View, &Dx11Process::GetInstance()->Proj);
	MatrixMultiply(&VP_VP, &VP, &Dx11Process::GetInstance()->Vp);
	//�ϊ���̍��W�擾
	VectorMatrixMultiply(p, &VP_VP);
}

PolygonData2D::PolygonData2D(){

	dx = Dx11Process::GetInstance();
	pVertexLayout = NULL;
	pVertexShader = NULL;
	pPixelShader = NULL;
	pMyVB = NULL;
	d3varray = NULL;
	lockV = FALSE;
	pTexview = NULL;
}

PolygonData2D::~PolygonData2D(){

	RELEASE(pTexview);
	if (pMyVB != NULL){
		RELEASE(pMyVB);
		free(d3varray);
		d3varray = NULL;
	}
}

void PolygonData2D::GetVBarray2D(int pieces){

	ver = pieces * 4;//pieces==�l�p�`�̌�

	//���_�o�b�t�@�擾
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(MY_VERTEX2) * ver;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	//���_�z��擾
	d3varray = (MY_VERTEX2*)malloc(sizeof(MY_VERTEX2) * ver);
	//���_�o�b�t�@�ݒ�
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = d3varray;
	Dx11Process::GetInstance()->pDevice->CreateBuffer(&bd, &InitData, &pMyVB);
}

void PolygonData2D::GetShaderPointer(){

	if (pTexview == NULL){
		pVertexLayout = dx->pVertexLayout_2D;
		pVertexShader = dx->pVertexShader_2D;
		pPixelShader = dx->pPixelShader_2D;
	}
	else{
		pVertexLayout = dx->pVertexLayout_2DTC;
		pVertexShader = dx->pVertexShader_2DTC;
		pPixelShader = dx->pPixelShader_2DTC;
	}
}

void PolygonData2D::Draw(bool a, bool lock){//2D�`��

	GetShaderPointer();

	//�A���t�@�u�����h,�A���t�@�e�X�g�؂�ւ�
	bool at = a;
	if (d3varray[0].color.w != 1.0f)at = FALSE;
	dx->ChangeBlendState(at, a);

	//�g�p����V�F�[�_�[�̃Z�b�g
	dx->pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	dx->pDeviceContext->PSSetShader(pPixelShader, NULL, 0);

	//���_�z�񏑂������̏ꍇ����(�ŏ���1��͕K�����s�����,���̌�͔C��)
	D3D11_MAPPED_SUBRESOURCE pData;
	if (lockV == FALSE || lock == TRUE){
		dx->pDeviceContext->Map(pMyVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		memcpy_s(pData.pData, pData.RowPitch, (void*)d3varray, sizeof(MY_VERTEX2) * ver);
		dx->pDeviceContext->Unmap(pMyVB, 0);
		lockV = TRUE;
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(MY_VERTEX2);
	UINT offset = 0;
	dx->pDeviceContext->IASetVertexBuffers(0, 1, &pMyVB, &stride, &offset);

	//�g�p���钸�_�C���v�b�g���C�A�E�g���Z�b�g
	dx->pDeviceContext->IASetInputLayout(pVertexLayout);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	if (pTexview != NULL){
		dx->pDeviceContext->PSSetSamplers(0, 1, &dx->pSampleLinear);
		dx->pDeviceContext->PSSetShaderResources(0, 1, &pTexview);
	}
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	dx->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�v���~�e�B�u�������_�����O
	dx->pDeviceContext->Draw(ver, 0);
}

