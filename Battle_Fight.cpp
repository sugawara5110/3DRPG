//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                      Fight関数                                      **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <new>     //placement new
#include <stdlib.h>
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"

Result Battle::Fight(Hero *hero, Directionkey direction, Result result){

	float view_rev = 80.0f - (battlefirsttime / 12.5f);
	float view_x = 0.0f;
	float view_y = 0.0f;
	switch ((int)h_pos->theta){
	case 360:
	case 0:
		view_y = -view_rev;
		break;
	case 90:
		view_x = view_rev;
		break;
	case 180:
		view_y = view_rev;
		break;
	case 270:
		view_x = -view_rev;
		break;
	}
	float cx1 = h_pos->cx1 + h_pos->cx1 - h_pos->cx2 + view_x;//視点
	float cy1 = h_pos->cy1 + h_pos->cy1 - h_pos->cy2 + view_y;//視点
	float cx2 = h_pos->cx2 + view_x;//注視点
	float cy2 = h_pos->cy2 + view_y;//注視点
	float cz = h_pos->pz * 100.0f + 35.0f;
	int i1 = -1;
	for (int i = 0; i < 4; i++){
		if (h_draw[i].command_run == TRUE){
			i1 = i;
			break;
		}
	}
	if (i1 != -1){
		cx1 = h_pos->BtPos_x[i1] + h_pos->BtPos_x[i1] - h_pos->BtPos_x1[i1];
		cy1 = h_pos->BtPos_y[i1] + h_pos->BtPos_y[i1] - h_pos->BtPos_y1[i1];
		cx2 = h_pos->BtPos_x1[i1];
		cy2 = h_pos->BtPos_y1[i1];
	}

	dx->Cameraset(cx1, cx2, cy1, cy2, cz);
	Act_fin_flg act;
	//敵戦闘不能アクションフラグ
	//LOSTは発生したらずっとなので注意
	for (int i = 0; i < e_num; i++)if (enemy[i].Dieflg() == TRUE)e_draw[i].action = LOST;
	//敵表示
	for (int i = 0; i < e_num; i++){
		if (e_draw[i].LOST_fin == FALSE){//敵側はLOST_fin==TRUE状態だと表示されないようにする
			act = enemy[i].Enemydraw(this, &E_select_obj, e_draw[i].action, e_draw[i].Magrun);
			switch (act){
			case AT_FIN://敵側攻撃,回復終了
				ValueDraw<Enemy>(enemy, h_draw, e_draw, 4, e_num);
				break;
			case LOST_FIN://敵側戦闘不能アクション終了
				e_draw[i].LOST_fin = TRUE;
				break;
			}
		}
		e_draw[i].action = enemy[i].Normal_act_get();//↑actionは一度入れればOKな構造なので入れたら即初期化する。これやらないとずっとactionしっぱなし。
	}

	//プレイヤー戦闘不能アクションフラグ
	//LOSTは発生したらずっとなので注意
	for (int i = 0; i < 4; i++)if (hero[i].Dieflg() == TRUE)h_draw[i].action = LOST;
	//プレイヤーステータス表示
	for (int i = 0; i < 4; i++){
		act = hero[i].Statedraw(this, &select_obj, h_pos, e_pos, h_draw[i].AGmeter / METER_MAX, h_draw[i].command_run, h_draw[i].action, h_draw[i].Magrun);
		switch (act){
		case AT_FIN://プレイヤー側攻撃,回復終了
			ValueDraw<Hero>(hero, e_draw, h_draw, e_num, 4);
			break;
		case LOST_FIN:
			h_draw[i].LOST_fin = TRUE;
			break;
		}
		h_draw[i].action = NORMAL;
	}

	//Debug(enemy);//デバック用

	//GAMEOVER時ループスキップ
	if (result == DIE){
		MovieSoundManager::Die_sound(TRUE);
		return DIE;
	}

	MovieSoundManager::Enemy_sound(TRUE);

	//戦闘開始時間
	if (battlefirst == FALSE){
		if ((battlefirsttime += tfloat.Add(1.0f)) < 1000.0f)return IN_BATTLE;
		battlefirst = TRUE;
	}

	//敵攻撃パターン決定
	E_com_select = E_AT_select(hero);//行動は即決定するがテンプレート関数使用の関係でプレイヤー側と合わせている。

	//敵攻撃
	if (time_stop_flg == FALSE && E_com_select != NOSELECT && E_com_select != OTHER){//上に同じ
		//数値保管初期化
		for (int i = 0; i < 4; i++)h_draw[i].DMdata = -1;
		for (int i = 0; i < e_num; i++)e_draw[i].RCVdata = -1;
		
		if (E_com_select == ATT){
			for (int i = 0; i < e_num; i++){
				if (enemy[i].Dieflg() == FALSE && e_draw[i].command_run == TRUE){
					//↓time_stop始まり
					ATprocess<Hero, Enemy>(&hero[E_select_obj], &enemy[i], &h_draw[E_select_obj], &e_draw[i]);
					break;
				}
			}
		}
		if (E_com_select == MAG){
			for (int i = 0; i < e_num; i++){
				if (enemy[i].Dieflg() == FALSE && e_draw[i].command_run == TRUE){
					//↓time_stop始まり
					MAGprocess<Hero, Enemy>(hero, enemy, &enemy[i], &e_draw[i], e_draw, h_draw, &E_select_obj, &E_MAG_select, E_ATT);
					break;
				}
			}
		}
		E_com_select = NOSELECT;
	}
	//敵回復
	for (int i = 0; i < e_num; i++){
		E_drawPos(i);
		RCVdraw<Enemy>(&enemy[i], &e_draw[i], -50.0f, 0.0f);//←time_stop終わり
	}
	//プレイヤーダメージ
	for (int i = 0; i < 4; i++){
		H_drawPos(i);
		DMdraw<Hero>(&hero[i], &h_draw[i], -100.0f, 0.0f);//←time_stop終わり
	}

	//プレイヤー全滅
	if (hero[0].Dieflg() == TRUE && hero[1].Dieflg() == TRUE && hero[2].Dieflg() == TRUE && hero[3].Dieflg() == TRUE){
		MovieSoundManager::Enemy_soundoff();
		MovieSoundManager::Die_sound(FALSE);
		SelectPermissionMove(hero); return DIE;
	}

	//戦闘不能時コマンド選択権破棄,アクション初期化これやらないと戦闘不能者の所でコマンド入力止まったまま
	for (int i = 0; i < 4; i++){
		if (hero[i].Dieflg() == TRUE){
			if (h_draw[i].command_run == TRUE){
				SelectPermissionMove(hero);
				com_select = NOSELECT;
			}
			hero[i].Act_f_init();
		}
	}

	//コマンド入力
	if (com_select == NOSELECT || com_select == OTHER){//敵行動中でも選択可能状態に入ってる場合は選択のみ実行できる
		for (int i = 0; i < 4; i++){
			com_select = H_AT_select(hero, i, direction);
			if (com_select != NOSELECT)break;
		}
	}

	//プレイヤー攻撃
	if (time_stop_flg == FALSE && com_select != NOSELECT && com_select != OTHER){//選択済みの場合実行される
		//数値保管初期化
		for (int i = 0; i < e_num; i++)e_draw[i].DMdata = -1;
		for (int i = 0; i < 4; i++)h_draw[i].RCVdata = -1;
		
		if (com_select == ATT){
			for (int i = 0; i < 4; i++){
				if (hero[i].Dieflg() == FALSE && h_draw[i].command_run == TRUE){
					//↓time_stop始まり
					ATprocess<Enemy, Hero>(&enemy[select_obj], &hero[i], &e_draw[select_obj], &h_draw[i]);
					//選択権移動これやらないと他のメンバがコマンド選択できない
					SelectPermissionMove(hero);
					break;
				}
			}
		}
		if (com_select == MAG){
			for (int i = 0; i < 4; i++){
				if (hero[i].Dieflg() == FALSE && h_draw[i].command_run == TRUE){
					//↓time_stop始まり
					MAGprocess<Enemy, Hero>(enemy, hero, &hero[i], &h_draw[i], h_draw, e_draw, &select_obj, &MAG_select, H_ATT);
					//選択権移動これやらないと他のメンバがコマンド選択できない
					SelectPermissionMove(hero);
					break;
				}
			}
		}
		if (com_select == ESCAPE){
			int h_esc = 0;
			int e_esc = 0;
			for (int i = 0; i < 4; i++){
				if (hero[i].Dieflg() == TRUE)continue;
				h_esc += hero[i].s_AG();
			}
			for (int i = 0; i < e_num; i++){
				if (enemy[i].Dieflg() == TRUE)continue;
				e_esc += enemy[i].s_AG();
			}
			//エスケープ可否確認
			bool es = TRUE;
			for (int i = 0; i < e_num; i++){
				if (enemy[i].Dieflg() == TRUE)continue;
				if (enemy[i].s_esc() == FALSE){
					es = FALSE;
					break;
				}
			}
			if (es == TRUE && h_esc > e_esc)Escape_s = TRUE;
			else Escape_s = FALSE;
			Escape_f = 300;
			SelectPermissionMove(hero);
			for (int i = 0; i < 4; i++)h_draw[i].AGmeter = 0;
		}
		com_select = NOSELECT;
	}
	//プレイヤー回復
	for (int i = 0; i < 4; i++){
		H_drawPos(i);
		RCVdraw<Hero>(&hero[i], &h_draw[i], -100.0f, 0.0f);//←time_stop終わり
	}
	//敵ダメージ
	for (int i = 0; i < e_num; i++){
		E_drawPos(i);
		DMdraw<Enemy>(&enemy[i], &e_draw[i], -50.0f, 0.0f);//←time_stop終わり
	}

	//エスケープ表示
	if (Escapedraw() == FALSE)if (Escape_s == TRUE)return WIN;

	//プレイヤー全部メーター貯まってない状態での選択権破棄
	if (h_draw[0].AGmeter <= METER_MAX && h_draw[1].AGmeter <= METER_MAX && h_draw[2].AGmeter <= METER_MAX && h_draw[3].AGmeter <= METER_MAX){
		SelectPermissionMove(hero);
	}

	//敵全滅→Exp処理
	if (e_draw[0].LOST_fin == TRUE &&
		(e_pos[1].element == FALSE || e_draw[1].LOST_fin == TRUE) &&
		(e_pos[2].element == FALSE || e_draw[2].LOST_fin == TRUE) &&
		(e_pos[3].element == FALSE || e_draw[3].LOST_fin == TRUE)){
		int GetExp = 0;
		for (int i = 0; i < e_num; i++){
			if (e_pos[i].element == TRUE)GetExp += enemy[i].s_Exp();
		}
		for (int i = 0; i < 4; i++){
			if (hero[i].Dieflg() == TRUE)continue;
			hero[i].s_Exp(GetExp);
		}
		for (int i = 0; i < 4; i++){
			hero[i].Act_f_init();
		}
		return WIN;
	}

	return IN_BATTLE;
}

