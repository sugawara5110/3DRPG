//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　      Endingクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Ending_Header
#define Class_Ending_Header

#include "Dx9Process.h"
#include "MovieSoundManager.h"

class Ending{

private:
	Dx9Process *dx;
	int str_y;

public:
	Ending();
	void StaffRoll();
	~Ending();
};

#endif