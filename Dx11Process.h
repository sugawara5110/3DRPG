//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          Dx11Processクラス                                 **//
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
#include "Function.h"
#include <string.h>
#include <tchar.h>
#include <time.h>

#define RELEASE(p) if(p){ p->Release(); p=NULL;}
#define LIGHT_PCS 150
#define LIGHT_PCS_init 7
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//*****フレンドクラス*****//
class DxText;
class MeshData;
class PolygonData;
class PolygonData2D;
class ParticleData;
//***********************//

class Dx11Process{

private:
	friend DxText;
	friend MeshData;
	friend PolygonData;
	friend PolygonData2D;
	friend ParticleData;
	ID3D11Device                *pDevice;
	ID3D11DeviceContext        *pDeviceContext;
	IDXGISwapChain             *pSwapChain;
	ID3D11RenderTargetView    *pRTV;
	ID3D11Texture2D            *pDS;
	ID3D11DepthStencilView    *pDSV;

	//各シェーダーはこのクラスでコンパイル, 保持, 解放を行う 使用する時はポインタを渡す
	ID3D11GeometryShader       *pGeometryShader_PSO;
	ID3D11GeometryShader       *pGeometryShader_P;

	ID3D11HullShader           *pHullShader_MESH_D;
	ID3D11HullShader           *pHullShader_DISPL;
	ID3D11HullShader           *pHullShader_DISP;

	ID3D11DomainShader         *pDomainShader_MESH_D;
	ID3D11DomainShader         *pDomainShader_DISPL;
	ID3D11DomainShader         *pDomainShader_DISP;

	ID3D11InputLayout          *pVertexLayout_P;
	ID3D11InputLayout          *pVertexLayout_MESH_D;
	ID3D11InputLayout          *pVertexLayout_MESH;
	ID3D11InputLayout          *pVertexLayout_DISPL;
	ID3D11InputLayout          *pVertexLayout_DISP;
	ID3D11InputLayout          *pVertexLayout_TCL;
	ID3D11InputLayout          *pVertexLayout_TC;
	ID3D11InputLayout          *pVertexLayout_BC;
	ID3D11InputLayout          *pVertexLayout_2D;
	ID3D11InputLayout          *pVertexLayout_2DTC;

	ID3D11VertexShader         *pVertexShader_PSO;
	ID3D11VertexShader         *pVertexShader_P;
	ID3D11VertexShader         *pVertexShader_MESH_D;
	ID3D11VertexShader         *pVertexShader_MESH;
	ID3D11VertexShader         *pVertexShader_DISPL;
	ID3D11VertexShader         *pVertexShader_DISP;
	ID3D11VertexShader         *pVertexShader_TCL;
	ID3D11VertexShader         *pVertexShader_TC;
	ID3D11VertexShader         *pVertexShader_BC;
	ID3D11VertexShader         *pVertexShader_2D;
	ID3D11VertexShader         *pVertexShader_2DTC;

	ID3D11PixelShader          *pPixelShader_P;
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
	ID3D11ShaderResourceView  **pTexture;     //通常のテクスチャ
	ID3D11Texture2D            **pTexCPUAcc;   //ピクセルデータ読み込み用
	ID3D11BlendState           *pBlendState;
	ID3D11RasterizerState     *pRasterizeState;
	D3D11_BLEND_DESC            bld;
	D3D11_RASTERIZER_DESC      RSDesc;

	char                         **binary_ch; //デコード後バイナリ
	int                          *binary_size;//バイナリサイズ

	//マトリックス
	MATRIX	            Proj;    //カメラの画角
	MATRIX	            View;	//カメラの配置
	MATRIX	            World; //モデルの配置

	//現在位置
	float cx, cy, cz;
	//カメラ画角
	float ViewY_theta;
	//アスペクト比
	float aspect;
	//nearプレーン
	float NearPlane;
	//farプレーン
	float FarPlane;

