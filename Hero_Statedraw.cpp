//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                   Statedraw関数                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dx9Process.h"
#include "Hero.h"
#include "Battle.h"

Act_fin_flg Hero::Statedraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos, float me, bool command_run, Action action, MagicSelect H_Magrun){

	char str[30];
	static bool clr_f = TRUE;
	static DWORD r = 0x99;
	float x;
	if (o_no == 0)x = 20.0f;
	if (o_no == 1)x = 190.0f;
	if (o_no == 2)x = 370.0f;
	if (o_no == 3)x = 540.0f;

	//NORMAL以外のアクション中にNORMAL以外のアクション発生時の初期化
	if (action != NORMAL){
		mov_y = 0.0f;
		mov_x = 0.0f;
		up = TRUE;
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
	case RECOVER:
		act_f = NORMAL;
		break;
	}

	switch (act_f){
	case ATTACK:
		if (effect_f == FALSE && up == TRUE && (mov_y -= 1.0f) < -30.0f)up = FALSE;
		if (effect_f == FALSE && up == FALSE && (mov_y += 1.0f) > 0.0f){
			up = TRUE;
			mov_y = 0.0f;
			effect_f = TRUE;
		}
		if (effect_f == TRUE){
			effect.tex_no = 0;
			if (Effectdraw(battle, select_obj, h_pos, e_pos) == FALSE){
				effect_f = FALSE;
				act_f = NORMAL;
				return AT_FIN;
			}
		}
		break;
	case MAGIC:
		int mx, my;
		MovieSoundManager::Magic_sound(TRUE);
		dx->D3primitive(SQUARE, &mag, 1, h_pos->cx1, h_pos->cy1, (float)h_pos->pz * 100.0f + 25.0f, (float)count++, TRUE, FALSE, FALSE);
		switch (o_no){
		case 0:
			mx = 0;
			my = 400;
			break;
		case 1:
			mx = 170;
			my = 400;
			break;
		case 2:
			mx = 350;
			my = 400;
			break;
		case 3:
			mx = 520;
			my = 400;
			break;
		}
		switch (H_Magrun){
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
		if (effect_f == FALSE && up == TRUE && (mov_y -= 1.0f) < -30.0f)up = FALSE;
		if (effect_f == FALSE && up == FALSE && (mov_y += 1.0f) > 0.0f){
			up = TRUE;
			mov_y = 0.0f;
			count = 0;
			if (H_Magrun == NOSEL){
				act_f = NORMAL;
				return AT_FIN;
			}
			effect_f = TRUE;
		}
		if (effect_f == TRUE){
			if (Effectdraw(battle, select_obj, h_pos, e_pos) == FALSE){
				effect_f = FALSE;
				act_f = NORMAL;
				return AT_FIN;
			}
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
		}
		else {
			count = 0;
			act_f = NORMAL;
			mov_x = mov_y = 0.0f;
		}
		break;
	}

	Statecreate(command_run);
	sprintf(str, "囚人№ %d", o_no);
	dx->text(str, (int)(x + mov_x), (int)(470 + mov_y), TRUE, 0xff00ffff);
	sprintf(str, "HP %d/%d", p_data.HP, s_MHP());
	dx->text(str, (int)(x + mov_x) - 5, (int)(490 + mov_y), TRUE, 0xff00ffff);
	sprintf(str, "MP %d/%d", p_data.MP, s_MMP());
	dx->text(str, (int)(x + mov_x) - 5, (int)(510 + mov_y), TRUE, 0xff00ffff);
	if (p_data.HP <= 0){
		dx->text("戦闘不能", (int)(x + mov_x), (int)(525 + mov_y), FALSE, 0xffff0000);
		return NOT_FIN;
	}
	if (me >= 1.0f){
		if (clr_f){
			if ((r -= 1) <= 0x77)clr_f = FALSE;
		}
		else{
			if ((r += 1) >= 0xff)clr_f = TRUE;
		}
		dx->text("臨戦態勢", (int)(x + mov_x), (int)(525 + mov_y), FALSE, 0x00ffffff + (r << 24));
		return NOT_FIN;
	}
	Metercreate(me);

	return NOT_FIN;
}

void Hero::Act_f_init(){
	act_f = NORMAL;
}