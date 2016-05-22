//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Map�N���X                                       **//
//**                                    Mapcreate�֐�                                    **//
//*****************************************************************************************//

#include "Map.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int Map::map_no_s;
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

Map::Map(Position::H_Pos *h_p,Hero *hero){

	map_no = map_no_s;
	dx = Dx11Process::GetInstance();
	text = DxText::GetInstance();
	he = hero;
	walkI = -1;
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
	wood = NULL;
	wall1 = NULL;

	switch (map_no){
	case 0:
		//�o��
		poEXIT.GetVBarray(SQUARE, 1);
		Mapcreate_EXIT(400.0f, 0.5f, 100.0f, 100.0f);
		//�n�ʃ��C��
		poGroundM.GetTexture(1);
		poGroundM.GetVBarrayCPUNotAccess(1050);
		poGroundM.Light(TRUE);
		Mapcreate_Ground(&poGroundM, 30, 35, 0.0f, 1.0f);
		//�󃁃C��
		poCeilingM.GetTexture(2);
		poCeilingM.GetVBarrayCPUNotAccess(1050);
		poCeilingM.Light(TRUE);
		Mapcreate_Ceiling(&poCeilingM, 30, 35, 100.0f, 1.0f);
		break;
	case 1:
		//�R
		mountain.GetVBarray("./dat/mesh/mountain.obj", FALSE);
		//�n�ʓ����
		poGroundF.GetTexture(1);
		poGroundF.GetVBarrayCPUNotAccess(6);
		poGroundF.Light(TRUE);
		Mapcreate_Ground(&poGroundF, 2, 3, 2.0f, 1.0f);
		//������
		poCeilingF.GetTexture(2);
		poCeilingF.GetVBarrayCPUNotAccess(6);
		poCeilingF.Light(TRUE);
		Mapcreate_Ceiling(&poCeilingF, 2, 3, 100.0f, 1.0f);
		//�n�ʃ��C��
		poGroundM.GetTexture(5);
		poGroundM.GetVBarrayCPUNotAccess(1600);
		poGroundM.Light(TRUE);
		Mapcreate_Ground(&poGroundM, 40, 40, 0.0f, 1.0f);
		//�󃁃C��
		poBackground.GetTexture(6);
		poBackground.GetVBarray(SQUARE, 5);
		Mapcreate_Background(-3500.0f, 7500.0f);
		//�J
		poRain.GetVBarray(LINE_L, 200);
		//�n�ʏo��
		poGroundE.GetTexture(12);
		poGroundE.GetVBarrayCPUNotAccess(6);
		poGroundE.Light(TRUE);
		Mapcreate_Ground(&poGroundE, 2, 3, 2.0f, 1.0f);
		//��o��
		poCeilingE.GetTexture(11);
		poCeilingE.GetVBarrayCPUNotAccess(6);
		poCeilingE.Light(TRUE);
		Mapcreate_Ceiling(&poCeilingE, 2, 3, 100.0f, 1.0f);
		break;
	case 2:
		//����
		poEXIT.GetVBarray(SQUARE, 1);
		Mapcreate_EXIT(-50.0f, -50.0f, 100.0f, 100.0f);
		//�n�ʃ��C��
		poGroundM.GetTexture(12);
		poGroundM.GetVBarrayCPUNotAccess(1050);
		poGroundM.Light(TRUE);
		Mapcreate_Ground(&poGroundM, 30, 35, 0.0f, 1.0f);
		//�󃁃C��
		poCeilingM.GetTexture(11);
		poCeilingM.GetVBarrayCPUNotAccess(1050);
		poCeilingM.Light(TRUE);
		Mapcreate_Ceiling(&poCeilingM, 30, 35, 200.0f, 1.0f);
		//�n�ʏo��
		poGroundE.GetTexture(15);
		poGroundE.GetVBarrayCPUNotAccess(8);
		poGroundE.Light(TRUE);
		Mapcreate_Ground(&poGroundE, 2, 4, 0.0f, 1.0f);
		//��o��
		poCeilingE.GetTexture(15);
		poCeilingE.GetVBarrayCPUNotAccess(8);
		poCeilingE.Light(TRUE);
		Mapcreate_Ceiling(&poCeilingE, 2, 4, 200.0f, 1.0f);
		break;
	case 3:
		//�n�ʓ����
		poGroundF.GetTexture(12);
		poGroundF.GetVBarrayCPUNotAccess(6);
		poGroundF.Light(TRUE);
		Mapcreate_Ground(&poGroundF, 2, 3, 0.0f, 1.0f);
		//������
		poCeilingF.GetTexture(11);
		poCeilingF.GetVBarrayCPUNotAccess(6);
		poCeilingF.Light(TRUE);
		Mapcreate_Ceiling(&poCeilingF, 2, 3, 100.0f, 1.0f);
		//�n�ʃ��C��
		poGroundM.GetTexture(15);
		poGroundM.GetVBarrayCPUNotAccess(900);
		poGroundM.Light(TRUE);
		Mapcreate_Ground(&poGroundM, 30, 30, 0.0f, 1.0f);
		//�󃁃C��
		poCeilingM.GetTexture(15);
		poCeilingM.GetVBarrayCPUNotAccess(900);
		poCeilingM.Light(TRUE);
		Mapcreate_Ceiling(&poCeilingM, 30, 30, 200.0f, 1.0f);
		break;
	case 4:
		//�n�ʃ��C��
		poGroundM.GetTexture(27);
		poGroundM.GetVBarrayCPUNotAccess(900);
		poGroundM.Light(TRUE);
		Mapcreate_Ground(&poGroundM, 30, 30, 0.0f, 1.0f);
		//�󃁃C��
		poCeilingM.GetTexture(28);
		poCeilingM.GetVBarrayCPUNotAccess(900);
		poCeilingM.Light(TRUE);
		Mapcreate_Ceiling(&poCeilingM, 30, 30, 800.0f, 1.0f);
		break;
	}

	Mapfilter(h_p);

	//�z�u�ʒu���炵�p
	wood = new OBJPosRandomValue[woodcount];
	wall1 = new OBJPosRandomValue[squarecount];

	//��
	if (woodcount > 0){
		mWood.GetVBarray("./dat/mesh/tree.obj", FALSE);
	}
	
	//��(��)
	if (squarecount >= 1){
		poWall1.GetTexture(4);
		poWall1.GetVBarray(SQUARE, 1);
		poWall1.Light(TRUE);
		Mapcreate_Wall1();
	}

	//��(�u���b�N)
	if (blockcountA >= 1){
		poWallA.GetTexture(0);
		poWallA.GetVBarrayCPUNotAccess(blockcountA);
		poWallA.Light(TRUE);
		Mapcreate_Wall(&poWallA, 49, 52, 100.0f, -1.0f, 1.0f);
	}
	if (blockcountB >= 1){
		poWallB.GetTexture(8);
		poWallB.GetVBarrayCPUNotAccess(blockcountB);
		poWallB.Light(TRUE);
		Mapcreate_Wall(&poWallB, 74, 74, 100.0f, -1.0f, 1.0f);
	}
	if (blockcountC >= 1){
		poWallC.GetTexture(11);
		poWallC.GetVBarrayCPUNotAccess(blockcountC);
		poWallC.Light(TRUE);
		Mapcreate_Wall(&poWallC, 75, 79, 100.0f, -1.0f, 1.0f);
	}
	if (blockcountD >= 1){
		poWallD.GetTexture(15);
		poWallD.GetVBarrayCPUNotAccess(blockcountD);
		poWallD.Light(TRUE);
		Mapcreate_Wall(&poWallD, 76, 76, 100.0f, -2.0f, 2.0f);
	}
	if (blockcountE >= 1){
		poWallE.GetTexture(26);
		poWallE.GetVBarrayCPUNotAccess(blockcountE);
		poWallE.Light(TRUE);
		Mapcreate_Wall(&poWallE, 77, 78, 100.0f, -2.0f, 2.0f);
	}

	//���J�o�[�|�C���g
	if (r_point_count >= 1){
		poRecover.GetTexture(70);
		poRecover.GetVBarray(SQUARE, r_point_count);
		poRecoverLine.GetVBarray(LINE_S, r_point_count * 1440);
		Mapcreate_Recover();
	}

	//����e�N�X�`������
	if (mo_count >= 1){
		poMo.TextureInit(128, 128);
		poMo.GetVBarray(SQUARE, 1);
		Mapcreate_Ds();
	}

	//����e�N�X�`������
	if (f_wall_count >= 1){
		poF_Wall.TextureInit(256, 256);
		poF_Wall.GetVBarrayCPUNotAccess(f_wall_count);
		Mapcreate_Wall(&poF_Wall, 73, 73, 200.0f, -6.0f, 6.0f);
	}

	//�{�X�o���|�C���g
	if (boss_count >= 1){
		poBoss.GetTexture(61);
		poBoss.GetVBarray(SQUARE, boss_count);
		Mapcreate_BossPoint();
	}

	//�G���x�[�^�[
	if (Elevator_count >= 1){
		poElevator.GetTexture(70);
		poElevator.GetVBarray(SQUARE, Elevator_count);
		Mapcreate_Elevator();
	}

	//���C�g�|�W�V�����\���̊m��
	light = (LightPos*)malloc(sizeof(LightPos) * lightcount);
	//�|�C���g���C�g���Z�b�g
	dx->ResetPointLight();
}

