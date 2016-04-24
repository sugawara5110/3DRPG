//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Enemyクラス                                     **//
//**                                    Enemydraw関数                                    **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Enemy.h"
#include "Battle.h"

Act_fin_flg Enemy::Enemydraw(Battle *battle, int *E_select_obj, Action action, MagicSelect E_Magrun){

	//MOVE,LOST以外のアクション中にMOVE,LOST以外のアクション発生時の初期化
	if (action != NORMAL && action != MOVE && action != LOST){
		mov_z = 0.0f;
		mov_y = 0.0f;
		mov_x = 0.0f;
		up = TRUE;
		zoom = TRUE;
		count = 0;
	}

	switch (action){
	case ATTACK:
		act_f = ATTACK;
		break;

	case MAGIC:
		act_f = MAGIC;
		break;

	case DAMAGE:
		if (act_f == ATTACK || act_f == MAGIC)break;
		act_f = DAMAGE;
		break;

	case LOST:
		act_f = LOST;
		break;

	case RECOVER:
		RecoverActionInit();
		break;
	}

	float m;
	switch (act_f){
	case MOVE:
		m = tfloat.Add(0.005f);
		if (up == TRUE && (mov_z -= m) < -4.0f)up = FALSE;
		if (up == FALSE && (mov_z += m) > 0.0f){
			up = TRUE; mov_z = 0.0f;
		}
		break;

	case ATTACK:
		AttackAction();
		if (effect_f == TRUE){
			effect.tex_no = 0;
			if (Effectdraw(battle, E_select_obj) == FALSE){
				effect_f = FALSE;
				act_f = normal_action;
				return AT_FIN;
			}
		}
		break;

	case MAGIC:
		float mx, my;
		switch (o_no){
		case 0:
			mx = 400;
			my = 200;
			break;
		case 1:
			mx = 200;
			my = 200;
			break;
		case 2:
			mx = 400;
			my = 170;
			break;
		case 3:
			mx = 600;
			my = 200;
			break;
		}
		switch (E_Magrun){
		case FLAME:
			text->Drawtext(L"フレイムＬＶ", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->Drawtext(text->CreateTextValue(GetFlameATT_LV()), mx + 180.0f, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect.tex_no = 1;
			break;
		case HEAL:
			text->Drawtext(L"ヒーリングＬＶ", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->Drawtext(text->CreateTextValue(GetHealing_LV()), mx + 210.0f, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect.tex_no = 2;
			break;
		case RECOV:
			text->Drawtext(L"リカバリＬＶ", mx, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			text->Drawtext(text->CreateTextValue(GetRecover_LV()), mx + 180.0f, my, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			effect.tex_no = 3;
			break;
		case NOSEL:
			text->Drawtext(L"MPが足りない", mx, my, 30.0f, { 1.0f, 0.5f, 0.5f, 1.0f });
			break;
		}

		if (effect_f == FALSE && Magiccreate(e_pos[o_no].x, e_pos[o_no].y, e_pos[o_no].z) == FALSE){
			if (E_Magrun == NOSEL){
				act_f = normal_action;
				return AT_FIN;
			}
			effect_f = TRUE;
		}
		if (effect_f == TRUE && Effectdraw(battle, E_select_obj) == FALSE){
			effect_f = FALSE;
			act_f = normal_action;
			return AT_FIN;
		}
		break;

	case DAMAGE:
		DamageAction();
		break;

	case RECOVER:
		RecoverAction();
		break;

	case LOST:
		if (LostAction(e_pos[o_no].x, e_pos[o_no].y, e_pos[o_no].z) == TRUE){
			return LOST_FIN;
		}
		break;
	}
	dx->P_ShadowBright(0.3f);
	ObjDraw(e_pos[o_no].x + mov_x, e_pos[o_no].y + mov_y, e_pos[o_no].z + mov_z, cr, cg, cb, e_pos[o_no].theta);

	return NOT_FIN;
}

Action Enemy::Normal_act_get(){

	return normal_action;
}

bool Enemy::M_run_flg(){
	return FALSE;
}

void Enemy::M_select(int *r, int *r1){}

bool Enemy::s_esc(){
	return esc;
}

void Enemy::PosOffset(int o_no){

	if (e_pos[0].theta == 0 || e_pos[0].theta == 360)e_pos[o_no].y -= pos_offset;
	if (e_pos[0].theta == 90)e_pos[o_no].x += pos_offset;
	if (e_pos[0].theta == 180)e_pos[o_no].y += pos_offset;
	if (e_pos[0].theta == 270)e_pos[o_no].x -= pos_offset;
}