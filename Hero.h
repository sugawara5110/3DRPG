//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Hero_Header
#define Class_Hero_Header

#include "Parameter.h"
#include "Position.h"

//前方宣言
class Battle;
//前方宣言

class Hero:public Parameter{

protected:
	int o_no;//オブジェクトナンバー
	Dx11Process::PolygonData2D state, meter;
	Dx11Process::PolygonData mag, effect;
	float mov_y;
	float mov_x;
	Action act_f;
	bool up;
	float count;
	bool effect_f;//エフェクトON,OFF
	float tx, ty;//エフェクト用テクスチャ座標
	float tt;     //エフェクト速度調整用

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