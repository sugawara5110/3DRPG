//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         DxText�N���X                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_DxText_Header
#define Class_DxText_Header

#include "Dx11Process.h"

class DxText{

private:
	Dx11Process *dx;
	PolygonData2D text[20];  //�����`��p
	PolygonData2D value[10];//�ϐ����p
	TCHAR str[20][40];                 //�o�^�e�L�X�g
	float f_size[20];                  //�o�^�e�L�X�g�̃t�H���g�T�C�Y
	int strcnt[20];                   //�o�^�e�L�X�g������ 
	HFONT hFont, oldFont;           //�t�H���g
	HDC hdc;                       //�f�o�C�X�R���e�L�X�g
	int CreateTextNo;              //�e�L�X�g�쐬�^�[�Q�b�g
	BYTE *ptr;                    //�r�b�g�}�b�v�z��
	TEXTMETRIC *TM;    //�������C�A�E�g
	GLYPHMETRICS *GM; //�������C�A�E�g
	DWORD *allsize;   //�e�v�f�܂ł̍��v�T�C�Y

	static DxText *textobj;

	DxText(const DxText &obj);     // �R�s�[�R���X�g���N�^�֎~
	void operator=(const DxText& obj);// ������Z�q�֎~
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