void Battle::E_drawPos(int i){
	VECTOR3 p3;
	p3.as(e_pos[i].x, e_pos[i].y, e_pos[i].z);
	PolygonData2D::Pos2DCompute(&p3);
	e_draw[i].draw_x = p3.x;
	e_draw[i].draw_y = p3.y;
}

void Battle::H_drawPos(int i){
	VECTOR3 p3;
	p3.as(h_pos->BtPos_x[i], h_pos->BtPos_y[i], h_pos->pz * 100.0f);
	PolygonData2D::Pos2DCompute(&p3);
	h_draw[i].draw_x = p3.x;
	h_draw[i].draw_y = p3.y;
}

bool Battle::Escapedraw(){

	float m = tfloat.Add(0.1f);
	if (Escape_f != 0){
		if ((Escape_f += m) < 330){
			if (Escape_s == TRUE)text->Drawtext(L"エスケープ成功", 300.0f, 300.0f, 35.0f, { 0.0f, 0.7f, 1.0f, 1.0f });
			if (Escape_s == FALSE)text->Drawtext(L"エスケープ失敗", 300.0f, 300.0f, 35.0f, { 0.0f, 0.7f, 1.0f, 1.0f });
			return TRUE;
		}
		else {
			Escape_f = 0;
			return FALSE;
		}
	}
	return TRUE;
}

bool Battle::GetH_DM(int element){

	if (h_draw[element].DMdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetH_RCV(int element){

	if (h_draw[element].RCVdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetE_DM(int element){

	if (e_pos[element].element == FALSE)return FALSE;
	if (e_draw[element].DMdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetE_RCV(int element){

	if (e_pos[element].element == FALSE)return FALSE;
	if (e_draw[element].RCVdata >= 0)return TRUE;

	return FALSE;
}

void Battle::Debug(Enemy *enemy){//デバック用

	for (int i = 0; i < 4; i++){
		if (e_pos[i].element == FALSE)continue;
		text->DrawValue(enemy[i].s_HP(), 500.0f, 10.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->DrawValue(enemy[i].s_MHP(), 580.0f, 10.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->DrawValue(enemy[i].s_MP(), 500.0f, 25.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->DrawValue(enemy[i].s_MMP(), 580.0f, 25.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
		text->DrawValue((int)e_draw[i].AGmeter, 500.0f, 40.0f + 90.0f * i, 15.0f, 5, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}