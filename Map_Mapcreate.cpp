//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                    Mapcreate関数                                    **//
//*****************************************************************************************//

#include "Map.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int Map::map_no_s = 0;
MapStPos Map::MPos = POS_ST;
int Map::boss_killed[5];

int Map::GetMapNo(){
	return map_no_s;
}

void Map::SetMapNo(int no){
	map_no_s = no;
}

void Map::SetBossKilled(int i, int f){

	if (i == -1){
		for (int i1 = 0; i1 < 5; i1++){
			boss_killed[i1] = 0;
		}
		return;
	}
	boss_killed[i] = f;
}

int *Map::GetBossKilled(){
	return boss_killed;
}

Map::Map(){}

Map::Map(Position::H_Pos *h_p){

	map_no = map_no_s;
	dx = Dx9Process::GetInstance();
	moving = FALSE;
	direction_move = NOTPRESS;
	m_theta = 0;
	stepx = 0.0f;
	stepy = 0.0f;
	map_text_f = 0;
	recover_p_f = FALSE;
	boss_p_f = FALSE;
	elevator_UP = FALSE;
	elevator_DOWN = FALSE;
	elevator_step = 0.0f;
	MovieSoundManager::ObjCreate_map(map_no);

	//no=ダンジョン№
	int Wno;
	int Wno1;
	int Gno;
	int Cno;
	int Enter;
	int Exit;
	float Cei_st, Cei_end, Cei_height;
	float Gro_st, Gro_end;
	float En_x, En_y, En_z, En_si;
	float Ex_x, Ex_y, Ex_z, Ex_si;

	switch (map_no){
	case 0:
		Wno = 0; Gno = 1; Cno = 2; Exit = 3;
		Cei_st = 0.0f; Cei_end = 3000.0f; Cei_height = 100.0f;
		Gro_st = 0.0f; Gro_end = 3000.0f;
		Ex_x = 400.0f; Ex_y = 100.5f; Ex_z = 100.0f; Ex_si = 100.0f;
		break;
	case 1:
		Wno = 8; Wno1 = 4; Gno = 5; Cno = 6; Enter = 9; Exit = 7;
		Cei_st = -8000.0f; Cei_end = 11000.0f; Cei_height = 500.0f;
		Gro_st = -750.0f; Gro_end = 4500.0f;
		En_x = 1110.0f; En_y = 2899.5f; En_z = 80.0f; En_si = 80.0f;
		Ex_x = 910.0f; Ex_y = 100.5f; Ex_z = 80.0f; Ex_si = 80.0f;
		break;
	case 2:
		Wno = 11; Gno = 12; Cno = 11; Enter = 13; Exit = 14;
		Cei_st = 0.0f; Cei_end = 3000.0f; Cei_height = 200.0f;
		Gro_st = 0.0f; Gro_end = 3000.0f;
		En_x = 110.0f; En_y = 2899.5f; En_z = 80.0f; En_si = 80.0f;
		Ex_x = 110.0f; Ex_y = 100.5f; Ex_z = 80.0f; Ex_si = 80.0f;
		break;
	case 3:
		Wno = 15; Gno = 15; Cno = 15; Enter = 16;
		Cei_st = 0.0f; Cei_end = 3000.0f; Cei_height = 200.0f;
		Gro_st = 0.0f; Gro_end = 3000.0f;
		En_x = 210.0f; En_y = 2899.5f; En_z = 80.0f; En_si = 80.0f;
		break;
	case 4:
		Wno = 26; Gno = 27; Cno = 28; Enter = 29;
		Cei_st = 0.0f; Cei_end = 3000.0f; Cei_height = 800.0f;
		Gro_st = 0.0f; Gro_end = 3000.0f;
		En_x = 1110.0f; En_y = 2899.5f; En_z = 80.0f; En_si = 80.0f;
		break;
	}

	Mapfilter(h_p);
	//壁(板)
	if (squarecount >= 1){
		dx->GetTexture(&poWall1, Wno1);
		dx->GetVBarray(SQUARE, &poWall1, 1);
	}

	//壁(ブロック)
	if (blockcount >= 1){
		dx->GetTexture(&poWall, Wno);
		dx->GetVBarray(SQUARE, &poWall, blockcount * 6);
		Mapcreate_Wall();
	}

	if (map_no == 1){
		//外背景
		dx->GetTexture(&poBackground, 10);
		dx->GetVBarray(SQUARE, &poBackground, 4);
		Mapcreate_Background(Gro_st, Gro_end, Cei_height);
		//雨
		dx->GetVBarray(LINE_L, &poRain, 200);
	}

	//地面
	dx->GetTexture(&poGround, Gno);
	dx->GetVBarray(SQUARE, &poGround, 900);
	Mapcreate_Ground(Gro_st, Gro_end);

	//空
	dx->GetTexture(&poCeiling, Cno);
	dx->GetVBarray(SQUARE, &poCeiling, 900);
	Mapcreate_Ceiling(Cei_st, Cei_end, Cei_height);

	//出口
	if (map_no != 4 && map_no != 3){
		dx->GetTexture(&poEXIT, Exit);
		dx->GetVBarray(SQUARE, &poEXIT, 1);
		Mapcreate_EXIT(Ex_x, Ex_y, Ex_z, Ex_si);
	}

	//入り口
	if (map_no != 0){
		dx->GetTexture(&poENTER, Enter);
		dx->GetVBarray(SQUARE, &poENTER, 1);
		Mapcreate_ENTER(En_x, En_y, En_z, En_si);
	}

	//リカバーポイント
	if (r_point_count >= 1){
		dx->GetTexture(&poRecover, 70);
		dx->GetVBarray(SQUARE, &poRecover, r_point_count);
		dx->GetVBarray(LINE_S, &poRecoverLine, r_point_count * 1440);
	}

	//動画テクスチャ松明
	if (mo_count >= 1){
		dx->TextureInit(&poMo, 512, 512);
		dx->SetTextureMPixel(&poMo, MovieSoundManager::Torch_GetFrame(512, 512), 0xff, 0xff, 0xff, 200);
		dx->GetVBarray(SQUARE, &poMo, 1);
	}

	//動画テクスチャ炎壁
	if (f_wall_count >= 1){
		dx->TextureInit(&poF_Wall, 512, 512);
		dx->SetTextureMPixel(&poF_Wall, MovieSoundManager::FireWall_GetFrame(512, 512), 0xff, 0xff, 0xff, 200);
		dx->GetVBarray(SQUARE, &poF_Wall, f_wall_count * 4);
		Mapcreate_F_Wall();
	}

	//ボス出現ポイント
	if (boss_count >= 1){
		dx->GetTexture(&poBoss, 61);
		dx->GetVBarray(SQUARE, &poBoss, boss_count);
		Mapcreate_BossPoint();
	}

	//エレベーター
	if (Elevator_count >= 1){
		dx->GetTexture(&poElevator, 70);
		dx->GetVBarray(SQUARE, &poElevator, Elevator_count);
		Mapcreate_Elevator();
	}
}