	//ポイントライト
	struct PointLight{
		VECTOR4 LightPos[LIGHT_PCS];   //光源
		VECTOR4 LightColor[LIGHT_PCS];//色
		VECTOR4 Lightst[LIGHT_PCS];  //レンジ,明るさ,減衰の大きさ,オンオフ
		float        ShadowLow_val; //影の下限値
		int          LightPcs;     //ライト個数
	};
	PointLight plight;

	//平行光源
	struct DirectionLight{
		VECTOR4 Direction;  //方向
		VECTOR4 LightColor;//色
		VECTOR4 Lightst;  //明るさx,オンオフy,影の下限値
	};
	DirectionLight dlight;

	//フォグ
	struct Fog{
		VECTOR4  FogColor;//フォグの色
		float         Amount;  //フォグ量
		float         Density;//密度
		float         on_off;
	};
	Fog fog;

	//シェーダー受け渡し用バッファ
	struct CONSTANT_BUFFER
	{
		MATRIX World;
		MATRIX WVP;
		VECTOR4 C_Pos;       //視点位置
		VECTOR4 AddObjColor;//オブジェクトの色変化用
		//ポイントライト
		VECTOR4 pLightPos[LIGHT_PCS];
		VECTOR4 pLightColor[LIGHT_PCS];
		VECTOR4 pLightst[LIGHT_PCS];
		VECTOR4 pShadowLow_Lpcs;//影の下限値, ライト個数, ライト有無
		//ディレクショナルライト
		VECTOR4 dDirection;
		VECTOR4 dLightColor;
		VECTOR4 dLightst;
		//フォグ
		VECTOR4  FogAmo_Density; //フォグ量x, フォグの密度y, onoffz
		VECTOR4  FogColor;   //フォグの色
		//ディスプレイトメントマッピングの起伏量x(0入力の場合デフォルト値3になる)
		VECTOR4  DispAmount;
	};

	static Dx11Process *dx;//クラス内でオブジェクト生成し使いまわす

	Dx11Process();//外部からのオブジェクト生成禁止
	Dx11Process(const Dx11Process &obj);     // コピーコンストラクタ禁止
	void operator=(const Dx11Process& obj);// 代入演算子禁止
	~Dx11Process();
	void TextureBinaryDecode(char *Bpass, int i);//暗号化済み画像バイナリデコード
	HRESULT MakeShader(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob);
	HRESULT MakeShaderGeometrySO(LPSTR szFileName, size_t size, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob, D3D11_SO_DECLARATION_ENTRY *Decl, UINT Declsize);
	void ChangeBlendState(BOOL at, BOOL a);
	void MatrixMap(ID3D11Buffer *pCBuffer, float x, float y, float z, float r, float g, float b, float theta, float size, float disp);
	
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
		float brightness, float attenuation, bool on_off);//0:視点, 1:ラスボス, 2:出入り口, 3456:戦闘
	void DirectionLight(float x, float y, float z, float r, float g, float b, float bright, float ShadowBright);
	void SetDirectionLight(bool onoff);
	void Fog(float r, float g, float b, float amount, float density, bool onoff);
	void TextureBinaryDecodeAll();
	void GetTexture();
	void Drawscreen();
	float GetViewY_theta();
	float Getaspect();
	float GetNearPlane();
	float GetFarPlane();
};

//*********************************MeshDataクラス*************************************//

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

	ID3D11Buffer                *pConstantBuffer_MESH;//クラス内生成解放(マテリアル渡し用)
	ID3D11Buffer                *pMyVB;       //頂点バッファ
	ID3D11Buffer                **pMyVBI;    //インデックスバッファ(マテリアルの数だけ必要)
	DWORD                        VerCount;   //頂点数    
	DWORD                        FaceCount;     //ポリゴン数
	DWORD                        MaterialCount;//マテリアル数

	struct MY_VERTEX_MESH{
		VECTOR3 Pos;     
		VECTOR3 normal;
		VECTOR2 tex;
	};

	struct MY_MATERIAL{
		CHAR MaterialName[110];
		VECTOR4 Kd;//ディフューズ
		CHAR TextureName[110];//テクスチャーファイル名
		int tex_no;
		DWORD FaceCnt;//そのマテリアルであるポリゴン数
		MY_MATERIAL()
		{
			ZeroMemory(this, sizeof(MY_MATERIAL));
			tex_no = -1;
		}
		~MY_MATERIAL()
		{}
	};
	MY_MATERIAL* pMaterial;

	struct CONSTANT_BUFFER_MESH{

		VECTOR4 vDiffuse;//ディフューズ色
		VECTOR4 tex_f;   //テクスチャ有り無し
	};
	
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL** ppMaterial);
	void GetShaderPointer(bool disp);

