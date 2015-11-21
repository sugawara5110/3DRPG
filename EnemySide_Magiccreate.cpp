//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	       EnemySide�N���X                                   **//
//**                                   Magiccreate�֐�                                   **//
//*****************************************************************************************//

#include "Dx9Process.h"
#include "EnemySide.h"

EnemySide::EnemySide(){}

EnemySide::EnemySide(int t_no, int no, Position::H_Pos *h_po, Position::E_Pos *e_po) :Enemy(t_no, no){

	h_pos = h_po;
	e_pos = e_po;

	pos_offset = 0.0f;
	size_x = 50.0f;
	size_y = 50.0f;
	//t_no=�G��
	int e;
	switch (t_no){
		//�}�b�v0
	case 0:
		e = 30;
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 1;
		p_data.Magic = 1;
		p_data.Agility = 1;
		p_data.Vitality = 1;
		p_data.Exp = 1;
		p_data.LV = 1;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 30.0f;
		size_y = 20.0f;
		break;
	case 1:
		e = 31;
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 2;
		p_data.Magic = 2;
		p_data.Agility = 2;
		p_data.Vitality = 2;
		p_data.Exp = 2;
		p_data.LV = 2;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 25.0f;
		size_y = 35.0f;
		break;
	case 2:
		e = 32;
		normal_action = MOVE;
		M_run_flg_rnd = 3;
		p_data.Attack = 3;
		p_data.Magic = 6;
		p_data.Agility = 3;
		p_data.Vitality = 4;
		p_data.Exp = 3;
		p_data.LV = 4;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 2;
		p_data.Hpoint = 20;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 45.0f;
		size_y = 35.0f;
		break;
	case 3:
		e = 33;
		normal_action = NORMAL;
		M_run_flg_rnd = 3;
		p_data.Attack = 7;
		p_data.Magic = 6;
		p_data.Agility = 5;
		p_data.Vitality = 6;
		p_data.Exp = 6;
		p_data.LV = 6;
		p_data.FlameATT_LV = 3;
		p_data.Fpoint = 30;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 40.0f;
		size_y = 50.0f;
		break;
		//�}�b�v1
	case 4:
		e = 34;
		normal_action = NORMAL;
		M_run_flg_rnd = 3;
		p_data.Attack = 7;
		p_data.Magic = 9;
		p_data.Agility = 8;
		p_data.Vitality = 8;
		p_data.Exp = 8;
		p_data.LV = 8;
		p_data.FlameATT_LV = 4;
		p_data.Fpoint = 40;
		p_data.Healing_LV = 2;
		p_data.Hpoint = 20;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 5:
		e = 35;
		normal_action = MOVE;
		M_run_flg_rnd = 3;
		p_data.Attack = 9;
		p_data.Magic = 12;
		p_data.Agility = 9;
		p_data.Vitality = 10;
		p_data.Exp = 7;
		p_data.LV = 10;
		p_data.FlameATT_LV = 4;
		p_data.Fpoint = 40;
		p_data.Healing_LV = 3;
		p_data.Hpoint = 30;
		p_data.Recover_LV = 1;
		p_data.Rpoint = 10;
		break;
	case 6:
		e = 36;
		normal_action = NORMAL;
		M_run_flg_rnd = 3;
		p_data.Attack = 10;
		p_data.Magic = 16;
		p_data.Agility = 12;
		p_data.Vitality = 10;
		p_data.Exp = 10;
		p_data.LV = 12;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 7;
		p_data.Hpoint = 70;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 7:
		e = 37;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 16;
		p_data.Magic = 12;
		p_data.Agility = 14;
		p_data.Vitality = 14;
		p_data.Exp = 10;
		p_data.LV = 14;
		p_data.FlameATT_LV = 8;
		p_data.Fpoint = 80;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
		//�}�b�v2
	case 8:
		e = 38;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 17;
		p_data.Magic = 18;
		p_data.Agility = 13;
		p_data.Vitality = 16;
		p_data.Exp = 15;
		p_data.LV = 16;
		p_data.FlameATT_LV = 8;
		p_data.Fpoint = 80;
		p_data.Healing_LV = 5;
		p_data.Hpoint = 50;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 9:
		e = 39;
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 27;
		p_data.Magic = 5;
		p_data.Agility = 18;
		p_data.Vitality = 22;
		p_data.Exp = 15;
		p_data.LV = 18;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 10:
		e = 40;
		normal_action = MOVE;
		M_run_flg_rnd = 2;
		p_data.Attack = 20;
		p_data.Magic = 20;
		p_data.Agility = 20;
		p_data.Vitality = 20;
		p_data.Exp = 15;
		p_data.LV = 20;
		p_data.FlameATT_LV = 14;
		p_data.Fpoint = 140;
		p_data.Healing_LV = 10;
		p_data.Hpoint = 100;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 11:
		e = 41;
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 28;
		p_data.Magic = 5;
		p_data.Agility = 22;
		p_data.Vitality = 33;
		p_data.Exp = 17;
		p_data.LV = 22;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 40.0f;
		size_y = 50.0f;
		break;
		//�}�b�v3
	case 12:
		e = 42;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 24;
		p_data.Magic = 24;
		p_data.Agility = 24;
		p_data.Vitality = 24;
		p_data.Exp = 28;
		p_data.LV = 24;
		p_data.FlameATT_LV = 15;
		p_data.Fpoint = 150;
		p_data.Healing_LV = 15;
		p_data.Hpoint = 150;
		p_data.Recover_LV = 15;
		p_data.Rpoint = 150;
		size_x = 45.0f;
		size_y = 45.0f;
		break;
	case 13:
		e = 43;
		normal_action = NORMAL;
		M_run_flg_rnd = 1;
		p_data.Attack = 39;
		p_data.Magic = 3;
		p_data.Agility = 26;
		p_data.Vitality = 36;
		p_data.Exp = 30;
		p_data.LV = 26;
		p_data.FlameATT_LV = 0;
		p_data.Fpoint = 10;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		size_x = 40.0f;
		size_y = 50.0f;
		break;
	case 14:
		e = 44;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 28;
		p_data.Magic = 28;
		p_data.Agility = 28;
		p_data.Vitality = 28;
		p_data.Exp = 34;
		p_data.LV = 28;
		p_data.FlameATT_LV = 20;
		p_data.Fpoint = 200;
		p_data.Healing_LV = 20;
		p_data.Hpoint = 200;
		p_data.Recover_LV = 10;
		p_data.Rpoint = 100;
		break;
	case 15:
		e = 45;
		normal_action = MOVE;
		M_run_flg_rnd = 2;
		p_data.Attack = 30;
		p_data.Magic = 30;
		p_data.Agility = 30;
		p_data.Vitality = 30;
		p_data.Exp = 30;
		p_data.LV = 30;
		p_data.FlameATT_LV = 30;
		p_data.Fpoint = 300;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
		//�}�b�v4(���X�g)
	case 16:
		e = 46;
		normal_action = MOVE;
		M_run_flg_rnd = 2;
		p_data.Attack = 32;
		p_data.Magic = 32;
		p_data.Agility = 32;
		p_data.Vitality = 32;
		p_data.Exp = 50;
		p_data.LV = 32;
		p_data.FlameATT_LV = 32;
		p_data.Fpoint = 320;
		p_data.Healing_LV = 30;
		p_data.Hpoint = 300;
		p_data.Recover_LV = 20;
		p_data.Rpoint = 200;
		break;
	case 17:
		e = 47;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 30;
		p_data.Magic = 38;
		p_data.Agility = 34;
		p_data.Vitality = 34;
		p_data.Exp = 55;
		p_data.LV = 34;
		p_data.FlameATT_LV = 35;
		p_data.Fpoint = 350;
		p_data.Healing_LV = 30;
		p_data.Hpoint = 300;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		break;
	case 18:
		e = 48;
		normal_action = MOVE;
		M_run_flg_rnd = 2;
		p_data.Attack = 33;
		p_data.Magic = 39;
		p_data.Agility = 36;
		p_data.Vitality = 36;
		p_data.Exp = 50;
		p_data.LV = 36;
		p_data.FlameATT_LV = 35;
		p_data.Fpoint = 350;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 40;
		p_data.Rpoint = 400;
		break;
	case 19:
		e = 49;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 50;
		p_data.Magic = 50;
		p_data.Agility = 50;
		p_data.Vitality = 50;
		p_data.Exp = 150;
		p_data.LV = 50;
		p_data.FlameATT_LV = 50;
		p_data.Fpoint = 500;
		p_data.Healing_LV = 50;
		p_data.Hpoint = 500;
		p_data.Recover_LV = 50;
		p_data.Rpoint = 500;
		size_x = 70.0f;
		size_y = 70.0f;
		break;
	}
	p_data.HP = s_MHP();
	p_data.MP = s_MMP();
	esc = TRUE;

	PosOffset(o_no);

	dx->GetTexture(&en, e);
	dx->GetVBarray(SQUARE, &en, 1);
	mag = new Dx9Process::PolygonData();
	dx->GetTexture(mag, 60);
	dx->GetVBarray(SQUARE, mag, 1);

	Enemycreate(size_x, size_y);
}

