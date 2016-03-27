//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Heroクラス                                      **//
//**                                   Statecreate関数                                   **//
//*****************************************************************************************//

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

	state.GetVBarray2D(1);
	meter.GetVBarray2D(1);

	mag.GetTexture(60);
	mag.GetVBarray(SQUARE, 1);
	effect.GetTexture(81);
	effect.tex_no = 1;
	effect.GetTexture(82);
	effect.tex_no = 2;
	effect.GetTexture(83);
	effect.tex_no = 3;
	effect.GetTexture(84);
	effect.GetVBarray(SQUARE, 1);

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
	VECTOR4 clr;
	float m = tfloat.Add(0.002f);
	if (command_run == FALSE)clr.as(0.6f, 0.6f, 0.6f, 0.5f);
	if (command_run == TRUE){
		if (clr_f){
			if ((r -= m) <= 0.0f)clr_f = FALSE;
		}
		else{
			if ((r += m) >= 1.0f)clr_f = TRUE;
		}
		clr.as(r, r, r, 0.5f);
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

	state.d3varray[1].x = 120.0f + x + mov_x;
	state.d3varray[1].y = 450.0f + mov_y;
	state.d3varray[1].z = 0.2f;
	state.d3varray[1].color = clr;

	state.d3varray[2].x = x + mov_x;
	state.d3varray[2].y = 550.0f + mov_y;
	state.d3varray[2].z = 0.2f;
	state.d3varray[2].color = clr;

	state.d3varray[3].x = 120.0f + x + mov_x;
	state.d3varray[3].y = 550.0f + mov_y;
	state.d3varray[3].z = 0.2f;
	state.d3varray[3].color = clr;

	state.D2primitive(TRUE, TRUE);
}

void Hero::Metercreate(float me){

	float x;
	if (o_no == 0)x = 10.0f;
	if (o_no == 1)x = 180.0f;
	if (o_no == 2)x = 360.0f;
	if (o_no == 3)x = 530.0f;

	VECTOR4 clr = { 1.0f, 1.0f, 1.0f, 1.0f };

	//メーター1
	meter.d3varray[0].x = 1.0f + x + mov_x;
	meter.d3varray[0].y = 539.0f + mov_y;
	meter.d3varray[0].z = 0.1f;
	meter.d3varray[0].color = clr;

	meter.d3varray[1].x = 119.0f + x + mov_x;
	meter.d3varray[1].y = 539.0f + mov_y;
	meter.d3varray[1].z = 0.1f;
	meter.d3varray[1].color = clr;

	meter.d3varray[2].x = 1.0f + x + mov_x;
	meter.d3varray[2].y = 549.0f + mov_y;
	meter.d3varray[2].z = 0.1f;
	meter.d3varray[2].color = clr;

	meter.d3varray[3].x = 119.0f + x + mov_x;
	meter.d3varray[3].y = 549.0f + mov_y;
	meter.d3varray[3].z = 0.1f;
	meter.d3varray[3].color = clr;

	meter.D2primitive(TRUE, TRUE);

	clr.as(1.0f, 0.5f, 0.0f, 1.0f);

	//メーター2
	meter.d3varray[0].x = 3.0f + x + mov_x;
	meter.d3varray[0].y = 541.0f + mov_y;
	meter.d3varray[0].z = 0.0f;
	meter.d3varray[0].color = clr;

	meter.d3varray[1].x = 3.0f + 114.0f * me + x + mov_x;
	meter.d3varray[1].y = 541.0f + mov_y;
	meter.d3varray[1].z = 0.0f;
	meter.d3varray[1].color = clr;

	meter.d3varray[2].x = 3.0f + x + mov_x;
	meter.d3varray[2].y = 547.0f + mov_y;
	meter.d3varray[2].z = 0.0f;
	meter.d3varray[2].color = clr;

	meter.d3varray[3].x = 3.0f + 114.0f * me + x + mov_x;
	meter.d3varray[3].y = 547.0f + mov_y;
	meter.d3varray[3].z = 0.0f;
	meter.d3varray[3].color = clr;

	meter.D2primitive(TRUE, TRUE);
}

