//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	       EnemyBossクラス                                   **//
//**                                   Magiccreate関数                                   **//
//*****************************************************************************************//

#include "EnemyBoss.h"
#include <time.h>

EnemyBoss::EnemyBoss(){}

EnemyBoss::EnemyBoss(int t_no, int no, Position::H_Pos *h_po, Position::E_Pos *e_po) :Enemy(t_no, no){

	h_pos = h_po;
	e_pos = e_po;
	mag_size = 0.1f;
	//t_no=敵№
	int e;
	switch (t_no){
	case 0:
		e = 50;
		normal_action = NORMAL;
		M_run_flg_rnd = 3;
		p_data.Attack = 25;
		p_data.Magic = 10;
		p_data.Agility = 20;
		p_data.Vitality = 40;
		p_data.Exp = 100;
		p_data.LV = 20;
		p_data.FlameATT_LV = 15;
		p_data.Fpoint = 150;
		p_data.Healing_LV = 0;
		p_data.Hpoint = 10;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		pos_offset = 30.0f;
		size_x = 90.0f;
		size_y = 90.0f;//z位置補正で使用
		break;
	case 1:
		e = 51;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 16;
		p_data.Magic = 36;
		p_data.Agility = 26;
		p_data.Vitality = 105;
		p_data.Exp = 200;
		p_data.LV = 26;
		p_data.FlameATT_LV = 27;
		p_data.Fpoint = 270;
		p_data.Healing_LV = 27;
		p_data.Hpoint = 270;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		pos_offset = 30.0f;
		size_x = 60.0f;
		size_y = 60.0f;
		break;
	case 2:
		e = 52;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 30;
		p_data.Magic = 50;
		p_data.Agility = 40;
		p_data.Vitality = 150;
		p_data.Exp = 400;
		p_data.LV = 40;
		p_data.FlameATT_LV = 36;
		p_data.Fpoint = 360;
		p_data.Healing_LV = 36;
		p_data.Hpoint = 360;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		pos_offset = 30.0f;
		size_x = 60.0f;
		size_y = 60.0f;
		break;
	case 3:
		e = 53;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 65;
		p_data.Magic = 65;
		p_data.Agility = 65;
		p_data.Vitality = 250;
		p_data.Exp = 800;
		p_data.LV = 65;
		p_data.FlameATT_LV = 70;
		p_data.Fpoint = 700;
		p_data.Healing_LV = 70;
		p_data.Hpoint = 700;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		pos_offset = 40.0f;
		size_x = 120.0f;
		size_y = 120.0f;
		mag_size = 0.15f;
		break;
	case 4:
		e = 59;
		normal_action = NORMAL;
		M_run_flg_rnd = 2;
		p_data.Attack = 90;
		p_data.Magic = 90;
		p_data.Agility = 90;
		p_data.Vitality = 400;
		p_data.Exp = 3000;
		p_data.LV = 90;
		p_data.FlameATT_LV = 100;
		p_data.Fpoint = 1000;
		p_data.Healing_LV = 100;
		p_data.Hpoint = 1000;
		p_data.Recover_LV = 0;
		p_data.Rpoint = 10;
		pos_offset = 100.0f;
		size_x = 250.0f;
		size_y = 135.0f;
		mag_size = 0.25f;
		break;
	}
	p_data.HP = s_MHP();
	p_data.MP = s_MMP();
	esc = FALSE;

	PosOffset(o_no);

	en_boss = new MeshData();
	char str[50];
	switch (e){
	case 50:
		en_boss->GetVBarray("./dat/mesh/boss1.obj", FALSE);
		en_boss_att = new MeshData[13];
		for (int i = 0; i < 13; i++){
			sprintf_s(str, sizeof(str), "./dat/mesh/boss1att/boss1bone_0000%02d.obj", i + 1);
			en_boss_att[i].GetVBarray(str, FALSE);
		}
		break;
	case 51:
		en_boss->GetVBarray("./dat/mesh/boss2.obj", FALSE);
		en_boss_att = new MeshData[19];
		for (int i = 0; i < 19; i++){
			sprintf_s(str, sizeof(str), "./dat/mesh/boss2att/boss2bone_0000%02d.obj", i + 1);
			en_boss_att[i].GetVBarray(str, FALSE);
		}
		break;
	case 52:
		en_boss->GetVBarray("./dat/mesh/boss3.obj", FALSE);
		break;
	case 53:
		en_boss->GetVBarray("./dat/mesh/boss4.obj", FALSE);
		en_boss_att = new MeshData[11];
		for (int i = 0; i < 11; i++){
			sprintf_s(str, sizeof(str), "./dat/mesh/boss4att/boss4bone_0000%02d.obj", i + 1);
			en_boss_att[i].GetVBarray(str, FALSE);
		}
		break;
	case 59:
		en_boss->GetVBarray("./dat/mesh/lastboss.obj", FALSE);
		en_boss_att = new MeshData[15];
		for (int i = 0; i < 15; i++){
			sprintf_s(str, sizeof(str), "./dat/mesh/lastbossatt/lastbossbone_0000%02d.obj", i + 1);
			en_boss_att[i].GetVBarray(str, FALSE);
		}
		break;
	}

	mag_boss = new ParticleData();
	mag_boss->CreateParticle(61, mag_size, 5.0f);
}

