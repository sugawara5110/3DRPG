//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx11Process�N���X                                 **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Dx11Process_Header
#define Class_Dx11Process_Header

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d10_1.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include "WICTextureLoader.h"
#include "Enum.h"
#include <string.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h >

#define LIGHT_PCS 150
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//*****�t�����h�N���X*****//
class DxText;
class MeshData;
class PolygonData;
class PolygonData2D;
//***********************//

struct MATRIX{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};

struct VECTOR4{
	float x;
	float y;
	float z;
	float w;

	void as(float x1, float y1, float z1, float w1){
		x = x1;
		y = y1;
		z = z1;
		w = w1;
	}
};

struct VECTOR3{
	float x;
	float y;
	float z;

	void as(float x1, float y1, float z1){
		x = x1;
		y = y1;
		z = z1;
	}
};

struct VECTOR2{
	float x;
	float y;

	void as(float x1, float y1){
		x = x1;
		y = y1;
	}
};

class Dx11Process{

private:
	friend DxText;
	friend MeshData;
	friend PolygonData;
	friend PolygonData2D;
	ID3D11Device                *pDevice;
	ID3D11DeviceContext        *pDeviceContext;
	IDXGISwapChain             *pSwapChain;
	ID3D11RenderTargetView    *pRTV;
	ID3D11Texture2D            *pDS;
	ID3D11DepthStencilView    *pDSV;

	//�e�V�F�[�_�[�͂��̃N���X�ŃR���p�C��, �ێ�, ������s�� �g�p���鎞�̓|�C���^��n��
	ID3D11HullShader           *pHullShader_MESH_D;
	ID3D11HullShader           *pHullShader_DISPL;
	ID3D11HullShader           *pHullShader_DISP;

	ID3D11DomainShader         *pDomainShader_MESH_D;
	ID3D11DomainShader         *pDomainShader_DISPL;
	ID3D11DomainShader         *pDomainShader_DISP;

	ID3D11InputLayout          *pVertexLayout_MESH_D;
	ID3D11InputLayout          *pVertexLayout_MESH;
	ID3D11InputLayout          *pVertexLayout_DISPL;
	ID3D11InputLayout          *pVertexLayout_DISP;
	ID3D11InputLayout          *pVertexLayout_TCL;
	ID3D11InputLayout          *pVertexLayout_TC;
	ID3D11InputLayout          *pVertexLayout_BC;
	ID3D11InputLayout          *pVertexLayout_2D;
	ID3D11InputLayout          *pVertexLayout_2DTC;

	ID3D11VertexShader         *pVertexShader_MESH_D;
	ID3D11VertexShader         *pVertexShader_MESH;
	ID3D11VertexShader         *pVertexShader_DISPL;
	ID3D11VertexShader         *pVertexShader_DISP;
	ID3D11VertexShader         *pVertexShader_TCL;
	ID3D11VertexShader         *pVertexShader_TC;
	ID3D11VertexShader         *pVertexShader_BC;
	ID3D11VertexShader         *pVertexShader_2D;
	ID3D11VertexShader         *pVertexShader_2DTC;

	ID3D11PixelShader          *pPixelShader_MESH_D;
	ID3D11PixelShader          *pPixelShader_MESH;
	ID3D11PixelShader          *pPixelShader_DISPL;
	ID3D11PixelShader          *pPixelShader_DISP;
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
	MATRIX	            Proj;    //�J�����̉�p
	MATRIX	            View;	//�J�����̔z�u
	MATRIX	            World; //���f���̔z�u

	//���݈ʒu
	float cx, cy, cz;

	//�|�C���g���C�g
	struct PointLight{
		VECTOR4 LightPos[LIGHT_PCS];   //����
		VECTOR4 LightColor[LIGHT_PCS];//�F
		VECTOR4 Lightst[LIGHT_PCS];  //�����W,���邳,�����̑傫��,�I���I�t
		float        ShadowLow_val; //�e�̉����l
		int          LightPcs;     //���C�g��
	};
	PointLight plight;

	//���s����
	struct DirectionLight{
		VECTOR4 Direction;  //����
		VECTOR4 LightColor;//�F
		VECTOR4 Lightst;  //���邳x,�I���I�ty,�e�̉����l
	};
	DirectionLight dlight;

	//�t�H�O
	struct Fog{
		VECTOR4  FogColor;//�t�H�O�̐F
		float         Amount;  //�t�H�O��
		float         Density;//���x
		float         on_off;
	};
	Fog fog;

	//�V�F�[�_�[�󂯓n���p�o�b�t�@
	struct CONSTANT_BUFFER
	{
		MATRIX World;
		MATRIX WVP;
		VECTOR4 C_Pos;       //���_�ʒu
		VECTOR4 AddObjColor;//�I�u�W�F�N�g�̐F�ω��p
		//�|�C���g���C�g
		VECTOR4 pLightPos[LIGHT_PCS];
		VECTOR4 pLightColor[LIGHT_PCS];
		VECTOR4 pLightst[LIGHT_PCS];
		VECTOR4 pShadowLow_Lpcs;//�e�̉����l, ���C�g��, ���C�g�L��
		//�f�B���N�V���i�����C�g
		VECTOR4 dDirection;
		VECTOR4 dLightColor;
		VECTOR4 dLightst;
		//�t�H�O
		VECTOR4  FogAmo_Density; //�t�H�O��x, �t�H�O�̖��xy, onoffz
		VECTOR4  FogColor;   //�t�H�O�̐F
		//�f�B�X�v���C�g�����g�}�b�s���O�̋N����x(0���͂̏ꍇ�f�t�H���g�l3�ɂȂ�)
		VECTOR4  DispAmount;
	};

