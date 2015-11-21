//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Enemyクラス                                     **//
//**                                    Enemydraw関数                                    **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Dx9Process.h"
#include "Enemy.h"
#include "Battle.h"

Act_fin_flg Enemy::Enemydraw(Battle *battle, int *E_select_obj, Action action, MagicSelect E_Magrun){

	char str[30];
	//MOVE,LOST以外のアクション中にMOVE,LOST以外のアクション発生時の初期化
	if (action != NORMAL && action != MOVE && action != LOST){
		mov_z = 0.0f;
		mov_y = 0.0f;
		mov_x = 0.0f;
		up = TRUE;
		zoom = TRUE;
		count = 0;
		theta_recov = 0;
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
		theta_recov = 90.0f;
		act_f = RECOVER;
		break;
	}

	switch (act_f){
	case MOVE:
		if (up == TRUE && (mov_z -= 0.05f) < -4.0f)up = FALSE;
		if (up == FALSE && (mov_z += 0.05f) > 0.0f){
			up = TRUE; mov_z = 0.0f;
		}
		break;

	case ATTACK:
		if (effect_f == FALSE && (e_pos[o_no].theta >= 338.0f || e_pos[o_no].theta <= 22.0f)){
			if (zoom == TRUE && (mov_y += 1.0f) > 30.0f)zoom = FALSE;
			if (zoom == FALSE && (mov_y -= 1.0f) < 0.0f){
				zoom = TRUE;
				mov_y = 0.0f;
				effect_f = TRUE;
			}
		}
		if (effect_f == FALSE && e_pos[o_no].theta >= 68.0f && e_pos[o_no].theta <= 112.0f){
			if (zoom == TRUE && (mov_x -= 1.0f) < -30.0f)zoom = FALSE;
			if (zoom == FALSE && (mov_x += 1.0f) > 0.0f){
				zoom = TRUE;
				mov_y = 0.0f;
				effect_f = TRUE;
			}
		}
		if (effect_f == FALSE && e_pos[o_no].theta >= 158.0f && e_pos[o_no].theta <= 202.0f){
			if (zoom == TRUE && (mov_y -= 1.0f) < -30.0f)zoom = FALSE;
			if (zoom == FALSE && (mov_y += 1.0f) > 0.0f){
				zoom = TRUE;
				mov_y = 0.0f;
				effect_f = TRUE;
			}
		}
		if (effect_f == FALSE && e_pos[o_no].theta >= 248.0f && e_pos[o_no].theta <= 292.0f){
			if (zoom == TRUE && (mov_x += 1.0f) > 30.0f)zoom = FALSE;
			if (zoom == FALSE && (mov_x -= 1.0f) < 0.0f){
				zoom = TRUE;
				mov_y = 0.0f;
				effect_f = TRUE;
			}
		}
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
		int mx, my;
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
			my = 200;
			break;
		case 3:
			mx = 600;
			my = 200;
			break;
		}
		switch (E_Magrun){
		case FLAME:
			sprintf(str, "フレイム LV%d", GetFlameATT_LV());
			effect.tex_no = 1;
			break;
		case HEAL:
			sprintf(str, "ヒーリング LV%d", GetHealing_LV());
			effect.tex_no = 2;
			break;
		case RECOV:
			sprintf(str, "リカバリ LV%d", GetRecover_LV());
			effect.tex_no = 3;
			break;
		case NOSEL:
			sprintf(str, "MPが足りない");
			break;
		}
		dx->text(str, mx, my, FALSE, 0xffffffff);

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
		if (count++ < 10){
			int rnd = rand() % 20;
			rnd -= 10;
			mov_x = (float)rnd;
			rnd = rand() % 20;
			rnd -= 10;
			mov_y = (float)rnd;
			rnd = rand() % 20;
			rnd -= 10;
			mov_z = (float)rnd;
		}
		else {
			count = 0;
			act_f = normal_action;
			mov_x = mov_y = mov_z = 0.0f;
		}
		break;

	case RECOVER:
		if (theta_recov-- < 0){
			theta_recov = 0;
			act_f = normal_action;
		}
		break;

	case LOST:
		if (mov_z-- < -100.0f){
			mov_z = 0.0f; return LOST_FIN;
		}
		break;
	}

	dx->D3primitive(SQUARE, &en, 1, e_pos[o_no].x + mov_x, e_pos[o_no].y + mov_y, e_pos[o_no].z + mov_z, e_pos[o_no].theta + theta_recov, TRUE, FALSE, FALSE);

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