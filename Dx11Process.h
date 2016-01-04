//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          Dx11Processクラス                                 **//
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

class DxText;//フレンドクラス

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

	char                         **binary_ch; //デコード後バイナリ
	int                          *binary_size;//バイナリサイズ

	//マトリックス
	D3DXMATRIX	            Proj;    //カメラの画角
	D3DXMATRIX	            View;	//カメラの配置
	D3DXMATRIX	            World; //モデルの配置

	//ライト
	D3DXVECTOR4 LightPos[10];   //光源
	D3DXVECTOR4 LightColor[10];//色
	D3DXVECTOR4 Lightst[10];  //レンジ,明るさ,減衰の大きさ,オンオフ

	//壁,地面用頂点(3D)
	struct MY_VERTEX{          //頂点データの構造体
		D3DXVECTOR3 Pos;     //位置
		D3DXVECTOR3 normal;  //法線
		D3DXVECTOR4 color;   //色
		D3DXVECTOR2 tex;    //テクスチャ座標
	};

	//メニュー用頂点(2D)
	struct MY_VERTEX2{
		float         x, y, z;
		D3DXVECTOR4 color;
		D3DXVECTOR2 tex;    //テクスチャ座標
	};

	//シェーダー受け渡し用バッファ
	struct CONSTANT_BUFFER
	{
		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX Proj;
		D3DXVECTOR4 LightPos[10];
		D3DXVECTOR4 LightColor[10];
		D3DXVECTOR4 Lightst[10];
	};

	static Dx11Process *dx;//クラス内でオブジェクト生成し使いまわす

	Dx11Process();//外部からのオブジェクト生成禁止
	Dx11Process(const Dx11Process &obj);     // コピーコンストラクタ禁止
	void operator=(const Dx11Process& obj);// 代入演算子禁止
	~Dx11Process();
	void TextureBinaryDecode(char *Bpass, int i);//暗号化済み画像バイナリデコード
	HRESULT MakeShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob);

public:
	class PolygonData{
	public:
		int                          tex_no;//0～4
		int                          load_tex_no[4];//0～100 静止画テクスチャ用
		ID3D11Texture2D           *pTex;     //directshowからの書き込み用
		ID3D11ShaderResourceView *pTexview;//動画テクスチャ用view
		D3D11_BUFFER_DESC          bd;      //バッファリソース内容
		D3D11_BUFFER_DESC          bdI;     //インデックスリソース内容
		ID3D11Buffer               *pMyVB;       //頂点バッファ
		ID3D11Buffer               *pMyVBI;     //インデックスバッファ
		MY_VERTEX                   *d3varray;  //頂点配列
		UINT                        *d3varrayI;//頂点インデックス
		bool                          lock;    //ロック,頂点設定済み
		bool                          lockI;   //ロック,インデックス設定済み

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
		ID3D11ShaderResourceView  *pTexview; //文字でしか使ってない
		D3D11_BUFFER_DESC        bd;         //バッファリソース内容
		ID3D11Buffer             *pMyVB;     //頂点バッファ
		MY_VERTEX2               *d3varray;  //頂点配列
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

//移動量一定化
class T_float{

private:
	static clock_t f, time;

public:
	static void GetTime();//常に実行
	float Add(float f);
};

#endif
