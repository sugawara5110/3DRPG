//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Battleクラス                                    **//
//**                                    Menucreate関数                                   **//
//*****************************************************************************************//

#include "Dx9Process.h"
#include <new>     //placement new
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"

Battle::Battle(){}

Battle::Battle(Position::E_Pos *e_po, Position::H_Pos *h_po, Encount encount, int no){

	dx = Dx9Process::GetInstance();
	e_pos = e_po;//ポジションアドレス
	h_pos = h_po;//ポジションアドレス
	command = (Dx9Process::MY_VERTEX2*)malloc(sizeof(Dx9Process::MY_VERTEX2) * 4);
	h_select = (Dx9Process::MY_VERTEX2*)malloc(sizeof(Dx9Process::MY_VERTEX2) * 4);
	Escape_f = 0;
	Escape_s = FALSE;
	dx->GetVBarray(SQUARE, &E_select, 1);

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
		enemyside = new EnemySide[4];

		//アップキャスト前に初期化
		for (int i = 0; i < 4; i++) {
			int rnd = rand() % 4 + no * 4;
			new(enemyside + i) EnemySide(rnd, i, h_pos, e_pos);// 配列をplacement newを使って初期化する
		}

		//アップキャスト
		enemy = enemyside;
	}
	if (encount == BOSS){
		//ボスのサウンド
		MovieSoundManager::Enemy_sound(FALSE);
		//ボス生成
		enemyboss = new EnemyBoss[4];

		//アップキャスト前に初期化
		for (int i = 0; i < 4; i++) {
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
	e_draw = new Draw[4];
	h_draw = new Draw[4];
	Menucreate();

	for (int i = 0; i < 4; i++){
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
	e_draw[1].draw_x = 150;
	e_draw[2].draw_x = 370;
	e_draw[3].draw_x = 650;

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
}

void Battle::Menucreate(){

	//コマンドウインドウ
	command[0].x = 0.0f;
	command[0].y = 0.0f;
	command[0].z = 0.0f;
	command[0].rhw = 0.0f;
	command[0].color = (0 << 16) + (0 << 8) + 200;
	command[0].tu = 0.0f;
	command[0].tv = 0.0f;

	command[1].x = 0.0f;
	command[1].y = 200.0f;
	command[1].z = 0.0f;
	command[1].rhw = 0.0f;
	command[1].color = (0 << 16) + (0 << 8) + 200;
	command[1].tu = 0.0f;
	command[1].tv = 0.0f;

	command[2].x = 130.0f;
	command[2].y = 200.0f;
	command[2].z = 0.0f;
	command[2].rhw = 0.0f;
	command[2].color = (0 << 16) + (0 << 8) + 200;
	command[2].tu = 0.0f;
	command[2].tv = 0.0f;

	command[3].x = 130.0f;
	command[3].y = 0.0f;
	command[3].z = 0.0f;
	command[3].rhw = 0.0f;
	command[3].color = (0 << 16) + (0 << 8) + 200;
	command[3].tu = 0.0f;
	command[3].tv = 0.0f;
}

void Battle::Cursor_h(int no){

	static bool clr_f = TRUE;
	static int r = 255;
	D3DCOLOR clr;

	if (clr_f){
		if ((r -= 5) <= 0)clr_f = FALSE;
	}
	else{
		if ((r += 5) >= 255)clr_f = TRUE;
	}
	clr = (r << 16) + (r << 8) + 200;

	float x;
	if (no == 0)x = 0.0f;
	if (no == 1)x = 170.0f;
	if (no == 2)x = 350.0f;
	if (no == 3)x = 520.0f;

	//回復対象カーソル左
	h_select[0].x = x - 10.0f;
	h_select[0].y = 440.0f;
	h_select[0].z = 0.0f;
	h_select[0].rhw = 0.0f;
	h_select[0].color = clr;
	h_select[0].tu = 0.0f;
	h_select[0].tv = 0.0f;

	h_select[1].x = x - 10.0f;
	h_select[1].y = 560.0f;
	h_select[1].z = 0.0f;
	h_select[1].rhw = 0.0f;
	h_select[1].color = clr;
	h_select[1].tu = 0.0f;
	h_select[1].tv = 0.0f;

	h_select[2].x = x - 5.0f;
	h_select[2].y = 560.0f;
	h_select[2].z = 0.0f;
	h_select[2].rhw = 0.0f;
	h_select[2].color = clr;
	h_select[2].tu = 0.0f;
	h_select[2].tv = 0.0f;

	h_select[3].x = x - 5.0f;
	h_select[3].y = 440.0f;
	h_select[3].z = 0.0f;
	h_select[3].rhw = 0.0f;
	h_select[3].color = clr;
	h_select[3].tu = 0.0f;
	h_select[3].tv = 0.0f;
	dx->D2primitive(1, h_select);

	//回復対象カーソル右
	h_select[0].x = x + 105.0f;
	h_select[0].y = 440.0f;
	h_select[0].z = 0.0f;
	h_select[0].rhw = 0.0f;
	h_select[0].color = clr;
	h_select[0].tu = 0.0f;
	h_select[0].tv = 0.0f;

	h_select[1].x = x + 105.0f;
	h_select[1].y = 560.0f;
	h_select[1].z = 0.0f;
	h_select[1].rhw = 0.0f;
	h_select[1].color = clr;
	h_select[1].tu = 0.0f;
	h_select[1].tv = 0.0f;

	h_select[2].x = x + 110.0f;
	h_select[2].y = 560.0f;
	h_select[2].z = 0.0f;
	h_select[2].rhw = 0.0f;
	h_select[2].color = clr;
	h_select[2].tu = 0.0f;
	h_select[2].tv = 0.0f;

	h_select[3].x = x + 110.0f;
	h_select[3].y = 440.0f;
	h_select[3].z = 0.0f;
	h_select[3].rhw = 0.0f;
	h_select[3].color = clr;
	h_select[3].tu = 0.0f;
	h_select[3].tv = 0.0f;
	dx->D2primitive(1, h_select);

	//回復対象カーソル上
	h_select[0].x = x - 5.0f;
	h_select[0].y = 440.0f;
	h_select[0].z = 0.0f;
	h_select[0].rhw = 0.0f;
	h_select[0].color = clr;
	h_select[0].tu = 0.0f;
	h_select[0].tv = 0.0f;

	h_select[1].x = x - 5.0f;
	h_select[1].y = 445.0f;
	h_select[1].z = 0.0f;
	h_select[1].rhw = 0.0f;
	h_select[1].color = clr;
	h_select[1].tu = 0.0f;
	h_select[1].tv = 0.0f;

	h_select[2].x = x + 105.0f;
	h_select[2].y = 445.0f;
	h_select[2].z = 0.0f;
	h_select[2].rhw = 0.0f;
	h_select[2].color = clr;
	h_select[2].tu = 0.0f;
	h_select[2].tv = 0.0f;

	h_select[3].x = x + 105.0f;
	h_select[3].y = 440.0f;
	h_select[3].z = 0.0f;
	h_select[3].rhw = 0.0f;
	h_select[3].color = clr;
	h_select[3].tu = 0.0f;
	h_select[3].tv = 0.0f;
	dx->D2primitive(1, h_select);

	//回復対象カーソル下
	h_select[0].x = x - 5.0f;
	h_select[0].y = 555.0f;
	h_select[0].z = 0.0f;
	h_select[0].rhw = 0.0f;
	h_select[0].color = clr;
	h_select[0].tu = 0.0f;
	h_select[0].tv = 0.0f;

	h_select[1].x = x - 5.0f;
	h_select[1].y = 560.0f;
	h_select[1].z = 0.0f;
	h_select[1].rhw = 0.0f;
	h_select[1].color = clr;
	h_select[1].tu = 0.0f;
	h_select[1].tv = 0.0f;

	h_select[2].x = x + 105.0f;
	h_select[2].y = 560.0f;
	h_select[2].z = 0.0f;
	h_select[2].rhw = 0.0f;
	h_select[2].color = clr;
	h_select[2].tu = 0.0f;
	h_select[2].tv = 0.0f;

	h_select[3].x = x + 105.0f;
	h_select[3].y = 555.0f;
	h_select[3].z = 0.0f;
	h_select[3].rhw = 0.0f;
	h_select[3].color = clr;
	h_select[3].tu = 0.0f;
	h_select[3].tv = 0.0f;
	dx->D2primitive(1, h_select);
}

void Battle::Cursor_e(int select){

	static int theta = 0;
	static int r = 255;
	static int b = 0;
	static bool clr_f = TRUE;
	if (clr_f){
		b++;
		if (r-- <= 0)clr_f = FALSE;
	}
	else{
		b--;
		if (r++ >= 255)clr_f = TRUE;
	}

	//カーソル左上
	E_select.d3varrayI[0] = E_select.d3varrayI[3] = 0;
	E_select.d3varray[0].p = D3DXVECTOR3((float)-25.0f, (float)-25.0f, 1.0f);
	E_select.d3varray[0].color = (r << 16) + (0 << 8) + b;
	E_select.d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//カーソル左下
	E_select.d3varrayI[4] = 2;
	E_select.d3varray[2].p = D3DXVECTOR3((float)-25.0f, (float)25.0f, 1.0f);
	E_select.d3varray[2].color = (b << 16) + (0 << 8) + r;
	E_select.d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//カーソル右下
	E_select.d3varrayI[2] = E_select.d3varrayI[5] = 3;
	E_select.d3varray[3].p = D3DXVECTOR3((float)25.0f, (float)25.0f, 1.0f);
	E_select.d3varray[3].color = (0 << 16) + (r << 8) + b;
	E_select.d3varray[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//カーソル右上
	E_select.d3varrayI[1] = 1;
	E_select.d3varray[1].p = D3DXVECTOR3((float)25.0f, (float)-25.0f, 1.0f);
	E_select.d3varray[1].color = (r << 16) + (b << 8) + 0;
	E_select.d3varray[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	dx->D3primitive(SQUARE, &E_select, 1, e_pos[select].x, e_pos[select].y, e_pos[select].z, (float)(theta = theta % 361 + 1), FALSE, FALSE, FALSE);
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
	free(command);
	command = NULL;
	free(h_select);
	h_select = NULL;
}