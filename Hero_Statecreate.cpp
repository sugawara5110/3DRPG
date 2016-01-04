//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                   Statecreate関数                                   **//
//*****************************************************************************************//

#include "Dx11Process.h"
#include "Hero.h"
#include "Battle.h"

Hero::Hero(){}

Hero::Hero(P_Data *p_dat, int no){

	o_no = no;
	effect_f = FALSE;
	tx = ty = 0.0f;
	tt = 0;
	p_data.Attack = p_dat->Attack;
	p_data.Magic = p_dat->Magic;
	p_data.Agility = p_dat->Agility;
	p_data.Vitality = p_dat->Vitality;
	p_data.Exp = p_dat->Exp;
	p_data.LV = p_dat->LV;
	p_data.ParameterPoint = p_dat->ParameterPoint;
	p_data.HP = p_dat->HP;
	p_data.MP = p_dat->MP;
	p_data.FlameATT_LV = p_dat->FlameATT_LV;
	p_data.Healing_LV = p_dat->Healing_LV;
	p_data.Recover_LV = p_dat->Recover_LV;
	p_data.Fpoint = p_dat->Fpoint;
	p_data.Hpoint = p_dat->Hpoint;
	p_data.Rpoint = p_dat->Rpoint;

	dx->GetVBarray2D(&state, 1);
	dx->GetVBarray2D(&meter, 1);

	dx->GetTexture(&mag, 60);
	dx->GetVBarray(SQUARE, &mag, 1);
	dx->GetTexture(&effect, 81);
	effect.tex_no = 1;
	dx->GetTexture(&effect, 82);
	effect.tex_no = 2;
	dx->GetTexture(&effect, 83);
	effect.tex_no = 3;
	dx->GetTexture(&effect, 84);
	dx->GetVBarray(SQUARE, &effect, 1);

	Magiccreate();

	mov_y = 0.0f;
	mov_x = 0.0f;
	act_f = NORMAL;
	up = TRUE;
	count = 0.0f;
}

void Hero::Statecreate(bool command_run){

	static bool clr_f = TRUE;
	static float r = 1.0f;
	D3DXVECTOR4 clr;
	float m = tfloat.Add(0.002f);
	if (command_run == FALSE)clr = D3DXVECTOR4(0.0f, 0.0f, 0.8f, 1.0f);
	if (command_run == TRUE){
		if (clr_f){
			if ((r -= m) <= 0.0f)clr_f = FALSE;
		}
		else{
			if ((r += m) >= 1.0f)clr_f = TRUE;
		}
		clr = D3DXVECTOR4(r, r, 0.8f, 1.0f);
	}

	float x;
	if (o_no == 0)x = 10.0f;
	if (o_no == 1)x = 180.0f;
	if (o_no == 2)x = 360.0f;
	if (o_no == 3)x = 530.0f;

	//ステータスウインドウ
	state.d3varray[0].x = x + mov_x;
	state.d3varray[0].y = 450.0f + mov_y;
	state.d3varray[0].z = 0.2f;
	state.d3varray[0].color = clr;

	state.d3varray[1].x = x + mov_x;
	state.d3varray[1].y = 550.0f + mov_y;
	state.d3varray[1].z = 0.2f;
	state.d3varray[1].color = clr;

	state.d3varray[2].x = 120.0f + x + mov_x;
	state.d3varray[2].y = 450.0f + mov_y;
	state.d3varray[2].z = 0.2f;
	state.d3varray[2].color = clr;

	state.d3varray[3].x = 120.0f + x + mov_x;
	state.d3varray[3].y = 550.0f + mov_y;
	state.d3varray[3].z = 0.2f;
	state.d3varray[3].color = clr;

	dx->D2primitive(&state, 1, TRUE);
}