void Map::Mapcreate_Wall1(){
	//左前
	poWall1.d3varrayI[0] = poWall1.d3varrayI[3] = 0;
	poWall1.d3varray[0].p = D3DXVECTOR3(-50.0f, 0.0f, 100.0f);
	poWall1.d3varray[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	poWall1.d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	poWall1.d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//左奥
	poWall1.d3varrayI[4] = 2;
	poWall1.d3varray[2].p = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);
	poWall1.d3varray[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	poWall1.d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	poWall1.d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//右奥
	poWall1.d3varrayI[2] = poWall1.d3varrayI[5] = 3;
	poWall1.d3varray[3].p = D3DXVECTOR3(50.0f, 0.0f, 0.0f);
	poWall1.d3varray[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	poWall1.d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	poWall1.d3varray[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//右前
	poWall1.d3varrayI[1] = 1;
	poWall1.d3varray[1].p = D3DXVECTOR3(50.0f, 0.0f, 100.0f);
	poWall1.d3varray[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	poWall1.d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	poWall1.d3varray[1].tex = D3DXVECTOR2(1.0f, 0.0f);

	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 53)continue;
				dx->D3primitive(SQUARE, &poWall1, 1, (float)i * 100.0f + 50.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f, src_theta, TRUE, FALSE, FALSE);
			}
		}
	}
}

