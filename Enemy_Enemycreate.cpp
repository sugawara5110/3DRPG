//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Enemy�N���X                                     **//
//**                                   Enemycreate�֐�                                   **//
//*****************************************************************************************//

#include "Dx9Process.h"
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
	count = 0;
	theta_recov = 0.0f;
	effect_f = FALSE;
	tx = ty = 0.0f;

	dx->GetTexture(&effect, 81);
	effect.tex_no = 1;
	dx->GetTexture(&effect, 82);
	effect.tex_no = 2;
	dx->GetTexture(&effect, 83);
	effect.tex_no = 3;
	dx->GetTexture(&effect, 84);
	dx->GetVBarray(SQUARE, &effect, 1);
}

void Enemy::Enemycreate(float x, float y){

	//�G���O
	en.d3varrayI[0] = en.d3varrayI[3] = 0;
	en.d3varray[0].p = D3DXVECTOR3((float)-(x / 2), (float)0.0f, y);
	en.d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	en.d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//�G����
	en.d3varrayI[4] = 2;
	en.d3varray[2].p = D3DXVECTOR3((float)-(x / 2), (float)0.0f, 0.0f);
	en.d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	en.d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//�G�E��
	en.d3varrayI[2] = en.d3varrayI[5] = 3;
	en.d3varray[3].p = D3DXVECTOR3((float)(x / 2), (float)0.0f, 0.0f);
	en.d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	en.d3varray[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�G�E�O
	en.d3varrayI[1] = 1;
	en.d3varray[1].p = D3DXVECTOR3((float)(x / 2), (float)0.0f, y);
	en.d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	en.d3varray[1].tex = D3DXVECTOR2(1.0f, 0.0f);
}

bool Enemy::Magiccreate(float x, float y, float z){ return TRUE; }

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

	if (effect.tex_no == 0 || effect.tex_no == 1)ver = 3;
	else ver = 25;

	//���O
	effect.d3varrayI[0] = effect.d3varrayI[3] = 0;
	effect.d3varray[0].p = D3DXVECTOR3((float)-ver, (float)0.0f, ver * 2);
	effect.d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	effect.d3varray[0].tex = D3DXVECTOR2(tx, ty);

	//����
	effect.d3varrayI[4] = 2;
	effect.d3varray[2].p = D3DXVECTOR3((float)-ver, (float)0.0f, 0.0f);
	effect.d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	effect.d3varray[2].tex = D3DXVECTOR2(tx, ty + py);

	//�E��
	effect.d3varrayI[2] = effect.d3varrayI[5] = 3;
	effect.d3varray[3].p = D3DXVECTOR3((float)ver, (float)0.0f, 0.0f);
	effect.d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	effect.d3varray[3].tex = D3DXVECTOR2(tx + px, ty + py);

	//�E�O
	effect.d3varrayI[1] = 1;
	effect.d3varray[1].p = D3DXVECTOR3((float)ver, (float)0.0f, ver * 2);
	effect.d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	effect.d3varray[1].tex = D3DXVECTOR2(tx + px, ty);

	if ((tx += px) + px > 1.0f){
		tx = 0; return FALSE;
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
			dx->D3primitive(SQUARE, &effect, 1, e_pos[*E_select_obj].x + ex, e_pos[*E_select_obj].y + ey, e_pos[*E_select_obj].z, e_pos[*E_select_obj].theta, TRUE, TRUE, FALSE);
		}
		else {
			for (int i = 0; i < 4; i++){
				if (battle->GetE_RCV(i) == FALSE)continue;
				dx->D3primitive(SQUARE, &effect, 1, e_pos[i].x + ex, e_pos[i].y + ey, e_pos[i].z, e_pos[i].theta, TRUE, TRUE, FALSE);
			}
		}
	}
	if (effect.tex_no == 0 || effect.tex_no == 1){
		if (effect.tex_no == 0)MovieSoundManager::Att_sound(TRUE);
		if (effect.tex_no == 1)MovieSoundManager::Flame_sound(TRUE);
		switch (*E_select_obj){
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
				if (battle->GetH_DM(i) == FALSE)continue;
				dx->D3primitive(SQUARE, &effect, 1, h_pos->cx1 + hx[i], h_pos->cy1 + hy[i], (float)h_pos->pz * 100.0f + 30.0f, h_pos->theta, TRUE, TRUE, FALSE);
			}
			break;
		}
	}
	return TRUE;
}

Enemy::~Enemy(){

}
