//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx11Process�N���X                                 **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Dx11Process_Header
#define Class_Dx11Process_Header

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(push)
#pragma warning(disable:4005)
#include <windows.h>
#include <d3d10_1.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#pragma warning(pop)
#include "Enum.h"
#include <string.h>
#include <tchar.h>
#include <time.h>

#define LIGHT_PCS 150
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//*****�t�����h�N���X*****//
class DxText;
class PolygonData;
class PolygonData2D;
//***********************//

class Dx11Process{

private:
	friend DxText;
	friend PolygonData;
	friend PolygonData2D;
	ID3D11Device                *pDevice;
	ID3D11DeviceContext        *pDeviceContext;
	IDXGISwapChain             *pSwapChain;
	ID3D11RenderTargetView    *pRTV;
	ID3D11Texture2D            *pDS;
	ID3D11DepthStencilView    *pDSV;

	//�e�V�F�[�_�[�͂��̃N���X�ŃR���p�C��, �ێ�, ������s�� �g�p���鎞�̓|�C���^��n��
	ID3D11HullShader           *pHullShader_DISPL;
	ID3D11HullShader           *pHullShader_DISP;

	ID3D11DomainShader         *pDomainShader_DISPL;
	ID3D11DomainShader         *pDomainShader_DISP;

	ID3D11InputLayout          *pVertexLayout_DISPL;
	ID3D11InputLayout          *pVertexLayout_DISP;
	ID3D11InputLayout          *pVertexLayout_TC;
	ID3D11InputLayout          *pVertexLayout_BC;
	ID3D11InputLayout          *pVertexLayout_2D;
	ID3D11InputLayout          *pVertexLayout_2DTC;

	ID3D11VertexShader         *pVertexShader_DISPL;
	ID3D11VertexShader         *pVertexShader_DISP;
	ID3D11VertexShader         *pVertexShader_TC;
	ID3D11VertexShader         *pVertexShader_BC;
	ID3D11VertexShader         *pVertexShader_2D;
	ID3D11VertexShader         *pVertexShader_2DTC;

	ID3D11PixelShader          *pPixelShader_DISPL;
	ID3D11PixelShader          *pPixelShader_DISP;
	ID3D11PixelShader          *pPixelShader_TC;
	ID3D11PixelShader          *pPixelShader_BC;
	ID3D11PixelShader          *pPixelShader_2D;
	ID3D11PixelShader          *pPixelShader_2DTC;

	ID3D11Buffer                *pConstantBuffer;
	ID3D11SamplerState         *pSampleLinear;
	ID3D11ShaderResourceView  **pTexture;
	ID3D11BlendState           *pBlendState;
	ID3D11RasterizerState     *pRasterizeState;
	D3D11_BLEND_DESC            bld;
	D3D11_RASTERIZER_DESC      RSDesc;

	char                         **binary_ch; //�f�R�[�h��o�C�i��
	int                          *binary_size;//�o�C�i���T�C�Y

	//�}�g���b�N�X
	D3DXMATRIX	            Proj;    //�J�����̉�p
	D3DXMATRIX	            View;	//�J�����̔z�u
	D3DXMATRIX	            World; //���f���̔z�u

	//���݈ʒu
	float cx, cy, cz;

	//���C�g
	D3DXVECTOR4 LightPos[LIGHT_PCS];   //����
	D3DXVECTOR4 LightColor[LIGHT_PCS];//�F
	D3DXVECTOR4 Lightst[LIGHT_PCS];  //�����W,���邳,�����̑傫��,�I���I�t
	float        ShadowLow_val; //�e�̉����l
	bool         SetLight_f;   //���C�g�L������

	//�V�F�[�_�[�󂯓n���p�o�b�t�@
	struct CONSTANT_BUFFER
	{
		D3DXMATRIX World;
		D3DXMATRIX WVP;
		D3DXVECTOR4 C_Pos;
		D3DXVECTOR4 AddObjColor;
		D3DXVECTOR4 LightPos[LIGHT_PCS];
		D3DXVECTOR4 LightColor[LIGHT_PCS];
		D3DXVECTOR4 Lightst[LIGHT_PCS];
		D3DXVECTOR4 ShadowLow;
	};

	static Dx11Process *dx;//�N���X���ŃI�u�W�F�N�g�������g���܂킷

	Dx11Process();//�O������̃I�u�W�F�N�g�����֎~
	Dx11Process(const Dx11Process &obj);     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const Dx11Process& obj);// ������Z�q�֎~
	~Dx11Process();
	void TextureBinaryDecode(char *Bpass, int i);//�Í����ς݉摜�o�C�i���f�R�[�h
	HRESULT MakeShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob);