void Map::Mapcreate_Wall(){

	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){

				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 49 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 52)continue;//1のアスキーコード49
				//壁ブロック頂点設定
				//正面左上
				poWall.d3varrayI[k1] = poWall.d3varrayI[k1 + 3] = k;
				poWall.d3varray[k].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				poWall.d3varray[k].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k].tex = D3DXVECTOR2(0.0f, 0.0f);

				//正面左下
				poWall.d3varrayI[k1 + 1] = k + 6;
				poWall.d3varray[k + 6].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 6].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				poWall.d3varray[k + 6].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 6].tex = D3DXVECTOR2(0.0f, 1.0f);

				//正面右下
				poWall.d3varrayI[k1 + 2] = poWall.d3varrayI[k1 + 5] = k + 10;
				poWall.d3varray[k + 10].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 10].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				poWall.d3varray[k + 10].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 10].tex = D3DXVECTOR2(1.0f, 1.0f);

				//正面右上
				poWall.d3varrayI[k1 + 4] = k + 14;
				poWall.d3varray[k + 14].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 14].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				poWall.d3varray[k + 14].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 14].tex = D3DXVECTOR2(1.0f, 0.0f);

				//左面上前
				poWall.d3varrayI[k1 + 12] = poWall.d3varrayI[k1 + 15] = k + 2;
				poWall.d3varray[k + 2].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 2].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
				poWall.d3varray[k + 2].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 2].tex = D3DXVECTOR2(0.0f, 0.0f);

				//左面上後
				poWall.d3varrayI[k1 + 13] = k + 5;
				poWall.d3varray[k + 5].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 5].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
				poWall.d3varray[k + 5].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 5].tex = D3DXVECTOR2(0.0f, 1.0f);

				//左面下前
				poWall.d3varrayI[k1 + 16] = k + 7;
				poWall.d3varray[k + 7].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 7].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
				poWall.d3varray[k + 7].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 7].tex = D3DXVECTOR2(1.0f, 0.0f);

				//左面下後
				poWall.d3varrayI[k1 + 14] = poWall.d3varrayI[k1 + 17] = k + 8;
				poWall.d3varray[k + 8].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 8].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
				poWall.d3varray[k + 8].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 8].tex = D3DXVECTOR2(1.0f, 1.0f);

				//右面上前
				poWall.d3varrayI[k1 + 18] = poWall.d3varrayI[k1 + 21] = k + 16;
				poWall.d3varray[k + 16].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 16].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				poWall.d3varray[k + 16].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 16].tex = D3DXVECTOR2(1.0f, 0.0f);

				//右面上後
				poWall.d3varrayI[k1 + 19] = k + 18;
				poWall.d3varray[k + 18].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 18].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				poWall.d3varray[k + 18].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 18].tex = D3DXVECTOR2(1.0f, 1.0f);

				//右面下前
				poWall.d3varrayI[k1 + 22] = k + 11;
				poWall.d3varray[k + 11].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 11].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				poWall.d3varray[k + 11].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 11].tex = D3DXVECTOR2(0.0f, 0.0f);

				//右面下後
				poWall.d3varrayI[k1 + 20] = poWall.d3varrayI[k1 + 23] = k + 12;
				poWall.d3varray[k + 12].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 12].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				poWall.d3varray[k + 12].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 12].tex = D3DXVECTOR2(0.0f, 1.0f);

				//上面左前
				poWall.d3varrayI[k1 + 6] = poWall.d3varrayI[k1 + 9] = k + 1;
				poWall.d3varray[k + 1].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 1].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				poWall.d3varray[k + 1].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 1].tex = D3DXVECTOR2(1.0f, 0.0f);

				//上面左後
				poWall.d3varrayI[k1 + 7] = k + 3;
				poWall.d3varray[k + 3].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 3].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				poWall.d3varray[k + 3].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

				//上面右前
				poWall.d3varrayI[k1 + 10] = k + 15;
				poWall.d3varray[k + 15].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 15].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				poWall.d3varray[k + 15].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 15].tex = D3DXVECTOR2(0.0f, 0.0f);

				//上面右後
				poWall.d3varrayI[k1 + 8] = poWall.d3varrayI[k1 + 11] = k + 17;
				poWall.d3varray[k + 17].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 17].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				poWall.d3varray[k + 17].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 17].tex = D3DXVECTOR2(0.0f, 1.0f);

				//裏面左上
				poWall.d3varrayI[k1 + 25] = k + 4;
				poWall.d3varray[k + 4].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				poWall.d3varray[k + 4].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 4].tex = D3DXVECTOR2(1.0f, 0.0f);

				//裏面左下
				poWall.d3varrayI[k1 + 26] = poWall.d3varrayI[k1 + 29] = k + 9;
				poWall.d3varray[k + 9].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 9].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				poWall.d3varray[k + 9].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 9].tex = D3DXVECTOR2(1.0f, 1.0f);

				//裏面右下
				poWall.d3varrayI[k1 + 28] = k + 13;
				poWall.d3varray[k + 13].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 13].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				poWall.d3varray[k + 13].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 13].tex = D3DXVECTOR2(0.0f, 1.0f);

				//裏面右上
				poWall.d3varrayI[k1 + 24] = poWall.d3varrayI[k1 + 27] = k + 19;
				poWall.d3varray[k + 19].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 19].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				poWall.d3varray[k + 19].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 19].tex = D3DXVECTOR2(0.0f, 0.0f);

				//底面左前
				poWall.d3varrayI[k1 + 30] = poWall.d3varrayI[k1 + 33] = k + 20;
				poWall.d3varray[k + 20].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 20].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				poWall.d3varray[k + 20].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 20].tex = D3DXVECTOR2(0.0f, 0.0f);

				//底面左後
				poWall.d3varrayI[k1 + 34] = k + 22;
				poWall.d3varray[k + 22].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 22].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				poWall.d3varray[k + 22].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 22].tex = D3DXVECTOR2(0.0f, 1.0f);

				//底面右前
				poWall.d3varrayI[k1 + 31] = k + 21;
				poWall.d3varray[k + 21].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f);
				poWall.d3varray[k + 21].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				poWall.d3varray[k + 21].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 21].tex = D3DXVECTOR2(1.0f, 0.0f);

				//底面右後
				poWall.d3varrayI[k1 + 32] = poWall.d3varrayI[k1 + 35] = k + 23;
				poWall.d3varray[k + 23].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poWall.d3varray[k + 23].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				poWall.d3varray[k + 23].color = (255 << 16) + (255 << 8) + 255;
				poWall.d3varray[k + 23].tex = D3DXVECTOR2(1.0f, 1.0f);

				k += 24;
				k1 += 36;
			}
		}
	}
}