//@Override
void EnemyBoss::AttackAction(){
	float m;

	if (effect_f == FALSE){
		switch (e_no){
		case 0:
			m = tfloat.Add(0.15f);
			if ((en_boss_att_cnt += m) < 56.0f){//カウントcnt Max18まで
				int cnt = (int)(en_boss_att_cnt / 3.0f);
				if (cnt < 13)en_boss_att_Ind = cnt;
				else en_boss_att_Ind = 24 - cnt;
			}
			else{
				en_boss_att_cnt = 0.0f;
				en_boss_att_Ind = -1;
				effect_f = TRUE;
			}
			break;
		case 1:
			m = tfloat.Add(0.17f);
			if ((en_boss_att_cnt += m) < 65.0f){//カウントcnt Max21まで
				int cnt = (int)(en_boss_att_cnt / 3.0f);
				if (cnt < 19)en_boss_att_Ind = cnt;
				else en_boss_att_Ind = 21 - cnt;
			}
			else{
				en_boss_att_cnt = 0.0f;
				en_boss_att_Ind = -1;
				effect_f = TRUE;
			}
			break;
		case 3:
			m = tfloat.Add(0.08f);
			if ((en_boss_att_cnt += m) < 65.0f){//カウントcnt Max21まで
				int cnt = (int)(en_boss_att_cnt / 3.0f);
				if (cnt < 11)en_boss_att_Ind = cnt;
				else en_boss_att_Ind = 20 - cnt;
			}
			else{
				en_boss_att_cnt = 0.0f;
				en_boss_att_Ind = -1;
				effect_f = TRUE;
			}
			break;
		case 4:
			m = tfloat.Add(0.08f);
			if ((en_boss_att_cnt += m) < 44.0f){//カウントcnt Max14まで
				int cnt = (int)(en_boss_att_cnt / 3.0f);
				en_boss_att_Ind = cnt;
			}
			else{
				en_boss_att_cnt = 0.0f;
				en_boss_att_Ind = -1;
				effect_f = TRUE;
			}
			break;
		default:
			m = tfloat.Add(0.15f);
			if (e_pos[o_no].theta >= 338.0f || e_pos[o_no].theta <= 22.0f){
				if (zoom == TRUE && (mov_y += m) > 30.0f)zoom = FALSE;
				if (zoom == FALSE && (mov_y -= m) < 0.0f){
					zoom = TRUE;
					mov_y = 0.0f;
					effect_f = TRUE;
				}
			}
			if (e_pos[o_no].theta >= 68.0f && e_pos[o_no].theta <= 112.0f){
				if (zoom == TRUE && (mov_x -= m) < -30.0f)zoom = FALSE;
				if (zoom == FALSE && (mov_x += m) > 0.0f){
					zoom = TRUE;
					mov_y = 0.0f;
					effect_f = TRUE;
				}
			}
			if (e_pos[o_no].theta >= 158.0f && e_pos[o_no].theta <= 202.0f){
				if (zoom == TRUE && (mov_y -= m) < -30.0f)zoom = FALSE;
				if (zoom == FALSE && (mov_y += m) > 0.0f){
					zoom = TRUE;
					mov_y = 0.0f;
					effect_f = TRUE;
				}
			}
			if (e_pos[o_no].theta >= 248.0f && e_pos[o_no].theta <= 292.0f){
				if (zoom == TRUE && (mov_x += m) > 30.0f)zoom = FALSE;
				if (zoom == FALSE && (mov_x -= m) < 0.0f){
					zoom = TRUE;
					mov_y = 0.0f;
					effect_f = TRUE;
				}
			}
			break;
		}
	}
}

//@Override
void EnemyBoss::DamageAction(){

	float m = tfloat.Add(0.01f);
	if (cg < -0.9f){
		cg = cb = 0.0f;
		act_f = normal_action;
	}
	else{
		cg -= m;
		cb -= m;
	}
}

//@Override
void EnemyBoss::RecoverActionInit(){
	cr = cg = cb = -1.0f;
	act_f = RECOVER;
}

