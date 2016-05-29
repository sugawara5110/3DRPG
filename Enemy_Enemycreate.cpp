//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Enemy�N���X                                     **//
//**                                   Enemycreate�֐�                                   **//
//*****************************************************************************************//

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
	en = NULL;
	en_boss_att = NULL;
	en_boss_att_cnt = 0.0f;
	en_boss_att_Ind = -1;
	en_boss_att_pass = NULL;
	ObjCntMax = 0;

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

	//�G���O
	en->SetVertex(0, 0,
		(float)-(x / 2), (float)0.0f, y,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//�G����
	en->SetVertex(1, 1,
		(float)-(x / 2), (float)0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//�G�E��
	en->SetVertex(2, 2,
		(float)(x / 2), (float)0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);

	//�G�E�O
	en->SetVertex(3, 3,
		(float)(x / 2), (float)0.0f, y,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);
}

void Enemy::AttackAction(){}

void Enemy::DamageAction(){}

void Enemy::RecoverActionInit(){}

void Enemy::RecoverAction(){}

bool Enemy::LostAction(float x, float y, float z){ return TRUE; }

bool Enemy::Magiccreate(float x, float y, float z){ return TRUE; }

void Enemy::ObjDraw(float x, float y, float z, float r, float g, float b, float theta){}

bool Enemy::Effectdraw(Battle *battle, int *E_select_obj){

	float px, py;
	float ver;
	switch (effect.tex_no){
		//�ʏ�U��
	case 0:
		px = 0.1111f;
		py = 1.0f;
		break;
		//�t���C��
	case 1:
		px = 0.1428f;
		py = 1.0f;
		break;
		//�q�[��
	case 2:
		px = 0.1f;
		py = 1.0f;
		break;
		//���J�o��
	case 3:
		px = 0.1f;
		py = 1.0f;
		break;
	}

	ver = 25;
	//���O
	effect.SetVertex(0, 0,
		(float)-ver, (float)0.0f, ver * 2,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		tx, ty);

	//�E�O
	effect.SetVertex(1, 4, 1,
		(float)ver, (float)0.0f, ver * 2,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		tx + px, ty);

	//����
	effect.SetVertex(2, 3, 2,
		(float)-ver, (float)0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		tx, ty + py);

	//�E��
	effect.SetVertex(5, 3,
		(float)ver, (float)0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		tx + px, ty + py);

	if ((tt += tfloat.Add(0.8f)) > 10.0f){//���x�����p
		tt = 0;
		if ((tx += px) + px > 1.0f){
			for (int i = 3; i < 7; i++)dx->PointLightPosSet(i, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FALSE);
			tx = 0; return FALSE;
		}
	}

	float ex = 0.0f;
	float ey = 0.0f;
	switch ((int)h_pos->theta){
	case 360:
	case 0:
		ey = 15.0f;
		break;
	case 90:
		ex = -15.0f;
		break;
	case 180:
		ey = -15.0f;
		break;
	case 270:
		ex = 15.0f;
		break;
	}

	if (effect.tex_no == 2 || effect.tex_no == 3){
		MovieSoundManager::Heal_sound(TRUE);
		float r, g, b;
		if (effect.tex_no == 2){ r = 0.2f, g = 0.7f, b = 0.3f; }
		if (effect.tex_no == 3){ r = 0.2f, g = 0.3f, b = 0.7f; }
		if (*E_select_obj != 4){
			effect.Draw(e_pos[*E_select_obj].x + ex, e_pos[*E_select_obj].y + ey, e_pos[*E_select_obj].z, 0, 0, 0, e_pos[*E_select_obj].theta, TRUE, TRUE, 0);
			dx->PointLightPosSet(3, e_pos[*E_select_obj].x + ex, e_pos[*E_select_obj].y + ey, e_pos[*E_select_obj].z, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
		}
		else {
			for (int i = 0; i < 4; i++){
				if (battle->GetE_RCV(i) == FALSE)continue;
				effect.Draw(e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, 0, 0, 0, e_pos[i].theta, TRUE, TRUE, 0);
				dx->PointLightPosSet(i + 3, e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			}
		}
	}

	Position::Bt_H_Pos *b_pos = battle->GetBtPos(h_pos);
	if (effect.tex_no == 0 || effect.tex_no == 1){
		if (effect.tex_no == 0)MovieSoundManager::Att_sound(TRUE);
		if (effect.tex_no == 1)MovieSoundManager::Flame_sound(TRUE);
		float r, g, b;
		if (effect.tex_no == 0){ r = 1.0f, g = 1.0f, b = 1.0f; }
		if (effect.tex_no == 1){ r = 0.7f, g = 0.3f, b = 0.2f; }
		if (*E_select_obj != 4){
			effect.Draw(b_pos[*E_select_obj].BtPos_x1, b_pos[*E_select_obj].BtPos_y1, (float)h_pos->pz * 100.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
			dx->PointLightPosSet(3, b_pos[*E_select_obj].BtPos_x1, b_pos[*E_select_obj].BtPos_y1, (float)h_pos->pz * 100.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
		}
		else{
			for (int i = 0; i < 4; i++){
				if (battle->GetH_DM(i) == FALSE)continue;
				effect.Draw(b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, (float)h_pos->pz * 100.0f, 0, 0, 0, h_pos->theta, TRUE, TRUE, 0);
				dx->PointLightPosSet(i + 3, b_pos[i].BtPos_x1, b_pos[i].BtPos_y1, (float)h_pos->pz * 100.0f, r, g, b, 1.0f, 50.0f, 20.0f, 2.0f, TRUE);
			}
		}
	}
	return TRUE;
}

Enemy::~Enemy(){

}