void Map::Mapcreate_F_Wall(){

	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 73)continue;
				//左面上前
				poF_Wall.d3varrayI[k1] = poF_Wall.d3varrayI[k1 + 3] = k;
				poF_Wall.d3varray[k].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 200.0f);
				poF_Wall.d3varray[k].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k].tex = D3DXVECTOR2(0.0f, 0.0f);

				//左面上後
				poF_Wall.d3varrayI[k1 + 1] = k + 1;
				poF_Wall.d3varray[k + 1].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poF_Wall.d3varray[k + 1].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 1].tex = D3DXVECTOR2(0.0f, 1.0f);

				//左面下後
				poF_Wall.d3varrayI[k1 + 2] = poF_Wall.d3varrayI[k1 + 5] = k + 2;
				poF_Wall.d3varray[k + 2].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poF_Wall.d3varray[k + 2].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 2].tex = D3DXVECTOR2(1.0f, 1.0f);

				//左面下前
				poF_Wall.d3varrayI[k1 + 4] = k + 3;
				poF_Wall.d3varray[k + 3].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 200.0f);
				poF_Wall.d3varray[k + 3].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//底面左前
				poF_Wall.d3varrayI[k1 + 6] = poF_Wall.d3varrayI[k1 + 9] = k + 4;
				poF_Wall.d3varray[k + 4].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 200.0f);
				poF_Wall.d3varray[k + 4].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 4].tex = D3DXVECTOR2(0.0f, 0.0f);

				//底面左後
				poF_Wall.d3varrayI[k1 + 7] = k + 5;
				poF_Wall.d3varray[k + 5].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poF_Wall.d3varray[k + 5].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 5].tex = D3DXVECTOR2(0.0f, 1.0f);

				//底面右後
				poF_Wall.d3varrayI[k1 + 8] = poF_Wall.d3varrayI[k1 + 11] = k + 6;
				poF_Wall.d3varray[k + 6].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poF_Wall.d3varray[k + 6].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 6].tex = D3DXVECTOR2(1.0f, 1.0f);

				//底面右前
				poF_Wall.d3varrayI[k1 + 10] = k + 7;
				poF_Wall.d3varray[k + 7].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 200.0f);
				poF_Wall.d3varray[k + 7].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 7].tex = D3DXVECTOR2(1.0f, 0.0f);

				//右面下前
				poF_Wall.d3varrayI[k1 + 12] = poF_Wall.d3varrayI[k1 + 15] = k + 8;
				poF_Wall.d3varray[k + 8].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 200.0f);
				poF_Wall.d3varray[k + 8].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 8].tex = D3DXVECTOR2(0.0f, 0.0f);

				//右面下後
				poF_Wall.d3varrayI[k1 + 13] = k + 9;
				poF_Wall.d3varray[k + 9].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f);
				poF_Wall.d3varray[k + 9].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 9].tex = D3DXVECTOR2(0.0f, 1.0f);

				//右面上後
				poF_Wall.d3varrayI[k1 + 14] = poF_Wall.d3varrayI[k1 + 17] = k + 10;
				poF_Wall.d3varray[k + 10].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poF_Wall.d3varray[k + 10].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 10].tex = D3DXVECTOR2(1.0f, 1.0f);

				//右面上前
				poF_Wall.d3varrayI[k1 + 16] = k + 11;
				poF_Wall.d3varray[k + 11].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 200.0f);
				poF_Wall.d3varray[k + 11].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 11].tex = D3DXVECTOR2(1.0f, 0.0f);

				//上面右前
				poF_Wall.d3varrayI[k1 + 18] = poF_Wall.d3varrayI[k1 + 21] = k + 12;
				poF_Wall.d3varray[k + 12].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 200.0f);
				poF_Wall.d3varray[k + 12].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 12].tex = D3DXVECTOR2(0.0f, 0.0f);

				//上面右後
				poF_Wall.d3varrayI[k1 + 19] = k + 13;
				poF_Wall.d3varray[k + 13].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poF_Wall.d3varray[k + 13].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 13].tex = D3DXVECTOR2(0.0f, 1.0f);

				//上面左後
				poF_Wall.d3varrayI[k1 + 20] = poF_Wall.d3varrayI[k1 + 23] = k + 14;
				poF_Wall.d3varray[k + 14].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f);
				poF_Wall.d3varray[k + 14].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 14].tex = D3DXVECTOR2(1.0f, 1.0f);

				//上面左前
				poF_Wall.d3varrayI[k1 + 22] = k + 15;
				poF_Wall.d3varray[k + 15].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 200.0f);
				poF_Wall.d3varray[k + 15].color = (255 << 16) + (255 << 8) + 255;
				poF_Wall.d3varray[k + 15].tex = D3DXVECTOR2(1.0f, 0.0f);

				k += 16;
				k1 += 24;
			}
		}
	}
}

