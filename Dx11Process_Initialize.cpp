//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx11Process�N���X                                 **//
//**                                   Initialize�֐�                                    **//
//*****************************************************************************************//

#include "Dx11Process.h"
#include "ShaderMesh_D.h"
#include "ShaderMesh.h"
#include "ShaderDisp.h"
#include "Shader3D.h"
#include "Shader2D.h"
#include <string.h>
#include <stdio.h>

#define WINDOW_WIDTH 800 //�E�B���h�E��
#define WINDOW_HEIGHT 600 //�E�B���h�E����

Dx11Process *Dx11Process::dx = NULL;

void Dx11Process::InstanceCreate(){

	if (dx == NULL)dx = new Dx11Process();
}

Dx11Process *Dx11Process::GetInstance(){

	if (dx != NULL)return dx;
	return NULL;
}

void Dx11Process::DeleteInstance(){

	if (dx != NULL){
		delete dx;
		dx = NULL;
	}
}

Dx11Process::Dx11Process(){}
Dx11Process::Dx11Process(const Dx11Process &obj) {}      // �R�s�[�R���X�g���N�^�֎~
void Dx11Process::operator=(const Dx11Process& obj) {} // ������Z�q�֎~

//hlsl�t�@�C����ǂݍ��݃V�F�[�_�[���쐬����
HRESULT Dx11Process::MakeShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob){
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DCompile(szFileName, size, NULL, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, ppBlob, &pErrors)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		pDevice->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		pDevice->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		pDevice->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		pDevice->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		pDevice->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		pDevice->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);
	}
	if (pErrors != NULL){
		pErrors->Release();
		pErrors = NULL;
	}
	return S_OK;
}

void Dx11Process::ChangeBlendState(BOOL at, BOOL a){
	//�ύX�����̏ꍇ�������Ȃ�
	if (bld.AlphaToCoverageEnable == at && bld.RenderTarget[0].BlendEnable == a)return;
	//�ύX�L�̏ꍇ
	bld.AlphaToCoverageEnable = at;
	bld.RenderTarget[0].BlendEnable = a;
	pDevice->CreateBlendState(&bld, &pBlendState);
	UINT mask = 0xffffffff;
	pDeviceContext->OMSetBlendState(pBlendState, NULL, mask);
}

void Dx11Process::MatrixMap(ID3D11Buffer *pCBuffer, float x, float y, float z, float r, float g, float b, float theta, float size, float disp){

	MATRIX mov;
	MATRIX rot;
	MATRIX scale;
	MATRIX scro;
	MATRIX WV;

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER cb;
	pDeviceContext->Map(pCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);

	//�g��k��
	MatrixScaling(&scale, size, size, size);
	//�\���ʒu
	MatrixRotationZ(&rot, theta);
	MatrixTranslation(&mov, x, y, z);
	MatrixMultiply(&scro, &rot, &scale);
	MatrixMultiply(&World, &scro, &mov);

	//���[���h�A�J�����A�ˉe�s��A��
	cb.World = World;
	MatrixMultiply(&WV, &World, &View);
	MatrixMultiply(&cb.WVP, &WV, &Proj);
	cb.C_Pos.as(cx, cy, cz, 0.0f);
	cb.AddObjColor.as(r, g, b, 0.0f);
	cb.pShadowLow_Lpcs.as(plight.ShadowLow_val, (float)plight.LightPcs, 0.0f, 0.0f);
	memcpy(cb.pLightPos, plight.LightPos, sizeof(VECTOR4) * LIGHT_PCS);
	memcpy(cb.pLightColor, plight.LightColor, sizeof(VECTOR4) * LIGHT_PCS);
	memcpy(cb.pLightst, plight.Lightst, sizeof(VECTOR4) * LIGHT_PCS);
	cb.dDirection = dlight.Direction;
	cb.dLightColor = dlight.LightColor;
	cb.dLightst = dlight.Lightst;
	cb.FogAmo_Density.as(fog.Amount, fog.Density, fog.on_off, 0.0f);
	cb.FogColor = fog.FogColor;
	if (disp == 0.0f)disp = 3.0f;
	cb.DispAmount.as(disp, 0.0f, 0.0f, 0.0f);
	MatrixTranspose(&cb.World);
	MatrixTranspose(&cb.WVP);
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	pDeviceContext->Unmap(pCBuffer, 0);
}