void Map::Mapdraw_Wood(){

	int p = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 87)continue;
				float x = (float)i * 100.0f + 50.0f + wood[p].x;
				float y = (float)j * 100.0f + 50.0f + wood[p++].y;
				float z = (float)k3 * 100.0f;
				float xx = cax1 - x;
				float yy = cay1 - y;
				float zz = (float)posz * 100.0f - z;
				if (sqrt(xx * xx + yy * yy + zz * zz) > 600.0f)continue;
				mWood.Draw(x, y, z, 0, 0, 0, 0, 0, 0, 10.0f, 0.3f);
			}
		}
	}
}

void Map::Mapdraw_Mountain(){
	mountain.Draw(-1500.0f, 2000.0f, 0, 0, 0, 0, 0, 0, 0, 500.0f, 0.3f);
	mountain.Draw(5500.0f, 2000.0f, 0, 0, 0, 0, 0, 0, 0, 500.0f, 0.3f);
}

void Map::Mapcreate_Wall1(){
	//���O
	poWall1.SetVertex(0, 0,
		-50.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//�E�O
	poWall1.SetVertex(1, 4, 1,
		50.0f, 0.0f, 100.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);

	//����
	poWall1.SetVertex(2, 3, 2,
		-50.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//�E��
	poWall1.SetVertex(5, 3,
		50.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);
}

void Map::Mapdraw_Wall1(){

	int p = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 53)continue;
				float x = (float)i * 100.0f + 50.0f + wall1[p].x;
				float y = (float)j * 100.0f + 50.0f + wall1[p++].y;
				float z = (float)k3 * 100.0f;
				poWall1.Draw(x, y, z, 0, 0, 0, src_theta, TRUE, FALSE, 0);
			}
		}
	}
}