void Map::Mapcreate_Ground(float st, float end){

	//地面頂点
	int k = 0;
	int k1 = 0;
	float size = (end - st) / 30.0f;//1マスの大きさ
	for (int j = 0; j < 30; j++){
		for (int i = 0; i < 30; i++){
			float x = size * i + st;
			float y = size * j + st;
			//地面左上
			poGround.d3varrayI[k1] = poGround.d3varrayI[k1 + 3] = k;
			poGround.d3varray[k].p = D3DXVECTOR3(x, y, 0.0f);
			poGround.d3varray[k].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			poGround.d3varray[k].color = (255 << 16) + (255 << 8) + 255;
			poGround.d3varray[k].tex = D3DXVECTOR2(0.0f, 0.0f);

			//地面左下
			poGround.d3varrayI[k1 + 4] = k + 2;
			poGround.d3varray[k + 2].p = D3DXVECTOR3(x, y + size, 0.0f);
			poGround.d3varray[k + 2].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			poGround.d3varray[k + 2].color = (255 << 16) + (255 << 8) + 255;
			poGround.d3varray[k + 2].tex = D3DXVECTOR2(0.0f, 1.0f);

			//地面右下
			poGround.d3varrayI[k1 + 2] = poGround.d3varrayI[k1 + 5] = k + 3;
			poGround.d3varray[k + 3].p = D3DXVECTOR3(x + size, y + size, 0.0f);
			poGround.d3varray[k + 3].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			poGround.d3varray[k + 3].color = (255 << 16) + (255 << 8) + 255;
			poGround.d3varray[k + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//地面右上
			poGround.d3varrayI[k1 + 1] = k + 1;
			poGround.d3varray[k + 1].p = D3DXVECTOR3(x + size, y, 0.0f);
			poGround.d3varray[k + 1].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			poGround.d3varray[k + 1].color = (255 << 16) + (255 << 8) + 255;
			poGround.d3varray[k + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			k += 4;
			k1 += 6;
		}
	}
}

void Map::Mapcreate_Ceiling(float st, float end, float height){

	//天井頂点
	int k = 0;
	int k1 = 0;
	float size = (end - st) / 30.0f;//1マスの大きさ
	for (int j = 0; j < 30; j++){
		for (int i = 0; i < 30; i++){
			float x = size * i + st;
			float y = size * j + st;
			//天井左上
			poCeiling.d3varrayI[k1] = poCeiling.d3varrayI[k1 + 3] = k;
			poCeiling.d3varray[k].p = D3DXVECTOR3(x, y, height);
			poCeiling.d3varray[k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			poCeiling.d3varray[k].color = (255 << 16) + (255 << 8) + 255;
			poCeiling.d3varray[k].tex = D3DXVECTOR2(0.0f, 0.0f);

			//天井左下
			poCeiling.d3varrayI[k1 + 4] = k + 2;
			poCeiling.d3varray[k + 2].p = D3DXVECTOR3(x, y + size, height);
			poCeiling.d3varray[k + 2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			poCeiling.d3varray[k + 2].color = (255 << 16) + (255 << 8) + 255;
			poCeiling.d3varray[k + 2].tex = D3DXVECTOR2(0.0f, 1.0f);

			//天井右下
			poCeiling.d3varrayI[k1 + 2] = poCeiling.d3varrayI[k1 + 5] = k + 3;
			poCeiling.d3varray[k + 3].p = D3DXVECTOR3(x + size, y + size, height);
			poCeiling.d3varray[k + 3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			poCeiling.d3varray[k + 3].color = (255 << 16) + (255 << 8) + 255;
			poCeiling.d3varray[k + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//天井右上
			poCeiling.d3varrayI[k1 + 1] = k + 1;
			poCeiling.d3varray[k + 1].p = D3DXVECTOR3(x + size, y, height);
			poCeiling.d3varray[k + 1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			poCeiling.d3varray[k + 1].color = (255 << 16) + (255 << 8) + 255;
			poCeiling.d3varray[k + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			k += 4;
			k1 += 6;
		}
	}
}

void Map::Mapcreate_Background(float st, float end, float height){

	//上
	//左前
	poBackground.d3varrayI[0] = poBackground.d3varrayI[3] = 0;
	poBackground.d3varray[0].p = D3DXVECTOR3(st, st, height);
	poBackground.d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//左奥
	poBackground.d3varrayI[4] = 2;
	poBackground.d3varray[2].p = D3DXVECTOR3(st, st, 0.0f);
	poBackground.d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//右奥
	poBackground.d3varrayI[2] = poBackground.d3varrayI[5] = 3;
	poBackground.d3varray[3].p = D3DXVECTOR3(end, st, 0.0f);
	poBackground.d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[3].tex = D3DXVECTOR2(5.0f, 1.0f);

	//右前
	poBackground.d3varrayI[1] = 1;
	poBackground.d3varray[1].p = D3DXVECTOR3(end, st, height);
	poBackground.d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[1].tex = D3DXVECTOR2(5.0f, 0.0f);

	//下
	//左前
	poBackground.d3varrayI[6] = poBackground.d3varrayI[9] = 4;
	poBackground.d3varray[4].p = D3DXVECTOR3(st, end, height);
	poBackground.d3varray[4].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[4].tex = D3DXVECTOR2(0.0f, 0.0f);

	//左奥
	poBackground.d3varrayI[10] = 6;
	poBackground.d3varray[6].p = D3DXVECTOR3(st, end, 0.0f);
	poBackground.d3varray[6].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[6].tex = D3DXVECTOR2(0.0f, 1.0f);

	//右奥
	poBackground.d3varrayI[8] = poBackground.d3varrayI[11] = 7;
	poBackground.d3varray[7].p = D3DXVECTOR3(end, end, 0.0f);
	poBackground.d3varray[7].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[7].tex = D3DXVECTOR2(5.0f, 1.0f);

	//右前
	poBackground.d3varrayI[7] = 5;
	poBackground.d3varray[5].p = D3DXVECTOR3(end, end, height);
	poBackground.d3varray[5].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[5].tex = D3DXVECTOR2(5.0f, 0.0f);

	//右
	//上前
	poBackground.d3varrayI[12] = poBackground.d3varrayI[15] = 8;
	poBackground.d3varray[8].p = D3DXVECTOR3(end, st, height);
	poBackground.d3varray[8].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[8].tex = D3DXVECTOR2(0.0f, 0.0f);

	//上奥
	poBackground.d3varrayI[16] = 10;
	poBackground.d3varray[10].p = D3DXVECTOR3(end, st, 0.0f);
	poBackground.d3varray[10].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[10].tex = D3DXVECTOR2(0.0f, 1.0f);

	//下奥
	poBackground.d3varrayI[14] = poBackground.d3varrayI[17] = 11;
	poBackground.d3varray[11].p = D3DXVECTOR3(end, end, 0.0f);
	poBackground.d3varray[11].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[11].tex = D3DXVECTOR2(5.0f, 1.0f);

	//下前
	poBackground.d3varrayI[13] = 9;
	poBackground.d3varray[9].p = D3DXVECTOR3(end, end, height);
	poBackground.d3varray[9].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[9].tex = D3DXVECTOR2(5.0f, 0.0f);

	//左
	//上前
	poBackground.d3varrayI[18] = poBackground.d3varrayI[21] = 12;
	poBackground.d3varray[12].p = D3DXVECTOR3(st, st, height);
	poBackground.d3varray[12].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[12].tex = D3DXVECTOR2(0.0f, 0.0f);

	//上奥
	poBackground.d3varrayI[22] = 14;
	poBackground.d3varray[14].p = D3DXVECTOR3(st, st, 0.0f);
	poBackground.d3varray[14].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[14].tex = D3DXVECTOR2(0.0f, 1.0f);

	//下奥
	poBackground.d3varrayI[20] = poBackground.d3varrayI[23] = 15;
	poBackground.d3varray[15].p = D3DXVECTOR3(st, end, 0.0f);
	poBackground.d3varray[15].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[15].tex = D3DXVECTOR2(5.0f, 1.0f);

	//下前
	poBackground.d3varrayI[19] = 13;
	poBackground.d3varray[13].p = D3DXVECTOR3(st, end, height);
	poBackground.d3varray[13].color = (255 << 16) + (255 << 8) + 255;
	poBackground.d3varray[13].tex = D3DXVECTOR2(5.0f, 0.0f);
}

void Map::Mapcreate_Rain(){

	int k = 0;
	int x, y;
	for (int i = 0; i < 200; i++){
		x = rand() % 500;
		y = rand() % 500;
		poRain.d3varrayI[k] = k;
		poRain.d3varray[k].p = D3DXVECTOR3((float)x, (float)y, (float)(rand() % 300));
		poRain.d3varray[k].color = (255 << 16) + (255 << 8) + 255;

		poRain.d3varrayI[k + 1] = k + 1;
		poRain.d3varray[k + 1].p = D3DXVECTOR3((float)x, (float)y, 0.0f);
		poRain.d3varray[k + 1].color = (255 << 16) + (255 << 8) + 255;
		k += 2;
	}
	dx->D3primitive(LINE_L, &poRain, 200, cax1 - 250.0f, cay1 - 250.0f, 0.0f, 0, FALSE, TRUE, FALSE);
}

void Map::Mapcreate_ENTER(float x, float y, float z, float xsize){

	//左前
	poENTER.d3varrayI[0] = poENTER.d3varrayI[3] = 0;
	poENTER.d3varray[0].p = D3DXVECTOR3(x, y, z);
	poENTER.d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	poENTER.d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//左奥
	poENTER.d3varrayI[4] = 2;
	poENTER.d3varray[2].p = D3DXVECTOR3(x, y, 0.0f);
	poENTER.d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	poENTER.d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//右奥
	poENTER.d3varrayI[2] = poENTER.d3varrayI[5] = 3;
	poENTER.d3varray[3].p = D3DXVECTOR3(x + xsize, y, 0.0f);
	poENTER.d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	poENTER.d3varray[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//右前
	poENTER.d3varrayI[1] = 1;
	poENTER.d3varray[1].p = D3DXVECTOR3(x + xsize, y, z);
	poENTER.d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	poENTER.d3varray[1].tex = D3DXVECTOR2(1.0f, 0.0f);
}

void Map::Mapcreate_EXIT(float x, float y, float z, float xsize){
	//左前
	poEXIT.d3varrayI[0] = poEXIT.d3varrayI[3] = 0;
	poEXIT.d3varray[0].p = D3DXVECTOR3(x, y, z);
	poEXIT.d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	poEXIT.d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//左奥
	poEXIT.d3varrayI[4] = 2;
	poEXIT.d3varray[2].p = D3DXVECTOR3(x, y, 0.0f);
	poEXIT.d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	poEXIT.d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//右奥
	poEXIT.d3varrayI[2] = poEXIT.d3varrayI[5] = 3;
	poEXIT.d3varray[3].p = D3DXVECTOR3(x + xsize, y, 0.0f);
	poEXIT.d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	poEXIT.d3varray[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//右前
	poEXIT.d3varrayI[1] = 1;
	poEXIT.d3varray[1].p = D3DXVECTOR3(x + xsize, y, z);
	poEXIT.d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	poEXIT.d3varray[1].tex = D3DXVECTOR2(1.0f, 0.0f);
}

void Map::Mapcreate_Recover(){

	int k = 0;
	int k1 = 0;
	int k2 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 50)continue;//2のアスキーコード50
				//回復ポイント左上
				poRecover.d3varrayI[k1] = poRecover.d3varrayI[k1 + 3] = k;
				poRecover.d3varray[k].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 1.0f);
				poRecover.d3varray[k].color = (255 << 16) + (255 << 8) + 255;
				poRecover.d3varray[k].tex = D3DXVECTOR2(0.0f, 0.0f);

				//回復ポイント左下
				poRecover.d3varrayI[k1 + 4] = k + 2;
				poRecover.d3varray[k + 2].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f+ 100.0f, (float)k3 * 100.0f + 1.0f);
				poRecover.d3varray[k + 2].color = (255 << 16) + (255 << 8) + 255;
				poRecover.d3varray[k + 2].tex = D3DXVECTOR2(0.0f, 1.0f);

				//回復ポイント右下
				poRecover.d3varrayI[k1 + 2] = poRecover.d3varrayI[k1 + 5] = k + 3;
				poRecover.d3varray[k + 3].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 1.0f);
				poRecover.d3varray[k + 3].color = (255 << 16) + (255 << 8) + 255;
				poRecover.d3varray[k + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

				//回復ポイント右上
				poRecover.d3varrayI[k1 + 1] = k + 1;
				poRecover.d3varray[k + 1].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 1.0f);
				poRecover.d3varray[k + 1].color = (255 << 16) + (255 << 8) + 255;
				poRecover.d3varray[k + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				k += 4;
				k1 += 6;

				for (float i1 = 0; i1 < 360; i1 += 0.25f){
					float line_y = ((float)j * 100.0f + 50.0f) - (cos(i1 * 3.14f / 180.0f) * 50.0f);
					float line_x = ((float)i * 100.0f + 50.0f) + (sin(i1 * 3.14f / 180.0f) * 50.0f);
					int rnd = rand() % 20;

					poRecoverLine.d3varrayI[k2] = k2;
					poRecoverLine.d3varray[k2].p = D3DXVECTOR3(line_x, line_y, (float)k3 * 100.0f + 1.0f);
					poRecoverLine.d3varray[k2].color = (0 << 16) + (255 << 8) + 0;

					poRecoverLine.d3varrayI[k2 + 1] = k2 + 1;
					poRecoverLine.d3varray[k2 + 1].p = D3DXVECTOR3(line_x, line_y, 1.0f + (float)rnd + (float)k3 * 100.0f);
					poRecoverLine.d3varray[k2 + 1].color = (0 << 16) + (100 << 8) + 255;
					k2 += 2;
				}
			}
		}
	}
	dx->D3primitive(SQUARE, &poRecover, r_point_count, 0, 0, 0, 0, TRUE, FALSE, FALSE);
	dx->D3primitive(LINE_S, &poRecoverLine, r_point_count * 1440, 0, 0, 0, 0, FALSE, TRUE, FALSE);
}

void Map::Mapcreate_Ds(){

	//左前
	poMo.d3varrayI[0] = poMo.d3varrayI[3] = 0;
	poMo.d3varray[0].p = D3DXVECTOR3(-20.0f, 0.0f, 100.0f);
	poMo.d3varray[0].color = (255 << 16) + (255 << 8) + 255;
	poMo.d3varray[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//左奥
	poMo.d3varrayI[4] = 2;
	poMo.d3varray[2].p = D3DXVECTOR3(-20.0f, 0.0f, 60.0f);
	poMo.d3varray[2].color = (255 << 16) + (255 << 8) + 255;
	poMo.d3varray[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	//右奥
	poMo.d3varrayI[2] = poMo.d3varrayI[5] = 3;
	poMo.d3varray[3].p = D3DXVECTOR3(20.0f, 0.0f, 60.0f);
	poMo.d3varray[3].color = (255 << 16) + (255 << 8) + 255;
	poMo.d3varray[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//右前
	poMo.d3varrayI[1] = 1;
	poMo.d3varray[1].p = D3DXVECTOR3(20.0f, 0.0f, 100.0f);
	poMo.d3varray[1].color = (255 << 16) + (255 << 8) + 255;
	poMo.d3varray[1].tex = D3DXVECTOR2(1.0f, 0.0f);

	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){

				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 52)continue;//1のアスキーコード49

				dx->D3primitive(SQUARE, &poMo, 1, (float)i * 100.0f - 10.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f, src_theta, TRUE, FALSE, FALSE);
				dx->D3primitive(SQUARE, &poMo, 1, (float)i * 100.0f + 110.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f, src_theta, TRUE, FALSE, FALSE);
				dx->D3primitive(SQUARE, &poMo, 1, (float)i * 100.0f + 50.0f, (float)j * 100.0f - 10.0f, (float)k3 * 100.0f, src_theta, TRUE, FALSE, FALSE);
				dx->D3primitive(SQUARE, &poMo, 1, (float)i * 100.0f + 50.0f, (float)j * 100.0f + 110.0f, (float)k3 * 100.0f, src_theta, TRUE, FALSE, FALSE);
			}
		}
	}
}

void Map::Mapcreate_BossPoint(){

	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 51)continue;
				//ボスポイント左前
				poBoss.d3varrayI[k1] = poBoss.d3varrayI[k1 + 3] = k;
				poBoss.d3varray[k].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f);
				poBoss.d3varray[k].color = (255 << 16) + (255 << 8) + 255;
				poBoss.d3varray[k].tex = D3DXVECTOR2(0.0f, 0.0f);

				//ボスポイント左奥
				poBoss.d3varrayI[k1 + 4] = k + 2;
				poBoss.d3varray[k + 2].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f);
				poBoss.d3varray[k + 2].color = (255 << 16) + (255 << 8) + 255;
				poBoss.d3varray[k + 2].tex = D3DXVECTOR2(0.0f, 1.0f);

				//ボスポイント右奥
				poBoss.d3varrayI[k1 + 2] = poBoss.d3varrayI[k1 + 5] = k + 3;
				poBoss.d3varray[k + 3].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f);
				poBoss.d3varray[k + 3].color = (255 << 16) + (255 << 8) + 255;
				poBoss.d3varray[k + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

				//ボスポイント右前
				poBoss.d3varrayI[k1 + 1] = k + 1;
				poBoss.d3varray[k + 1].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f);
				poBoss.d3varray[k + 1].color = (255 << 16) + (255 << 8) + 255;
				poBoss.d3varray[k + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				k += 4;
				k1 += 6;
			}
		}
	}
}