public:
	static void InstanceCreate();
	static Dx11Process *GetInstance();
	static void DeleteInstance();
	HRESULT Initialize(HWND hWnd);
	HRESULT Sclear();
	void Cameraset(float cax1, float cax2, float cay1, float cay2, float caz);
	void SetLight(bool f);
	void ShadowBright(float val);
	void LightPosSet(int Idx, float x, float y, float z, float r, float g, float b, float a, float range,
		float brightness, float attenuation, bool on_off);
	void TextureBinaryDecodeAll();
	void GetTexture();
	void SetFog(bool on, float StartPos, float EndPos, DWORD r, DWORD g, DWORD b);
	void Drawscreen();
};

//*********************************PolygonData�N���X*************************************//

class PolygonData{

private:
	friend Dx11Process;
	Dx11Process                 *dx;
	//�V�F�[�_�[, �R���X�^���g�o�b�t�@���g���ꍇ��Dx11Process�N���X����|�C���^��n��,���̃N���X�Ő������Ȃ�
	ID3D11HullShader           *pHullShader;
	ID3D11DomainShader         *pDomainShader;
	ID3D11InputLayout          *pVertexLayout;
	ID3D11VertexShader         *pVertexShader;
	ID3D11PixelShader          *pPixelShader;
	ID3D11Buffer                *pConstantBuffer;
	int                           ver, verI; //���_��  
	D3D11_PRIMITIVE_TOPOLOGY  topology;

	int                          load_tex_no[4];//0�`100 �Î~��e�N�X�`���p
	ID3D11Texture2D           *pTex;     //directshow����̏������ݗp
	ID3D11ShaderResourceView *pTexview;//����e�N�X�`���pview
	D3D11_BUFFER_DESC          bd;      //�o�b�t�@���\�[�X���e
	D3D11_BUFFER_DESC          bdI;     //�C���f�b�N�X���\�[�X���e
	ID3D11Buffer               *pMyVB;       //���_�o�b�t�@
	ID3D11Buffer               *pMyVBI;     //�C���f�b�N�X�o�b�t�@
	//��,�n�ʗp���_(3D)
	struct MY_VERTEX{          //���_�f�[�^�̍\����
		D3DXVECTOR3 Pos;     //�ʒu
		D3DXVECTOR3 normal;  //�@��
		D3DXVECTOR4 color;   //�F
		D3DXVECTOR2 tex;    //�e�N�X�`�����W
	};
	MY_VERTEX                  *d3varray;  //���_�z��
	UINT                        *d3varrayI;//���_�C���f�b�N�X
	bool                         lock;     //���b�N,���_�ݒ�ς�
	bool                         lockI;   //���b�N,�C���f�b�N�X�ݒ�ς�
	bool                         CPUAccess;   //CPU�A�N�Z�X�L��
	bool                         lighteffect;//���C�g�̉e���L��

	void GetShaderPointer();

public:
	int                          tex_no;//0�`4

	PolygonData();
	~PolygonData();
	void SetVertex(int I1, int I2, int i,
		float vx, float vy, float vz,
		float nx, float ny, float nz,
		float r, float g, float b, float a,
		float u, float v);
	void SetVertex(int I1, int i,
		float vx, float vy, float vz,
		float nx, float ny, float nz,
		float r, float g, float b, float a,
		float u, float v);
	void Light(bool f);
	HRESULT GetVBarray(PrimitiveType type, int pieces);
	void GetVBarrayCPUNotAccess(int pieces);
	void TextureInit(int width, int height);
	void GetTexture(int no);
	void SetTextureMPixel(int **m_pix, BYTE r, BYTE g, BYTE b, int a);
	void D3primitive(float x, float y, float z, float r, float g, float b, float theta, bool a, bool lock);
};

//*********************************PolygonData2D�N���X*************************************//

class PolygonData2D{

private:
	friend DxText;
	Dx11Process                 *dx;
	ID3D11InputLayout          *pVertexLayout;
	ID3D11VertexShader         *pVertexShader;
	ID3D11PixelShader          *pPixelShader;
	int                           ver; //���_��             

	//���j���[�p���_(2D)
	struct MY_VERTEX2{
		float         x, y, z;
		D3DXVECTOR4 color;
		D3DXVECTOR2 tex;    //�e�N�X�`�����W
	};

	void GetShaderPointer();

public:
	ID3D11ShaderResourceView  *pTexview; //�����ł����g���ĂȂ�
	D3D11_BUFFER_DESC          bd;         //�o�b�t�@���\�[�X���e
	ID3D11Buffer                *pMyVB;     //���_�o�b�t�@
	MY_VERTEX2                 *d3varray;  //���_�z��
	bool                         lock;

	PolygonData2D();
	~PolygonData2D();
	HRESULT GetVBarray2D(int pieces);
	void D2primitive(bool a, bool lock);
};

//�ړ��ʈ�艻
class T_float{

private:
	static DWORD f, time;

public:
	static void GetTime();//��Ɏ��s
	float Add(float f);
};

#endif