void Map::Mapcreate_Wall(PolygonData *pd, int no1, int no2, float height, float adjust, float adjust2){

	int k = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){

				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != no1 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != no2)continue;
				//�ǃu���b�N���_�ݒ�
				//���ʌ���
				if (k3 < mxy.z - 1 && mxy.m[(k3 + 1) * mxy.y * mxy.x + j * mxy.x + i] != no1 &&
					mxy.m[(k3 + 1) * mxy.y * mxy.x + j * mxy.x + i] != no2){
					//���ʍ���
					pd->SetVertex(k, k,
						(float)i * 100.0f - adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust,
						0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//���ʍ���
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust,
						0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//���ʉE��
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust,
						0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//���ʉE��
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust,
						0.0f, 0.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//���ʌ���
				if (i > 0 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i - 1)] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i - 1)] != no2){
					//���ʏ�O
					pd->SetVertex(k, k,
						(float)i * 100.0f - adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust2,
						-1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//���ʏ��
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f - adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust2,
						-1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//���ʉ���
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f - adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust2,
						-1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//���ʉ��O
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f - adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust2,
						-1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//�E�ʌ���
				if (i < mxy.x - 1 && mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i + 1)] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + (i + 1)] != no2){
					//�E�ʉ��O
					pd->SetVertex(k, k,
						(float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f + height + adjust2,
						1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//�E�ʉ���
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust2,
						1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//�E�ʏ��
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust2,
						1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//�E�ʏ�O
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f + 100.0f + adjust, (float)j * 100.0f - adjust2, (float)k3 * 100.0f + height + adjust2,
						1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//��ʌ���
				if (j > 0 && mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] != no2){
					//��ʉE�O
					pd->SetVertex(k, k,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f + height + adjust2,
						0.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//��ʉE��
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f - adjust2,
						0.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//��ʍ���
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f - adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f - adjust2,
						0.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//��ʍ��O
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f - adjust2, (float)j * 100.0f - adjust, (float)k3 * 100.0f + height + adjust2,
						0.0f, -1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//���ʌ���
				if (k3 > 0 && mxy.m[(k3 - 1) * mxy.y * mxy.x + j * mxy.x + i] != no1 &&
					mxy.m[(k3 - 1) * mxy.y * mxy.x + j * mxy.x + i] != no2){
					//���ʉE��
					pd->SetVertex(k, k,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust,
						0.0f, 0.0f, -1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//���ʉE��
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust,
						0.0f, 0.0f, -1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//���ʍ���
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust2, (float)k3 * 100.0f - adjust,
						0.0f, 0.0f, -1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//���ʍ���
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f - adjust2, (float)j * 100.0f - adjust2, (float)k3 * 100.0f - adjust,
						0.0f, 0.0f, -1.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}

				//��ʌ���
				if (j < mxy.y - 1 && mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] != no1 &&
					mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] != no2){
					//��ʍ��O
					pd->SetVertex(k, k,
						(float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f + height + adjust2,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 0.0f);

					//��ʍ���
					pd->SetVertex(k + 1, k + 1,
						(float)i * 100.0f - adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f - adjust2,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						0.0f, 1.0f);

					//��ʉE��
					pd->SetVertex(k + 2, k + 2,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f - adjust2,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 1.0f);

					//��ʉE�O
					pd->SetVertex(k + 3, k + 3,
						(float)i * 100.0f + 100.0f + adjust2, (float)j * 100.0f + 100.0f + adjust, (float)k3 * 100.0f + height + adjust2,
						0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 1.0f,
						1.0f, 0.0f);
					k += 4;
				}
			}
		}
	}
}

