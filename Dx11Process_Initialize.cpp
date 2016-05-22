//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx11Process�N���X                                 **//
//**                                   Initialize�֐�                                    **//
//*****************************************************************************************//

#include "Dx11Process.h"
#include "ShaderParticle.h"
#include "ShaderMesh_D.h"
#include "ShaderMesh.h"
#include "ShaderDisp.h"
#include "Shader3D.h"
#include "Shader2D.h"
#include <string.h>
#include <stdio.h>

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
void Dx11Process::MakeShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob){
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DCompile(szFileName, size, NULL, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, ppBlob, &pErrors)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		throw p;
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
	RELEASE(pErrors);
}

void Dx11Process::MakeShaderGeometrySO(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob, D3D11_SO_DECLARATION_ENTRY *Decl, UINT Declsize){
	ID3DBlob *pErrors = NULL; 
	if (FAILED(D3DCompile(szFileName, size, NULL, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, ppBlob, &pErrors)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		throw p;
	}
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	UINT BufferStrides[] = { sizeof(ParticleData::PartPos) };
	HRESULT hr;
	hr = pDevice->CreateGeometryShaderWithStreamOutput(
		(*ppBlob)->GetBufferPointer(), //�o�C�g�E�R�[�h�ւ̃|�C���^
		(*ppBlob)->GetBufferSize(),   //�o�C�g�E�R�[�h�̒���
		Decl,                        //�o�͂���f�[�^��`
		Declsize,                //�o�͂���1�f�[�^�̗v�f��
		BufferStrides,          //�o�͂���1�f�[�^�̃T�C�Y
		_countof(BufferStrides),
		0,                        //�@�\���x�����u11.0�v���Ⴂ�ꍇ
		NULL,
		(ID3D11GeometryShader**)ppShader);//�쐬���ꂽ�W�I���g���E�V�F�[�_���󂯎��ϐ�
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

void Dx11Process::MatrixMap(ID3D11Buffer *pCBuffer, float x, float y, float z, float r, float g, float b, float thetaZ, float thetaY, float thetaX, float size, float disp){

	MATRIX mov;
	MATRIX rotZ, rotY, rotX, rotZY, rotZYX;
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
	MatrixRotationZ(&rotZ, thetaZ);
	MatrixRotationY(&rotY, thetaY);
	MatrixRotationX(&rotX, thetaX);
	MatrixMultiply(&rotZY, &rotZ, &rotY);
	MatrixMultiply(&rotZYX, &rotZY, &rotX);
	MatrixTranslation(&mov, x, y, z);
	MatrixMultiply(&scro, &rotZYX, &scale);
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

void Dx11Process::Initialize(HWND hWnd){
	
	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRTV = NULL;
	pDS = NULL;
	pDSV = NULL;
	pGeometryShader_PSO = NULL;
	pVertexShader_PSO = NULL;
	pGeometryShader_P = NULL;
	pVertexLayout_P = NULL;
	pVertexShader_P = NULL;
	pPixelShader_P = NULL;
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

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &pFeatureLevels
		, 1, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, pFeatureLevel, &pDeviceContext)))throw "D3D11CreateDeviceAndSwapChain�G���[";

	//�o�b�N�o�b�t�@�[�̃����_�[�^�[�Q�b�g�r���[(RTV)���쐬
	ID3D11Texture2D *pBack;
	if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack)))throw "pSwapChain->GetBuffer�G���[";

	if (FAILED(pDevice->CreateRenderTargetView(pBack, NULL, &pRTV))){
		RELEASE(pBack);
		throw "CreateRenderTargetView�G���[";
	}
	RELEASE(pBack);

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
	if (FAILED(pDevice->CreateTexture2D(&descDepth, NULL, &pDS)))throw "pDS�G���[";

	if (FAILED(pDevice->CreateDepthStencilView(pDS, NULL, &pDSV)))throw "pDSV�G���[";

	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���Z�b�g
	pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);

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
	//�J������p
	ViewY_theta = 55.0f;
	// �A�X�y�N�g��̌v�Z
	aspect = (float)vp.Width / (float)vp.Height;
	//near�v���[��
	NearPlane = 1.0f;
	//far�v���[��
	FarPlane = 10000.0f;
	//�ˉe�}�g���b�N�X�쐬
	MatrixPerspectiveFovLH(&Proj,
		ViewY_theta,
		aspect,
		NearPlane,
		FarPlane);
	//�r���[�|�[�g�s��쐬(3D���W��2D���W�ϊ��Ɏg�p)
	MatrixViewPort(&Vp);

	//�X�g���[���o�̓f�[�^��`(�p�[�e�B�N���p)
	D3D11_SO_DECLARATION_ENTRY Decl[] =
	{
		{ 0, "POSITION", 0, 0, 3, 0 }, //�ux,y,z�v���X���b�g�u0�v�́uPOSITION�v�ɏo��
		{ 0, "POSITION", 1, 0, 3, 0 },
		{ 0, "POSITION", 2, 0, 3, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 }
	};
	ID3DBlob *pCompiledShader = NULL;
	UINT Declsize = sizeof(Decl) / sizeof(Decl[0]);
	//**********************************�W�I���g���V�F�[�_�[***********************************************************//
	MakeShaderGeometrySO(ShaderParticle, strlen(ShaderParticle), "GS_Point_SO", "gs_5_0", (void**)&pGeometryShader_PSO, &pCompiledShader, Decl, Declsize);
	RELEASE(pCompiledShader);

	//**********************************���_�V�F�[�_�[***************************************************************//
	MakeShader(ShaderParticle, strlen(ShaderParticle), "VS_SO", "vs_5_0", (void**)&pVertexShader_PSO, &pCompiledShader);
	RELEASE(pCompiledShader);

	//�p�[�e�B�N�����_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout_P[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout_P) / sizeof(layout_P[0]);
	pCompiledShader = NULL;

	//**********************************�W�I���g���V�F�[�_�[***********************************************************//
	MakeShader(ShaderParticle, strlen(ShaderParticle), "GS_Point", "gs_5_0", (void**)&pGeometryShader_P, &pCompiledShader);
	RELEASE(pCompiledShader);

	//**********************************���_�V�F�[�_�[***************************************************************//
	MakeShader(ShaderParticle, strlen(ShaderParticle), "VS", "vs_5_0", (void**)&pVertexShader_P, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout_P, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_P);
	RELEASE(pCompiledShader);

	//**********************************�s�N�Z���V�F�[�_�[***********************************************************//
	MakeShader(ShaderParticle, strlen(ShaderParticle), "PS", "ps_5_0", (void**)&pPixelShader_P, &pCompiledShader);
	RELEASE(pCompiledShader);

	//���b�V�����_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout_MESH[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(layout_MESH) / sizeof(layout_MESH[0]);
	pCompiledShader = NULL;

	//**********************************�n���V�F�[�_�[***************************************************************//
	MakeShader(ShaderMesh_D, strlen(ShaderMesh_D), "HSMesh", "hs_5_0", (void**)&pHullShader_MESH_D, &pCompiledShader);
	RELEASE(pCompiledShader);
	//**********************************�n���V�F�[�_�[***************************************************************//

	//**********************************�h���C���V�F�[�_�[***********************************************************//
	MakeShader(ShaderMesh_D, strlen(ShaderMesh_D), "DSMesh", "ds_5_0", (void**)&pDomainShader_MESH_D, &pCompiledShader);
	RELEASE(pCompiledShader);
	//**********************************�h���C���V�F�[�_�[***********************************************************//

	//**********************************���_�V�F�[�_�[***************************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O�L
	MakeShader(ShaderMesh_D, strlen(ShaderMesh_D), "VSMesh", "vs_5_0", (void**)&pVertexShader_MESH_D, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout_MESH, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_MESH_D);
	RELEASE(pCompiledShader);

	//�f�B�X�v���C�g�����g�}�b�s���O��
	MakeShader(ShaderMesh, strlen(ShaderMesh), "VSMesh", "vs_5_0", (void**)&pVertexShader_MESH, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout_MESH, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_MESH);
	RELEASE(pCompiledShader);
	//**********************************���_�V�F�[�_�[***************************************************************//

	//**********************************�s�N�Z���V�F�[�_�[***********************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O�L
	MakeShader(ShaderMesh_D, strlen(ShaderMesh_D), "PSMesh", "ps_5_0", (void**)&pPixelShader_MESH_D, &pCompiledShader);
	RELEASE(pCompiledShader);

	//�f�B�X�v���C�g�����g�}�b�s���O��
	MakeShader(ShaderMesh, strlen(ShaderMesh), "PSMesh", "ps_5_0", (void**)&pPixelShader_MESH, &pCompiledShader);
	RELEASE(pCompiledShader);
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
	RELEASE(pCompiledShader);

	//�f�B�X�v���C�g�����g�}�b�s���O �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "HSDisp", "hs_5_0", (void**)&pHullShader_DISP, &pCompiledShader);
	RELEASE(pCompiledShader);
	//**********************************�n���V�F�[�_�[***************************************************************//

	//**********************************�h���C���V�F�[�_�[***********************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O, ���C�g�L �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "DSDispL", "ds_5_0", (void**)&pDomainShader_DISPL, &pCompiledShader);
	RELEASE(pCompiledShader);

	//�f�B�X�v���C�g�����g�}�b�s���O �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "DSDisp", "ds_5_0", (void**)&pDomainShader_DISP, &pCompiledShader);
	RELEASE(pCompiledShader);
	//**********************************�h���C���V�F�[�_�[***********************************************************//

	//**********************************���_�V�F�[�_�[***************************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O, ���C�g�L �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "VSDispL", "vs_5_0", (void**)&pVertexShader_DISPL, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_DISPL);
	RELEASE(pCompiledShader);

	//�f�B�X�v���C�g�����g�}�b�s���O �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "VSDisp", "vs_5_0", (void**)&pVertexShader_DISP, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_DISP);
	RELEASE(pCompiledShader);

	//�e�N�X�`���[,���C�g�L �쐬
	MakeShader(Shader3D, strlen(Shader3D), "VSTextureColorL", "vs_5_0", (void**)&pVertexShader_TCL, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_TCL);
	RELEASE(pCompiledShader);

	//�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader3D, strlen(Shader3D), "VSTextureColor", "vs_5_0", (void**)&pVertexShader_TC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_TC);
	RELEASE(pCompiledShader);

	//��{�F �쐬
	MakeShader(Shader3D, strlen(Shader3D), "VSBaseColor", "vs_5_0", (void**)&pVertexShader_BC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_BC);
	RELEASE(pCompiledShader);
	//**********************************���_�V�F�[�_�[***************************************************************//

	//**********************************�s�N�Z���V�F�[�_�[***********************************************************//
	//�f�B�X�v���C�g�����g�}�b�s���O, ���C�g�L �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "PSDispL", "ps_5_0", (void**)&pPixelShader_DISPL, &pCompiledShader);
	RELEASE(pCompiledShader);

	//�f�B�X�v���C�g�����g�}�b�s���O �쐬
	MakeShader(ShaderDisp, strlen(ShaderDisp), "PSDisp", "ps_5_0", (void**)&pPixelShader_DISP, &pCompiledShader);
	RELEASE(pCompiledShader);

	//�e�N�X�`���[,���C�g�L �쐬
	MakeShader(Shader3D, strlen(Shader3D), "PSTextureColorL", "ps_5_0", (void**)&pPixelShader_TCL, &pCompiledShader);
	RELEASE(pCompiledShader);

	//�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader3D, strlen(Shader3D), "PSTextureColor", "ps_5_0", (void**)&pPixelShader_TC, &pCompiledShader);
	RELEASE(pCompiledShader);

	//��{�F �쐬
	MakeShader(Shader3D, strlen(Shader3D), "PSBaseColor", "ps_5_0", (void**)&pPixelShader_BC, &pCompiledShader);
	RELEASE(pCompiledShader);
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
	RELEASE(pCompiledShader);

	//�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader2D, strlen(Shader2D), "VSTextureColor", "vs_5_0", (void**)&pVertexShader_2DTC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout2D, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_2DTC);
	RELEASE(pCompiledShader);
	//**********************************���_�V�F�[�_�[2D*************************************************************//

	//**********************************�s�N�Z���V�F�[�_�[2D*********************************************************//
	//��{�F �쐬
	MakeShader(Shader2D, strlen(Shader2D), "PSBaseColor", "ps_5_0", (void**)&pPixelShader_2D, &pCompiledShader);
	RELEASE(pCompiledShader);

	//�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader2D, strlen(Shader2D), "PSTextureColor", "ps_5_0", (void**)&pPixelShader_2DTC, &pCompiledShader);
	RELEASE(pCompiledShader);
	//**********************************�s�N�Z���V�F�[�_�[2D*********************************************************//

	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer)))throw "pConstantBuffer�G���[";

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
	if (FAILED(pDevice->CreateBlendState(&bld, &pBlendState)))throw "pBlendState�G���[";
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
	if (FAILED(pDevice->CreateRasterizerState(&RSDesc, &pRasterizeState)))throw "pRasterizeState�G���[";
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
	if (FAILED(pDevice->CreateSamplerState(&sadesc, &pSampleLinear)))throw "pSampleLinear�G���[";

	pTexture = (ID3D11ShaderResourceView**)malloc(sizeof(ID3D11ShaderResourceView*) * TEX_PCS);
	pTexCPUAcc = (ID3D11Texture2D**)malloc(sizeof(ID3D11Texture2D*) * TEX_PCS);
	binary_ch = (char**)malloc(sizeof(char*) * TEX_PCS);
	binary_size = (int*)malloc(sizeof(int) * TEX_PCS);
	for (int i = 0; i < TEX_PCS; i++){
		pTexture[i] = NULL;
		pTexCPUAcc[i] = NULL;
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
}

