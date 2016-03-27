//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                    Menucreate関数                                   **//
//*****************************************************************************************//

#include <new>     //placement new
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"

Battle::Battle(){}

Battle::Battle(Position::E_Pos *e_po, Position::H_Pos *h_po, Encount encount, int no, int e_nu){

	dx = Dx11Process::GetInstance();
	text = DxText::GetInstance();
	e_num = e_nu;//敵出現数
	e_pos = e_po;//ポジションアドレス
	h_pos = h_po;//ポジションアドレス
	command.GetVBarray2D(1);
	h_select.GetVBarray2D(1);
	Escape_f = 0;
	Escape_s = FALSE;
	E_select.GetVBarray(SQUARE, 1);
	battlefirst = FALSE;
	battlefirsttime = 0.0f;

	srand((unsigned)time(NULL));

	int en_bgm;
	if (encount == SIDE)en_bgm = 0;
	if (encount == BOSS){
		if (no <= 2)en_bgm = 1;
		if (no == 3)en_bgm = 2;
		if (no == 4)en_bgm = 3;
	}

	MovieSoundManager::ObjCreate_battle(en_bgm);
	if (encount == SIDE){
		//通常の敵のサウンド
		MovieSoundManager::Enemy_sound(FALSE);
		//通常の敵の生成
		enemyside = new EnemySide[e_num];

		int rnd;
		//アップキャスト前に初期化
		for (int i = 0; i < e_num; i++) {
			rnd = (rand() % 4) + no * 4;
			new(enemyside + i) EnemySide(rnd, i, h_pos, e_pos);// 配列をplacement newを使って初期化する
		}

		//アップキャスト
		enemy = enemyside;
	}
	if (encount == BOSS){
		//ボスのサウンド
		MovieSoundManager::Enemy_sound(FALSE);
		//ボス生成
		enemyboss = new EnemyBoss[e_num];

		//アップキャスト前に初期化
		for (int i = 0; i < e_num; i++) {
			new(enemyboss + i) EnemyBoss(no, i, h_pos, e_pos);// 配列をplacement newを使って初期化する
		}

		//アップキャスト
		enemy = enemyboss;
	}

	command_run_first_flg = FALSE;
	time_stop_flg = FALSE;
	com_select = NOSELECT;
	E_com_select = NOSELECT;
	select_obj = 0;
	E_select_obj = 0;
	MAG_select = NOSEL;
	E_MAG_select = NOSEL;
	e_draw = new Draw[e_num];
	h_draw = new Draw[4];
	Menucreate();

	for (int i = 0; i < e_num; i++){
		e_draw[i].AGmeter = 0.0f;
		e_draw[i].action = RECOVER;
		e_draw[i].RCVdrawY = 0;
		e_draw[i].RCVdata = -1;
		e_draw[i].DMdrawY = 0;
		e_draw[i].DMdata = -1;
		e_draw[i].command_run = FALSE;
		e_draw[i].LOST_fin = FALSE;
		if (e_pos[i].element == FALSE){
			e_draw[i].LOST_fin = TRUE; enemy[i].Dieflg(TRUE);
		}
	}
	e_draw[0].draw_x = 350;
	if (e_num > 1)e_draw[1].draw_x = 150;
	if (e_num > 2)e_draw[2].draw_x = 370;
	if (e_num > 3)e_draw[3].draw_x = 650;

	for (int i = 0; i < 4; i++){
		h_draw[i].AGmeter = 0.0f;
		h_draw[i].action = NORMAL;
		h_draw[i].RCVdrawY = 0;
		h_draw[i].RCVdata = -1;
		h_draw[i].DMdrawY = 0;
		h_draw[i].DMdata = -1;
		h_draw[i].command_run = FALSE;
		h_draw[i].manu = MAIN_M;
		h_draw[i].M_select = 0;
		h_draw[i].A_select = 0;
		h_draw[i].MA_select = 0;
		h_draw[i].R_select = 0;
	}
	h_draw[0].draw_x = 20;
	h_draw[1].draw_x = 190;
	h_draw[2].draw_x = 370;
	h_draw[3].draw_x = 540;

	//ロード時間による遅延分のリセット
	T_float::GetTime();
	T_float::GetTime();
}

void Battle::Menucreate(){

	VECTOR4 clr = { 0.6f, 0.6f, 0.6f, 0.5f };

	//コマンドウインドウ
	command.d3varray[0].x = 5.0f;
	command.d3varray[0].y = 0.0f;
	command.d3varray[0].z = 0.9f;
	command.d3varray[0].color = clr;

	command.d3varray[1].x = 140.0f;
	command.d3varray[1].y = 0.0f;
	command.d3varray[1].z = 0.9f;
	command.d3varray[1].color = clr;

	command.d3varray[2].x = 5.0f;
	command.d3varray[2].y = 200.0f;
	command.d3varray[2].z = 0.9f;
	command.d3varray[2].color = clr;

	command.d3varray[3].x = 140.0f;
	command.d3varray[3].y = 200.0f;
	command.d3varray[3].z = 0.9f;
	command.d3varray[3].color = clr;
}
	
