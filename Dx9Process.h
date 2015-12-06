//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          Dx9Processクラス                                  **//
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
	IDirect3D9               *pD3D;            //IDirect3D9インターフェイスへのポインタ
	IDirect3DDevice9        *pD3DDevice;     //IDirect3DDevice9インターフェイスへのポインタ
	D3DPRESENT_PARAMETERS   D3DPP;         //デバイスのプレゼンテーションパラメータ
	D3DDISPLAYMODE          d3ddm;         //ディスプレイモード
	LPD3DXFONT              pD3DFont_s;    //文字小
	LPD3DXFONT              pD3DFont_l;   //文字大
	LPDIRECT3DTEXTURE9     *pTexture;   //静止画テクスチャ保管
	char                     **binary_ch; //デコード後バイナリ
	int                      *binary_size;//バイナリサイズ
	D3DLIGHT9               light;     //ライト
	D3DMATERIAL9            material; //マテリアル  

	//マトリックス
	D3DXMATRIX	            m_proj;    //カメラの画角
	D3DXMATRIX	            m_view;	  //カメラの配置
	D3DXMATRIX	            m_world; //モデルの配置
	D3DVIEWPORT9            vp;   //ビューポート

	//壁,地面用頂点(3D)
	struct MY_VERTEX{          //頂点データの構造体
		float x, y, z;         //位置
		D3DXVECTOR3 normal;  //法線
		D3DCOLOR    color;   //色
		D3DXVECTOR2 tex;    //テクスチャ座標
	};

	static Dx9Process *dx;//クラス内でオブジェクト生成し使いまわす

	Dx9Process();//外部からのオブジェクト生成禁止
	Dx9Process(const Dx9Process &obj);     // コピーコンストラクタ禁止
	void operator=(const Dx9Process& obj);// 代入演算子禁止
	~Dx9Process();
	void TextureBinaryDecode(char *Bpass, int i);//暗号化済み画像バイナリデコード

public:
	//メニュー用頂点(2D)
	struct MY_VERTEX2{
		float    x, y, z;
		float    rhw;
		D3DCOLOR color;
		float    tu, tv;
	};

	class PolygonData{
	public:
		int                        tex_no;//0～4
		int                        load_tex_no[4];//0～100 静止画テクスチャ用
		LPDIRECT3DTEXTURE9       pTexture;//動画テクスチャ用
		LPDIRECT3DVERTEXBUFFER9 pMyVB;
		LPDIRECT3DINDEXBUFFER9  pMyVBI;
		MY_VERTEX                *d3varray;  //頂点配列
		DWORD                    *d3varrayI;//頂点インデックス
		bool                       lock;    //ロック,頂点設定済み

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

	//テクスチャーから取得したピクセルデータ
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