//@Override
void EnemyBoss::RecoverAction(){

	float m = tfloat.Add(0.01f);
	if (cr > -0.1f){
		cr = cg = cb = 0.0f;
		act_f = normal_action;
	}
	else{
		cr += m;
		cg += m;
		cb += m;
	}
}

//@Override
bool EnemyBoss::LostAction(float x, float y, float z){

	float m = tfloat.Add(0.02f);
	MovieSoundManager::Bosslost_sound(TRUE);
	if ((mov_z -= m) < -150.0f){
		mov_z = 0.0f; return TRUE;
	}
	return FALSE;
}

//@Override
bool EnemyBoss::Magiccreate(float x, float y, float z){
	float m = tfloat.Add(0.15f);
	MovieSoundManager::Magic_sound(TRUE);
	if (count == 0.0f)mag_boss->Draw(x + mov_x, y + mov_y, z + 5.0f + mov_z, (float)((int)count % 360), 0.3f, TRUE, mag_size);
	if (count != 0.0f)mag_boss->Draw(x + mov_x, y + mov_y, z + 5.0f + mov_z, (float)((int)count % 360), 0.3f, FALSE, mag_size);
	dx->PointLightPosSet(3, x, y, z, 0.7f, 0.2f, 0.2f, 1.0f, mag_size * 500.0f, mag_size * 100.0f, 2.0f, TRUE);

	if ((count += m) > 900){
		count = 0.0f;
		dx->PointLightPosSet(3, x, y, z, 0.7f, 0.2f, 0.2f, 1.0f, mag_size * 1000.0f, 100.0f, 2.0f, FALSE);
		return FALSE;
	}
	return TRUE;
}

//@Override
void EnemyBoss::ObjDraw(float x, float y, float z, float r, float g, float b, float theta){
	if (en_boss_att_Ind != -1)en_boss_att[en_boss_att_Ind].D3primitive(x, y, z + size_y * 0.5f, cr, cg, cb, theta, size_x * 0.5f, 0.1f);
	else en_boss->D3primitive(x, y, z + size_y * 0.5f, cr, cg, cb, theta, size_x * 0.5f, 0.1f);
}

//@Override
bool EnemyBoss::M_run_flg(){

	int rnd;
	switch (e_no){
	case 0:
		if (p_data.MP < p_data.FlameATT_LV * 4)return FALSE;
		rnd = rand() % M_run_flg_rnd;
		if (rnd == 1)return TRUE;
		else return FALSE;
		break;
	case 1:
		if (p_data.MP < p_data.FlameATT_LV * 4)return FALSE;
		return TRUE;
		break;
	case 2:
		if (p_data.MP < p_data.FlameATT_LV * 4)return FALSE;
		return TRUE;
		break;
	case 3:
		if (p_data.MP < p_data.FlameATT_LV * 4)return FALSE;
		rnd = rand() % M_run_flg_rnd;
		if (rnd == 1)return TRUE;
		else return FALSE;
		break;
	case 4:
		if (p_data.MP < p_data.FlameATT_LV * 4)return FALSE;
		rnd = rand() % M_run_flg_rnd;
		if (rnd == 1)return TRUE;
		else return FALSE;
		break;
	}
	return FALSE;
}

//@Override
void EnemyBoss::M_select(int *r, int *r1){

	//マジック選択 *r = 0:Flame, 1:Healing, 2:Recover 
	//全体 *r1 = 0 単体 *r1 = 1
	switch (e_no){
	case 0:
		*r = 0;
		*r1 = 1;
		break;
	case 1:
		if ((double)p_data.HP / s_MHP() < 0.5){
			*r = 1; *r1 = 1; break;
		}
		*r = 0; *r1 = rand() % 2;
		break;
	case 2:
		if ((double)p_data.HP / s_MHP() < 0.3){
			*r = 1; *r1 = 1; break;
		}
		*r = 0; *r1 = rand() % 2;
		break;
	case 3:
		if ((double)p_data.HP / s_MHP() < 0.2){
			*r = 1; *r1 = 1; break;
		}
		*r = 0; *r1 = rand() % 2;
		break;
	case 4:
		if ((double)p_data.HP / s_MHP() < 0.2){
			*r = 1; *r1 = 1; break;
		}
		*r = 0; *r1 = rand() % 2;
		break;
	}
}

EnemyBoss::~EnemyBoss(){

	delete mag_boss;
	mag_boss = NULL;
	delete en_boss;
	en_boss = NULL;
	if (en_boss_att != NULL){
		delete[] en_boss_att;
		en_boss_att = NULL;
	}
}