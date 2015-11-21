//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Hero�N���X                                      **//
//**                                   Statecreate�֐�                                   **//
//*****************************************************************************************//

#include "Dx9Process.h"
#include "Hero.h"
#include "Battle.h"

Hero::Hero(){}

Hero::Hero(P_Data *p_dat, int no){

	o_no = no;
	effect_f = FALSE;
	tx = ty = 0.0f;
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

	state = (Dx9Process::MY_VERTEX2*)malloc(sizeof(Dx9Process::MY_VERTEX2) * 4);
	meter = (Dx9Process::MY_VERTEX2*)malloc(sizeof(Dx9Process::MY_VERTEX2) * 4);

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
	count = 0;
}

void Hero::Statecreate(bool command_run){

	static bool clr_f = TRUE;
	static int r = 255;
	D3DCOLOR clr;

	if (command_run == FALSE)clr = (0 << 16) + (0 << 8) + 200;
	if (command_run == TRUE){
		if (clr_f){
			if ((r -= 5) <= 0)clr_f = FALSE;
		}
		else{
			if ((r += 5) >= 255)clr_f = TRUE;
		}
		clr = (r << 16) + (r << 8) + 200;
	}

	float x;
	if (o_no == 0)x = 0.0f;
	if (o_no == 1)x = 170.0f;
	if (o_no == 2)x = 350.0f;
	if (o_no == 3)x = 520.0f;

	//�X�e�[�^�X�E�C���h�E
	state[0].x = x + mov_x;
	state[0].y = 450.0f + mov_y;
	state[0].z = 0.0f;
	state[0].rhw = 0.0f;
	state[0].color = clr;
	state[0].tu = 0.0f;
	state[0].tv = 0.0f;

	state[1].x = x + mov_x;
	state[1].y = 550.0f + mov_y;
	state[1].z = 0.0f;
	state[1].rhw = 0.0f;
	state[1].color = clr;
	state[1].tu = 0.0f;
	state[1].tv = 0.0f;

	state[2].x = 100.0f + x + mov_x;
	state[2].y = 550.0f + mov_y;
	state[2].z = 0.0f;
	state[2].rhw = 0.0f;
	state[2].color = clr;
	state[2].tu = 0.0f;
	state[2].tv = 0.0f;

	state[3].x = 100.0f + x + mov_x;
	state[3].y = 450.0f + mov_y;
	state[3].z = 0.0f;
	state[3].rhw = 0.0f;
	state[3].color = clr;
	state[3].tu = 0.0f;
	state[3].tv = 0.0f;

	dx->D2primitive(1, state);
}

void Hero::Metercreate(float me){

	float x;
	if (o_no == 0)x = 0.0f;
	if (o_no == 1)x = 170.0f;
	if (o_no == 2)x = 350.0f;
	if (o_no == 3)x = 520.0f;

	//���[�^�[1
	meter[0].x = 1.0f + x + mov_x;
	meter[0].y = 539.0f + mov_y;
	meter[0].z = 0.0f;
	meter[0].rhw = 0.0f;
	meter[0].color = (255 << 16) + (255 << 8) + 255;
	meter[0].tu = 0.0f;
	meter[0].tv = 0.0f;

	meter[1].x = 1.0f + x + mov_x;
	meter[1].y = 549.0f + mov_y;
	meter[1].z = 0.0f;
	meter[1].rhw = 0.0f;
	meter[1].color = (255 << 16) + (255 << 8) + 255;
	meter[1].tu = 0.0f;
	meter[1].tv = 0.0f;

	meter[2].x = 99.0f + x + mov_x;
	meter[2].y = 549.0f + mov_y;
	meter[2].z = 0.0f;
	meter[2].rhw = 0.0f;
	meter[2].color = (255 << 16) + (255 << 8) + 255;
	meter[2].tu = 0.0f;
	meter[2].tv = 0.0f;

	meter[3].x = 99.0f + x + mov_x;
	meter[3].y = 539.0f + mov_y;
	meter[3].z = 0.0f;
	meter[3].rhw = 0.0f;
	meter[3].color = (255 << 16) + (255 << 8) + 255;
	meter[3].tu = 0.0f;
	meter[3].tv = 0.0f;

	dx->D2primitive(1, meter);

	//���[�^�[2
	meter[0].x = 3.0f + x + mov_x;
	meter[0].y = 541.0f + mov_y;
	meter[0].color = (255 << 16) + (100 << 8) + 0;

	meter[1].x = 3.0f + x + mov_x;
	meter[1].y = 547.0f + mov_y;
	meter[1].color = (255 << 16) + (100 << 8) + 0;

	meter[2].x = 3.0f + 94.0f * me + x + mov_x;
	meter[2].y = 547.0f + mov_y;
	meter[2].color = (255 << 16) + (100 << 8) + 0;

	meter[3].x = 3.0f + 94.0f * me + x + mov_x;
	meter[3].y = 541.0f + mov_y;
	meter[3].color = (255 << 16) + (100 << 8) + 0;

	dx->D2primitive(1, meter);
}

void Hero::Magiccreate(){

	//�}�W�b�N����
	mag.d3varrayI[0] = mag.d3varrayI[3] = 0;
	mag.d3varray[0].p = D3DXVECTOR3((float)-25.0f, (float)-25.0f, 1.0f);
	mag.d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	mag.d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//�}�W�b�N����
	mag.d3varrayI[4] = 2;
	mag.d3varray[2].p = D3DXVECTOR3((float)-25.0f, (float)25.0f, 1.0f);
	mag.d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	mag.d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//�}�W�b�N�E��
	mag.d3varrayI[2] = mag.d3varrayI[5] = 3;
	mag.d3varray[3].p = D3DXVECTOR3((float)25.0f, (float)25.0f, 1.0f);
	mag.d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	mag.d3varray[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�}�W�b�N�E��
	mag.d3varrayI[1] = 1;
	mag.d3varray[1].p = D3DXVECTOR3((float)25.0f, (float)-25.0f, 1.0f);
	mag.d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	mag.d3varray[1].tex = D3DXVECTOR2(1.0f, 0.0f);
}

bool Hero::Effectdraw(Battle *battle, int *select_obj, Position::H_Pos *h_pos, Position::E_Pos *e_pos){

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

	if (effect.tex_no == 0 || effect.tex_no == 1)ver = 25;
	else ver = 3;

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

	free(state);
	state = NULL;
	free(meter);
	meter = NULL;
}

