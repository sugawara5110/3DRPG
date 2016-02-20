//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Enemyクラス                                     **//
//**                                   Enemycreate関数                                   **//
//*****************************************************************************************//

#include "Dx11Process.h"
#include "Enemy.h"
#include "Battle.h"

Enemy::Enemy(){}

Enemy::Enemy(int t_no, int no){

	o_no = no;
	e_no = t_no;
	mov_z = 0.0f;
	mov_y = 0.0f;
	mov_x = 0.0f;
	act_f = normal_action;
	up = TRUE;
	zoom = TRUE;
	count = 0.0f;
	effect_f = FALSE;
	tx = ty = 0.0f;
	tt = 0;
	cr = cg = cb = 0.0f;

	effect.GetTexture(81);
	effect.tex_no = 1;
	effect.GetTexture(82);
	effect.tex_no = 2;
	effect.GetTexture(83);
	effect.tex_no = 3;
	effect.GetTexture(84);
	effect.GetVBarray(SQUARE, 1);
}

void Enemy::Enemycreate(float x, float y){

	//敵左前
	en.SetVertex(0, 0,
		(float)-(x / 2), (float)0.0f, y,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//敵左奥
	en.SetVertex(1, 1,
		(float)-(x / 2), (float)0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//敵右奥
	en.SetVertex(2, 2,
		(float)(x / 2), (float)0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);

	//敵右前
	en.SetVertex(3, 3,
		(float)(x / 2), (float)0.0f, y,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);
}

void Enemy::DamageAction(){}

void Enemy::RecoverActionInit(){}

void Enemy::RecoverAction(){}

bool Enemy::LostAction(float x, float y, float z){ return TRUE; }

bool Enemy::Magiccreate(float x, float y, float z){ return TRUE; }

bool Enemy::Effectdraw(Battle *battle, int *E_select_obj){

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

	if (effect.tex_no == 0 || effect.tex_no == 1)ver = 3;
	else ver = 25;

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

	if (effect.tex_no == 2 || effect.tex_no == 3){
		MovieSoundManager::Heal_sound(TRUE);
		if (*E_select_obj != 4){
			effect.D3primitive(e_pos[*E_select_obj].x + ex, e_pos[*E_select_obj].y + ey, e_pos[*E_select_obj].z, 0, 0, 0, e_pos[*E_select_obj].theta, TRUE, TRUE, 0);
		}
		else {
			for (int i = 0; i < 4; i++){
				if (battle->GetE_RCV(i) == FALSE)continue;
				effect.D3primitive(e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, 0, 0, 0, e_pos[i].theta, TRUE, TRUE, 0);
			}
		}
	}
	if (effect.tex_no == 0 || effect.tex_no == 1){
		if (effect.tex_no == 0)MovieSoundManager::Att_sound(TRUE);
		if (effect.tex_no == 1)MovieSoundManager::Flame_sound(TRUE);
		switch (*E_select_obj){
		case 0:
			effect.D3primitive(h_pos->cx1 + hx[0], h_pos->cy1 + hy[0], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			break;
		case 1:
			effect.D3primitive(h_pos->cx1 + hx[1], h_pos->cy1 + hy[1], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			break;
		case 2:
			effect.D3primitive(h_pos->cx1 + hx[2], h_pos->cy1 + hy[2], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			break;
		case 3:
			effect.D3primitive(h_pos->cx1 + hx[3], h_pos->cy1 + hy[3], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			break;
		case 4:
			for (int i = 0; i < 4; i++){
				if (battle->GetH_DM(i) == FALSE)continue;
				effect.D3primitive(h_pos->cx1 + hx[i], h_pos->cy1 + hy[i], (float)h_pos->pz * 100.0f + 30.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			}
			break;
		}
	}
	return TRUE;
}

Enemy::~Enemy(){

}