void Map::Mapcreate_Ground(PolygonData *pd, int pcsx, int pcsy, float height, float adjust){

	//�n�ʒ��_
	int k = 0;
	float size = 100.0f;//1�}�X�̑傫��
	for (int j = 0; j < pcsy; j++){
		for (int i = 0; i < pcsx; i++){
			float x = size * i;
			float y = size * j;
			//�n�ʍ���
			pd->SetVertex(k, k,
				x - adjust, y - adjust, height,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 0.0f);

			//�n�ʍ���
			pd->SetVertex(k + 1, k + 1,
				x - adjust, y + size + adjust, height,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 1.0f);

			//�n�ʉE��
			pd->SetVertex(k + 2, k + 2,
				x + size + adjust, y + size + adjust, height,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f);

			//�n�ʉE��
			pd->SetVertex(k + 3, k + 3,
				x + size + adjust, y - adjust, height,
				0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f);

			k += 4;
		}
	}
}

void Map::Mapcreate_Ceiling(PolygonData *pd, int pcsx, int pcsy, float height, float adjust){

	//�V�䒸�_
	int k = 0;
	float size = 100.0f;//1�}�X�̑傫��
	for (int j = 0; j < pcsy; j++){
		for (int i = 0; i < pcsx; i++){
			float x = size * i;
			float y = size * j;
			//�V�䍶��
			pd->SetVertex(k, k,
				x - adjust, y - adjust, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 0.0f);

			//�V��E��
			pd->SetVertex(k + 1, k + 1,
				x + size + adjust, y - adjust, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f);

			//�V��E��
			pd->SetVertex(k + 2, k + 2,
				x + size + adjust, y + size + adjust, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f);

			//�V�䍶��
			pd->SetVertex(k + 3, k + 3,
				x - adjust, y + size + adjust, height,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 1.0f);

			k += 4;
		}
	}
}