	static Dx11Process *dx;//�N���X���ŃI�u�W�F�N�g�������g���܂킷

	Dx11Process();//�O������̃I�u�W�F�N�g�����֎~
	Dx11Process(const Dx11Process &obj);     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const Dx11Process& obj);// ������Z�q�֎~
	~Dx11Process();
	void TextureBinaryDecode(char *Bpass, int i);//�Í����ς݉摜�o�C�i���f�R�[�h
	HRESULT MakeShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob);
	void ChangeBlendState(BOOL at, BOOL a);
	void MatrixMap(ID3D11Buffer *pCBuffer, float x, float y, float z, float r, float g, float b, float theta, float size, float disp);
	//�s�񏉊���
	void MatrixIdentity(MATRIX *mat);
	//�g��k��
	void MatrixScaling(MATRIX *mat, float sizex, float sizey, float sizez);
	//z����]
	void MatrixRotationZ(MATRIX *mat, float theta);
	//���s�ړ�
	void MatrixTranslation(MATRIX *mat, float movx, float movy, float movz);
	//�s��|���Z
	void MatrixMultiply(MATRIX *mat, MATRIX *mat1, MATRIX *mat2);
	//�]�u
	void MatrixTranspose(MATRIX *mat);
	//�����ϊ� 1:���_�ʒu�x�N�g�� 2:�����_�ʒu�x�N�g�� 3:�J����������x�N�g��
	void MatrixLookAtLH(MATRIX *mat, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	//�ˉe�ϊ�  �J�����̉�p, �A�X�y�N�g��, near�v���[��, far�v���[��
	void MatrixPerspectiveFovLH(MATRIX *mat, float theta, float aspect, float Near, float Far);

public:
	static void InstanceCreate();
	static Dx11Process *GetInstance();
	static void DeleteInstance();
	HRESULT Initialize(HWND hWnd);
	HRESULT Sclear();
	void Cameraset(float cax1, float cax2, float cay1, float cay2, float caz);
	void ResetPointLight();
	void P_ShadowBright(float val);
	void PointLightPosSet(int Idx, float x, float y, float z, float r, float g, float b, float a, float range,
		float brightness, float attenuation, bool on_off);
	void DirectionLight(float x, float y, float z, float r, float g, float b, float bright, float ShadowBright);
	void SetDirectionLight(bool onoff);
	void Fog(float r, float g, float b, float amount, float density, bool onoff);
	void TextureBinaryDecodeAll();
	void GetTexture();
	void Drawscreen();
};

//*********************************MeshData�N���X*************************************//

class MeshData{

private:
	friend Dx11Process;
	Dx11Process                 *dx;
	ID3D11HullShader           *pHullShader;
	ID3D11DomainShader         *pDomainShader;
	ID3D11InputLayout          *pVertexLayout;
	ID3D11VertexShader         *pVertexShader;
	ID3D11PixelShader          *pPixelShader;
	ID3D11Buffer                *pConstantBuffer;

	ID3D11Buffer                *pConstantBuffer_MESH;//�N���X���������
	ID3D11Buffer                *pMyVB;       //���_�o�b�t�@
	ID3D11Buffer                **pMyVBI;    //�C���f�b�N�X�o�b�t�@(�}�e���A���̐������K�v)
	DWORD                        VerCount;   //���_��    
	DWORD                        FaceCount;     //�|���S����
	DWORD                        MaterialCount;//�}�e���A����

	struct MY_VERTEX_MESH{
		VECTOR3 Pos;     
		VECTOR3 normal;
		VECTOR2 tex;
	};

	struct MY_MATERIAL{
		CHAR MaterialName[110];
		VECTOR4 Kd;//�f�B�t���[�Y
		CHAR TextureName[110];//�e�N�X�`���[�t�@�C����
		ID3D11ShaderResourceView* pTexture;
		DWORD FaceCnt;//���̃}�e���A���ł���|���S����
		MY_MATERIAL()
		{
			ZeroMemory(this, sizeof(MY_MATERIAL));
		}
		~MY_MATERIAL()
		{
			if (pTexture != NULL)pTexture->Release();
			pTexture = NULL;
		}
	};
	MY_MATERIAL* pMaterial;

	struct CONSTANT_BUFFER_MESH{

		VECTOR4 vDiffuse;//�f�B�t���[�Y�F
		VECTOR4 tex_f;   //�e�N�X�`���L�薳��
	};
	
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL** ppMaterial);
	void GetShaderPointer(bool disp);

public:
	MeshData();
	~MeshData();
	HRESULT GetVBarray(LPSTR FileName, bool disp);
	//��./dat/mesh/tree.obj
	void D3primitive(float x, float y, float z, float theta, float size, float disp);
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
		VECTOR3 Pos;     //�ʒu
		VECTOR3 normal;  //�@��
		VECTOR4 color;   //�F
		VECTOR2 tex;    //�e�N�X�`�����W
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
	void D3primitive(float x, float y, float z, float r, float g, float b, float theta, bool a, bool lock, float disp);
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
		VECTOR4 color;
		VECTOR2 tex;    //�e�N�X�`�����W
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
