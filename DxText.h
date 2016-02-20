//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         DxTextクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_DxText_Header
#define Class_DxText_Header

#include "Dx11Process.h"

class DxText{

private:
	Dx11Process *dx;
	PolygonData2D text[20];  //文字描画用
	PolygonData2D value[10];//可変数字用
	TCHAR str[20][40];                 //登録テキスト
	float f_size[20];                  //登録テキストのフォントサイズ
	int strcnt[20];                   //登録テキスト文字数 
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