public:
	MeshData();
	~MeshData();
	HRESULT GetVBarray(LPSTR FileName, bool disp);
	//木./dat/mesh/tree.obj
	void D3primitive(float x, float y, float z, float r, float g, float b, float theta, float size, float disp);
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
		VECTOR3 Pos;     //位置
		VECTOR3 normal;  //法線
		VECTOR4 color;   //色
		VECTOR2 tex;    //テクスチャ座標
	};
	MY_VERTEX                  *d3varray;  //頂点配列
	UINT                        *d3varrayI;//頂点インデックス
	bool                         lockV;    //ロック,頂点設定済み
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
	void D3primitive(float x, float y, float z, float r, float g, float b, float theta, bool a, bool lock, float disp);
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
		float    x, y, z;
		VECTOR4 color;
		VECTOR2 tex;    //テクスチャ座標
	};

	void GetShaderPointer();

public:
	ID3D11ShaderResourceView  *pTexview; //文字でしか使ってない
	D3D11_BUFFER_DESC          bd;         //バッファリソース内容
	ID3D11Buffer                *pMyVB;     //頂点バッファ
	MY_VERTEX2                 *d3varray;  //頂点配列
	bool                         lockV;

	PolygonData2D();
	~PolygonData2D();
	HRESULT GetVBarray2D(int pieces);
	void D2primitive(bool a, bool lock);
};

//**************移動量一定化*************//
class T_float{

private:
	static DWORD f, time;

public:
	static void GetTime();//常に実行
	float Add(float f);
};

//*********************************ParticleDataクラス*************************************//

class ParticleData{

private:
	friend Dx11Process;
	Dx11Process                 *dx;
	//シェーダー, コンスタントバッファを使う場合はDx11Processクラスからポインタを渡す,このクラスで生成しない
	ID3D11GeometryShader       *pGeometryShaderSO;
	ID3D11VertexShader         *pVertexShaderSO;
	ID3D11GeometryShader       *pGeometryShader;
	ID3D11InputLayout          *pVertexLayout;
	ID3D11VertexShader         *pVertexShader;
	ID3D11PixelShader          *pPixelShader;

	ID3D11Buffer                *pConstantBuffer;//クラス内生成(WVP用)

	int                          ver;        //頂点数
	ID3D11Buffer               *pMyVB;    //頂点バッファ描画
	ID3D11Buffer               *pMyVB_SO;//頂点バッファ計算後

	struct CONSTANT_BUFFER_P{
		MATRIX  WV;
		MATRIX  Proj;
		VECTOR4 size;//xパーティクル大きさ, yパーティクル初期化フラグ, zスピード
	};
	struct PartPos{
		VECTOR3 CurrentPos; //描画に使う
		VECTOR3 PosSt;     //開始位置
		VECTOR3 PosEnd;   //終了位置
		VECTOR4 Col;
	};
	PartPos *P_pos;//パーティクルデータ配列
	bool Drawfirst;

	void GetShaderPointer();
	void MatrixMap(float x, float y, float z, float theta, float size, bool init, float speed);

public:
	ParticleData();
	~ParticleData();
	void CreateParticle(int texture_no, float size, float density);//テクスチャを元にパーティクルデータ生成, 全体サイズ倍率, 密度
	void Draw(float x, float y, float z, float theta, float size, bool init, float speed);//sizeパーティクル1個のサイズ
};

#endif
