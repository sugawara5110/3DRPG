//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Hero�N���X                                      **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Hero_Header
#define Class_Hero_Header

#include "Parameter.h"
#include "Position.h"

//�O���錾
class Battle;
//�O���錾

class Hero:public Parameter{

protected:
	int o_no;//�I�u�W�F�N�g�i���o�[
	Dx11Process::PolygonData2D state, meter;
	Dx11Process::PolygonData mag, effect;
	float mov_y;
	float mov_x;
	Action act_f;
	bool up;
	float count;
	bool effect_f;//�G�t�F�N�gON,OFF
	float tx, ty;//�G�t�F�N�g�p�e�N�X�`�����W
	float tt;     //�G�t�F�N�g���x�����p

	void Statecreate(bool command_run);
	void Metercreate(float me);
	void Magiccreate();
	bool Effectdraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos);

public:
	Hero();
	Hero(P_Data *p_dat, int no);
	Act_fin_flg Statedraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos, float me, bool command_run, Action action, MagicSelect H_Magrun);
	void Act_f_init();
	virtual ~Hero();
};

#endif