void Hero::Magiccreate(){

	//マジック左上
	mag.SetVertex(0, 0,
		(float)-25.0f, (float)-25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//マジック右上
	mag.SetVertex(1, 4, 1,
		(float)25.0f, (float)-25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);

	//マジック左下
	mag.SetVertex(2, 3, 2,
		(float)-25.0f, (float)25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//マジック右下
	mag.SetVertex(5, 3,
		(float)25.0f, (float)25.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);
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
	effect.SetVertex(0, 0,
		(float)-ver, (float)0.0f, ver * 2,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		tx, ty);

	//右前
	effect.SetVertex(1, 4, 1,
		(float)ver, (float)0.0f, ver * 2,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		tx + px, ty);

	//左奥
	effect.SetVertex(2, 3, 2,
		(float)-ver, (float)0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		tx, ty + py);

	//右奥
	effect.SetVertex(5, 3,
		(float)ver, (float)0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		tx + px, ty + py);

	if ((tt += tfloat.Add(0.8f)) > 10.0f){//速度調整用
		tt = 0;
		if ((tx += px) + px > 1.0f){
			for (int i = 3; i < 7; i++)dx->PointLightPosSet(i, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FALSE);
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
		float r, g, b;
		if (effect.tex_no == 0){ r = 1.0f, g = 1.0f, b = 1.0f; }
		if (effect.tex_no == 1){ r = 0.7f, g = 0.3f, b = 0.2f; }
		if (*select_obj != 4){
			effect.D3primitive(e_pos[*select_obj].x + ex, e_pos[*select_obj].y + ey, e_pos[*select_obj].z, 0, 0, 0, e_pos[*select_obj].theta, TRUE, TRUE, 0);
			dx->PointLightPosSet(3, e_pos[*select_obj].x + ex, e_pos[*select_obj].y + ey, e_pos[*select_obj].z, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
		}
		else {
			for (int i = 0; i < 4; i++){
				if (battle->GetE_DM(i) == FALSE)continue;
				effect.D3primitive(e_pos[i].x + ex, e_pos[i].y + ey, e_pos[*select_obj].z, 0, 0, 0, e_pos[i].theta, TRUE, TRUE, 0);
				dx->PointLightPosSet(i + 3, e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			}
		}
	}
	if (effect.tex_no == 2 || effect.tex_no == 3){
		MovieSoundManager::Heal_sound(TRUE);
		float r, g, b;
		if (effect.tex_no == 2){ r = 0.2f, g = 0.7f, b = 0.3f; }
		if (effect.tex_no == 3){ r = 0.2f, g = 0.3f, b = 0.7f; }
		switch (*select_obj){
		case 0:
			effect.D3primitive(h_pos->cx1 + hx[0], h_pos->cy1 + hy[0], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			dx->PointLightPosSet(3, h_pos->cx1 + hx[0], h_pos->cy1 + hy[0], (float)h_pos->pz * 100.0f + 30.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			break;
		case 1:
			effect.D3primitive(h_pos->cx1 + hx[1], h_pos->cy1 + hy[1], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			dx->PointLightPosSet(3, h_pos->cx1 + hx[1], h_pos->cy1 + hy[1], (float)h_pos->pz * 100.0f + 30.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			break;
		case 2:
			effect.D3primitive(h_pos->cx1 + hx[2], h_pos->cy1 + hy[2], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			dx->PointLightPosSet(3, h_pos->cx1 + hx[2], h_pos->cy1 + hy[2], (float)h_pos->pz * 100.0f + 30.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			break;
		case 3:
			effect.D3primitive(h_pos->cx1 + hx[3], h_pos->cy1 + hy[3], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			dx->PointLightPosSet(3, h_pos->cx1 + hx[3], h_pos->cy1 + hy[3], (float)h_pos->pz * 100.0f + 30.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			break;
		case 4:
			for (int i = 0; i < 4; i++){
				if (battle->GetH_RCV(i) == FALSE)continue;
				effect.D3primitive(h_pos->cx1 + hx[i], h_pos->cy1 + hy[i], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
				dx->PointLightPosSet(i + 3, h_pos->cx1 + hx[i], h_pos->cy1 + hy[i], (float)h_pos->pz * 100.0f + 30.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			}
			break;
		}
	}
	return TRUE;
}

Hero::~Hero(){

}

