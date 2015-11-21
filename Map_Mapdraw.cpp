//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         Mapクラス                                       **//
//**                                    Mapdraw関数                                      **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "Map.h"

Encount Map::Mapdraw(MapState *mapstate, Directionkey direction, Encount encount, bool menu, bool title){

	Debug();//デバック用

	//ボス撃破後のマップ,テキスト処理
	if (mxy.m[POS_CE] == 51 && boss_killed[map_no] == 1){
		if (map_no == 3){
			map_text_f = 200;
			sprintf(m_tx, "炎壁の封印が解けた!!");
		}
		mxy.m[POS_CE] = 48;//ボス撃破後ボス出現ポイント消滅
		boss_count = 0;   //ポイント全て表示されないようにする。マップ中に複数使用の場合変更必要
	}

	if (title == FALSE && encount == NOENCOUNT && menu == FALSE)encount = Move(mapstate, direction);
	if (title == FALSE && encount == NOENCOUNT)MovieSoundManager::Dungeon_sound(TRUE); else MovieSoundManager::Dungeon_soundoff();
	if (title == FALSE && map_no == 1)MovieSoundManager::Rain_sound(TRUE);

	bool light_f = FALSE;
	dx->Cameraset(cax1, cax2, cay1, cay2, (float)posz * 100.0f + 35.0f + elevator_step);
	if (map_no != 1 && map_no != 3){
		dx->LightPosSet(0, cax1, cay1, (float)posz * 100.0f + 70.0f + elevator_step, 300.0f, 0.006f); light_f = TRUE;
	}
	if (map_no == 4)dx->LightPosSet(1, 1500.0f, 1000.0f, 650.0f, 600.0f, 0.003f);
	if (map_no == 1)dx->SetFog(TRUE, StartPos, EndPos, r, g, b);
	if (squarecount >= 1)Mapcreate_Wall1();
	if (blockcount >= 1)dx->D3primitive(SQUARE, &poWall, blockcount * 6, 0, 0, 0, 0, TRUE, FALSE, light_f);

	dx->D3primitive(SQUARE, &poGround, 900, 0, 0, 0, 0, TRUE, FALSE, light_f);
	dx->D3primitive(SQUARE, &poCeiling, 900, 0, 0, 0, 0, TRUE, FALSE, light_f);
	if (map_no == 1){
		dx->D3primitive(SQUARE, &poBackground, 4, 0, 0, 0, 0, TRUE, FALSE, FALSE);
		Mapcreate_Rain();
	}

	dx->SetFog(FALSE, StartPos, EndPos, r, g, b);
	if (map_no != 4 && map_no != 3)dx->D3primitive(SQUARE, &poEXIT, 1, 0, 0, 0, 0, TRUE, FALSE, FALSE);
	if (map_no != 0)dx->D3primitive(SQUARE, &poENTER, 1, 0, 0, 0, 0, TRUE, FALSE, FALSE);
	if (r_point_count >= 1)Mapcreate_Recover();
	if (boss_count >= 1)dx->D3primitive(SQUARE, &poBoss, boss_count, 0, 0, 0, 0, TRUE, FALSE, FALSE);
	if (Elevator_count >= 1)dx->D3primitive(SQUARE, &poElevator, Elevator_count, 0, 0, 0, 0, TRUE, FALSE, FALSE);
	if (mo_count >= 1){
		dx->SetTextureMPixel(&poMo, MovieSoundManager::Torch_GetFrame(512, 512), 0xff, 0xff, 0xff, 200);
		Mapcreate_Ds();
	}
	if (f_wall_count >= 1){
		dx->SetTextureMPixel(&poF_Wall, MovieSoundManager::FireWall_GetFrame(512, 512), 0xff, 0xff, 0xff, 200);
		dx->D3primitive(SQUARE, &poF_Wall, f_wall_count * 4, 0, 0, 0, 0, TRUE, FALSE, FALSE);
	}

	MapText(m_tx);

	return encount;
}

