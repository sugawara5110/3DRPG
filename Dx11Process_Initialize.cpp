//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx11Process�N���X                                 **//
//**                                   Initialize�֐�                                    **//
//*****************************************************************************************//

#include "Dx11Process.h"
#include "Shader.h"
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
	return S_OK;
}

HRESULT Dx11Process::Initialize(HWND hWnd){

	pDevice = NULL;
	pDeviceContext = NULL;
	pSwapChain = NULL;
	pRTV = NULL;
	pDS = NULL;
	pDSV = NULL;
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

	//�o�[�e�b�N�X�V�F�[�_�[�e�N�X�`���[,���C�g�L �쐬
	MakeShader(Shader, strlen(Shader), "VSTextureColorLight", "vs_5_0", (void**)&pVertexShader_TCL, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_TCL);

	//�o�[�e�b�N�X�V�F�[�_�[�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader, strlen(Shader), "VSTextureColor", "vs_5_0", (void**)&pVertexShader_TC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_TC);

	//�o�[�e�b�N�X�V�F�[�_�[��{�F �쐬
	MakeShader(Shader, strlen(Shader), "VSBaseColor", "vs_5_0", (void**)&pVertexShader_BC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_BC);

	//�s�N�Z���V�F�[�_�[�e�N�X�`���[,���C�g�L �쐬
	MakeShader(Shader, strlen(Shader), "PSTextureColorLight", "ps_5_0", (void**)&pPixelShader_TCL, &pCompiledShader);

	//�s�N�Z���V�F�[�_�[�e�N�X�`���[,���C�g�� �쐬
	MakeShader(Shader, strlen(Shader), "PSTextureColor", "ps_5_0", (void**)&pPixelShader_TC, &pCompiledShader);

	//�s�N�Z���V�F�[�_�[��{�F �쐬
	MakeShader(Shader, strlen(Shader), "PSBaseColor", "ps_5_0", (void**)&pPixelShader_BC, &pCompiledShader);

	pCompiledShader->Release();
	pCompiledShader = NULL;

	//2D���_�C���v�b�g���C�A�E�g��`
	D3D11_INPUT_ELEMENT_DESC layout2D[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 4, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(layout2D) / sizeof(layout2D[0]);
	pCompiledShader = NULL;

	//2D�o�[�e�b�N�X�V�F�[�_�[��{�F �쐬
	MakeShader(Shader2D, strlen(Shader2D), "VSBaseColor", "vs_5_0", (void**)&pVertexShader_2D, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout2D, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_2D);

	//2D�o�[�e�b�N�X�V�F�[�_�[ �쐬
	MakeShader(Shader2D, strlen(Shader2D), "VSTextureColor", "vs_5_0", (void**)&pVertexShader_2DTC, &pCompiledShader);
	//���_�C���v�b�g���C�A�E�g���쐬
	pDevice->CreateInputLayout(layout2D, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &pVertexLayout_2DTC);

	//2D�s�N�Z���V�F�[�_�[��{�F �쐬
	MakeShader(Shader2D, strlen(Shader2D), "PSBaseColor", "ps_5_0", (void**)&pPixelShader_2D, &pCompiledShader);

	//2D�s�N�Z���V�F�[�_�[ �쐬
	MakeShader(Shader2D, strlen(Shader2D), "PSTextureColor", "ps_5_0", (void**)&pPixelShader_2DTC, &pCompiledShader);

	pCompiledShader->Release();
	pCompiledShader = NULL;

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

	pTexture = (ID3D11ShaderResourceView**)malloc(sizeof(ID3D11ShaderResourceView*) * 100);
	binary_ch = (char**)malloc(sizeof(char*) * 100);
	binary_size = (int*)malloc(sizeof(int) * 100);
	for (int i = 0; i < 100; i++){
		pTexture[i] = NULL;
		binary_ch[i] = NULL;
		binary_size[i] = 0;
	}

	//���C�g�\���̏�����
	for (int i = 0; i < 10; i++){
		LightPos[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
		LightColor[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Lightst[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return S_OK;
}

HRESULT Dx11Process::GetVBarray(PrimitiveType type, PolygonData *pd, int pieces){//���_�o�b�t�@,�z��m��

	int ver;
	int verI;
	if (type == SQUARE){
		ver = pieces * 4;//pieces==�l�p�`�̌�
		verI = pieces * 6;
	}
	if (type == POINt){
		ver = pieces;//pieces==�_�̌�
		verI = pieces;
	}
	if (type == LINE_S || type == LINE_L){
		ver = pieces * 2;//pieces==���̌�
		verI = pieces * 2;
	}

	//���_�o�b�t�@�擾
	pd->bd.Usage = D3D11_USAGE_DYNAMIC;
	pd->bd.ByteWidth = sizeof(MY_VERTEX) * ver;
	pd->bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	pd->bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd->bd.MiscFlags = 0;
	pd->bd.StructureByteStride = 0;
	//���_�z��擾
	pd->d3varray = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * ver);
	//���_�o�b�t�@�ݒ�
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pd->d3varray;
	pDevice->CreateBuffer(&pd->bd, &InitData, &pd->pMyVB);

	//�C���f�b�N�X�o�b�t�@�擾
	pd->bdI.Usage = D3D11_USAGE_DYNAMIC;
	pd->bdI.ByteWidth = sizeof(UINT) * verI;
	pd->bdI.BindFlags = D3D11_BIND_INDEX_BUFFER;
	pd->bdI.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd->bdI.MiscFlags = 0;
	pd->bdI.StructureByteStride = 0;
	//�C���f�b�N�X�z��擾
	pd->d3varrayI = (UINT*)malloc(sizeof(UINT) * verI);
	//�C���f�b�N�X�o�b�t�@�ݒ�
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = pd->d3varrayI;
	pDevice->CreateBuffer(&pd->bdI, &IndexData, &pd->pMyVBI);

	return S_OK;
}

HRESULT Dx11Process::GetVBarray2D(PolygonData2D *p2, int pieces){

	int ver;
	ver = pieces * 4;//pieces==�l�p�`�̌�

	//���_�o�b�t�@�擾
	p2->bd.Usage = D3D11_USAGE_DYNAMIC;
	p2->bd.ByteWidth = sizeof(MY_VERTEX2) * ver;
	p2->bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	p2->bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	p2->bd.MiscFlags = 0;
	p2->bd.StructureByteStride = 0;
	//���_�z��擾
	p2->d3varray = (MY_VERTEX2*)malloc(sizeof(MY_VERTEX2) * ver);
	//���_�o�b�t�@�ݒ�
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = p2->d3varray;
	pDevice->CreateBuffer(&p2->bd, &InitData, &p2->pMyVB);

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

	// �J�����̈ʒu�ƕ�����ݒ�
	D3DXMatrixLookAtLH(&View,
		&D3DXVECTOR3(cax1, cay1, caz),   //�J�����̈ʒu
		&D3DXVECTOR3(cax2, cay2, caz),   //�J�����̕�����������_
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f)); //�J�����̏�̕���(�ʏ펋�_�ł̏������1.0f�ɂ���)
}

void Dx11Process::LightPosSet(int Idx, float x, float y, float z, float r, float g, float b, float a, float range, float brightness, float attenuation, bool on_off){

	if (Idx > 9 || Idx < 0)return;//�G���[�h�~
	float onoff;
	if (on_off == TRUE)onoff = 1.0f; else onoff = 0.0f;
	LightPos[Idx] = D3DXVECTOR4(x, y, z, 1.0f);
	LightColor[Idx] = D3DXVECTOR4(r, g, b, a);
	Lightst[Idx] = D3DXVECTOR4(range, brightness, attenuation, onoff);
};

void Dx11Process::TextureInit(PolygonData *pd, int width, int height){

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
	pDevice->CreateTexture2D(&desc, NULL, &pd->pTex);

	//�V�F�[�_���\�[�X�r���[�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srdesc;
	srdesc.Format = desc.Format;
	srdesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srdesc.Texture2D.MostDetailedMip = 0;
	srdesc.Texture2D.MipLevels = 1;
	pDevice->CreateShaderResourceView(pd->pTex, &srdesc, &pd->pTexview);
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

void Dx11Process::GetTexture(PolygonData *pd, int no){

	pd->load_tex_no[pd->tex_no] = no;
}

void Dx11Process::SetTextureMPixel(PolygonData *pd, int **m_pix, BYTE r, BYTE g, BYTE b, int a){

	D3D11_TEXTURE2D_DESC desc2D;
	pd->pTex->GetDesc(&desc2D);

	//�e�N�X�`���̉��T�C�Y�擾
	int width = desc2D.Width;
	//�e�N�X�`���̏c�T�C�Y�擾
	int height = desc2D.Height;

	//���\�[�X�̃}�b�v
	D3D11_MAPPED_SUBRESOURCE pData;
	pDeviceContext->Map(pd->pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	UCHAR *pTex = (UCHAR*)pData.pData;

	for (int j = 0; j < height; j++){
		UINT j1 = j * pData.RowPitch;
		for (int i = 0; i < width; i++){
			UINT pTexI = i * 4 + j1;
			pTex[pTexI + 0] = m_pix[j][i] >> 16 & r;
			pTex[pTexI + 1] = m_pix[j][i] >> 8 & g;
			pTex[pTexI + 2] = m_pix[j][i] & b;

			if ((m_pix[j][i] >> 16 & b) < 20 && (m_pix[j][i] >> 8 & g) < 20 && (m_pix[j][i] & r) < 20){
				pTex[pTexI + 3] = 0;
			}
			else{
				pTex[pTexI + 3] = a;
			}
		}
	}
	pDeviceContext->Unmap(pd->pTex, 0);
}

void Dx11Process::SetFog(bool on, float StartPos, float EndPos, DWORD r, DWORD g, DWORD b){


}

void Dx11Process::D3primitive(PrimitiveType type, PolygonData *pd, int pieces, float x, float y, float z, float r, bool a, bool lock, bool light_f){

	D3D11_PRIMITIVE_TOPOLOGY topology;
	D3DXMATRIX mov;
	D3DXMATRIX rot;

	int ver;
	int verI;
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

	//�A���t�@�u�����h,�A���t�@�e�X�g�؂�ւ�
	bld.AlphaToCoverageEnable = a;
	bld.RenderTarget[0].BlendEnable = a;
	pDevice->CreateBlendState(&bld, &pBlendState);
	UINT mask = 0xffffffff;
	pDeviceContext->OMSetBlendState(pBlendState, NULL, mask);

	//�g�p����V�F�[�_�[�̃Z�b�g
	if ((pd->load_tex_no[0] != -1 || pd->pTexview != NULL) && light_f == TRUE){
		pDeviceContext->VSSetShader(pVertexShader_TCL, NULL, 0);
		pDeviceContext->PSSetShader(pPixelShader_TCL, NULL, 0);
	}
	if ((pd->load_tex_no[0] != -1 || pd->pTexview != NULL) && light_f == FALSE){
		pDeviceContext->VSSetShader(pVertexShader_TC, NULL, 0);
		pDeviceContext->PSSetShader(pPixelShader_TC, NULL, 0);
	}
	if (pd->load_tex_no[0] == -1 && pd->pTexview == NULL){
		pDeviceContext->VSSetShader(pVertexShader_BC, NULL, 0);
		pDeviceContext->PSSetShader(pPixelShader_BC, NULL, 0);
	}

	D3D11_MAPPED_SUBRESOURCE pData, pData1, pData2;
	//�C���f�b�N�X�z�񏑂������̏ꍇ����(��{�I�ɍŏ��̈��̂�)
	if (pd->lockI == FALSE){
		pDeviceContext->Map(pd->pMyVBI, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		memcpy_s(pData.pData, pData.RowPitch, (void*)pd->d3varrayI, sizeof(UINT) * verI);
		pDeviceContext->Unmap(pd->pMyVBI, 0);
		pd->lockI = TRUE;
	}

	//���_�z�񏑂������̏ꍇ����(�ŏ���1��͕K�����s�����,���̌�͔C��)
	if (pd->lock == FALSE || lock == TRUE){
		pDeviceContext->Map(pd->pMyVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData1);
		memcpy_s(pData1.pData, pData1.RowPitch, (void*)pd->d3varray, sizeof(MY_VERTEX) * ver);
		pDeviceContext->Unmap(pd->pMyVB, 0);
		pd->lock = TRUE;
	}

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	CONSTANT_BUFFER cb;
	pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData2);

	//�\���ʒu
	D3DXMatrixRotationZ(&rot, (FLOAT)D3DXToRadian(r));
	D3DXMatrixTranslation(&mov, x, y, z);
	D3DXMatrixMultiply(&World, &rot, &mov);

	//���[���h�A�J�����A�ˉe�s���n��
	cb.World = World;
	cb.View = View;
	cb.Proj = Proj;
	memcpy(cb.LightPos, LightPos, sizeof(D3DXVECTOR4) * 10);
	memcpy(cb.LightColor, LightColor, sizeof(D3DXVECTOR4) * 10);
	memcpy(cb.Lightst, Lightst, sizeof(D3DXVECTOR4) * 10);
	D3DXMatrixTranspose(&cb.World, &cb.World);
	D3DXMatrixTranspose(&cb.View, &cb.View);
	D3DXMatrixTranspose(&cb.Proj, &cb.Proj);
	memcpy_s(pData2.pData, pData2.RowPitch, (void*)(&cb), sizeof(cb));
	pDeviceContext->Unmap(pConstantBuffer, 0);

	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	if (pd->load_tex_no[0] != -1){
		pDeviceContext->PSSetSamplers(0, 1, &pSampleLinear);
		pDeviceContext->PSSetShaderResources(0, 1, &pTexture[pd->load_tex_no[pd->tex_no]]);
	}
	if (pd->pTexview != NULL){
		pDeviceContext->PSSetSamplers(0, 1, &pSampleLinear);
		pDeviceContext->PSSetShaderResources(0, 1, &pd->pTexview);
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pd->pMyVB, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	pDeviceContext->IASetIndexBuffer(pd->pMyVBI, DXGI_FORMAT_R32_UINT, 0);

	//�g�p���钸�_�C���v�b�g���C�A�E�g���Z�b�g
	if ((pd->load_tex_no[0] != -1 || pd->pTexview != NULL) && light_f == TRUE){
		pDeviceContext->IASetInputLayout(pVertexLayout_TCL);
	}
	if ((pd->load_tex_no[0] != -1 || pd->pTexview != NULL) && light_f == FALSE){
		pDeviceContext->IASetInputLayout(pVertexLayout_TC);
	}
	if (pd->load_tex_no[0] == -1 && pd->pTexview == NULL){
		pDeviceContext->IASetInputLayout(pVertexLayout_BC);
	}

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(topology);
	//�v���~�e�B�u�������_�����O
	pDeviceContext->DrawIndexed(verI, 0, 0);
}

void Dx11Process::D2primitive(PolygonData2D *p2, int piece, bool lock){//2D�`��

	int ver = piece * 4;
	bool a = FALSE;

	//�A���t�@�u�����h,�A���t�@�e�X�g�؂�ւ�
	if (p2->pTexview != NULL)a = TRUE;
	bld.AlphaToCoverageEnable = a;
	bld.RenderTarget[0].BlendEnable = a;
	pDevice->CreateBlendState(&bld, &pBlendState);
	UINT mask = 0xffffffff;
	pDeviceContext->OMSetBlendState(pBlendState, NULL, mask);

	//�g�p����V�F�[�_�[�̃Z�b�g
	if (p2->pTexview == NULL){
		pDeviceContext->VSSetShader(pVertexShader_2D, NULL, 0);
		pDeviceContext->PSSetShader(pPixelShader_2D, NULL, 0);
	}
	else{
		pDeviceContext->VSSetShader(pVertexShader_2DTC, NULL, 0);
		pDeviceContext->PSSetShader(pPixelShader_2DTC, NULL, 0);
	}

	//���_�z�񏑂������̏ꍇ����(�ŏ���1��͕K�����s�����,���̌�͔C��)
	D3D11_MAPPED_SUBRESOURCE pData;
	if (p2->lock == FALSE || lock == TRUE){
		pDeviceContext->Map(p2->pMyVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		memcpy_s(pData.pData, pData.RowPitch, (void*)p2->d3varray, sizeof(MY_VERTEX2) * ver);
		pDeviceContext->Unmap(p2->pMyVB, 0);
		p2->lock = TRUE;
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(MY_VERTEX2);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &p2->pMyVB, &stride, &offset);

	//�g�p���钸�_�C���v�b�g���C�A�E�g���Z�b�g
	if (p2->pTexview == NULL){
		pDeviceContext->IASetInputLayout(pVertexLayout_2D);
	}
	else{
		pDeviceContext->IASetInputLayout(pVertexLayout_2DTC);
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		pDeviceContext->PSSetSamplers(0, 1, &pSampleLinear);
		pDeviceContext->PSSetShaderResources(0, 1, &p2->pTexview);
	}
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(ver, 0);
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

Dx11Process::PolygonData::PolygonData(){
	tex_no = 0;
	load_tex_no[0] = -1;
	pTexview = NULL;
	pTex = NULL;
	pMyVB = NULL;
	lock = FALSE;  //���b�N,���_�ݒ�O
	lockI = FALSE;//���b�N,�C���f�b�N�X�ݒ�O
}

Dx11Process::PolygonData::~PolygonData(){

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

void Dx11Process::PolygonData::SetVertex(int I1, int I2, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float r, float g, float b,
	float u, float v){
	d3varrayI[I1] = i;
	d3varrayI[I2] = i;
	d3varray[i].Pos = D3DXVECTOR3(vx, vy, vz);
	d3varray[i].normal = D3DXVECTOR3(nx, ny, nz);
	d3varray[i].color = { (FLOAT)r, (FLOAT)g, (FLOAT)b, 1.0f };
	d3varray[i].tex = D3DXVECTOR2(u, v);
}

void Dx11Process::PolygonData::SetVertex(int I1, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	float r, float g, float b,
	float u, float v){
	d3varrayI[I1] = i;
	d3varray[i].Pos = D3DXVECTOR3(vx, vy, vz);
	d3varray[i].normal = D3DXVECTOR3(nx, ny, nz);
	d3varray[i].color = { (FLOAT)r, (FLOAT)g, (FLOAT)b, 1.0f };
	d3varray[i].tex = D3DXVECTOR2(u, v);
}

Dx11Process::PolygonData2D::PolygonData2D(){
	pMyVB = NULL;
	d3varray = NULL;
	lock = FALSE;
	pTexview = NULL;
}

Dx11Process::PolygonData2D::~PolygonData2D(){

	if (pTexview != NULL){
		pTexview->Release();
		pTexview = NULL;
	}
	if (pMyVB != NULL){
		pMyVB->Release();
		pMyVB = NULL;
		free(d3varray);
		d3varray = NULL;
	}
}

//�ړ��ʈ�艻
clock_t T_float::f = clock();
clock_t T_float::time = 0;

void T_float::GetTime(){
	time = clock() - f;
	f = clock();
}

float T_float::Add(float f){
	float r = (float)(time * f) / 2;
	if (r <= 0.0f)return 0.005f;
	return r;
}