void Battle::Cursor_h(int no){

	static bool clr_f = TRUE;
	static float r = 1.0f;
	VECTOR4 clr;

	float m = tfloat.Add(0.002f);
	if (clr_f){
		if ((r -= m) <= 0)clr_f = FALSE;
	}
	else{
		if ((r += m) >= 1.0f)clr_f = TRUE;
	}
	clr.as(r, r, 0.7f, 1.0f);

	float x;
	if (no == 0)x = 10.0f;
	if (no == 1)x = 180.0f;
	if (no == 2)x = 360.0f;
	if (no == 3)x = 530.0f;

	//回復対象カーソル左
	h_select.d3varray[0].x = x - 10.0f;
	h_select.d3varray[0].y = 440.0f;
	h_select.d3varray[0].z = 0.0f;
	h_select.d3varray[0].color = clr;

	h_select.d3varray[1].x = x - 5.0f;
	h_select.d3varray[1].y = 440.0f;
	h_select.d3varray[1].z = 0.0f;
	h_select.d3varray[1].color = clr;

	h_select.d3varray[2].x = x - 10.0f;
	h_select.d3varray[2].y = 560.0f;
	h_select.d3varray[2].z = 0.0f;
	h_select.d3varray[2].color = clr;

	h_select.d3varray[3].x = x - 5.0f;
	h_select.d3varray[3].y = 560.0f;
	h_select.d3varray[3].z = 0.0f;
	h_select.d3varray[3].color = clr;
	h_select.D2primitive(TRUE, TRUE);

	//回復対象カーソル右
	h_select.d3varray[0].x = x + 125.0f;
	h_select.d3varray[0].y = 440.0f;
	h_select.d3varray[0].z = 0.0f;
	h_select.d3varray[0].color = clr;

	h_select.d3varray[1].x = x + 130.0f;
	h_select.d3varray[1].y = 440.0f;
	h_select.d3varray[1].z = 0.0f;
	h_select.d3varray[1].color = clr;

	h_select.d3varray[2].x = x + 125.0f;
	h_select.d3varray[2].y = 560.0f;
	h_select.d3varray[2].z = 0.0f;
	h_select.d3varray[2].color = clr;

	h_select.d3varray[3].x = x + 130.0f;
	h_select.d3varray[3].y = 560.0f;
	h_select.d3varray[3].z = 0.0f;
	h_select.d3varray[3].color = clr;
	h_select.D2primitive(TRUE, TRUE);

	//回復対象カーソル上
	h_select.d3varray[0].x = x - 5.0f;
	h_select.d3varray[0].y = 440.0f;
	h_select.d3varray[0].z = 0.0f;
	h_select.d3varray[0].color = clr;

	h_select.d3varray[1].x = x + 125.0f;
	h_select.d3varray[1].y = 440.0f;
	h_select.d3varray[1].z = 0.0f;
	h_select.d3varray[1].color = clr;

	h_select.d3varray[2].x = x - 5.0f;
	h_select.d3varray[2].y = 445.0f;
	h_select.d3varray[2].z = 0.0f;
	h_select.d3varray[2].color = clr;

	h_select.d3varray[3].x = x + 125.0f;
	h_select.d3varray[3].y = 445.0f;
	h_select.d3varray[3].z = 0.0f;
	h_select.d3varray[3].color = clr;
	h_select.D2primitive(TRUE, TRUE);

	//回復対象カーソル下
	h_select.d3varray[0].x = x - 5.0f;
	h_select.d3varray[0].y = 555.0f;
	h_select.d3varray[0].z = 0.0f;
	h_select.d3varray[0].color = clr;

	h_select.d3varray[1].x = x + 125.0f;
	h_select.d3varray[1].y = 555.0f;
	h_select.d3varray[1].z = 0.0f;
	h_select.d3varray[1].color = clr;

	h_select.d3varray[2].x = x - 5.0f;
	h_select.d3varray[2].y = 560.0f;
	h_select.d3varray[2].z = 0.0f;
	h_select.d3varray[2].color = clr;

	h_select.d3varray[3].x = x + 125.0f;
	h_select.d3varray[3].y = 560.0f;
	h_select.d3varray[3].z = 0.0f;
	h_select.d3varray[3].color = clr;
	h_select.D2primitive(TRUE, TRUE);
}