void Dx11Process::Sclear(){//�X�N���[���N���A

	//��ʃN���A
	float ClearColor[4] = { 0, 0, 0, 1 };// �N���A�F�쐬�@RGBA�̏�
	pDeviceContext->ClearRenderTargetView(pRTV, ClearColor);//�J���[�o�b�t�@�N���A
	pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�f�v�X�X�e���V���o�b�t�@�N���A
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

	static int pcs = LIGHT_PCS_init;//�������l(�\��ς�)

	if (Idx > LIGHT_PCS - 1 || Idx < 0)return;//�G���[�h�~

	if (Idx > LIGHT_PCS_init - 1 && on_off == TRUE)pcs = Idx + 1;

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

	Bdecode(Bpass, &binary_ch[i], &binary_size[i]);
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
	TextureBinaryDecode("./dat/texture/map/leaf.da", 9);
	TextureBinaryDecode("./dat/texture/map/wood.da", 10);
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
	//�v���C���[1�e�N�X�`��
	TextureBinaryDecode("./dat/texture/player/p1/brown_eye.da", 90);
	TextureBinaryDecode("./dat/texture/player/p1/classicshoes_texture_diffuse.da", 91);
	TextureBinaryDecode("./dat/texture/player/p1/eyebrow001.da", 92);
	TextureBinaryDecode("./dat/texture/player/p1/jacket01_diffuse.da", 93);
	TextureBinaryDecode("./dat/texture/player/p1/jeans01_black_diffuse.da", 94);
	TextureBinaryDecode("./dat/texture/player/p1/male01_diffuse_black.da", 95);
	TextureBinaryDecode("./dat/texture/player/p1/young_lightskinned_male_diffuse.da", 96);
	//�v���C���[2�e�N�X�`��
	TextureBinaryDecode("./dat/texture/player/p2/brown_eye.da", 100);
	TextureBinaryDecode("./dat/texture/player/p2/diffuse_black.da", 101);
	TextureBinaryDecode("./dat/texture/player/p2/eyebrow006.da", 102);
	TextureBinaryDecode("./dat/texture/player/p2/eyelashes03.da", 103);
	TextureBinaryDecode("./dat/texture/player/p2/shoes02_default.da", 104);
	TextureBinaryDecode("./dat/texture/player/p2/short01_black_diffuse.da", 105);
	TextureBinaryDecode("./dat/texture/player/p2/tshirt02_texture.da", 106);
	TextureBinaryDecode("./dat/texture/player/p2/young_lightskinned_female_diffuse.da", 107);
	//�v���C���[3�e�N�X�`��
	TextureBinaryDecode("./dat/texture/player/p3/brown_eye.da", 110);
	TextureBinaryDecode("./dat/texture/player/p3/classicshoes_texture_diffuse.da", 111);
	TextureBinaryDecode("./dat/texture/player/p3/jeans_basic_diffuse.da", 112);
	TextureBinaryDecode("./dat/texture/player/p3/young_darkskinned_male_diffuse.da", 113);
	//�v���C���[4�e�N�X�`��
	TextureBinaryDecode("./dat/texture/player/p4/afro.da", 120);
	TextureBinaryDecode("./dat/texture/player/p4/brown_eye.da", 121);
	TextureBinaryDecode("./dat/texture/player/p4/eyebrow007.da", 122);
	TextureBinaryDecode("./dat/texture/player/p4/eyelashes03.da", 123);
	TextureBinaryDecode("./dat/texture/player/p4/shoes02_default.da", 124);
	TextureBinaryDecode("./dat/texture/player/p4/short01_black_diffuse.da", 125);
	TextureBinaryDecode("./dat/texture/player/p4/tshirt_texture_blue.da", 126);
	TextureBinaryDecode("./dat/texture/player/p4/young_darkskinned_female_diffuse.da", 127);
}

