//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	      Positionクラス                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Position_Header
#define Class_Position_Header

class Position{

public:
	typedef struct{
		float theta;
		bool element;
		float x;
		float y;
		float z;
	}E_Pos;
	typedef struct{
		float cx1;
		float cx2;
		float cy1;
		float cy2;
		int px;
		int py;
		int pz;
		float theta;
	}H_Pos;
};

#endif