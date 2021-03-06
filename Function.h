//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         関数, データ型.h                                **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Function_Header
#define Function_Header

#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <stdlib.h >

#define WINDOW_WIDTH 800 //ウィンドウ幅
#define WINDOW_HEIGHT 600 //ウィンドウ高さ

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

void swap(float *a, float *b);
//行列初期化
void MatrixIdentity(MATRIX *mat);
//拡大縮小
void MatrixScaling(MATRIX *mat, float sizex, float sizey, float sizez);
//x軸回転
void MatrixRotationX(MATRIX *mat, float theta);
//y軸回転
void MatrixRotationY(MATRIX *mat, float theta);
//z軸回転
void MatrixRotationZ(MATRIX *mat, float theta);
//平行移動
void MatrixTranslation(MATRIX *mat, float movx, float movy, float movz);
//行列足し算
void MatrixAddition(MATRIX *mat, MATRIX *mat1, MATRIX *mat2);
//行列掛け算
void MatrixMultiply(MATRIX *mat, MATRIX *mat1, MATRIX *mat2);
//転置
void MatrixTranspose(MATRIX *mat);
//透視変換行列 1:視点位置ベクトル 2:注視点位置ベクトル 3:カメラ上方向ベクトル
void MatrixLookAtLH(MATRIX *mat, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
//射影変換行列  カメラの画角, アスペクト比, nearプレーン, farプレーン
void MatrixPerspectiveFovLH(MATRIX *mat, float theta, float aspect, float Near, float Far);
//ベクトル3, 行列掛け算
void VectorMatrixMultiply(VECTOR3 *v, MATRIX *mat);
//ビューポート変換行列
void MatrixViewPort(MATRIX *mat);

void Bdecode(char *bpass, char **binary, int *size);

#endif