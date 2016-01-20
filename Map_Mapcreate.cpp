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

int Map::GetBossKilled(int map_no){
	return boss_killed[map_no];
}

Map::Map(){}

Map::Map(Position::H_Pos *h_p){

	map_no = map_no_s;
	dx = Dx11Process::GetInstance();
	text = DxText::GetInstance();
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
		poWall1.GetTexture(Wno1);
		poWall1.GetVBarray(SQUARE, 1);
	}

	//壁(ブロック)
	if (blockcount >= 1){
		poWall.GetTexture(Wno);
		poWall.GetVBarrayCPUNotAccess(blockcount * 6);
		poWall.Light(TRUE);
		Mapcreate_Wall();
	}

	if (map_no == 1){
		//外背景
		poBackground.GetTexture(10);
		poBackground.GetVBarray(SQUARE, 4);
		Mapcreate_Background(Gro_st, Gro_end, Cei_height);
		//雨
		poRain.GetVBarray(LINE_L, 200);
	}

	//地面
	poGround.GetTexture(Gno);
	poGround.GetVBarrayCPUNotAccess(900);
	poGround.Light(TRUE);
	Mapcreate_Ground(Gro_st, Gro_end);

	//空
	poCeiling.GetTexture(Cno);
	poCeiling.GetVBarrayCPUNotAccess(900);
	poCeiling.Light(TRUE);
	Mapcreate_Ceiling(Cei_st, Cei_end, Cei_height);

	//出口
	if (map_no != 4 && map_no != 3){
		poEXIT.GetTexture(Exit);
		poEXIT.GetVBarray(SQUARE, 1);
		Mapcreate_EXIT(Ex_x, Ex_y, Ex_z, Ex_si);
	}

	//入り口
	if (map_no != 0){
		poENTER.GetTexture(Enter);
		poENTER.GetVBarray(SQUARE, 1);
		Mapcreate_ENTER(En_x, En_y, En_z, En_si);
	}

	//リカバーポイント
	if (r_point_count >= 1){
		poRecover.GetTexture(70);
		poRecover.GetVBarray(SQUARE, r_point_count);
		poRecoverLine.GetVBarray(LINE_S, r_point_count * 1440);
	}

	//動画テクスチャ松明
	if (mo_count >= 1){
		poMo.TextureInit(128, 128);
		poMo.GetVBarray(SQUARE, 1);
	}

	//動画テクスチャ炎壁
	if (f_wall_count >= 1){
		poF_Wall.TextureInit(256, 256);
		poF_Wall.GetVBarray(SQUARE, f_wall_count * 4);
		Mapcreate_F_Wall();
	}

	//ボス出現ポイント
	if (boss_count >= 1){
		poBoss.GetTexture(61);
		poBoss.GetVBarray(SQUARE, boss_count);
		Mapcreate_BossPoint();
	}

	//エレベーター
	if (Elevator_count >= 1){
		poElevator.GetTexture(70);
		poElevator.GetVBarray(SQUARE, Elevator_count);
		Mapcreate_Elevator();
	}

	//ライトポジション構造体確保
	light = (LightPos*)malloc(sizeof(LightPos) * lightcount);
}

void Map::Mapcreate_Wall1(){
	//左前
	poWall1.SetVertex(0, 3, 0,
		-50.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//左奥
	poWall1.SetVertex(4, 2,
		-50.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//右奥
	poWall1.SetVertex(2, 5, 3,
		50.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);

	//右前
	poWall1.SetVertex(1, 1,
		50.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);

	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 53)continue;
				poWall1.D3primitive((float)i * 100.0f + 50.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE);
			}
		}
	}
}

