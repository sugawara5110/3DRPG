//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	      EnemyBossクラス                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_EnemyBoss_Header
#define Class_EnemyBoss_Header

#include "Enemy.h"

class EnemyBoss :public Enemy{

protected:
	//@Override
	virtual bool Magiccreate(float x, float y, float z);

public:
	EnemyBoss();
	EnemyBoss(int t_no, int no, Position::H_Pos *h_pos, Position::E_Pos *e_pos);
	//@Override
	virtual bool M_run_flg();//マジック実行フラグ
	//@Override
	virtual void M_select(int *r, int *r1);//マジック選択
	virtual ~EnemyBoss();
};

#endif