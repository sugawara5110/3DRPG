//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	      EnemySide�N���X                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_EnemySide_Header
#define Class_EnemySide_Header

#include "Enemy.h"

class EnemySide :public Enemy{

protected:
	//@Override
	virtual bool Magiccreate(float x, float y, float z);

public:
	EnemySide();
	EnemySide(int t_no, int no, Position::H_Pos *h_pos, Position::E_Pos *e_pos);
	//@Override
	virtual bool M_run_flg();//�}�W�b�N���s�t���O
	//@Override
	virtual void M_select(int *r, int *r1);//�}�W�b�N�I��
	virtual ~EnemySide();
};

#endif