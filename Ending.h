//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　      Endingクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Ending_Header
#define Class_Ending_Header

#include "Dx11Process.h"
#include "MovieSoundManager.h"
#include "DxText.h"

class Ending{

private:
	Dx11Process *dx;
	DxText *text;
	float str_y;

public:
	Ending();
	void StaffRoll();
	~Ending();
};

#endif