void Hero::Metercreate(float me){

	float x;
	if (o_no == 0)x = 10.0f;
	if (o_no == 1)x = 180.0f;
	if (o_no == 2)x = 360.0f;
	if (o_no == 3)x = 530.0f;

	D3DXVECTOR4 clr = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	//メーター1
	meter.d3varray[0].x = 1.0f + x + mov_x;
	meter.d3varray[0].y = 539.0f + mov_y;
	meter.d3varray[0].z = 0.1f;
	meter.d3varray[0].color = clr;

	meter.d3varray[1].x = 1.0f + x + mov_x;
	meter.d3varray[1].y = 549.0f + mov_y;
	meter.d3varray[1].z = 0.1f;
	meter.d3varray[1].color = clr;

	meter.d3varray[2].x = 119.0f + x + mov_x;
	meter.d3varray[2].y = 539.0f + mov_y;
	meter.d3varray[2].z = 0.1f;
	meter.d3varray[2].color = clr;

	meter.d3varray[3].x = 119.0f + x + mov_x;
	meter.d3varray[3].y = 549.0f + mov_y;
	meter.d3varray[3].z = 0.1f;
	meter.d3varray[3].color = clr;

	dx->D2primitive(&meter, 1, TRUE);

	clr = D3DXVECTOR4(1.0f, 0.5f, 0.0f, 1.0f);

	//メーター2
	meter.d3varray[0].x = 3.0f + x + mov_x;
	meter.d3varray[0].y = 541.0f + mov_y;
	meter.d3varray[0].z = 0.0f;
	meter.d3varray[0].color = clr;

	meter.d3varray[1].x = 3.0f + x + mov_x;
	meter.d3varray[1].y = 547.0f + mov_y;
	meter.d3varray[1].z = 0.0f;
	meter.d3varray[1].color = clr;

	meter.d3varray[2].x = 3.0f + 114.0f * me + x + mov_x;
	meter.d3varray[2].y = 541.0f + mov_y;
	meter.d3varray[2].z = 0.0f;
	meter.d3varray[2].color = clr;

	meter.d3varray[3].x = 3.0f + 114.0f * me + x + mov_x;
	meter.d3varray[3].y = 547.0f + mov_y;
	meter.d3varray[3].z = 0.0f;
	meter.d3varray[3].color = clr;

	dx->D2primitive(&meter, 1, TRUE);
}

