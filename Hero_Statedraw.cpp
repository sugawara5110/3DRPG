//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                   Statedraw関数                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Hero.h"
#include "Battle.h"

Act_fin_flg Hero::Statedraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos, float me, bool command_run, Action action, MagicSelect H_Magrun){

	static bool clr_f = TRUE;
	static float r = 1.0f;
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

	float m;
	switch (act_f){
	case ATTACK:
		m = tfloat.Add(0.15f);
		if (effect_f == FALSE && up == TRUE && (mov_y -= m) < -30.0f)up = FALSE;
		if (effect_f == FALSE && up == FALSE && (mov_y += m) > 0.0f){
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
		m = tfloat.Add(0.15f);
		float mx, my;
		MovieSoundManager::Magic_sound(TRUE);
		mag.D3primitive(h_pos->cx1, h_pos->cy1, (float)h_pos->pz * 100.0f + 25.0f, 0, 0, 0, count += m, TRUE, FALSE, 0);
		switch (o_no){
		case 0:
			mx = 0.0f;
			my = 400.0f;
			break;
		case 1:
			mx = 170.0f;
			my = 400.0f;
			break;
		case 2:
			mx = 350.0f;
			my = 400.0f;
			break;
		case 3:
			mx = 520.0f;
			my = 400.0f;
			break;
		}
		switch (H_Magrun){
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
		if (effect_f == FALSE && up == TRUE && (mov_y -= m) < -30.0f)up = FALSE;
		if (effect_f == FALSE && up == FALSE && (mov_y += m) > 0.0f){
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
		m = tfloat.Add(0.05f);
		if ((count += m) < 10){
			int rnd = rand() % 20;
			rnd -= 10;
			mov_x = (float)rnd;
			rnd = rand() % 20;
			rnd -= 10;
			mov_y = (float)rnd;
		}
		else {
			count = 0.0f;
			act_f = NORMAL;
			mov_x = mov_y = 0.0f;
		}
		break;
	}

	Statecreate(command_run);
	text->Drawtext(L"囚人Ｎｏ", x + mov_x, 470.0f + mov_y, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->DrawValue(o_no, x + 60.0f + mov_x, 470.0f + mov_y, 15.0f, 1, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"ＨＰ", x + mov_x - 5.0f, 490.0f + mov_y, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->DrawValue(p_data.HP, x + mov_x + 25.0f, 490.0f + mov_y, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"ＭＰ", x + mov_x - 5.0f, 510.0f + mov_y, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->DrawValue(p_data.MP, x + mov_x + 25.0f, 510.0f + mov_y, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	if (p_data.HP <= 0){
		text->Drawtext(L"戦闘不能", x + mov_x, 525.0f + mov_y, 25.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
		return NOT_FIN;
	}
	if (me >= 1.0f){
		m = tfloat.Add(0.001f);
		if (clr_f){
			if ((r -= m) <= 0.5f)clr_f = FALSE;
		}
		else{
			if ((r += m) >= 1.0f)clr_f = TRUE;
		}
		text->Drawtext(L"臨戦態勢", x + mov_x, 525.0f + mov_y, 25.0f, { 1.0f, 1.0f, 1.0f, r });
		return NOT_FIN;
	}
	Metercreate(me);

	return NOT_FIN;
}

void Hero::Act_f_init(){
	act_f = NORMAL;
}