void Battle::Cursor_e(int select){

	static float theta = 0.0f;
	static float r = 1.0f;
	static float b = 0.0f;
	static bool clr_f = TRUE;
	float m = tfloat.Add(0.02f);
	if (clr_f){
		b += m;
		if ((r -= m) <= 0.0f)clr_f = FALSE;
	}
	else{
		b -= m;
		if ((r += m) >= 1.0f)clr_f = TRUE;
	}

	//カーソル左上
	E_select.SetVertex(0, 0,
		(float)-25.0f, (float)-25.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		r, 0.0f, b, 1.0f,
		0.0f, 0.0f);

	//カーソル右上
	E_select.SetVertex(1, 4, 1,
		(float)25.0f, (float)-25.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		r, b, 0.0f, 1.0f,
		1.0f, 0.0f);

	//カーソル左下
	E_select.SetVertex(2, 3, 2,
		(float)-25.0f, (float)25.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		b, 0.0f, r, 1.0f,
		0.0f, 1.0f);

	//カーソル右下
	E_select.SetVertex(5, 3,
		(float)25.0f, (float)25.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, r, b, 1.0f,
		1.0f, 1.0f);

	m = tfloat.Add(0.2f);
	if ((theta += m) > 360.0f)theta = 0.0f;
	E_select.D3primitive(e_pos[select].x, e_pos[select].y, e_pos[select].z, 0, 0, 0, theta, FALSE, FALSE, 0);
}

void Battle::SelectPermissionMove(Hero *hero){

	if (h_draw[0].command_run == TRUE){
		h_draw[0].command_run = FALSE;
		if (hero[1].Dieflg() == FALSE && h_draw[1].AGmeter > METER_MAX)h_draw[1].command_run = TRUE;
		else if (hero[2].Dieflg() == FALSE && h_draw[2].AGmeter > METER_MAX)h_draw[2].command_run = TRUE;
		else if (hero[3].Dieflg() == FALSE && h_draw[3].AGmeter > METER_MAX)h_draw[3].command_run = TRUE;
		else if (hero[0].Dieflg() == FALSE && h_draw[0].AGmeter > METER_MAX)h_draw[0].command_run = TRUE;
		else command_run_first_flg = FALSE;
		return;
	}
	if (h_draw[1].command_run == TRUE){
		h_draw[1].command_run = FALSE;
		if (hero[2].Dieflg() == FALSE && h_draw[2].AGmeter > METER_MAX)h_draw[2].command_run = TRUE;
		else if (hero[3].Dieflg() == FALSE && h_draw[3].AGmeter > METER_MAX)h_draw[3].command_run = TRUE;
		else if (hero[0].Dieflg() == FALSE && h_draw[0].AGmeter > METER_MAX)h_draw[0].command_run = TRUE;
		else if (hero[1].Dieflg() == FALSE && h_draw[1].AGmeter > METER_MAX)h_draw[1].command_run = TRUE;
		else command_run_first_flg = FALSE;
		return;
	}
	if (h_draw[2].command_run == TRUE){
		h_draw[2].command_run = FALSE;
		if (hero[3].Dieflg() == FALSE && h_draw[3].AGmeter > METER_MAX)h_draw[3].command_run = TRUE;
		else if (hero[0].Dieflg() == FALSE && h_draw[0].AGmeter > METER_MAX)h_draw[0].command_run = TRUE;
		else if (hero[1].Dieflg() == FALSE && h_draw[1].AGmeter > METER_MAX)h_draw[1].command_run = TRUE;
		else if (hero[2].Dieflg() == FALSE && h_draw[2].AGmeter > METER_MAX)h_draw[2].command_run = TRUE;
		else command_run_first_flg = FALSE;
		return;
	}
	if (h_draw[3].command_run == TRUE){
		h_draw[3].command_run = FALSE;
		if (hero[0].Dieflg() == FALSE && h_draw[0].AGmeter > METER_MAX)h_draw[0].command_run = TRUE;
		else if (hero[1].Dieflg() == FALSE && h_draw[1].AGmeter > METER_MAX)h_draw[1].command_run = TRUE;
		else if (hero[2].Dieflg() == FALSE && h_draw[2].AGmeter > METER_MAX)h_draw[2].command_run = TRUE;
		else if (hero[3].Dieflg() == FALSE && h_draw[3].AGmeter > METER_MAX)h_draw[3].command_run = TRUE;
		else command_run_first_flg = FALSE;
		return;
	}
}

Battle::~Battle(){

	MovieSoundManager::ObjDelete_battle();
	delete[] enemy;
	enemy = NULL;
	delete[] e_draw;
	e_draw = NULL;
	delete[] h_draw;
	h_draw = NULL;
}