//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx11Process�N���X                                 **//
//**                                   Initialize�֐�                                    **//
//*****************************************************************************************//

#include "Dx11Process.h"
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
	if (FAILED(D3DX11CompileFromMemory(szFileName, size, NULL, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
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

HRESULT Dx11Process::Initialize(HWND hWnd){

	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRTV = NULL;
	pDS = NULL;
	pDSV = NULL;
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
	D3DXMatrixIdentity(&View);

	D3DXMatrixIdentity(&Proj);

	D3DXMatrixIdentity(&World);

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
	D3DXMatrixPerspectiveFovLH(&Proj,
		(float)(D3DXToRadian(55.0f)),	// �J�����̉�p
		aspect,				   // �A�X�y�N�g��
		1.0f,		          // near�v���[��
		10000.0f);			 // far�v���[��

	//3D���_�C���v�b�g���C�A�E�g���`, NORMAL�ɂ�POSITION��float�^4�o�C�g�~3���L�q
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	ID3DBlob *pCompiledShader = NULL;

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
	RSDesc.CullMode = D3D11_CULL_NONE;//�|���S�����ʕ`�惂�[�h(D3D11_CULL_BACK�ŕ\�̂ݕ`��)
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
	sadesc.MinLOD = -FLT_MAX;
	sadesc.MaxLOD = FLT_MAX;
	pDevice->CreateSamplerState(&sadesc, &pSampleLinear);

	pTexture = (ID3D11ShaderResourceView**)malloc(sizeof(ID3D11ShaderResourceView*) * 100);
	binary_ch = (char**)malloc(sizeof(char*) * 100);
	binary_size = (int*)malloc(sizeof(int) * 100);
	for (int i = 0; i < 100; i++){
		pTexture[i] = NULL;
		binary_ch[i] = NULL;
		binary_size[i] = 0;
	}

	//���C�g�\���̏�����
	for (int i = 0; i < LIGHT_PCS; i++){
		LightPos[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
		LightColor[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Lightst[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
	}
	SetLight_f = FALSE;
	ShadowLow_val = 0.0f;
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
	D3DXMatrixLookAtLH(&View,
		&D3DXVECTOR3(cax1, cay1, caz),   //�J�����̈ʒu
		&D3DXVECTOR3(cax2, cay2, caz),   //�J�����̕�����������_
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f)); //�J�����̏�̕���(�ʏ펋�_�ł̏������1.0f�ɂ���)
	//�V�F�[�_�[�v�Z�p���W�o�^
	cx = cax1;
	cy = cay1;
	cz = caz;
}

void Dx11Process::SetLight(bool f){
	SetLight_f = f;
}

void Dx11Process::ShadowBright(float val){
	ShadowLow_val = val;
}

void Dx11Process::LightPosSet(int Idx, float x, float y, float z, float r, float g, float b, float a, float range,
	float brightness, float attenuation, bool on_off){

	if (Idx > 49 || Idx < 0)return;//�G���[�h�~
	float onoff;
	if (on_off == TRUE)onoff = 1.0f; else onoff = 0.0f;
	LightPos[Idx] = D3DXVECTOR4(x, y, z, 1.0f);
	LightColor[Idx] = D3DXVECTOR4(r, g, b, a);
	Lightst[Idx] = D3DXVECTOR4(range, brightness, attenuation, onoff);
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
	TextureBinaryDecode("./dat/texture/map/EXIT1.da", 3);
	//�}�b�v1
	TextureBinaryDecode("./dat/texture/map/wall2.da", 4);
	TextureBinaryDecode("./dat/texture/map/ground2.da", 5);
	TextureBinaryDecode("./dat/texture/map/ceiling2.da", 6);
	TextureBinaryDecode("./dat/texture/map/EXIT2.da", 7);
	TextureBinaryDecode("./dat/texture/map/wall2-1.da", 8);
	TextureBinaryDecode("./dat/texture/map/ENTER2.da", 9);
	TextureBinaryDecode("./dat/texture/map/background.da", 10);
	//�}�b�v2
	TextureBinaryDecode("./dat/texture/map/ceiling3_wall3.da", 11);
	TextureBinaryDecode("./dat/texture/map/ground3.da", 12);
	TextureBinaryDecode("./dat/texture/map/ENTER3.da", 13);
	TextureBinaryDecode("./dat/texture/map/EXIT3.da", 14);
	//�}�b�v3
	TextureBinaryDecode("./dat/texture/map/ceiling4_ground4.da", 15);
	TextureBinaryDecode("./dat/texture/map/ENTER4.da", 16);
	//�}�b�v4
	TextureBinaryDecode("./dat/texture/map/wall5.da", 26);
	TextureBinaryDecode("./dat/texture/map/ground5.da", 27);
	TextureBinaryDecode("./dat/texture/map/ceiling5.da", 28);
	TextureBinaryDecode("./dat/texture/map/ENTER5.da", 29);
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

	//�ǂݍ��ރe�N�X�`�����
	D3DX11_IMAGE_LOAD_INFO info;
	info.Width = 0;
	info.Height = 0;
	info.Depth = 0;
	info.FirstMipLevel = 0;
	info.MipLevels = 1;
	info.Usage = D3D11_USAGE_DEFAULT;
	info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	info.CpuAccessFlags = 0;
	info.MiscFlags = 0;
	info.Format = DXGI_FORMAT_FROM_FILE;
	info.Filter = D3DX11_FILTER_LINEAR;
	info.MipFilter = D3DX11_FILTER_LINEAR;
	info.pSrcInfo = NULL;

	for (int i = 0; i < 100; i++){
		if (binary_size[i] == 0)continue;
		//�e�N�X�`���[�쐬
		D3DX11CreateShaderResourceViewFromMemory(pDevice, binary_ch[i], binary_size[i], &info, NULL, &pTexture[i], NULL);
	}
	for (int i = 0; i < 100; i++){
		if (binary_ch[i] == NULL)continue;
		free(binary_ch[i]);
		binary_ch[i] = NULL;
	}
	free(binary_ch);
	free(binary_size);
}

void Dx11Process::SetFog(bool on, float StartPos, float EndPos, DWORD r, DWORD g, DWORD b){


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

	pHullShader_DISPL->Release();
	pHullShader_DISPL = NULL;
	pHullShader_DISP->Release();
	pHullShader_DISP = NULL;

	pDomainShader_DISPL->Release();
	pDomainShader_DISPL = NULL;
	pDomainShader_DISP->Release();
	pDomainShader_DISP = NULL;

	pPixelShader_DISPL->Release();
	pPixelShader_DISPL = NULL;
	pPixelShader_DISP->Release();
	pPixelShader_DISP = NULL;
	pPixelShader_TC->Release();
	pPixelShader_TC = NULL;
	pPixelShader_BC->Release();
	pPixelShader_BC = NULL;
	pPixelShader_2D->Release();
	pPixelShader_2D = NULL;
	pPixelShader_2DTC->Release();
	pPixelShader_2DTC = NULL;

	pVertexShader_DISPL->Release();
	pVertexShader_DISPL = NULL;
	pVertexShader_DISP->Release();
	pVertexShader_DISP = NULL;
	pVertexShader_TC->Release();
	pVertexShader_TC = NULL;
	pVertexShader_BC->Release();
	pVertexShader_BC = NULL;
	pVertexShader_2D->Release();
	pVertexShader_2D = NULL;
	pVertexShader_2DTC->Release();
	pVertexShader_2DTC = NULL;

	pVertexLayout_DISPL->Release();
	pVertexLayout_DISPL = NULL;
	pVertexLayout_DISP->Release();
	pVertexLayout_DISP = NULL;
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