void Map::Mapcreate_Wall(){

	int k = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){

				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 49 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 52)continue;//1のアスキーコード49
				//壁ブロック頂点設定
				//正面左上
				poWall.SetVertex(k, k,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//正面左下
				poWall.SetVertex(k + 1, k + 1,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//正面右下
				poWall.SetVertex(k + 2, k + 2,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//正面右上
				poWall.SetVertex(k + 3, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//左面上前
				poWall.SetVertex(k + 4, k + 4,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f,
					-1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//左面上後
				poWall.SetVertex(k + 5, k + 5,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					-1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//左面下後
				poWall.SetVertex(k + 6, k + 6,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					-1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//左面下前
				poWall.SetVertex(k + 7, k + 7,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f,
					-1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//右面下前
				poWall.SetVertex(k + 8, k + 8,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//右面下後
				poWall.SetVertex(k + 9, k + 9,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//右面上後
				poWall.SetVertex(k + 10, k + 10,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//右面上前
				poWall.SetVertex(k + 11, k + 11,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//上面右前
				poWall.SetVertex(k + 12, k + 12,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, -1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//上面右後
				poWall.SetVertex(k + 13, k + 13,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					0.0f, -1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//上面左後
				poWall.SetVertex(k + 14, k + 14,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					0.0f, -1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//上面左前
				poWall.SetVertex(k + 15, k + 15,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, -1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//裏面右上
				poWall.SetVertex(k + 16, k + 16,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, -1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//裏面右下
				poWall.SetVertex(k + 17, k + 17,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, -1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//裏面左下
				poWall.SetVertex(k + 18, k + 18,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, -1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//裏面左上
				poWall.SetVertex(k + 19, k + 19,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, -1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//底面左前
				poWall.SetVertex(k + 20, k + 20,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//底面左後
				poWall.SetVertex(k + 21, k + 21,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//底面右後
				poWall.SetVertex(k + 22, k + 22,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//底面右前
				poWall.SetVertex(k + 23, k + 23,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				k += 24;
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
				poF_Wall.SetVertex(k1, k1 + 3, k,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 200.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//左面上後
				poF_Wall.SetVertex(k1 + 1, k + 1,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//左面下後
				poF_Wall.SetVertex(k1 + 2, k1 + 5, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//左面下前
				poF_Wall.SetVertex(k1 + 4, k + 3,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 200.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//底面左前
				poF_Wall.SetVertex(k1 + 6, k1 + 9, k + 4,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 200.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//底面左後
				poF_Wall.SetVertex(k1 + 7, k + 5,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//底面右後
				poF_Wall.SetVertex(k1 + 8, k1 + 11, k + 6,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//底面右前
				poF_Wall.SetVertex(k1 + 10, k + 7,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 200.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//右面下前
				poF_Wall.SetVertex(k1 + 12, k1 + 15, k + 8,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 200.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//右面下後
				poF_Wall.SetVertex(k1 + 13, k + 9,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//右面上後
				poF_Wall.SetVertex(k1 + 14, k1 + 17, k + 10,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//右面上前
				poF_Wall.SetVertex(k1 + 16, k + 11,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 200.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//上面右前
				poF_Wall.SetVertex(k1 + 18, k1 + 21, k + 12,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 200.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//上面右後
				poF_Wall.SetVertex(k1 + 19, k + 13,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//上面左後
				poF_Wall.SetVertex(k1 + 20, k1 + 23, k + 14,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//上面左前
				poF_Wall.SetVertex(k1 + 22, k + 15,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 200.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				k += 16;
				k1 += 24;
			}
		}
	}
}

void Map::Mapcreate_Ground(float st, float end){

	//地面頂点
	int k = 0;
	float size = (end - st) / 30.0f;//1マスの大きさ
	for (int j = 0; j < 30; j++){
		for (int i = 0; i < 30; i++){
			float x = size * i + st;
			float y = size * j + st;
			//地面左上
			poGround.SetVertex(k, k,
				x, y, 0.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 0.0f);

			//地面左下
			poGround.SetVertex(k + 1, k + 1,
				x, y + size, 0.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 1.0f);

			//地面右下
			poGround.SetVertex(k + 2, k + 2,
				x + size, y + size, 0.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f);

			//地面右上
			poGround.SetVertex(k + 3, k + 3,
				x + size, y, 0.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f);

			k += 4;
		}
	}
}

void Map::Mapcreate_Ceiling(float st, float end, float height){

	//天井頂点
	int k = 0;
	float size = (end - st) / 30.0f;//1マスの大きさ
	for (int j = 0; j < 30; j++){
		for (int i = 0; i < 30; i++){
			float x = size * i + st;
			float y = size * j + st;
			//天井左上
			poCeiling.SetVertex(k, k,
				x, y, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 0.0f);

			//天井左下
			poCeiling.SetVertex(k + 1, k + 1,
				x, y + size, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 1.0f);

			//天井右下
			poCeiling.SetVertex(k + 2, k + 2,
				x + size, y + size, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f);

			//天井右上
			poCeiling.SetVertex(k + 3, k + 3,
				x + size, y, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f);

			k += 4;
		}
	}
}

void Map::Mapcreate_Background(float st, float end, float height){

	//上
	//左前
	poBackground.SetVertex(0, 3, 0,
		st, st, height,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//左奥
	poBackground.SetVertex(4, 2,
		st, st, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//右奥
	poBackground.SetVertex(2, 5, 3,
		end, st, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 1.0f);

	//右前
	poBackground.SetVertex(1, 1,
		end, st, height,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//下
	//左前
	poBackground.SetVertex(6, 9, 4,
		st, end, height,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//左奥
	poBackground.SetVertex(10, 6,
		st, end, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//右奥
	poBackground.SetVertex(8, 11, 7,
		end, end, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 1.0f);

	//右前
	poBackground.SetVertex(7, 5,
		end, end, height,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//右
	//上前
	poBackground.SetVertex(12, 15, 8,
		end, st, height,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//上奥
	poBackground.SetVertex(16, 10,
		end, st, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//下奥
	poBackground.SetVertex(14, 17, 11,
		end, end, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 1.0f);

	//下前
	poBackground.SetVertex(13, 9,
		end, end, height,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//左
	//上前
	poBackground.SetVertex(18, 21, 12,
		st, st, height,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//上奥
	poBackground.SetVertex(22, 14,
		st, st, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//下奥
	poBackground.SetVertex(20, 23, 15,
		st, end, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 1.0f);

	//下前
	poBackground.SetVertex(19, 13,
		st, end, height,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);
}

void Map::Mapcreate_Rain(){

	int k = 0;
	int x, y;
	for (int i = 0; i < 200; i++){
		x = rand() % 500;
		y = rand() % 500;
		poRain.SetVertex(k, k,
			(float)x, (float)y, (float)(rand() % 300),
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f);

		poRain.SetVertex(k + 1, k + 1,
			(float)x, (float)y, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f);
		k += 2;
	}
	poRain.D3primitive(cax1 - 250.0f, cay1 - 250.0f, 0.0f, 0, 0, 0, 0, FALSE, TRUE);
}

void Map::Mapcreate_ENTER(float x, float y, float z, float xsize){

	//左前
	poENTER.SetVertex(0, 3, 0,
		x, y, z,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//左奥
	poENTER.SetVertex(4, 2,
		x, y, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//右奥
	poENTER.SetVertex(2, 5, 3,
		x + xsize, y, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);

	//右前
	poENTER.SetVertex(1, 1,
		x + xsize, y, z,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);
}

void Map::Mapcreate_EXIT(float x, float y, float z, float xsize){

	//左前
	poEXIT.SetVertex(0, 3, 0,
		x, y, z,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//左奥
	poEXIT.SetVertex(4, 2,
		x, y, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//右奥
	poEXIT.SetVertex(2, 5, 3,
		x + xsize, y, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);

	//右前
	poEXIT.SetVertex(1, 1,
		x + xsize, y, z,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);
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
				poRecover.SetVertex(k1, k1 + 3, k,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//回復ポイント左下
				poRecover.SetVertex(k1 + 4, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//回復ポイント右下
				poRecover.SetVertex(k1 + 2, k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//回復ポイント右上
				poRecover.SetVertex(k1 + 1, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				k += 4;
				k1 += 6;

				for (float i1 = 0; i1 < 360; i1 += 0.25f){
					float line_y = ((float)j * 100.0f + 50.0f) - (cos(i1 * 3.14f / 180.0f) * 50.0f);
					float line_x = ((float)i * 100.0f + 50.0f) + (sin(i1 * 3.14f / 180.0f) * 50.0f);
					int rnd = rand() % 20;

					poRecoverLine.SetVertex(k2, k2,
						line_x, line_y, (float)k3 * 100.0f + 1.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 0.0f);

					poRecoverLine.SetVertex(k2 + 1, k2 + 1,
						line_x, line_y, 1.0f + (float)rnd + (float)k3 * 100.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, 0.4f, 1.0f, 1.0f,
						0.0f, 0.0f);

					k2 += 2;
				}
			}
		}
	}
	poRecover.D3primitive(0, 0, 0, 0, 0, 0, 0, TRUE, FALSE);
	poRecoverLine.D3primitive(0, 0, 0, 0, 0, 0, 0, FALSE, TRUE);
}

void Map::Mapcreate_Ds(){

	//左前
	poMo.SetVertex(0, 3, 0,
		-20.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.8f,
		0.0f, 0.0f);

	//左奥
	poMo.SetVertex(4, 2,
		-20.0f, 0.0f, 60.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.8f,
		0.0f, 1.0f);

	//右奥
	poMo.SetVertex(2, 5, 3,
		20.0f, 0.0f, 60.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.8f,
		1.0f, 1.0f);

	//右前
	poMo.SetVertex(1, 1,
		20.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.8f,
		1.0f, 0.0f);

	int licnt = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){

				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 52)continue;//1のアスキーコード49

				//左(マップ端に配置する場合アクセス違反防止を追加すること)
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i - 1] == 48){
					poMo.D3primitive((float)i * 100.0f - 10.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE);
					light[licnt].x = i * 100.0f - 10.0f;
					light[licnt].y = j * 100.0f + 50.0f;
					light[licnt].z = k3 * 100.0f + 75.0f;
					light[licnt].r = 1.0f;
					light[licnt].g = 0.4f;
					light[licnt].b = 0.4f;
					light[licnt].a = 1.0f;
					light[licnt].range = 80.0f;
					light[licnt].brightness = 0.6f;
					light[licnt].attenuation = 2.0f;
					light[licnt].on_off = TRUE;
					licnt++;
				}
				//右
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i + 1] == 48){
					poMo.D3primitive((float)i * 100.0f + 110.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE);
					light[licnt].x = i * 100.0f + 110.0f;
					light[licnt].y = j * 100.0f + 50.0f;
					light[licnt].z = k3 * 100.0f + 75.0f;
					light[licnt].r = 1.0f;
					light[licnt].g = 0.4f;
					light[licnt].b = 0.4f;
					light[licnt].a = 1.0f;
					light[licnt].range = 80.0f;
					light[licnt].brightness = 0.6f;
					light[licnt].attenuation = 2.0f;
					light[licnt].on_off = TRUE;
					licnt++;
				}
				//上
				if (mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] == 48){
					poMo.D3primitive((float)i * 100.0f + 50.0f, (float)j * 100.0f - 10.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE);
					light[licnt].x = i * 100.0f + 50.0f;
					light[licnt].y = j * 100.0f - 10.0f;
					light[licnt].z = k3 * 100.0f + 75.0f;
					light[licnt].r = 1.0f;
					light[licnt].g = 0.4f;
					light[licnt].b = 0.4f;
					light[licnt].a = 1.0f;
					light[licnt].range = 80.0f;
					light[licnt].brightness = 0.6f;
					light[licnt].attenuation = 2.0f;
					light[licnt].on_off = TRUE;
					licnt++;
				}
				//下
				if (mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] == 48){
					poMo.D3primitive((float)i * 100.0f + 50.0f, (float)j * 100.0f + 110.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE);
					light[licnt].x = i * 100.0f + 50.0f;
					light[licnt].y = j * 100.0f + 110.0f;
					light[licnt].z = k3 * 100.0f + 75.0f;
					light[licnt].r = 1.0f;
					light[licnt].g = 0.4f;
					light[licnt].b = 0.4f;
					light[licnt].a = 1.0f;
					light[licnt].range = 80.0f;
					light[licnt].brightness = 0.6f;
					light[licnt].attenuation = 2.0f;
					light[licnt].on_off = TRUE;
					licnt++;
				}
			}
		}
	}
	int loopcount = 2;//ライトのインデックス(0,1は固定)
	//各ライト設定
	for (int i = 0; i < licnt && loopcount < LIGHT_PCS; i++){
		dx->LightPosSet(loopcount, light[i].x, light[i].y, light[i].z, light[i].r, light[i].g, light[i].b, light[i].a, light[i].range, light[i].brightness, light[i].attenuation, light[i].on_off);
		loopcount++;
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
				poBoss.SetVertex(k1, k1 + 3, k,
					(float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//ボスポイント左奥
				poBoss.SetVertex(k1 + 4, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//ボスポイント右奥
				poBoss.SetVertex(k1 + 2, k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//ボスポイント右前
				poBoss.SetVertex(k1 + 1, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

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
				//エレベータ左上
				poElevator.SetVertex(k1, k1 + 3, k,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//エレベータ左下
				poElevator.SetVertex(k1 + 4, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//エレベータ右下
				poElevator.SetVertex(k1 + 2, k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				//エレベータ右上
				poElevator.SetVertex(k1 + 1, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				k += 4;
				k1 += 6;
			}
		}
	}
}

Map::~Map(){

	free(light);
	light = NULL;
	free(mxy.m);
	mxy.m = NULL;
	MovieSoundManager::ObjDelete_map();
}