//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	       EnemyBossクラス                                   **//
//**                                   Magiccreate関数                                   **//
//*****************************************************************************************//

#include "Dx9Process.h"
#include "EnemyBoss.h"
#include <time.h>

EnemyBoss::EnemyBoss(){}

EnemyBoss::EnemyBoss(int t_no, int no, Position::H_Pos *h_po, Position::E_Pos *e_po) :Enemy(t_no, no){

	h_pos = h_po;
	e_pos = e_po;
	mag_size = 700;//MAX1600
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
		size_y = 90.0f;
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
		size_x = 50.0f;
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
		size_x = 50.0f;
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
		pos_offset = 30.0f;
		size_x = 110.0f;
		size_y = 110.0f;
		mag_size = 900;
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
		size_x = 200.0f;
		size_y = 135.0f;
		mag_size = 1600;
		break;
	}
	p_data.HP = s_MHP();
	p_data.MP = s_MMP();
	esc = FALSE;

	PosOffset(o_no);

	dx->GetTexture(&en, e);
	dx->GetVBarray(SQUARE, &en, 1);
	mag = new Dx9Process::PolygonData[50];
	dx->GetTexture(&mag[0], 61);//[0]番目にテクスチャを保管(1枚しか使用しない)
	//テクスチャピクセルデータ一時確保用(使うクラス内で確保する事)
	p_array = (Dx9Process::T_xyz*)malloc(sizeof(Dx9Process::T_xyz) * 3 * mag_size * mag_size);
	//ピクセル移動用Z座標初期化
	for (int i = 0; i < 50; i++)mv[i] = (float)i;
	//ピクセル個数初期化
	for (int i = 0; i < 50; i++)ver_pcs[i] = 0;
	//ピクセルデータ取り出し
	dx->GetTexturePixelArray(&mag[0], p_array, mag_size);
	//描画対象ピクセル個数カウント(各グループ50個飛びでピクセル取得する為50個飛びでカウント)

	for (int p = 0; p < 50; p++){
		for (int k = 0; k < 3; k++){
			for (int j = 0; j < mag_size; j++){
				for (int i = p; i < mag_size; i += 50){
					if ((p_array[k * mag_size * mag_size + j * mag_size + i].color & 0xff) >= 10)ver_pcs[p]++;//描画ピクセル個数カウント
				}
			}
		}
	}
	int cnt = 0;
	for (int i = 0; i < 50; i++)cnt += ver_pcs[i];

	//頂点バッファ
	for (int i = 0; i < 50; i++)dx->GetVBarray(POINt, &mag[i], ver_pcs[i]);

	//頂点座標設定
	for (int p = 0; p < 50; p++){
		int k = 0;
		for (int k1 = 0; k1 < 3; k1++){
			for (int j = 0; j < mag_size; j++){
				for (int i = p; i < mag_size; i += 50){
					int ind = k1 * mag_size * mag_size + j * mag_size + i;
					if ((p_array[ind].color & 0xff) < 10 || k >= ver_pcs[p])continue;
					mag[p].d3varrayI[k] = k;
					mag[p].d3varray[k].p = D3DXVECTOR3(p_array[ind].x - mag_size / 10,
						p_array[ind].y - mag_size / 10, p_array[ind].z);
					mag[p].d3varray[k++].color = (p_array[ind].color >> 8) / 2;//アルファ値抜かす
				}
			}
		}
	}

	Enemycreate(size_x, size_y);
}

//@Override
bool EnemyBoss::Magiccreate(float x, float y, float z){
	
	//ピクセル座標移動
	for (int i = 0; i < 50; i++){
		mv[i] = mv[i] -= 0.5f;
		if (mv[i] <= 0.0f)mv[i] = 0.0f;
	}

	MovieSoundManager::Magic_sound(TRUE);
	for (int i = 0; i < 50; i++)
		dx->D3primitive(POINt, &mag[i], ver_pcs[i], x + mov_x, y + mov_y, z + 10.0f + mov_z + mv[i], (float)count + mv[i], FALSE, FALSE, FALSE);
		
	if (count++ > 200){
		count = 0;
		//ピクセル移動開始位置初期化
		for (int i = 0; i < 50; i++)mv[i] = (float)i;
		return FALSE;
	}
	return TRUE;
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

	delete[] mag;
	mag = NULL;

	free(p_array);
	p_array = NULL;
}