Position::E_Pos *Map::Getposition(int p){

	if (src_theta == 0 || src_theta == 360){
		e_pos[0].theta = 0.0f;
		e_pos[0].element = TRUE;
		e_pos[0].x = cax1 + (int)(sin(0.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].y = cay1 - (int)(cos(0.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].z = posz * 100.0f;
		if (p > 0 && posy - 2 >= 0 && posx - 1 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy - 2) * mxy.x + (posx - 1)] == 48){
			e_pos[1].theta = 338.0f;
			e_pos[1].element = TRUE;
			e_pos[1].x = cax1 + (int)(sin(338.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].y = cay1 - (int)(cos(338.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].z = posz * 100.0f;
		}
		else e_pos[1].element = FALSE;
		if (p > 1 && posy - 2 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy - 2) * mxy.x + posx] == 48){
			e_pos[2].theta = 0.0f;
			e_pos[2].element = TRUE;
			e_pos[2].x = cax1 + (int)(sin(0.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].y = cay1 - (int)(cos(0.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].z = posz * 100.0f;
		}
		else e_pos[2].element = FALSE;
		if (p > 2 && posy - 2 >= 0 && posx + 1 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy - 2) * mxy.x + (posx + 1)] == 48){
			e_pos[3].theta = 22.0f;
			e_pos[3].element = TRUE;
			e_pos[3].x = cax1 + (int)(sin(22.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].y = cay1 - (int)(cos(22.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].z = posz * 100.0f;
		}
		else e_pos[3].element = FALSE;
	}

	if (src_theta == 90){
		e_pos[0].theta = 90.0f;
		e_pos[0].element = TRUE;
		e_pos[0].x = cax1 + (int)(sin(90.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].y = cay1 - (int)(cos(90.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].z = posz * 100.0f;
		if (p > 0 && posy - 1 >= 0 && posx + 2 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy - 1) * mxy.x + (posx + 2)] == 48){
			e_pos[1].theta = 68.0f;
			e_pos[1].element = TRUE;
			e_pos[1].x = cax1 + (int)(sin(68.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].y = cay1 - (int)(cos(68.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].z = posz * 100.0f;
		}
		else e_pos[1].element = FALSE;
		if (p > 1 && posx + 2 <= 29 && mxy.m[posz * mxy.y * mxy.x + posy * mxy.x + (posx + 2)] == 48){
			e_pos[2].theta = 90.0f;
			e_pos[2].element = TRUE;
			e_pos[2].x = cax1 + (int)(sin(90.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].y = cay1 - (int)(cos(90.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].z = posz * 100.0f;
		}
		else e_pos[2].element = FALSE;
		if (p > 2 && posy + 1 <= 29 && posx + 2 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy + 1) * mxy.x + (posx + 2)] == 48){
			e_pos[3].theta = 112.0f;
			e_pos[3].element = TRUE;
			e_pos[3].x = cax1 + (int)(sin(112.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].y = cay1 - (int)(cos(112.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].z = posz * 100.0f;
		}
		else e_pos[3].element = FALSE;
	}

	if (src_theta == 180){
		e_pos[0].theta = 180.0f;
		e_pos[0].element = TRUE;
		e_pos[0].x = cax1 + (int)(sin(180.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].y = cay1 - (int)(cos(180.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].z = posz * 100.0f;
		if (p > 0 && posy + 2 <= 29 && posx + 1 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy + 2) * mxy.x + (posx + 1)] == 48){
			e_pos[1].theta = 158.0f;
			e_pos[1].element = TRUE;
			e_pos[1].x = cax1 + (int)(sin(158.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].y = cay1 - (int)(cos(158.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].z = posz * 100.0f;
		}
		else e_pos[1].element = FALSE;
		if (p > 1 && posy + 2 <= 29 && mxy.m[posz * mxy.y * mxy.x + (posy + 2) * mxy.x + posx] == 48){
			e_pos[2].theta = 180.0f;
			e_pos[2].element = TRUE;
			e_pos[2].x = cax1 + (int)(sin(180.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].y = cay1 - (int)(cos(180.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].z = posz * 100.0f;
		}
		else e_pos[2].element = FALSE;
		if (p > 2 && posy + 2 <= 29 && posx - 1 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy + 2) * mxy.x + (posx - 1)] == 48){
			e_pos[3].theta = 202.0f;
			e_pos[3].element = TRUE;
			e_pos[3].x = cax1 + (int)(sin(202.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].y = cay1 - (int)(cos(202.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].z = posz * 100.0f;
		}
		else e_pos[3].element = FALSE;
	}

	if (src_theta == 270){
		e_pos[0].theta = 270.0f;
		e_pos[0].element = TRUE;
		e_pos[0].x = cax1 + (int)(sin(270.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].y = cay1 - (int)(cos(270.0f * 3.14f / 180.0f) * 100.0f);
		e_pos[0].z = posz * 100.0f;
		if (p > 0 && posy + 1 <= 29 && posx - 2 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy + 1) * mxy.x + (posx - 2)] == 48){
			e_pos[1].theta = 248.0f;
			e_pos[1].element = TRUE;
			e_pos[1].x = cax1 + (int)(sin(248.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].y = cay1 - (int)(cos(248.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[1].z = posz * 100.0f;
		}
		else e_pos[1].element = FALSE;
		if (p > 1 && posx - 2 >= 0 && mxy.m[posz * mxy.y * mxy.x + posy * mxy.x + (posx - 2)] == 48){
			e_pos[2].theta = 270.0f;
			e_pos[2].element = TRUE;
			e_pos[2].x = cax1 + (int)(sin(270.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].y = cay1 - (int)(cos(270.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[2].z = posz * 100.0f;
		}
		else e_pos[2].element = FALSE;
		if (p > 2 && posy - 1 >= 0 && posx - 2 >= 0 && mxy.m[posz * mxy.y * mxy.x + (posy - 1) * mxy.x + (posx - 2)] == 48){
			e_pos[3].theta = 292.0f;
			e_pos[3].element = TRUE;
			e_pos[3].x = cax1 + (int)(sin(292.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].y = cay1 - (int)(cos(292.0f * 3.14f / 180.0f) * 200.0f);
			e_pos[3].z = posz * 100.0f;
		}
		else e_pos[3].element = FALSE;
	}
	return e_pos;
}

Position::H_Pos *Map::Getposition(){

	h_pos.cx1 = cax1;
	h_pos.cx2 = cax2;
	h_pos.cy1 = cay1;
	h_pos.cy2 = cay2;
	h_pos.px = posx;
	h_pos.py = posy;
	h_pos.pz = posz;
	h_pos.theta = src_theta;

	return &h_pos;
}

void Map::Debug(){//デバック用

	char str[30];
	sprintf(str, "src_theta = %f", src_theta);
	dx->text(str, 100, 10, TRUE, 0xffffffff);
	sprintf(str, "posx = %d", posx);
	dx->text(str, 100, 25, TRUE, 0xffffffff);
	sprintf(str, "posy = %d", posy);
	dx->text(str, 100, 40, TRUE, 0xffffffff);
	sprintf(str, "posz = %d", posz);
	dx->text(str, 100, 55, TRUE, 0xffffffff);
	sprintf(str, "cax1 = %f", cax1);
	dx->text(str, 100, 70, TRUE, 0xffffffff);
	sprintf(str, "cax2 = %f", cax2);
	dx->text(str, 100, 85, TRUE, 0xffffffff);
	sprintf(str, "cay1 = %f", cay1);
	dx->text(str, 100, 100, TRUE, 0xffffffff);
	sprintf(str, "cay2 = %f", cay2);
	dx->text(str, 100, 115, TRUE, 0xffffffff);
	sprintf(str, "bosskil[0] = %d", boss_killed[0]);
	dx->text(str, 100, 130, TRUE, 0xffffffff);
	sprintf(str, "bosskil[1] = %d", boss_killed[1]);
	dx->text(str, 100, 145, TRUE, 0xffffffff);
	sprintf(str, "bosskil[2] = %d", boss_killed[2]);
	dx->text(str, 100, 160, TRUE, 0xffffffff);
	sprintf(str, "bosskil[3] = %d", boss_killed[3]);
	dx->text(str, 100, 175, TRUE, 0xffffffff);
	sprintf(str, "bosskil[4] = %d", boss_killed[4]);
	dx->text(str, 100, 190, TRUE, 0xffffffff);
	dx->text("開発中画面です", 300, 10, TRUE, 0xffffffff);
	dx->text("決定:Ctrl  キャンセル:Delete", 300, 25, TRUE, 0xffffffff);
}