//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx9Process�N���X                                  **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Dx9Process_Header
#define Class_Dx9Process_Header

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Enum.h"

class Dx9Process{

private:
	IDirect3D9               *pD3D;            //IDirect3D9�C���^�[�t�F�C�X�ւ̃|�C���^
	IDirect3DDevice9        *pD3DDevice;     //IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	D3DPRESENT_PARAMETERS   D3DPP;         //�f�o�C�X�̃v���[���e�[�V�����p�����[�^
	D3DDISPLAYMODE          d3ddm;         //�f�B�X�v���C���[�h
	LPD3DXFONT              pD3DFont_s;    //������
	LPD3DXFONT              pD3DFont_l;   //������
	LPDIRECT3DTEXTURE9     *pTexture;   //�Î~��e�N�X�`���ۊ�
	char                     **binary_ch; //�f�R�[�h��o�C�i��
	int                      *binary_size;//�o�C�i���T�C�Y
	D3DLIGHT9               light;     //���C�g
	D3DMATERIAL9            material; //�}�e���A��  

	//�}�g���b�N�X
	D3DXMATRIX	            m_proj;    //�J�����̉�p
	D3DXMATRIX	            m_view;	  //�J�����̔z�u
	D3DXMATRIX	            m_world; //���f���̔z�u
	D3DVIEWPORT9            vp;   //�r���[�|�[�g

	//��,�n�ʗp���_(3D)
	struct MY_VERTEX{          //���_�f�[�^�̍\����
		float x, y, z;         //�ʒu
		D3DXVECTOR3 normal;  //�@��
		D3DCOLOR    color;   //�F
		D3DXVECTOR2 tex;    //�e�N�X�`�����W
	};

	static Dx9Process *dx;//�N���X���ŃI�u�W�F�N�g�������g���܂킷

	Dx9Process();//�O������̃I�u�W�F�N�g�����֎~
	Dx9Process(const Dx9Process &obj);     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const Dx9Process& obj);// ������Z�q�֎~
	~Dx9Process();
	void TextureBinaryDecode(char *Bpass, int i);//�Í����ς݉摜�o�C�i���f�R�[�h

public:
	//���j���[�p���_(2D)
	struct MY_VERTEX2{
		float    x, y, z;
		float    rhw;
		D3DCOLOR color;
		float    tu, tv;
	};

	class PolygonData{
	public:
		int                        tex_no;//0�`4
		int                        load_tex_no[4];//0�`100 �Î~��e�N�X�`���p
		LPDIRECT3DTEXTURE9       pTexture;//����e�N�X�`���p
		LPDIRECT3DVERTEXBUFFER9 pMyVB;
		LPDIRECT3DINDEXBUFFER9  pMyVBI;
		MY_VERTEX                *d3varray;  //���_�z��
		DWORD                    *d3varrayI;//���_�C���f�b�N�X
		bool                       lock;    //���b�N,���_�ݒ�ς�

		PolygonData();
		~PolygonData();
		void SetVertex(int I1, int I2, int i,
			float vx, float vy, float vz,
			float nx, float ny, float nz,
			int r, int g, int b,
			float u, float v);
		void SetVertex(int I1, int i,
			float vx, float vy, float vz,
			float nx, float ny, float nz,
			int r, int g, int b,
			float u, float v);
	};

	//�e�N�X�`���[����擾�����s�N�Z���f�[�^
	typedef struct{
		DWORD color;
		float x;
		float y;
		float z;
	}T_xyz;

	static void InstanceCreate();
	static Dx9Process *GetInstance();
	static void DeleteInstance();
	HRESULT Initialize(HWND hWnd);
	HRESULT GetVBarray(PrimitiveType type, PolygonData *pd, int pieces);
	HRESULT Sclear();
	void Cameraset(float cax1, float cax2, float cay1, float cay2, float caz);
	void LightPosSet(int Idx, float x, float y, float z, float range, float attenuation);
	void TextureInit(PolygonData *pd, int width, int height);
	void TextureBinaryDecodeAll();
	void GetTexture();
	void GetTexture(PolygonData *pd, int no);
	void GetTexturePixelArray(PolygonData *pd, T_xyz *p_arr, int size_x, int size_y, int z);
	void SetTextureMPixel(PolygonData *pd, int **m_pix, BYTE r, BYTE g, BYTE b, int a);
	void SetFog(bool on, float StartPos, float EndPos, DWORD r, DWORD g, DWORD b);
	void D3primitive(PrimitiveType type, PolygonData *pd, int pieces, float x, float y, float z, float r, bool a, bool lock, bool light_f);
	void D2primitive(int piece, MY_VERTEX2 *d2varray);
	void text(char str[30], int x, int y, bool size_s, DWORD cl);
	void Drawscreen();
};

#endif