void Dx11Process::GetTexture(){
	ID3D11Resource *t;
	char str[50];
	for (int i = 0; i < TEX_PCS; i++){
		if (binary_size[i] == 0)continue;
		//CPU�A�N�Z�X�e�N�X�`���[�쐬(�s�N�Z���擾�p)
		if (FAILED(DirectX::CreateWICTextureFromMemoryEx(pDevice, (uint8_t*)binary_ch[i], binary_size[i], binary_size[i],
			D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_READ, 0, TRUE, &t, NULL))){
			sprintf(str, "CPU�A�N�Z�X�e�N�X�`����%d�ǂݍ��݃G���[", i);
			throw str;
		}
		pTexCPUAcc[i] = (ID3D11Texture2D*)t;
		//CPU�A�N�Z�X�s�e�N�X�`���[�쐬
		if (FAILED(DirectX::CreateWICTextureFromMemoryEx(pDevice, (uint8_t*)binary_ch[i], binary_size[i], binary_size[i],
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, TRUE, NULL, &pTexture[i]))){
			sprintf(str, "CPU�A�N�Z�X�s�e�N�X�`����%d�ǂݍ��݃G���[", i);
			throw str;
		}
	}
	for (int i = 0; i < TEX_PCS; i++){
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

float Dx11Process::GetViewY_theta(){
	return ViewY_theta;
}

float Dx11Process::Getaspect(){
	return aspect;
}

float Dx11Process::GetNearPlane(){
	return NearPlane;
}

float Dx11Process::GetFarPlane(){
	return FarPlane;
}

Dx11Process::~Dx11Process(){

	for (int i = 0; i < TEX_PCS; i++){
		RELEASE(pTexture[i]);
		RELEASE(pTexCPUAcc[i]);
	}

	RELEASE(pRasterizeState);
	RELEASE(pBlendState);
	RELEASE(pSampleLinear);
	RELEASE(pConstantBuffer);
	RELEASE(pGeometryShader_PSO);
	RELEASE(pGeometryShader_P);

	RELEASE(pHullShader_MESH_D);
	RELEASE(pHullShader_DISPL);
	RELEASE(pHullShader_DISP);

	RELEASE(pDomainShader_MESH_D);
	RELEASE(pDomainShader_DISPL);
	RELEASE(pDomainShader_DISP);

	RELEASE(pPixelShader_P);
	RELEASE(pPixelShader_MESH_D);
	RELEASE(pPixelShader_MESH);
	RELEASE(pPixelShader_DISPL);
	RELEASE(pPixelShader_DISP);
	RELEASE(pPixelShader_TCL);
	RELEASE(pPixelShader_TC);
	RELEASE(pPixelShader_BC);
	RELEASE(pPixelShader_2D);
	RELEASE(pPixelShader_2DTC);

	RELEASE(pVertexShader_PSO);
	RELEASE(pVertexShader_P);
	RELEASE(pVertexShader_MESH_D);
	RELEASE(pVertexShader_MESH);
	RELEASE(pVertexShader_DISPL);
	RELEASE(pVertexShader_DISP);
	RELEASE(pVertexShader_TCL);
	RELEASE(pVertexShader_TC);
	RELEASE(pVertexShader_BC);
	RELEASE(pVertexShader_2D);
	RELEASE(pVertexShader_2DTC);

	RELEASE(pVertexLayout_P);
	RELEASE(pVertexLayout_MESH_D);
	RELEASE(pVertexLayout_MESH);
	RELEASE(pVertexLayout_DISPL);
	RELEASE(pVertexLayout_DISP);
	RELEASE(pVertexLayout_TCL);
	RELEASE(pVertexLayout_TC);
	RELEASE(pVertexLayout_BC);
	RELEASE(pVertexLayout_2D);
	RELEASE(pVertexLayout_2DTC);

	RELEASE(pDSV);
	RELEASE(pDS);
	RELEASE(pRTV);
	RELEASE(pSwapChain);
	RELEASE(pDeviceContext);
	RELEASE(pDevice);
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

//�G���[���b�Z�[�W
void ErrorMessage(char *E_mes){
	MessageBoxA(0, E_mes, 0, MB_OK);
}