void Dx11Process::MatrixIdentity(MATRIX *mat){
	mat->_11 = 1.0f; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = 1.0f; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void Dx11Process::MatrixScaling(MATRIX *mat, float sizex, float sizey, float sizez){
	mat->_11 = sizex; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = sizey; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = sizez; mat->_34 = 0.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void Dx11Process::MatrixRotationZ(MATRIX *mat, float theta){
	float the = theta * 3.14f / 180.0f;
	mat->_11 = cos(the); mat->_12 = sin(the); mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = -sin(the); mat->_22 = cos(the); mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void Dx11Process::MatrixTranslation(MATRIX *mat, float movx, float movy, float movz){
	mat->_11 = 1.0f; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = 1.0f; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
	mat->_41 = movx; mat->_42 = movy; mat->_43 = movz; mat->_44 = 1.0f;
}

void Dx11Process::MatrixMultiply(MATRIX *mat, MATRIX *mat1, MATRIX *mat2){
	mat->_11 = mat1->_11 * mat2->_11 + mat1->_12 * mat2->_21 + mat1->_13 * mat2->_31 + mat1->_14 * mat2->_41;
	mat->_12 = mat1->_11 * mat2->_12 + mat1->_12 * mat2->_22 + mat1->_13 * mat2->_32 + mat1->_14 * mat2->_42;
	mat->_13 = mat1->_11 * mat2->_13 + mat1->_12 * mat2->_23 + mat1->_13 * mat2->_33 + mat1->_14 * mat2->_43;
	mat->_14 = mat1->_11 * mat2->_14 + mat1->_12 * mat2->_24 + mat1->_13 * mat2->_34 + mat1->_14 * mat2->_44;

	mat->_21 = mat1->_21 * mat2->_11 + mat1->_22 * mat2->_21 + mat1->_23 * mat2->_31 + mat1->_24 * mat2->_41;
	mat->_22 = mat1->_21 * mat2->_12 + mat1->_22 * mat2->_22 + mat1->_23 * mat2->_32 + mat1->_24 * mat2->_42;
	mat->_23 = mat1->_21 * mat2->_13 + mat1->_22 * mat2->_23 + mat1->_23 * mat2->_33 + mat1->_24 * mat2->_43;
	mat->_24 = mat1->_21 * mat2->_14 + mat1->_22 * mat2->_24 + mat1->_23 * mat2->_34 + mat1->_24 * mat2->_44;

	mat->_31 = mat1->_31 * mat2->_11 + mat1->_32 * mat2->_21 + mat1->_33 * mat2->_31 + mat1->_34 * mat2->_41;
	mat->_32 = mat1->_31 * mat2->_12 + mat1->_32 * mat2->_22 + mat1->_33 * mat2->_32 + mat1->_34 * mat2->_42;
	mat->_33 = mat1->_31 * mat2->_13 + mat1->_32 * mat2->_23 + mat1->_33 * mat2->_33 + mat1->_34 * mat2->_43;
	mat->_34 = mat1->_31 * mat2->_14 + mat1->_32 * mat2->_24 + mat1->_33 * mat2->_34 + mat1->_34 * mat2->_44;

	mat->_41 = mat1->_41 * mat2->_11 + mat1->_42 * mat2->_21 + mat1->_43 * mat2->_31 + mat1->_44 * mat2->_41;
	mat->_42 = mat1->_41 * mat2->_12 + mat1->_42 * mat2->_22 + mat1->_43 * mat2->_32 + mat1->_44 * mat2->_42;
	mat->_43 = mat1->_41 * mat2->_13 + mat1->_42 * mat2->_23 + mat1->_43 * mat2->_33 + mat1->_44 * mat2->_43;
	mat->_44 = mat1->_41 * mat2->_14 + mat1->_42 * mat2->_24 + mat1->_43 * mat2->_34 + mat1->_44 * mat2->_44;
}

void swap(float *a, float *b){
	float tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void Dx11Process::MatrixTranspose(MATRIX *mat){
	swap(&mat->_12, &mat->_21);
	swap(&mat->_13, &mat->_31);
	swap(&mat->_14, &mat->_41);
	swap(&mat->_23, &mat->_32);
	swap(&mat->_24, &mat->_42);
	swap(&mat->_34, &mat->_43);
}

void Dx11Process::MatrixLookAtLH(MATRIX *mat, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	//z��
	float zx = x2 - x1;
	float zy = y2 - y1;
	float zz = z2 - z1;
	//���K��
	float zzz = sqrt(zx * zx + zy * zy + zz * zz);
	if (zzz != 0.0f){
		zx = zx / zzz;
		zy = zy / zzz;
		zz = zz / zzz;
	}

	//x��(�O��)
	float xx = y3 * zz - z3 * zy;
	float xy = z3 * zx - x3 * zz;
	float xz = x3 * zy - y3 * zx;
	float xxx = sqrt(xx * xx + xy * xy + xz * xz);
	if (xxx != 0.0f){
		xx = xx / xxx;
		xy = xy / xxx;
		xz = xz / xxx;
	}

	//y��(�O��)
	float yx = zy * xz - zz * xy;
	float yy = zz * xx - zx * xz;
	float yz = zx * xy - zy * xx;

	//���s�ړ�(����)
	float mx = -(x1 * xx + y1 * xy + z1 * xz);
	float my = -(x1 * yx + y1 * yy + z1 * yz);
	float mz = -(x1 * zx + y1 * zy + z1 * zz);

	mat->_11 = xx; mat->_12 = yx; mat->_13 = zx; mat->_14 = 0.0f;
	mat->_21 = xy; mat->_22 = yy; mat->_23 = zy; mat->_24 = 0.0f;
	mat->_31 = xz; mat->_32 = yz; mat->_33 = zz; mat->_34 = 0.0f;
	mat->_41 = mx; mat->_42 = my; mat->_43 = mz; mat->_44 = 1.0f;
}

void Dx11Process::MatrixPerspectiveFovLH(MATRIX *mat, float theta, float aspect, float Near, float Far){
	float the = theta * 3.14f / 180.0f;
	//�����ϊ���y�����X�P�[�����O
	float sy = 1.0f / (tan(the / 2.0f));
	//x�����X�P�[�����O
	float sx = sy / aspect;
	//z�����X�P�[�����O
	float sz = Far / (Far - Near);

	mat->_11 = sx; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = sy; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = sz; mat->_34 = 1.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = -(sz * Near); mat->_44 = 0.0f;
}

HRESULT Dx11Process::Initialize(HWND hWnd){

	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRTV = NULL;
	pDS = NULL;
	pDSV = NULL;
	pDomainShader_MESH_D = NULL;
	pHullShader_MESH_D = NULL;
	pVertexLayout_MESH_D = NULL;
	pVertexShader_MESH_D = NULL;
	pPixelShader_MESH_D = NULL;
	pVertexLayout_MESH = NULL;
	pVertexShader_MESH = NULL;
	pPixelShader_MESH = NULL;
	pDomainShader_DISPL = NULL;
	pHullShader_DISPL = NULL;
	pVertexLayout_DISPL = NULL;
	pVertexShader_DISPL = NULL;
	pPixelShader_DISPL = NULL;
	pDomainShader_DISP = NULL;
	pHullShader_DISP = NULL;
	pVertexLayout_DISP = NULL;
	pVertexShader_DISP = NULL;
	pPixelShader_DISP = NULL;
	pVertexLayout_TCL = NULL;
	pVertexShader_TCL = NULL;
	pPixelShader_TCL = NULL;
	pVertexLayout_TC = NULL;
	pVertexShader_TC = NULL;
	pPixelShader_TC = NULL;
	pVertexLayout_BC = NULL;
	pVertexShader_BC = NULL;
	pPixelShader_BC = NULL;
	pVertexLayout_2D = NULL;
	pVertexShader_2D = NULL;
	pPixelShader_2D = NULL;
	pVertexLayout_2DTC = NULL;
	pVertexShader_2DTC = NULL;
	pPixelShader_2DTC = NULL;
	pConstantBuffer = NULL;
	pSampleLinear = NULL;
	pBlendState = NULL;
	pRasterizeState = NULL;

	//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &pFeatureLevels
		, 1, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, pFeatureLevel, &pDeviceContext);

	//�o�b�N�o�b�t�@�[�̃����_�[�^�[�Q�b�g�r���[(RTV)���쐬
	ID3D11Texture2D *pBack;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);

	pDevice->CreateRenderTargetView(pBack, NULL, &pRTV);
	pBack->Release();

	//�f�v�X�X�e���V���r���[(DSV)���쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &pDS);

	pDevice->CreateDepthStencilView(pDS, NULL, &pDSV);

	//�e�}�g���b�N�X������
	MatrixIdentity(&View);
	MatrixIdentity(&Proj);
	MatrixIdentity(&World);

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1, &vp);
	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;
	// �ˉe�}�g���b�N�X���쐬
	MatrixPerspectiveFovLH(&Proj,
		55.0f,           // �J�����̉�p
		aspect,		    // �A�X�y�N�g��
		1.0f,		   // near�v���[��
		10000.0f);	  // far�v���[��

	//���b�V�����_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout_MESH[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout_MESH) / sizeof(layout_MESH[0]);
	ID3DBlob *pCompiledShader = NULL;

	//**********************************�n���V�F�[�_�[***************************************************************//
	MakeShader(ShaderMesh_D, strlen(ShaderMesh_D), "HSMesh", "hs_5_0", (void**)&pHullShader_MESH_D, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************�n���V�F�[�_�[***************************************************************//

	//**********************************�h���C���V�F�[�_�[***********************************************************//
	MakeShader(ShaderMesh_D, strlen(ShaderMesh_D), "DSMesh", "ds_5_0", (void**)&pDomainShader_MESH_D, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************�h���C���V�F�[�_�[***********************************************************//

	//**********************************���_�V�F�[�_�[***************************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O�L
	MakeShader(ShaderMesh_D, strlen(ShaderMesh_D), "VSMesh", "vs_5_0", (void**)&pVertexShader_MESH_D, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout_MESH, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_MESH_D);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�f�B�X�v���C�g�����g�}�b�s���O��
	MakeShader(ShaderMesh, strlen(ShaderMesh), "VSMesh", "vs_5_0", (void**)&pVertexShader_MESH, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout_MESH, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_MESH);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************���_�V�F�[�_�[***************************************************************//

	//**********************************�s�N�Z���V�F�[�_�[***********************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O�L
	MakeShader(ShaderMesh_D, strlen(ShaderMesh_D), "PSMesh", "ps_5_0", (void**)&pPixelShader_MESH_D, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�f�B�X�v���C�g�����g�}�b�s���O��
	MakeShader(ShaderMesh, strlen(ShaderMesh), "PSMesh", "ps_5_0", (void**)&pPixelShader_MESH, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************�s�N�Z���V�F�[�_�[***********************************************************//

	//3D���_�C���v�b�g���C�A�E�g���`, NORMAL�ɂ�POSITION��float�^4�o�C�g�~3���L�q
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(layout) / sizeof(layout[0]);
	pCompiledShader = NULL;

	//**********************************�n���V�F�[�_�[***************************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O, ���C�g�L �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "HSDispL", "hs_5_0", (void**)&pHullShader_DISPL, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�f�B�X�v���C�g�����g�}�b�s���O �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "HSDisp", "hs_5_0", (void**)&pHullShader_DISP, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************�n���V�F�[�_�[***************************************************************//

	//**********************************�h���C���V�F�[�_�[***********************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O, ���C�g�L �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "DSDispL", "ds_5_0", (void**)&pDomainShader_DISPL, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�f�B�X�v���C�g�����g�}�b�s���O �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "DSDisp", "ds_5_0", (void**)&pDomainShader_DISP, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************�h���C���V�F�[�_�[***********************************************************//

	//**********************************���_�V�F�[�_�[***************************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O, ���C�g�L �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "VSDispL", "vs_5_0", (void**)&pVertexShader_DISPL, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_DISPL);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�f�B�X�v���C�g�����g�}�b�s���O �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "VSDisp", "vs_5_0", (void**)&pVertexShader_DISP, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_DISP);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�e�N�X�`���[,���C�g�L �쐬
	MakeShader(Shader3D, strlen(Shader3D), "VSTextureColorL", "vs_5_0", (void**)&pVertexShader_TCL, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_TCL);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader3D, strlen(Shader3D), "VSTextureColor", "vs_5_0", (void**)&pVertexShader_TC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_TC);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//��{�F �쐬
	MakeShader(Shader3D, strlen(Shader3D), "VSBaseColor", "vs_5_0", (void**)&pVertexShader_BC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_BC);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************���_�V�F�[�_�[***************************************************************//

	//**********************************�s�N�Z���V�F�[�_�[***********************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O, ���C�g�L �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "PSDispL", "ps_5_0", (void**)&pPixelShader_DISPL, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�f�B�X�v���C�g�����g�}�b�s���O �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "PSDisp", "ps_5_0", (void**)&pPixelShader_DISP, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�e�N�X�`���[,���C�g�L �쐬
	MakeShader(Shader3D, strlen(Shader3D), "PSTextureColorL", "ps_5_0", (void**)&pPixelShader_TCL, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader3D, strlen(Shader3D), "PSTextureColor", "ps_5_0", (void**)&pPixelShader_TC, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//��{�F �쐬
	MakeShader(Shader3D, strlen(Shader3D), "PSBaseColor", "ps_5_0", (void**)&pPixelShader_BC, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************�s�N�Z���V�F�[�_�[***********************************************************//

	//2D���_�C���v�b�g���C�A�E�g��`
	D3D11_INPUT_ELEMENT_DESC layout2D[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(layout2D) / sizeof(layout2D[0]);
	pCompiledShader = NULL;

	//**********************************���_�V�F�[�_�[2D*************************************************************//
	//��{�F �쐬
	MakeShader(Shader2D, strlen(Shader2D), "VSBaseColor", "vs_5_0", (void**)&pVertexShader_2D, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout2D, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_2D);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader2D, strlen(Shader2D), "VSTextureColor", "vs_5_0", (void**)&pVertexShader_2DTC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout2D, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_2DTC);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************���_�V�F�[�_�[2D*************************************************************//

	//**********************************�s�N�Z���V�F�[�_�[2D*********************************************************//
	//��{�F �쐬
	MakeShader(Shader2D, strlen(Shader2D), "PSBaseColor", "ps_5_0", (void**)&pPixelShader_2D, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;

	//�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader2D, strlen(Shader2D), "PSTextureColor", "ps_5_0", (void**)&pPixelShader_2DTC, &pCompiledShader);
	pCompiledShader->Release();
	pCompiledShader = NULL;
	//**********************************�s�N�Z���V�F�[�_�[2D*********************************************************//

	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	//png�t�@�C�����ɃA���t�@��񂪂���B�A���t�@�ɂ�蓧�߂���悤�w�肵�Ă���
	ZeroMemory(&bld, sizeof(D3D11_BLEND_DESC));
	bld.IndependentBlendEnable = false;
	bld.AlphaToCoverageEnable = true;//Directx9�̃A���t�@�e�X�g�Ɠ���
	bld.RenderTarget[0].BlendEnable = true;
	bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bld.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bld.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pDevice->CreateBlendState(&bld, &pBlendState);
	UINT mask = 0xffffffff;
	pDeviceContext->OMSetBlendState(pBlendState, NULL, mask);

	//���X�^���C�U
	RSDesc.FillMode = D3D11_FILL_SOLID;//�h��Ԃ��`��
	//RSDesc.FillMode = D3D11_FILL_WIREFRAME;//���C���t���[��
	//RSDesc.CullMode = D3D11_CULL_NONE;//�|���S�����ʕ`�惂�[�h(D3D11_CULL_BACK�ŕ\�̂ݕ`��)
	RSDesc.CullMode = D3D11_CULL_BACK;
	RSDesc.FrontCounterClockwise = FALSE;//���v��肪�\��
	RSDesc.DepthBias = 0;
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = FALSE;//�[�x�N���b�s���O����
	RSDesc.ScissorEnable = FALSE;
	RSDesc.MultisampleEnable = FALSE;
	RSDesc.AntialiasedLineEnable = FALSE;
	pDevice->CreateRasterizerState(&RSDesc, &pRasterizeState);
	pDeviceContext->RSSetState(pRasterizeState);

	//�T���v���[
	D3D11_SAMPLER_DESC sadesc;
	sadesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sadesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sadesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sadesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sadesc.MipLODBias = 0.0f;
	sadesc.MaxAnisotropy = 2;
	sadesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sadesc.BorderColor[0] = 0.0f;
	sadesc.BorderColor[1] = 0.0f;
	sadesc.BorderColor[2] = 0.0f;
	sadesc.BorderColor[3] = 0.0f;
	sadesc.MinLOD = -3.402823466e+38F;
	sadesc.MaxLOD = 3.402823466e+38F;
	pDevice->CreateSamplerState(&sadesc, &pSampleLinear);

	pTexture = (ID3D11ShaderResourceView**)malloc(sizeof(ID3D11ShaderResourceView*) * 100);
	binary_ch = (char**)malloc(sizeof(char*) * 100);
	binary_size = (int*)malloc(sizeof(int) * 100);
	for (int i = 0; i < 100; i++){
		pTexture[i] = NULL;
		binary_ch[i] = NULL;
		binary_size[i] = 0;
	}

	//�|�C���g���C�g�\���̏�����
	ResetPointLight();

	//���s����������
	dlight.Direction.as(0.0f, 0.0f, 0.0f, 0.0f);
	dlight.LightColor.as(1.0f, 1.0f, 1.0f, 1.0f);
	dlight.Lightst.as(1.0f, 0.0f, 0.3f, 0.0f);

	//�t�H�O������
	fog.FogColor.as(1.0f, 1.0f, 1.0f, 1.0f);
	fog.Amount = 0.0f;
	fog.Density = 0.0f;
	fog.on_off = 0.0f;

	return S_OK;
}

HRESULT Dx11Process::Sclear(){//�X�N���[���N���A

	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���Z�b�g
	pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
	//��ʃN���A
	float ClearColor[4] = { 0, 0, 0.5, 1 };// �N���A�F�쐬�@RGBA�̏�
	pDeviceContext->ClearRenderTargetView(pRTV, ClearColor);//�J���[�o�b�t�@�N���A
	pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�f�v�X�X�e���V���o�b�t�@�N���A
	return S_OK;
}

void Dx11Process::Cameraset(float cax1, float cax2, float cay1, float cay2, float caz){//�J�����Z�b�g

	//�J�����̈ʒu�ƕ�����ݒ�
	MatrixLookAtLH(&View,
		cax1, cay1, caz,   //�J�����̈ʒu
		cax2, cay2, caz,   //�J�����̕�����������_
		0.0f, 0.0f, 1.0f); //�J�����̏�̕���(�ʏ펋�_�ł̏������1.0f�ɂ���)
	//�V�F�[�_�[�v�Z�p���W�o�^
	cx = cax1;
	cy = cay1;
	cz = caz;
}

void Dx11Process::ResetPointLight(){
	for (int i = 0; i < LIGHT_PCS; i++){
		plight.LightPos[i].as(0.0f, 0.0f, 0.0f, 0.0f);
		plight.LightColor[i].as(0.0f, 0.0f, 0.0f, 0.0f);
		plight.Lightst[i].as(0.0f, 0.0f, 0.0f, 0.0f);
	}
	plight.ShadowLow_val = 0.0f;
}

void Dx11Process::P_ShadowBright(float val){
	plight.ShadowLow_val = val;
}

void Dx11Process::PointLightPosSet(int Idx, float x, float y, float z, float r, float g, float b, float a, float range,
	float brightness, float attenuation, bool on_off){

	static int pcs = 3;//�������l(�\��ς�)

	if (Idx > LIGHT_PCS - 1 || Idx < 0)return;//�G���[�h�~

	if (Idx > 2 && on_off == TRUE)pcs = Idx + 1;

	float onoff;
	if (on_off == TRUE)onoff = 1.0f; else onoff = 0.0f;
	plight.LightPos[Idx].as(x, y, z, 1.0f);
	plight.LightColor[Idx].as(r, g, b, a);
	plight.Lightst[Idx].as(range, brightness, attenuation, onoff);
	plight.LightPcs = pcs;
}

void Dx11Process::DirectionLight(float x, float y, float z, float r, float g, float b, float bright, float ShadowBright){
	dlight.Direction.as(x, y, z, 0.0f);
	dlight.LightColor.as(r, g, b, 0.0f);
	dlight.Lightst.x = bright;
	dlight.Lightst.z = ShadowBright;
	dlight.Lightst.w = 0.0f;
}

void Dx11Process::SetDirectionLight(bool onoff){
	float f = 0.0f;
	if (onoff == TRUE)f = 1.0f;
	dlight.Lightst.y = f;
}

void Dx11Process::Fog(float r, float g, float b, float amount, float density, bool onoff){

	if (onoff == FALSE){
		fog.on_off = 0.0f;
		return;
	}
	fog.on_off = 1.0f;
	fog.FogColor.as(r, g, b, 1.0f);
	fog.Amount = amount;
	fog.Density = density;
}

void Dx11Process::TextureBinaryDecode(char *Bpass, int i){

	//�Í������R�[�h��
	/*
	FILE *fp;
	FILE *fp2;
	char *binary;
	char *binary_change;
	int size;
	char fname[64];
	char fname_out[64];
	int i1;
	int len;
	int enemy_no = 0;
	int boss_no = 0;
	int bk = 0;

	while (1){
	scanf_s("%s", fname, 64);
	len = strlen(fname);
	if (len == 0 || len > 64)break;
	i1 = 0;
	do{
	strncpy(&fname_out[i1], &fname[i1], 1);
	i1++;
	} while (fname[i1] != '.');
	strncpy(&fname_out[i1++], ".", 1);
	strncpy(&fname_out[i1++], "d", 1);
	strncpy(&fname_out[i1++], "a", 1);
	strncpy(&fname_out[i1], "\0", 1);

	i1 = 0;
	if (strncmp(fname, "enemy.", 6) == 0)enemy_no = 1;
	if (strncmp(fname, "boss.", 5) == 0)boss_no = 1;

	do{
	if (enemy_no >= 1){
	sprintf(fname, "enemy%d.png", enemy_no);
	sprintf(fname_out, "enemy%d.da", enemy_no);
	}
	if (boss_no >= 1){
	sprintf(fname, "boss%d.png", boss_no);
	sprintf(fname_out, "boss%d.da", boss_no);
	}
	fp = NULL;
	fp2 = NULL;
	binary = NULL;
	binary_change = NULL;
	size = 0;

	fp = fopen(fname, "rb");
	if (fp == NULL){
	bk = 1; break;
	}

	while (!feof(fp)){
	size++; fgetc(fp);
	}
	size++;//�I�[�����܂߂���

	//�|�C���^��擪�ɖ߂�
	fseek(fp, 0, SEEK_SET);

	binary = (char*)malloc(sizeof(char) * size);
	binary_change = (char*)malloc(sizeof(char) * size);

	for (int i = 0; i < size; i++){
	binary[i] = fgetc(fp);//�I�[�����܂œǂݍ���
	}

	for (int i = 0; i < 100; i++){
	strncpy(&binary_change[i], &binary[99 - i], 1);
	}
	for (int i = 100; i < size; i++){
	strncpy(&binary_change[i], &binary[i], 1);
	}

	fp2 = fopen(fname_out, "wb");

	fwrite(binary_change, size, 1, fp2);

	free(binary);
	free(binary_change);
	fclose(fp);
	fclose(fp2);
	if (enemy_no >= 1)enemy_no++;
	if (boss_no >= 1)boss_no++;
	} while (enemy_no >= 1 || boss_no >= 1);
	enemy_no = boss_no = 0;
	if (bk == 1)break;
	}
	*/

	//�����R�[�h��
	FILE *fp;
	fp = fopen(Bpass, "rb");

	while (!feof(fp)){
		binary_size[i]++; fgetc(fp);
	}
	binary_size[i]++;//�I�[�����܂߂���

	//�|�C���^��擪�ɖ߂�
	fseek(fp, 0, SEEK_SET);

	binary_ch[i] = (char*)malloc(sizeof(char) * binary_size[i]);

	for (int j = 99; j >= 0; j--){
		binary_ch[i][j] = fgetc(fp);
	}

	for (int j = 100; j < binary_size[i]; j++){
		binary_ch[i][j] = fgetc(fp);
	}

	fclose(fp);
}

void Dx11Process::TextureBinaryDecodeAll(){

	//�}�b�v0
	TextureBinaryDecode("./dat/texture/map/wall1.da", 0);
	TextureBinaryDecode("./dat/texture/map/ground1.da", 1);
	TextureBinaryDecode("./dat/texture/map/ceiling1.da", 2);
	//�}�b�v1
	TextureBinaryDecode("./dat/texture/map/wall2.da", 4);
	TextureBinaryDecode("./dat/texture/map/ground2.da", 5);
	TextureBinaryDecode("./dat/texture/map/ceiling2.da", 6);
	TextureBinaryDecode("./dat/texture/map/wall2-1.da", 8);
	//�}�b�v2
	TextureBinaryDecode("./dat/texture/map/ceiling3_wall3.da", 11);
	TextureBinaryDecode("./dat/texture/map/ground3.da", 12);
	//�}�b�v3
	TextureBinaryDecode("./dat/texture/map/ceiling4_ground4.da", 15);
	//�}�b�v4
	TextureBinaryDecode("./dat/texture/map/wall5.da", 26);
	TextureBinaryDecode("./dat/texture/map/ground5.da", 27);
	TextureBinaryDecode("./dat/texture/map/ceiling5.da", 28);
	//�ʏ�G
	TextureBinaryDecode("./dat/texture/enemy/enemy1.da", 30);
	TextureBinaryDecode("./dat/texture/enemy/enemy2.da", 31);
	TextureBinaryDecode("./dat/texture/enemy/enemy3.da", 32);
	TextureBinaryDecode("./dat/texture/enemy/enemy4.da", 33);
	TextureBinaryDecode("./dat/texture/enemy/enemy5.da", 34);
	TextureBinaryDecode("./dat/texture/enemy/enemy6.da", 35);
	TextureBinaryDecode("./dat/texture/enemy/enemy7.da", 36);
	TextureBinaryDecode("./dat/texture/enemy/enemy8.da", 37);
	TextureBinaryDecode("./dat/texture/enemy/enemy9.da", 38);
	TextureBinaryDecode("./dat/texture/enemy/enemy10.da", 39);
	TextureBinaryDecode("./dat/texture/enemy/enemy11.da", 40);
	TextureBinaryDecode("./dat/texture/enemy/enemy12.da", 41);
	TextureBinaryDecode("./dat/texture/enemy/enemy13.da", 42);
	TextureBinaryDecode("./dat/texture/enemy/enemy14.da", 43);
	TextureBinaryDecode("./dat/texture/enemy/enemy15.da", 44);
	TextureBinaryDecode("./dat/texture/enemy/enemy16.da", 45);
	TextureBinaryDecode("./dat/texture/enemy/enemy17.da", 46);
	TextureBinaryDecode("./dat/texture/enemy/enemy18.da", 47);
	TextureBinaryDecode("./dat/texture/enemy/enemy19.da", 48);
	TextureBinaryDecode("./dat/texture/enemy/enemy20.da", 49);
	//�{�X
	TextureBinaryDecode("./dat/texture/enemy/boss1.da", 50);
	TextureBinaryDecode("./dat/texture/enemy/boss2.da", 51);
	TextureBinaryDecode("./dat/texture/enemy/boss3.da", 52);
	TextureBinaryDecode("./dat/texture/enemy/boss4.da", 53);
	//���X�g�{�X
	TextureBinaryDecode("./dat/texture/enemy/lastboss.da", 59);
	//�����w�ʏ�
	TextureBinaryDecode("./dat/texture/magic/side_magic.da", 60);
	//�����w�{�X
	TextureBinaryDecode("./dat/texture/magic/boss_magic.da", 61);
	//�񕜃|�C���g
	TextureBinaryDecode("./dat/texture/magic/recover.da", 70);
	//�G�t�F�N�g
	TextureBinaryDecode("./dat/texture/effect/e_att.da", 80);
	TextureBinaryDecode("./dat/texture/effect/h_att.da", 81);
	TextureBinaryDecode("./dat/texture/effect/flame.da", 82);
	TextureBinaryDecode("./dat/texture/effect/healing.da", 83);
	TextureBinaryDecode("./dat/texture/effect/recov.da", 84);
}

void Dx11Process::GetTexture(){

	for (int i = 0; i < 100; i++){
		if (binary_size[i] == 0)continue;
		//�e�N�X�`���[�쐬
		DirectX::CreateWICTextureFromMemoryEx(pDevice, (uint8_t*)binary_ch[i], binary_size[i], binary_size[i],
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, TRUE, NULL, &pTexture[i]);
	}
	for (int i = 0; i < 100; i++){
		if (binary_ch[i] == NULL)continue;
		free(binary_ch[i]);
		binary_ch[i] = NULL;
	}
	free(binary_ch);
	free(binary_size);
}

void  Dx11Process::Drawscreen(){//�X�N���[���`��
	pSwapChain->Present(0, 0);//��ʍX�V
}

Dx11Process::~Dx11Process(){

	for (int i = 0; i < 100; i++){
		if (pTexture[i] == NULL)continue;
		pTexture[i]->Release();
		pTexture[i] = NULL;
	}

	if (pRasterizeState != NULL){
		pRasterizeState->Release();
		pRasterizeState = NULL;
	}
	pBlendState->Release();
	pBlendState = NULL;
	if (pSampleLinear != NULL){
		pSampleLinear->Release();
		pSampleLinear = NULL;
	}
	pConstantBuffer->Release();
	pConstantBuffer = NULL;

	pHullShader_MESH_D->Release();
	pHullShader_MESH_D = NULL;
	pHullShader_DISPL->Release();
	pHullShader_DISPL = NULL;
	pHullShader_DISP->Release();
	pHullShader_DISP = NULL;

	pDomainShader_MESH_D->Release();
	pDomainShader_MESH_D = NULL;
	pDomainShader_DISPL->Release();
	pDomainShader_DISPL = NULL;
	pDomainShader_DISP->Release();
	pDomainShader_DISP = NULL;

	pPixelShader_MESH_D->Release();
	pPixelShader_MESH_D = NULL;
	pPixelShader_MESH->Release();
	pPixelShader_MESH = NULL;
	pPixelShader_DISPL->Release();
	pPixelShader_DISPL = NULL;
	pPixelShader_DISP->Release();
	pPixelShader_DISP = NULL;
	pPixelShader_TCL->Release();
	pPixelShader_TCL = NULL;
	pPixelShader_TC->Release();
	pPixelShader_TC = NULL;
	pPixelShader_BC->Release();
	pPixelShader_BC = NULL;
	pPixelShader_2D->Release();
	pPixelShader_2D = NULL;
	pPixelShader_2DTC->Release();
	pPixelShader_2DTC = NULL;

	pVertexShader_MESH_D->Release();
	pVertexShader_MESH_D = NULL;
	pVertexShader_MESH->Release();
	pVertexShader_MESH = NULL;
	pVertexShader_DISPL->Release();
	pVertexShader_DISPL = NULL;
	pVertexShader_DISP->Release();
	pVertexShader_DISP = NULL;
	pVertexShader_TCL->Release();
	pVertexShader_TCL = NULL;
	pVertexShader_TC->Release();
	pVertexShader_TC = NULL;
	pVertexShader_BC->Release();
	pVertexShader_BC = NULL;
	pVertexShader_2D->Release();
	pVertexShader_2D = NULL;
	pVertexShader_2DTC->Release();
	pVertexShader_2DTC = NULL;

	pVertexLayout_MESH_D->Release();
	pVertexLayout_MESH_D = NULL;
	pVertexLayout_MESH->Release();
	pVertexLayout_MESH = NULL;
	pVertexLayout_DISPL->Release();
	pVertexLayout_DISPL = NULL;
	pVertexLayout_DISP->Release();
	pVertexLayout_DISP = NULL;
	pVertexLayout_TCL->Release();
	pVertexLayout_TCL = NULL;
	pVertexLayout_TC->Release();
	pVertexLayout_TC = NULL;
	pVertexLayout_BC->Release();
	pVertexLayout_BC = NULL;
	pVertexLayout_2D->Release();
	pVertexLayout_2D = NULL;
	pVertexLayout_2DTC->Release();
	pVertexLayout_2DTC = NULL;

	pDSV->Release();
	pDSV = NULL;
	pDS->Release();
	pDS = NULL;
	pRTV->Release();
	pRTV = NULL;
	pSwapChain->Release();
	pSwapChain = NULL;
	pDeviceContext->Release();
	pDeviceContext = NULL;
	pDevice->Release();
	pDevice = NULL;
}

//�ړ��ʈ�艻
DWORD T_float::f = timeGetTime();
DWORD T_float::time = 0;

void T_float::GetTime(){
	time = timeGetTime() - f;
	f = timeGetTime();
}

float T_float::Add(float f){
	float r = ((float)time * f) / 2.0f;
	if (r <= 0.0f)return 0.01f;
	return r;
}