//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         DxTextクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_DxText_Header
#define Class_DxText_Header

#include "Dx11Process.h"
#define STRTEX_MAX_PCS 40
#define STR_MAX_LENGTH 40
#define VAL_PCS 10

class DxText{

private:
	Dx11Process *dx;
	PolygonData2D text[STRTEX_MAX_PCS];       //文字描画用
	PolygonData2D value[VAL_PCS];              //可変数字用
	TCHAR str[STRTEX_MAX_PCS][STR_MAX_LENGTH]; //登録テキスト
	float f_size[STRTEX_MAX_PCS];              //登録テキストのフォントサイズ
	int strcnt[STRTEX_MAX_PCS];       //登録テキスト文字数 
	HFONT hFont, oldFont;           //フォント
	HDC hdc;                       //デバイスコンテキスト
	int CreateTextNo;              //テキスト作成ターゲット
	BYTE *ptr;                    //ビットマップ配列
	TEXTMETRIC *TM;    //文字レイアウト
	GLYPHMETRICS *GM; //文字レイアウト
	DWORD *allsize;   //各要素までの合計サイズ

	static DxText *textobj;

	DxText(const DxText &obj);     // コピーコンストラクタ禁止
	void operator=(const DxText& obj);// 代入演算子禁止
	DxText();
	~DxText();
	int CreateText(PolygonData2D *p2, TCHAR *c, int texNo, float fontsize);

public:
	static void InstanceCreate();
	static DxText *GetInstance();
	static void DeleteInstance();
	TCHAR *CreateTextValue(int val);
	void Drawtext(TCHAR *c, float x, float y, float fontsize, VECTOR4 cl);
	void DrawValue(int val, float x, float y, float fontsize, int pcs, VECTOR4 cl);
};

#endif