void Hero::Magiccreate(){

	//マジック左上
	mag.SetVertex(0, 3, 0,
		(float)-25.0f, (float)-25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//マジック左下
	mag.SetVertex(4, 2,
		(float)-25.0f, (float)25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//マジック右下
	mag.SetVertex(2, 5, 3,
		(float)25.0f, (float)25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);

	//マジック右上
	mag.SetVertex(1, 1,
		(float)25.0f, (float)-25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);
}

bool Hero::Effectdraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos){

	float px, py;
	float ver;
	switch (effect.tex_no){
		//通常攻撃
	case 0:
		px = 0.1111f;
		py = 1.0f;
		break;
		//フレイム
	case 1:
		px = 0.1428f;
		py = 1.0f;
		break;
		//ヒール
	case 2:
		px = 0.1f;
		py = 1.0f;
		break;
		//リカバリ
	case 3:
		px = 0.1f;
		py = 1.0f;
		break;
	}

	if (effect.tex_no == 0 || effect.tex_no == 1)ver = 25;
	else ver = 3;

	//左前
	effect.SetVertex(0, 3, 0,
		(float)-ver, (float)0.0f, ver * 2,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		tx, ty);

	//左奥
	effect.SetVertex(4, 2,
		(float)-ver, (float)0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		tx, ty + py);

	//右奥
	effect.SetVertex(2, 5, 3,
		(float)ver, (float)0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		tx + px, ty + py);

	//右前
	effect.SetVertex(1, 1,
		(float)ver, (float)0.0f, ver * 2,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		tx + px, ty);

	if ((tt += tfloat.Add(0.8f)) > 10.0f){//速度調整用
		tt = 0;
		if ((tx += px) + px > 1.0f){
			tx = 0; return FALSE;
		}
	}

	float ex = 0.0f;
	float ey = 0.0f;
	float hx[4];
	float hy[4];
	switch ((int)h_pos->theta){
	case 360:
	case 0:
		ey = 5.0f;
		hx[0] = -6.5f; hy[0] = -10.0f;
		hx[1] = -3.0f; hy[1] = -10.0f;
		hx[2] = 0.5f; hy[2] = -10.0f;
		hx[3] = 3.5f; hy[3] = -10.0f;
		break;
	case 90:
		ex = -5.0f;
		hx[0] = 10.0f; hy[0] = -6.5f;
		hx[1] = 10.0f; hy[1] = -3.0f;
		hx[2] = 10.0f; hy[2] = 0.5f;
		hx[3] = 10.0f; hy[3] = 3.5f;
		break;
	case 180:
		ey = -5.0f;
		hx[0] = 6.5f; hy[0] = 10.0f;
		hx[1] = 3.0f; hy[1] = 10.0f;
		hx[2] = -0.5f; hy[2] = 10.0f;
		hx[3] = -3.5f; hy[3] = 10.0f;
		break;
	case 270:
		ex = 5.0f;
		hx[0] = -10.0f; hy[0] = 6.5f;
		hx[1] = -10.0f; hy[1] = 3.0f;
		hx[2] = -10.0f; hy[2] = -0.5f;
		hx[3] = -10.0f; hy[3] = -3.5f;
		break;
	}

	if (effect.tex_no == 0 || effect.tex_no == 1){
		if (effect.tex_no == 0)MovieSoundManager::Att_sound(TRUE);
		if (effect.tex_no == 1)MovieSoundManager::Flame_sound(TRUE);
		if (*select_obj != 4){
			dx->D3primitive(SQUARE, &effect, 1, e_pos[*select_obj].x + ex, e_pos[*select_obj].y + ey, e_pos[*select_obj].z, e_pos[*select_obj].theta, TRUE, TRUE, FALSE);
		}
		else {
			for (int i = 0; i < 4; i++){
				if (battle->GetE_DM(i) == FALSE)continue;
				dx->D3primitive(SQUARE, &effect, 1, e_pos[i].x + ex, e_pos[i].y + ey, e_pos[*select_obj].z, e_pos[i].theta, TRUE, TRUE, FALSE);
			}
		}
	}
	if (effect.tex_no == 2 || effect.tex_no == 3){
		MovieSoundManager::Heal_sound(TRUE);
		switch (*select_obj){
		case 0:
			dx->D3primitive(SQUARE, &effect, 1, h_pos->cx1 + hx[0], h_pos->cy1 + hy[0], (float)h_pos->pz * 100.0f + 30.0f, h_pos->theta, TRUE, TRUE, FALSE);
			break;
		case 1:
			dx->D3primitive(SQUARE, &effect, 1, h_pos->cx1 + hx[1], h_pos->cy1 + hy[1], (float)h_pos->pz * 100.0f + 30.0f, h_pos->theta, TRUE, TRUE, FALSE);
			break;
		case 2:
			dx->D3primitive(SQUARE, &effect, 1, h_pos->cx1 + hx[2], h_pos->cy1 + hy[2], (float)h_pos->pz * 100.0f + 30.0f, h_pos->theta, TRUE, TRUE, FALSE);
			break;
		case 3:
			dx->D3primitive(SQUARE, &effect, 1, h_pos->cx1 + hx[3], h_pos->cy1 + hy[3], (float)h_pos->pz * 100.0f + 30.0f, h_pos->theta, TRUE, TRUE, FALSE);
			break;
		case 4:
			for (int i = 0; i < 4; i++){
				if (battle->GetH_RCV(i) == FALSE)continue;
				dx->D3primitive(SQUARE, &effect, 1, h_pos->cx1 + hx[i], h_pos->cy1 + hy[i], (float)h_pos->pz * 100.0f + 30.0f, h_pos->theta, TRUE, TRUE, FALSE);
			}
			break;
		}
	}
	return TRUE;
}

Hero::~Hero(){

}

