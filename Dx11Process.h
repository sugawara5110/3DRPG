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

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

class DxText;//�t�����h�N���X

class Dx11Process{

private:
	friend DxText;
	ID3D11Device                *pDevice;
	ID3D11DeviceContext        *pDeviceContext;
	IDXGISwapChain             *pSwapChain;
	ID3D11RenderTargetView    *pRTV;
	ID3D11Texture2D            *pDS;
	ID3D11DepthStencilView    *pDSV;
	ID3D11InputLayout          *pVertexLayout_TCL;
	ID3D11InputLayout          *pVertexLayout_TC;
	ID3D11InputLayout          *pVertexLayout_BC;
	ID3D11InputLayout          *pVertexLayout_2D;
	ID3D11InputLayout          *pVertexLayout_2DTC;
	ID3D11VertexShader         *pVertexShader_TCL;
	ID3D11VertexShader         *pVertexShader_TC;
	ID3D11VertexShader         *pVertexShader_BC;
	ID3D11VertexShader         *pVertexShader_2D;
	ID3D11VertexShader         *pVertexShader_2DTC;
	ID3D11PixelShader          *pPixelShader_TCL;
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

	//���C�g
	D3DXVECTOR4 LightPos[10];   //����
	D3DXVECTOR4 LightColor[10];//�F
	D3DXVECTOR4 Lightst[10];  //�����W,���邳,�����̑傫��,�I���I�t

	//��,�n�ʗp���_(3D)
	struct MY_VERTEX{          //���_�f�[�^�̍\����
		D3DXVECTOR3 Pos;     //�ʒu
		D3DXVECTOR3 normal;  //�@��
		D3DXVECTOR4 color;   //�F
		D3DXVECTOR2 tex;    //�e�N�X�`�����W
	};

	//���j���[�p���_(2D)
	struct MY_VERTEX2{
		float         x, y, z;
		D3DXVECTOR4 color;
		D3DXVECTOR2 tex;    //�e�N�X�`�����W
	};

	//�V�F�[�_�[�󂯓n���p�o�b�t�@
	struct CONSTANT_BUFFER
	{
		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX Proj;
		D3DXVECTOR4 LightPos[10];
		D3DXVECTOR4 LightColor[10];
		D3DXVECTOR4 Lightst[10];
	};

	static Dx11Process *dx;//�N���X���ŃI�u�W�F�N�g�������g���܂킷

	Dx11Process();//�O������̃I�u�W�F�N�g�����֎~
	Dx11Process(const Dx11Process &obj);     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const Dx11Process& obj);// ������Z�q�֎~
	~Dx11Process();
	void TextureBinaryDecode(char *Bpass, int i);//�Í����ς݉摜�o�C�i���f�R�[�h
	HRESULT MakeShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob);

public:
	class PolygonData{
	public:
		int                          tex_no;//0�`4
		int                          load_tex_no[4];//0�`100 �Î~��e�N�X�`���p
		ID3D11Texture2D           *pTex;     //directshow����̏������ݗp
		ID3D11ShaderResourceView *pTexview;//����e�N�X�`���pview
		D3D11_BUFFER_DESC          bd;      //�o�b�t�@���\�[�X���e
		D3D11_BUFFER_DESC          bdI;     //�C���f�b�N�X���\�[�X���e
		ID3D11Buffer               *pMyVB;       //���_�o�b�t�@
		ID3D11Buffer               *pMyVBI;     //�C���f�b�N�X�o�b�t�@
		MY_VERTEX                   *d3varray;  //���_�z��
		UINT                        *d3varrayI;//���_�C���f�b�N�X
		bool                          lock;    //���b�N,���_�ݒ�ς�
		bool                          lockI;   //���b�N,�C���f�b�N�X�ݒ�ς�

		PolygonData();
		~PolygonData();
		void SetVertex(int I1, int I2, int i,
			float vx, float vy, float vz,
			float nx, float ny, float nz,
			float r, float g, float b,
			float u, float v);
		void SetVertex(int I1, int i,
			float vx, float vy, float vz,
			float nx, float ny, float nz,
			float r, float g, float b,
			float u, float v);
	};

	class PolygonData2D{
	public:
		ID3D11ShaderResourceView  *pTexview; //�����ł����g���ĂȂ�
		D3D11_BUFFER_DESC        bd;         //�o�b�t�@���\�[�X���e
		ID3D11Buffer             *pMyVB;     //���_�o�b�t�@
		MY_VERTEX2               *d3varray;  //���_�z��
		bool                       lock;

		PolygonData2D();
		~PolygonData2D();
	};

	static void InstanceCreate();
	static Dx11Process *GetInstance();
	static void DeleteInstance();
	HRESULT Initialize(HWND hWnd);
	HRESULT GetVBarray(PrimitiveType type, PolygonData *pd, int pieces);
	HRESULT GetVBarray2D(PolygonData2D *p2, int pieces);
	HRESULT Sclear();
	void Cameraset(float cax1, float cax2, float cay1, float cay2, float caz);
	void LightPosSet(int Idx, float x, float y, float z, float r, float g, float b, float a, float range, float brightness, float attenuation, bool on_off);
	void TextureInit(PolygonData *pd, int width, int height);
	void TextureBinaryDecodeAll();
	void GetTexture();
	void GetTexture(PolygonData *pd, int no);
	void SetTextureMPixel(PolygonData *pd, int **m_pix, BYTE r, BYTE g, BYTE b, int a);
	void SetFog(bool on, float StartPos, float EndPos, DWORD r, DWORD g, DWORD b);
	void D3primitive(PrimitiveType type, PolygonData *pd, int pieces, float x, float y, float z, float r, bool a, bool lock, bool light_f);
	void D2primitive(PolygonData2D *p2, int piece, bool lock);
	void Drawscreen();
};

//�ړ��ʈ�艻
class T_float{

private:
	static clock_t f, time;

public:
	static void GetTime();//��Ɏ��s
	float Add(float f);
};

#endif