//@Override
bool EnemySide::Magiccreate(float x, float y, float z){

	//�}�W�b�N����
	mag->d3varrayI[0] = mag->d3varrayI[3] = 0;
	mag->d3varray[0].p = D3DXVECTOR3((float)-35.0f, (float)-35.0f, 1.0f);
	mag->d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	mag->d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//�}�W�b�N����
	mag->d3varrayI[4] = 2;
	mag->d3varray[2].p = D3DXVECTOR3((float)-35.0f, (float)35.0f, 1.0f);
	mag->d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	mag->d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//�}�W�b�N�E��
	mag->d3varrayI[2] = mag->d3varrayI[5] = 3;
	mag->d3varray[3].p = D3DXVECTOR3((float)35.0f, (float)35.0f, 1.0f);
	mag->d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	mag->d3varray[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�}�W�b�N�E��
	mag->d3varrayI[1] = 1;
	mag->d3varray[1].p = D3DXVECTOR3((float)35.0f, (float)-35.0f, 1.0f);
	mag->d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	mag->d3varray[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	MovieSoundManager::Magic_sound(TRUE);
	dx->D3primitive(SQUARE, mag, 1, x + mov_x, y + mov_y, z + 1.0f + mov_z, (float)count, TRUE, FALSE, FALSE);

	if (count++ > 100){
		count = 0;
		return FALSE;
	}
	return TRUE;
}

//@Override
bool EnemySide::M_run_flg(){

	int rnd = rand() % M_run_flg_rnd;
	if (rnd == 1)return TRUE;
	else return FALSE;
}

//@Override
void EnemySide::M_select(int *r, int *r1){

	//�}�W�b�N�I�� �����_���� 0:Flame, 1:Healing, 2:Recover 
	if (GetFlameATT_LV() != 0){
		if (GetHealing_LV() != 0 && GetRecover_LV() != 0)*r = rand() % 3;
		if (GetHealing_LV() != 0 && GetRecover_LV() == 0)*r = rand() % 2;
		if (GetHealing_LV() == 0 && GetRecover_LV() != 0){
			*r = rand() % 2;
			if (*r == 1)*r = 2;
		}
		if (GetHealing_LV() == 0 && GetRecover_LV() == 0)*r = 0;
	}
	if (GetHealing_LV() != 0){
		if (GetFlameATT_LV() == 0 && GetRecover_LV() != 0){
			*r = rand() % 2;
			if (*r == 0)*r = 1;
			else *r = 2;
		}
		if (GetFlameATT_LV() == 0 && GetRecover_LV() == 0)*r = 1;
	}
	if (GetRecover_LV() != 0 && GetFlameATT_LV() == 0 && GetHealing_LV() == 0)*r = 2;

	//�I���}�W�b�N���̏���,�S��,�̑I��
	*r1 = rand() % 2;
}

EnemySide::~EnemySide(){
	delete mag;
	mag = NULL;
}