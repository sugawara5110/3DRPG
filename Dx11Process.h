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

#define LIGHT_PCS 150
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//*****フレンドクラス*****//
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

	//各シェーダーはこのクラスでコンパイル, 保持, 解放を行う 使用する時はポインタを渡す
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

	char                         **binary_ch; //デコード後バイナリ
	int                          *binary_size;//バイナリサイズ

	//マトリックス
	D3DXMATRIX	            Proj;    //カメラの画角
	D3DXMATRIX	            View;	//カメラの配置
	D3DXMATRIX	            World; //モデルの配置

	//現在位置
	float cx, cy, cz;

	//ライト
	D3DXVECTOR4 LightPos[LIGHT_PCS];   //光源
	D3DXVECTOR4 LightColor[LIGHT_PCS];//色
	D3DXVECTOR4 Lightst[LIGHT_PCS];  //レンジ,明るさ,減衰の大きさ,オンオフ
	float        ShadowLow_val; //影の下限値
	bool         SetLight_f;   //ライト有効無効

	//シェーダー受け渡し用バッファ
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

	static Dx11Process *dx;//クラス内でオブジェクト生成し使いまわす

	Dx11Process();//外部からのオブジェクト生成禁止
	Dx11Process(const Dx11Process &obj);     // コピーコンストラクタ禁止
	void operator=(const Dx11Process& obj);// 代入演算子禁止
	~Dx11Process();
	void TextureBinaryDecode(char *Bpass, int i);//暗号化済み画像バイナリデコード
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

//*********************************PolygonDataクラス*************************************//

class PolygonData{

private:
	friend Dx11Process;
	Dx11Process                 *dx;
	//シェーダー, コンスタントバッファを使う場合はDx11Processクラスからポインタを渡す,このクラスで生成しない
	ID3D11HullShader           *pHullShader;
	ID3D11DomainShader         *pDomainShader;
	ID3D11InputLayout          *pVertexLayout;
	ID3D11VertexShader         *pVertexShader;
	ID3D11PixelShader          *pPixelShader;
	ID3D11Buffer                *pConstantBuffer;
	int                           ver, verI; //頂点個数  
	D3D11_PRIMITIVE_TOPOLOGY  topology;

	int                          load_tex_no[4];//0～100 静止画テクスチャ用
	ID3D11Texture2D           *pTex;     //directshowからの書き込み用
	ID3D11ShaderResourceView *pTexview;//動画テクスチャ用view
	D3D11_BUFFER_DESC          bd;      //バッファリソース内容
	D3D11_BUFFER_DESC          bdI;     //インデックスリソース内容
	ID3D11Buffer               *pMyVB;       //頂点バッファ
	ID3D11Buffer               *pMyVBI;     //インデックスバッファ
	//壁,地面用頂点(3D)
	struct MY_VERTEX{          //頂点データの構造体
		D3DXVECTOR3 Pos;     //位置
		D3DXVECTOR3 normal;  //法線
		D3DXVECTOR4 color;   //色
		D3DXVECTOR2 tex;    //テクスチャ座標
	};
	MY_VERTEX                  *d3varray;  //頂点配列
	UINT                        *d3varrayI;//頂点インデックス
	bool                         lock;     //ロック,頂点設定済み
	bool                         lockI;   //ロック,インデックス設定済み
	bool                         CPUAccess;   //CPUアクセス有無
	bool                         lighteffect;//ライトの影響有無

	void GetShaderPointer();

public:
	int                          tex_no;//0～4

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

//*********************************PolygonData2Dクラス*************************************//

class PolygonData2D{

private:
	friend DxText;
	Dx11Process                 *dx;
	ID3D11InputLayout          *pVertexLayout;
	ID3D11VertexShader         *pVertexShader;
	ID3D11PixelShader          *pPixelShader;
	int                           ver; //頂点数             

	//メニュー用頂点(2D)
	struct MY_VERTEX2{
		float         x, y, z;
		D3DXVECTOR4 color;
		D3DXVECTOR2 tex;    //テクスチャ座標
	};

	void GetShaderPointer();

public:
	ID3D11ShaderResourceView  *pTexview; //文字でしか使ってない
	D3D11_BUFFER_DESC          bd;         //バッファリソース内容
	ID3D11Buffer                *pMyVB;     //頂点バッファ
	MY_VERTEX2                 *d3varray;  //頂点配列
	bool                         lock;

	PolygonData2D();
	~PolygonData2D();
	HRESULT GetVBarray2D(int pieces);
	void D2primitive(bool a, bool lock);
};

//移動量一定化
class T_float{

private:
	static DWORD f, time;

public:
	static void GetTime();//常に実行
	float Add(float f);
};

#endif