void Map::Mapcreate_Background(float st, float end){

	float height = end - st;
	//����
	//�E��
	poBackground.SetVertex(0, 0,
		end, st, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//����
	poBackground.SetVertex(1, 4, 1,
		st, st, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//�E��
	poBackground.SetVertex(2, 3, 2,
		end, end, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//����
	poBackground.SetVertex(5, 3,
		st, end, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//��
	//���O
	poBackground.SetVertex(6, 4,
		st, st, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//�E�O
	poBackground.SetVertex(7, 10, 5,
		end, st, height,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//����
	poBackground.SetVertex(8, 9, 6,
		st, st, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//�E��
	poBackground.SetVertex(11, 7,
		end, st, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//��
	//�E�O
	poBackground.SetVertex(12, 8,
		end, end, height,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//���O
	poBackground.SetVertex(13, 16, 9,
		st, end, height,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//�E��
	poBackground.SetVertex(14, 15, 10,
		end, end, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//����
	poBackground.SetVertex(17, 11,
		st, end, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//�E
	//��O
	poBackground.SetVertex(18, 12,
		end, st, height,
		-1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);

	//���O
	poBackground.SetVertex(19, 22, 13,
		end, end, height,
		-1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//�㉜
	poBackground.SetVertex(20, 21, 14,
		end, st, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//����
	poBackground.SetVertex(23, 15,
		end, end, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//��
	//���O
	poBackground.SetVertex(24, 16,
		st, end, height,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 5.0f);

	//��O
	poBackground.SetVertex(25, 28, 17,
		st, st, height,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//����
	poBackground.SetVertex(26, 27, 18,
		st, end, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 5.0f);

	//�㉜
	poBackground.SetVertex(29, 19,
		st, st, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		5.0f, 0.0f);
}

void Map::Mapdraw_Rain(){

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
	poRain.Draw(cax1 - 250.0f, cay1 - 250.0f, 0.0f, 0, 0, 0, 0, FALSE, TRUE, 0);
}

void Map::Mapcreate_Recover(){

	int k = 0;
	int k1 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 50)continue;//2�̃A�X�L�[�R�[�h50
				//�񕜃|�C���g����
				poRecover.SetVertex(k1, k,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//�񕜃|�C���g�E��
				poRecover.SetVertex(k1 + 1, k1 + 4, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//�񕜃|�C���g����
				poRecover.SetVertex(k1 + 2, k1 + 3, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//�񕜃|�C���g�E��
				poRecover.SetVertex(k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				k += 4;
				k1 += 6;
			}
		}
	}
}

void Map::Mapdraw_Recover(){

	int k2 = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 50)continue;//2�̃A�X�L�[�R�[�h50

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
	poRecover.Draw(0, 0, 4.0f, 0, 0, 0, 0, TRUE, FALSE, 0);
	poRecoverLine.Draw(0, 0, 0, 0, 0, 0, 0, FALSE, TRUE, 0);
}

void Map::Mapcreate_Ds(){

	//���O
	poMo.SetVertex(0, 0,
		-20.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.8f,
		0.0f, 0.0f);

	//�E�O
	poMo.SetVertex(1, 4, 1,
		20.0f, 0.0f, 100.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.8f,
		1.0f, 0.0f);

	//����
	poMo.SetVertex(2, 3, 2,
		-20.0f, 0.0f, 60.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.8f,
		0.0f, 1.0f);

	//�E��
	poMo.SetVertex(5, 3,
		20.0f, 0.0f, 60.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.8f,
		1.0f, 1.0f);
}

void Map::Mapdraw_Ds(){

	int licnt = 0;
	for (int k3 = 0; k3 < mxy.z; k3++){
		for (int j = 0; j < mxy.y; j++){
			for (int i = 0; i < mxy.x; i++){
				
				//�����L�ǈȊO�X�L�b�v
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 52 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 78 &&
					mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i] != 79)continue;

				//��(�}�b�v�[�ɔz�u����ꍇ�A�N�Z�X�ᔽ�h�~��ǉ����邱��)
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i - 1] == 48){
					//����O�X�L�b�v
					if (ViewCulling((float)i * 100.0f - 10.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f) == TRUE){
						poMo.Draw((float)i * 100.0f - 10.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE, 0);
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
				}
				//�E
				if (mxy.m[k3 * mxy.y * mxy.x + j * mxy.x + i + 1] == 48){
					//����O�X�L�b�v
					if (ViewCulling((float)i * 100.0f + 110.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f) == TRUE){
						poMo.Draw((float)i * 100.0f + 110.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE, 0);
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
				}
				//��
				if (mxy.m[k3 * mxy.y * mxy.x + (j - 1) * mxy.x + i] == 48){
					//����O�X�L�b�v
					if (ViewCulling((float)i * 100.0f + 50.0f, (float)j * 100.0f - 10.0f, (float)k3 * 100.0f) == TRUE){
						poMo.Draw((float)i * 100.0f + 50.0f, (float)j * 100.0f - 10.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE, 0);
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
				}
				//��
				if (mxy.m[k3 * mxy.y * mxy.x + (j + 1) * mxy.x + i] == 48){
					//����O�X�L�b�v
					if (ViewCulling((float)i * 100.0f + 50.0f, (float)j * 100.0f + 110.0f, (float)k3 * 100.0f) == TRUE){
						poMo.Draw((float)i * 100.0f + 50.0f, (float)j * 100.0f + 110.0f, (float)k3 * 100.0f, 0, 0, 0, src_theta, TRUE, FALSE, 0);
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
	}
	int loopcount = LIGHT_PCS_init;//���C�g�̃C���f�b�N�X(0:���_�p, 1:���X�{�X�p, 2:�o�����p, 3,4,5,6:�퓬�p�͌Œ�)
	//�e���C�g�ݒ�
	for (int i = 0; i < licnt && loopcount < LIGHT_PCS; i++){
		dx->PointLightPosSet(loopcount, light[i].x, light[i].y, light[i].z, light[i].r, light[i].g, light[i].b, light[i].a, light[i].range, light[i].brightness, light[i].attenuation, light[i].on_off);
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
				//�{�X�|�C���g���O
				poBoss.SetVertex(k1, k,
					(float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//�{�X�|�C���g�E�O
				poBoss.SetVertex(k1 + 1, k1 + 4, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f + 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//�{�X�|�C���g����
				poBoss.SetVertex(k1 + 2, k1 + 3, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//�{�X�|�C���g�E��
				poBoss.SetVertex(k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 50.0f, (float)k3 * 100.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

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
				//�G���x�[�^����
				poElevator.SetVertex(k1, k,
					(float)i * 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 0.0f);

				//�G���x�[�^�E��
				poElevator.SetVertex(k1 + 1, k1 + 4, k + 1,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 0.0f);

				//�G���x�[�^����
				poElevator.SetVertex(k1 + 2, k1 + 3, k + 2,
					(float)i * 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					0.0f, 1.0f);

				//�G���x�[�^�E��
				poElevator.SetVertex(k1 + 5, k + 3,
					(float)i * 100.0f + 100.0f, (float)j * 100.0f + 100.0f, (float)k3 * 100.0f + 2.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f);

				k += 4;
				k1 += 6;
			}
		}
	}
}

void Map::Mapcreate_EXIT(float x, float y, float z, float xsize){

	//���O
	poEXIT.SetVertex(0, 0,
		x, y, z,
		255.0f, 255.0f, 255.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f);

	//�E�O
	poEXIT.SetVertex(1, 4, 1,
		x + xsize, y, z,
		255.0f, 255.0f, 255.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f);

	//����
	poEXIT.SetVertex(2, 3, 2,
		x, y, 0.0f,
		255.0f, 255.0f, 255.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f);

	//�E��
	poEXIT.SetVertex(5, 3,
		x + xsize, y, 0.0f,
		255.0f, 255.0f, 255.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f);
}

Map::~Map(){
	dx->PointLightPosSet(2, 450.0f, 0.0f, 50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 250.0f, 300.0f, 2.0f, FALSE);//�o�����C�g����
	ARR_DELETE(wood);
	ARR_DELETE(wall1);
	free(light);
	light = NULL;
	free(mxy.m);
	mxy.m = NULL;
}

bool Map::ViewCulling(float obj_x, float obj_y, float obj_z){
	//�ΏۃI�u�W�F�N�g�܂ł̕��p�v�Z
	int theta;
	float radian;
	float cx = cax1 + cax1 - cax2;//���_
	float cy = cay1 + cay1 - cay2;//���_
	float dist_x = obj_x - cx;
	float dist_y = cy - obj_y;

	//�I�u�W�F�N�g�ƌ��݈ʒu��xy���������ꍇ(2�t���A�ȏ�̏ꍇ��������)
	if (dist_x == 0 && dist_y == 0)return FALSE;

	if (dist_x == 0){
		if (cy > obj_y)theta = 0;
		if (cy < obj_y)theta = 180;
	}
	if (dist_y == 0){
		if (cx < obj_x)theta = 90;
		if (cx > obj_x)theta = 270;
	}
	if (cx != obj_x && cy != obj_y){
		radian = atan(dist_x / dist_y);
		theta = (int)(180.0 * radian / 3.14159265359);
		if (cx < obj_x && cy < obj_y)theta += 180;
		if (cx > obj_x){
			if (cy < obj_y)theta += 180;
			if (cy > obj_y)theta += 360;
		}
	}

	//XY���������
	int viewleft = (int)(src_theta - dx->GetViewY_theta() / 1.5);
	int viewright = (int)(src_theta + dx->GetViewY_theta() / 1.5);
	viewleft = (viewleft + 360) % 360;
	viewright = viewright % 360;
	bool ret = FALSE;
	if (viewleft < viewright){
		if (viewleft < theta && viewright > theta)ret = TRUE;
	}
	if (viewleft > viewright){
		if (viewleft < theta || viewright > theta)ret = TRUE;
	}

	//XYZ���������
	if (ret == TRUE){
		float dist_z = fabsf(posz * 100.0f - obj_z);
		if (dist_z == 0)return TRUE;
		float dist_xy = sqrt(dist_x * dist_x + dist_y * dist_y);
		radian = atan(dist_z / dist_xy);
		theta = (int)(180.0 * radian / 3.14159265359);
		if (theta < dx->GetViewY_theta() / 1.5)return TRUE;
	}

	return FALSE;
}