void Map::Mapcreate_Elevator(){

	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 65 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 66)continue;
				//回復ポイント左上
				poElevator.d3varrayI[k1] = poElevator.d3varrayI[k1 + 3] = k;
				poElevator.d3varray[k].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 1.0f);
				poElevator.d3varray[k].color = (255 << 16) + (255 << 8) + 255;
				poElevator.d3varray[k].tex = D3DXVECTOR2(0.0f, 0.0f);

				//回復ポイント左下
				poElevator.d3varrayI[k1 + 4] = k + 2;
				poElevator.d3varray[k + 2].p = D3DXVECTOR3((float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 1.0f);
				poElevator.d3varray[k + 2].color = (255 << 16) + (255 << 8) + 255;
				poElevator.d3varray[k + 2].tex = D3DXVECTOR2(0.0f, 1.0f);

				//回復ポイント右下
				poElevator.d3varrayI[k1 + 2] = poElevator.d3varrayI[k1 + 5] = k + 3;
				poElevator.d3varray[k + 3].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 1.0f);
				poElevator.d3varray[k + 3].color = (255 << 16) + (255 << 8) + 255;
				poElevator.d3varray[k + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

				//回復ポイント右上
				poElevator.d3varrayI[k1 + 1] = k + 1;
				poElevator.d3varray[k + 1].p = D3DXVECTOR3((float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 1.0f);
				poElevator.d3varray[k + 1].color = (255 << 16) + (255 << 8) + 255;
				poElevator.d3varray[k + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				k += 4;
				k1 += 6;
			}
		}
	}
}

Map::~Map(){

	free(mxy.m);
	mxy.m = NULL;

	MovieSoundManager::ObjDelete_map();
}