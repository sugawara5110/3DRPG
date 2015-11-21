//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Enemyクラス                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Enemy_Header
#define Class_Enemy_Header

#include "Parameter.h"
#include "Position.h"

//前方宣言
class Battle;
//前方宣言

class Enemy :public Parameter{

protected:
	int o_no;//オブジェクトナンバー
	int e_no;//敵ナンバー
	float pos_offset;//
	Position::H_Pos *h_pos;
	Position::E_Pos *e_pos;
	Dx9Process::PolygonData en, *mag, effect;
	Dx9Process::T_xyz *p_array;//テクスチャピクセルカラーXYZ配列(ボス用)
	int ver_pcs[50];            //↑の使用するピクセルのグループ毎の個数(白以外)(ボス用)
	float mv[50];              //Z方向の各グループ座標(ボス用)
	int mag_size;             //ボス用
	bool esc;//エスケープ可否 TRUE==エスケープ可
	float mov_z;
	float mov_y;
	float mov_x;
	float size_x;
	float size_y;
	Action normal_action;//通常時のアクション
	Action act_f;
	int M_run_flg_rnd;  //マジック実行フラグ生成用ランダム
	bool up;
	bool zoom;
	int count;
	float theta_recov;
	bool effect_f;//エフェクトON,OFF
	float tx, ty;//エフェクト用テクスチャ座標

	void Enemycreate(float x, float y);
	virtual bool Magiccreate(float x, float y, float z);
	bool Effectdraw(Battle *battle, int *E_select_obj);
	void PosOffset(int o_no);

public:
	Enemy();
	Enemy(int t_no, int no);
	Act_fin_flg Enemydraw(Battle *battle, int *E_select_obj, Action action, MagicSelect E_Magrun);
	Action Normal_act_get();
	virtual bool M_run_flg();//マジック実行フラグ
	virtual void M_select(int *r, int *r1);//マジック選択
	bool s_esc();
	virtual ~Enemy();
};

#endif
