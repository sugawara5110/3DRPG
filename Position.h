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
		float BtPos_x[4];
		float BtPos_x1[4];
		float BtPos_y[4];
		float BtPos_y1[4];
	}H_Pos;
};

#endif