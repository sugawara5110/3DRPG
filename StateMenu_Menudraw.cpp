//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         StateMenuクラス                                 **//
//**                                      Menudraw関数                                   **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Dx9Process.h"
#include "Hero.h"
#include "StateMenu.h"

bool StateMenu::TitleMenu(Directionkey direction){

	MovieSoundManager::Title_sound(TRUE);
	static int select = 0;

	dx->text("NEW GAME", 330, 250, FALSE, 0xffffffff);
	dx->text("CONTINUE", 330, 290, FALSE, 0xffffffff);

	switch (select){
	case 0:
		dx->text("NEW GAME", 330, 250, FALSE, 0xff00ffff);
		break;
	case 1:
		dx->text("CONTINUE", 330, 290, FALSE, 0xff00ffff);
		break;
	}

	switch (direction){
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		select = 0;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		select = 1;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		if (select == 0){
			for (int i = 0; i < 4; i++){
				/*
				savedata.p_data[i].Attack = 1;
				savedata.p_data[i].Magic = 1;
				savedata.p_data[i].Agility = 1;
				savedata.p_data[i].Vitality = 1;
				savedata.p_data[i].Exp = 10;
				savedata.p_data[i].LV = 1;
				savedata.p_data[i].ParameterPoint = 0;
				savedata.p_data[i].HP = s_MAX_HP;
				savedata.p_data[i].MP = s_MAX_MP;
				savedata.p_data[i].FlameATT_LV = 0;
				savedata.p_data[i].Fpoint = 10;
				savedata.p_data[i].Healing_LV = 0;
				savedata.p_data[i].Hpoint = 10;
				savedata.p_data[i].Recover_LV = 0;
				savedata.p_data[i].Rpoint = 10;
				*/
				
				savedata.p_data[i].Attack = 100;
				savedata.p_data[i].Magic = 100;
				savedata.p_data[i].Agility = 100;
				savedata.p_data[i].Vitality = 100;
				savedata.p_data[i].Exp = 1000;
				savedata.p_data[i].LV = 100;
				savedata.p_data[i].ParameterPoint = 0;
				savedata.p_data[i].HP = s_MAX_HP;
				savedata.p_data[i].MP = s_MAX_MP;
				savedata.p_data[i].FlameATT_LV = 100;
				savedata.p_data[i].Fpoint = 1000;
				savedata.p_data[i].Healing_LV = 100;
				savedata.p_data[i].Hpoint = 1000;
				savedata.p_data[i].Recover_LV = 100;
				savedata.p_data[i].Rpoint = 1000;
				
			}

			savedata.h_pos.cx1 = 1150.0f;
			savedata.h_pos.cx2 = 1150.0f;
			savedata.h_pos.cy1 = 2850.0f;
			savedata.h_pos.cy2 = 2800.0f;
			savedata.h_pos.px = 11;
			savedata.h_pos.py = 28;
			savedata.h_pos.pz = 0;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 0;

			/*
			savedata.h_pos.cx1 = 150.0f;
			savedata.h_pos.cx2 = 150.0f;
			savedata.h_pos.cy1 = 2750.0f;
			savedata.h_pos.cy2 = 2700.0f;
			savedata.h_pos.px = 1;
			savedata.h_pos.py = 27;
			savedata.h_pos.pz = 0;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 2;
			*/
			/*
			savedata.h_pos.cx1 = 250.0f;
			savedata.h_pos.cx2 = 250.0f;
			savedata.h_pos.cy1 = 2750.0f;
			savedata.h_pos.cy2 = 2700.0f;
			savedata.h_pos.px = 2;
			savedata.h_pos.py = 27;
			savedata.h_pos.pz = 0;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 3;
			*/
			
			//ラスダン
			savedata.h_pos.cx1 = 1150.0f;
			savedata.h_pos.cx2 = 1150.0f;
			savedata.h_pos.cy1 = 2750.0f;
			savedata.h_pos.cy2 = 2700.0f;
			savedata.h_pos.px = 11;
			savedata.h_pos.py = 27;
			savedata.h_pos.pz = 6;
			savedata.h_pos.theta = 0.0f;
			savedata.map_no = 4;
			
		}
		if (select == 1){
			FILE *fp = fopen("./dat/save/save.da1", "rb");
			FILE *fp2 = fopen("./dat/save/save.da2", "rb");
			if (fp == NULL || fp2 == NULL)return TRUE;//読み込み失敗
			int cnt1 = fread(&key1, sizeof(key1), 1, fp);
			int cnt2 = fread(&key2, sizeof(key2), 1, fp2);
			fclose(fp); //ファイルを閉じる
			fclose(fp2);
			if (cnt1 < 1 || cnt2 < 1)return TRUE;//読み込みエラー
			//セーブデータチェック
			// 論理和 fは全て1111, 0は元数値と変わらず
			//論理積 0は全て0000,fは元数値と変わらず
			for (int i = 0; i < 4; i++){
				//論理和チェック
				if (((unsigned int)key1.p_data[i].Attack | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Magic | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Agility | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Vitality | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Exp | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].LV | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].ParameterPoint | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].HP | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].MP | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].FlameATT_LV | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Fpoint | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Healing_LV | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Hpoint | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Recover_LV | Decryptionkey) != 0xffffff ||
					((unsigned int)key1.p_data[i].Rpoint | Decryptionkey) != 0xffffff ||
					//論理積チェック
					((unsigned int)key2.p_data[i].Attack         & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Magic          & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Agility        & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Vitality       & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Exp            & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].LV             & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].ParameterPoint & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].HP             & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].MP             & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].FlameATT_LV    & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Fpoint         & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Healing_LV     & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Hpoint         & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Recover_LV     & Decryptionkey) != 0x000000 ||
					((unsigned int)key2.p_data[i].Rpoint         & Decryptionkey) != 0x000000)return TRUE;
			}
			//論理和チェック
			if (((unsigned int)key1.h_pos.cx1 | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.cx2 | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.cy1 | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.cy2 | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.px | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.py | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.pz | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.h_pos.theta | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.map_no | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[0] | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[1] | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[2] | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[3] | Decryptionkey) != 0xffffff ||
				((unsigned int)key1.boss_kil[4] | Decryptionkey) != 0xffffff ||
				//論理積チェック
				((unsigned int)key2.h_pos.cx1 & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.cx2 & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.cy1 & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.cy2 & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.px  & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.py  & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.pz  & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.h_pos.theta & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.map_no    & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[0] & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[1] & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[2] & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[3] & Decryptionkey) != 0x000000 ||
				((unsigned int)key2.boss_kil[4] & Decryptionkey) != 0x000000)return TRUE;

			//セーブデータ復元
			for (int i = 0; i < 4; i++){
				savedata.p_data[i].Attack = (key2.p_data[i].Attack + ((unsigned int)key1.p_data[i].Attack         & Decryptionkey));
				savedata.p_data[i].Magic = (key2.p_data[i].Magic + ((unsigned int)key1.p_data[i].Magic          & Decryptionkey));
				savedata.p_data[i].Agility = (key2.p_data[i].Agility + ((unsigned int)key1.p_data[i].Agility        & Decryptionkey));
				savedata.p_data[i].Vitality = (key2.p_data[i].Vitality + ((unsigned int)key1.p_data[i].Vitality       & Decryptionkey));
				savedata.p_data[i].Exp = (key2.p_data[i].Exp + ((unsigned int)key1.p_data[i].Exp            & Decryptionkey));
				savedata.p_data[i].LV = (key2.p_data[i].LV + ((unsigned int)key1.p_data[i].LV             & Decryptionkey));
				savedata.p_data[i].ParameterPoint = (key2.p_data[i].ParameterPoint + ((unsigned int)key1.p_data[i].ParameterPoint & Decryptionkey));
				savedata.p_data[i].HP = (key2.p_data[i].HP + ((unsigned int)key1.p_data[i].HP             & Decryptionkey));
				savedata.p_data[i].MP = (key2.p_data[i].MP + ((unsigned int)key1.p_data[i].MP             & Decryptionkey));
				savedata.p_data[i].FlameATT_LV = (key2.p_data[i].FlameATT_LV + ((unsigned int)key1.p_data[i].FlameATT_LV    & Decryptionkey));
				savedata.p_data[i].Fpoint = (key2.p_data[i].Fpoint + ((unsigned int)key1.p_data[i].Fpoint         & Decryptionkey));
				savedata.p_data[i].Healing_LV = (key2.p_data[i].Healing_LV + ((unsigned int)key1.p_data[i].Healing_LV     & Decryptionkey));
				savedata.p_data[i].Hpoint = (key2.p_data[i].Hpoint + ((unsigned int)key1.p_data[i].Hpoint         & Decryptionkey));
				savedata.p_data[i].Recover_LV = (key2.p_data[i].Recover_LV + ((unsigned int)key1.p_data[i].Recover_LV     & Decryptionkey));
				savedata.p_data[i].Rpoint = (key2.p_data[i].Rpoint + ((unsigned int)key1.p_data[i].Rpoint         & Decryptionkey));
			}
			savedata.h_pos.cx1 = (key2.h_pos.cx1 + ((unsigned int)key1.h_pos.cx1   & Decryptionkey));
			savedata.h_pos.cx2 = (key2.h_pos.cx2 + ((unsigned int)key1.h_pos.cx2   & Decryptionkey));
			savedata.h_pos.cy1 = (key2.h_pos.cy1 + ((unsigned int)key1.h_pos.cy1   & Decryptionkey));
			savedata.h_pos.cy2 = (key2.h_pos.cy2 + ((unsigned int)key1.h_pos.cy2   & Decryptionkey));
			savedata.h_pos.px = (key2.h_pos.px + ((unsigned int)key1.h_pos.px    & Decryptionkey));
			savedata.h_pos.py = (key2.h_pos.py + ((unsigned int)key1.h_pos.py    & Decryptionkey));
			savedata.h_pos.pz = (key2.h_pos.pz + ((unsigned int)key1.h_pos.pz    & Decryptionkey));
			savedata.h_pos.theta = (key2.h_pos.theta + ((unsigned int)key1.h_pos.theta & Decryptionkey));
			savedata.map_no = (key2.map_no + ((unsigned int)key1.map_no      & Decryptionkey));
			for (int i = 0; i < 5; i++)savedata.boss_kil[i] = (key2.boss_kil[i] + ((unsigned int)key1.boss_kil[i] & Decryptionkey));
		}
		MovieSoundManager::ObjDelete_title();
		return FALSE;
		break;
	}
	return TRUE;
}

Parameter::P_Data *StateMenu::SetP_Data(int i){
	return &savedata.p_data[i];
}

Position::H_Pos *StateMenu::SetH_Pos(){
	return &savedata.h_pos;
}

int StateMenu::SetMap_No(){
	return savedata.map_no;
}

int StateMenu::Set_boss_kil(int i){
	return savedata.boss_kil[i];
}

bool StateMenu::Menudraw(Position::H_Pos *h_p, int map_no, int *boss_killed, Hero *hero, Directionkey direction){

	switch (menu_select){
	case MAIN:
		return Main_Menu(h_p, map_no, boss_killed, hero, direction);
	case ST:
		St_Menu(hero, direction);
		break;
	case RE:
		Re_Menu(hero, direction);
		break;
	case PP:
		Pp_Menu(hero, direction);
		break;
	}
	return TRUE;
}

bool StateMenu::Main_Menu(Position::H_Pos *h_p, int map_no, int *boss_killed, Hero *hero, Directionkey direction){

	Menucreate();
	char str[30];
	for (int i = 0; i < 4; i++){
		sprintf(str, "囚人№ %d", i);
		dx->text(str, 170, 10 + 110 * i, FALSE, 0xffffffff);
		sprintf(str, "HP %d/%d", hero[i].s_HP(), hero[i].s_MHP());
		dx->text(str, 170, 40 + 110 * i, FALSE, 0xffffffff);
		sprintf(str, "MP %d/%d", hero[i].s_MP(), hero[i].s_MMP());
		dx->text(str, 170, 70 + 110 * i, FALSE, 0xffffffff);
	}

	dx->text("ステータス", 10, 10, TRUE, 0xffffffff);
	dx->text("回復", 10, 40, TRUE, 0xffffffff);
	dx->text("PP振り分け", 10, 70, TRUE, 0xffffffff);
	dx->text("セーブ", 10, 100, TRUE, 0xffffffff);

	switch (main_select){
	case 0:
		dx->text("ステータス", 10, 10, TRUE, 0xff00ffff);
		break;
	case 1:
		dx->text("回復", 10, 40, TRUE, 0xff00ffff);
		break;
	case 2:
		dx->text("PP振り分け", 10, 70, TRUE, 0xff00ffff);
		break;
	case 3:
		dx->text("セーブ", 10, 100, TRUE, 0xff00ffff);
		break;
	}

	switch (direction){
	case LEFT:
		break;
	case RIGHT:
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (main_select == 0)break;
		main_select--;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (main_select == 3)break;
		main_select++;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		switch (main_select){
		case 0:
			menu_select = ST;
			break;
		case 1:
			menu_select = RE;
			break;
		case 2:
			menu_select = PP;
			break;
		case 3:
			//セーブデータ作成
			// 論理和 fは全て1111, 0は元数値と変わらず
			//論理積 0は全て0000,fは元数値と変わらず
			for (int i = 0; i < 4; i++){
				//論理和データ
				key1.p_data[i].Attack =         ((unsigned int)hero[i].s_ATT() | Encryptionkey);
				key1.p_data[i].Magic =          ((unsigned int)hero[i].s_MAG() | Encryptionkey);
				key1.p_data[i].Agility =        ((unsigned int)hero[i].s_AG() | Encryptionkey);
				key1.p_data[i].Vitality =       ((unsigned int)hero[i].s_VIT() | Encryptionkey);
				key1.p_data[i].Exp =            ((unsigned int)hero[i].s_Exp() | Encryptionkey);
				key1.p_data[i].LV =             ((unsigned int)hero[i].s_LV() | Encryptionkey);
				key1.p_data[i].ParameterPoint = ((unsigned int)hero[i].s_ParameterPoint() | Encryptionkey);
				key1.p_data[i].HP =             ((unsigned int)hero[i].s_HP() | Encryptionkey);
				key1.p_data[i].MP =             ((unsigned int)hero[i].s_MP() | Encryptionkey);
				key1.p_data[i].FlameATT_LV =    ((unsigned int)hero[i].GetFlameATT_LV() | Encryptionkey);
				key1.p_data[i].Fpoint =         ((unsigned int)hero[i].s_Fp() | Encryptionkey);
				key1.p_data[i].Healing_LV =     ((unsigned int)hero[i].GetHealing_LV() | Encryptionkey);
				key1.p_data[i].Hpoint =         ((unsigned int)hero[i].s_Hp() | Encryptionkey);
				key1.p_data[i].Recover_LV =     ((unsigned int)hero[i].GetRecover_LV() | Encryptionkey);
				key1.p_data[i].Rpoint =         ((unsigned int)hero[i].s_Rp() | Encryptionkey);
				//論理積データ
				key2.p_data[i].Attack =         ((unsigned int)hero[i].s_ATT() & Encryptionkey);
				key2.p_data[i].Magic =          ((unsigned int)hero[i].s_MAG() & Encryptionkey);
				key2.p_data[i].Agility =        ((unsigned int)hero[i].s_AG() & Encryptionkey);
				key2.p_data[i].Vitality =       ((unsigned int)hero[i].s_VIT() & Encryptionkey);
				key2.p_data[i].Exp =            ((unsigned int)hero[i].s_Exp() & Encryptionkey);
				key2.p_data[i].LV =             ((unsigned int)hero[i].s_LV() & Encryptionkey);
				key2.p_data[i].ParameterPoint = ((unsigned int)hero[i].s_ParameterPoint() & Encryptionkey);
				key2.p_data[i].HP =             ((unsigned int)hero[i].s_HP() & Encryptionkey);
				key2.p_data[i].MP =             ((unsigned int)hero[i].s_MP() & Encryptionkey);
				key2.p_data[i].FlameATT_LV =    ((unsigned int)hero[i].GetFlameATT_LV() & Encryptionkey);
				key2.p_data[i].Fpoint =         ((unsigned int)hero[i].s_Fp() & Encryptionkey);
				key2.p_data[i].Healing_LV =     ((unsigned int)hero[i].GetHealing_LV() & Encryptionkey);
				key2.p_data[i].Hpoint =         ((unsigned int)hero[i].s_Hp() & Encryptionkey);
				key2.p_data[i].Recover_LV =     ((unsigned int)hero[i].GetRecover_LV() & Encryptionkey);
				key2.p_data[i].Rpoint =         ((unsigned int)hero[i].s_Rp() & Encryptionkey);
			}
			//論理和データ
			key1.h_pos.cx1 = (float)((unsigned int)(h_p->px * 100 + 50) | Encryptionkey);
			key1.h_pos.cx2 = (float)((unsigned int)(h_p->px * 100 + 50) | Encryptionkey);
			key1.h_pos.cy1 = (float)((unsigned int)(h_p->py * 100 + 50) | Encryptionkey);
			key1.h_pos.cy2 = (float)((unsigned int)(h_p->py * 100) | Encryptionkey);
			key1.h_pos.px = ((unsigned int)h_p->px | Encryptionkey);
			key1.h_pos.py = ((unsigned int)h_p->py | Encryptionkey);
			key1.h_pos.pz = ((unsigned int)h_p->pz | Encryptionkey);
			key1.h_pos.theta = (float)((unsigned int)0.0f | Encryptionkey);
			key1.map_no = ((unsigned int)map_no | Encryptionkey);
			for (int i = 0; i < 5; i++)key1.boss_kil[i] = ((unsigned int)boss_killed[i] | Encryptionkey);
			//論理積データ
			key2.h_pos.cx1 = (float)((unsigned int)(h_p->px * 100 + 50) & Encryptionkey);
			key2.h_pos.cx2 = (float)((unsigned int)(h_p->px * 100 + 50) & Encryptionkey);
			key2.h_pos.cy1 = (float)((unsigned int)(h_p->py * 100 + 50) & Encryptionkey);
			key2.h_pos.cy2 = (float)((unsigned int)(h_p->py * 100) & Encryptionkey);
			key2.h_pos.px = ((unsigned int)h_p->px & Encryptionkey);
			key2.h_pos.py = ((unsigned int)h_p->py & Encryptionkey);
			key2.h_pos.pz = ((unsigned int)h_p->pz & Encryptionkey);
			key2.h_pos.theta = (float)((unsigned int)0.0f & Encryptionkey);
			key2.map_no = ((unsigned int)map_no & Encryptionkey);
			for (int i = 0; i < 5; i++)key2.boss_kil[i] = ((unsigned int)boss_killed[i] & Encryptionkey);

			FILE *fp = fopen("./dat/save/save.da1", "wb");
			FILE *fp2 = fopen("./dat/save/save.da2", "wb");
			fwrite(&key1, sizeof(key1), 1, fp);
			fwrite(&key2, sizeof(key2), 1, fp2);
			fclose(fp); //ファイルを閉じる
			fclose(fp2);
			break;
		}
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		return FALSE;
	}
	return TRUE;
}

void StateMenu::St_Menu(Hero *hero, Directionkey direction){

	St_create();
	char str[30];
	sprintf(str, "囚人№ %d", st_select);
	dx->text(str, 120, 10, FALSE, 0xffffffff);
	sprintf(str, "LV            %d   %d/%d", hero[st_select].s_LV(),
		hero[st_select].s_LV() * 10 - hero[st_select].s_Exp(),
		hero[st_select].s_LV() * 10);
	dx->text(str, 120, 35, TRUE, 0xffffffff);
	sprintf(str, "力            %d", hero[st_select].s_ATT());
	dx->text(str, 120, 55, TRUE, 0xffffffff);
	sprintf(str, "魔力          %d", hero[st_select].s_MAG());
	dx->text(str, 120, 75, TRUE, 0xffffffff);
	sprintf(str, "素早さ        %d", hero[st_select].s_AG());
	dx->text(str, 120, 95, TRUE, 0xffffffff);
	sprintf(str, "体力          %d", hero[st_select].s_VIT());
	dx->text(str, 120, 115, TRUE, 0xffffffff);

	if (hero[st_select].GetFlameATT_LV() != 0){
		sprintf(str, "フレイムLV    %d   %d/%d", hero[st_select].GetFlameATT_LV(),
			hero[st_select].GetFlameATT_LV() * 10 - hero[st_select].s_Fp(),
			hero[st_select].GetFlameATT_LV() * 10);
		dx->text(str, 120, 135, TRUE, 0xffffffff);
	}
	if (hero[st_select].GetHealing_LV() != 0){
		sprintf(str, "ヒーリングLV  %d   %d/%d", hero[st_select].GetHealing_LV(),
			hero[st_select].GetHealing_LV() * 10 - hero[st_select].s_Hp(),
			hero[st_select].GetHealing_LV() * 10);
		dx->text(str, 120, 155, TRUE, 0xffffffff);
	}
	if (hero[st_select].GetRecover_LV() != 0){
		sprintf(str, "リカバリLV    %d   %d/%d", hero[st_select].GetRecover_LV(),
			hero[st_select].GetRecover_LV() * 10 - hero[st_select].s_Rp(),
			hero[st_select].GetRecover_LV() * 10);
		dx->text(str, 120, 175, TRUE, 0xffffffff);
	}

	switch (direction){
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (st_select == 0)break;
		st_select--;
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (st_select == 3)break;
		st_select++;
		break;
	case UP:
		break;
	case DOWN:
		break;
	case ENTER:
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		menu_select = MAIN;
		break;
	}
}

void StateMenu::Re_Menu(Hero *hero, Directionkey direction){

	St_create();
	Re_create();
	int cnt;
	char str[30];
	for (int i = 0; i < 4; i++){
		sprintf(str, "囚人№ %d", i);
		dx->text(str, 10, 10 + 20 * i, TRUE, 0xffffffff);
		dx->text(str, 170, 10 + 110 * i, FALSE, 0xffffffff);
		sprintf(str, "HP %d/%d", hero[i].s_HP(), hero[i].s_MHP());
		dx->text(str, 170, 40 + 110 * i, FALSE, 0xffffffff);
		sprintf(str, "MP %d/%d", hero[i].s_MP(), hero[i].s_MMP());
		dx->text(str, 170, 70 + 110 * i, FALSE, 0xffffffff);
	}

	if (re_select2 == 4){
		for (int i = 0; i < 4; i++){
			sprintf(str, "HP %d/%d", hero[i].s_HP(), hero[i].s_MHP());
			dx->text(str, 170, 40 + 110 * i, FALSE, 0xff00ffff);
		}
	}
	if (re_select2 != -1 && re_select2 != 4){
		sprintf(str, "HP %d/%d", hero[re_select2].s_HP(), hero[re_select2].s_MHP());
		dx->text(str, 170, 40 + 110 * re_select2, FALSE, 0xff00ffff);
	}

	sprintf(str, "囚人№ %d", re_select);
	dx->text(str, 10, 10 + 20 * re_select, TRUE, 0xff00ffff);

	if (re_select1 != -1){
		Re_create1();
		if (hero[re_select].GetHealing_LV() >= 1){
			sprintf(str, "ヒーリング LV%d", hero[re_select].GetHealing_LV());
			dx->text(str, 10, 160, TRUE, 0xffffffff);
		}
		if (hero[re_select].GetRecover_LV() >= 1){
			sprintf(str, "リカバリ LV%d", hero[re_select].GetRecover_LV());
			dx->text(str, 10, 190, TRUE, 0xffffffff);
		}
		if (re_select1 == 0){
			if (hero[re_select].GetRecover_LV() >= 1)re_select1 = 2;
			if (hero[re_select].GetHealing_LV() >= 1)re_select1 = 1;
		}
		switch (re_select1){
		case 1:
			if (hero[re_select].GetHealing_LV() >= 1){
				sprintf(str, "ヒーリング LV%d", hero[re_select].GetHealing_LV());
				dx->text(str, 10, 160, TRUE, 0xff00ffff);
			}
			break;
		case 2:
			if (hero[re_select].GetRecover_LV() >= 1){
				sprintf(str, "リカバリ LV%d", hero[re_select].GetRecover_LV());
				dx->text(str, 10, 190, TRUE, 0xff00ffff);
			}
			break;
		}
	}

	switch (direction){
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select1 == 1 && re_select2 != -1){
			if (re_select2 == 4){ re_select2 = 0; break; }
			if (re_select2 != 4){ re_select2 = 4; break; }
		}
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select1 == 1 && re_select2 != -1){
			if (re_select2 == 4){ re_select2 = 0; break; }
			if (re_select2 != 4){ re_select2 = 4; break; }
		}
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select2 != -1){
			if (re_select2 == 4){
				re_select2 = 3; break;
			}
			if (re_select2 == 0)break;
			re_select2--;
			break;
		}
		if (re_select1 != -1){
			if (hero[re_select].GetHealing_LV() >= 1){
				re_select1 = 1;
				break;
			}
			if (hero[re_select].GetRecover_LV() >= 1){
				re_select1 = 2;
				break;
			}
			break;
		}
		if (re_select == 0)break;
		re_select--;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select2 != -1){
			if (re_select2 == 4){
				re_select2 = 0; break;
			}
			if (re_select2 == 3)break;
			re_select2++;
			break;
		}
		if (re_select1 != -1){
			if (hero[re_select].GetRecover_LV() >= 1){
				re_select1 = 2;
				break;
			}
			if (hero[re_select].GetHealing_LV() >= 1){
				re_select1 = 1;
				break;
			}
			break;
		}
		if (re_select == 3)break;
		re_select++;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		if (hero[re_select].Dieflg() == TRUE)break;//戦闘不能中選択不可
		if (re_select1 == -1){
			re_select1 = 0;
			break;
		}
		if (re_select1 >= 1 && re_select2 == -1){
			re_select2 = 0;
			break;
		}

		//回復処理
		int rcv;
		cnt = 0;
		MagicSelect mag;
		if (re_select1 == 1)mag = HEAL;
		if (re_select1 == 2)mag = RECOV;
		if (hero[re_select].DownMP(mag) == FALSE)break;
		if (re_select2 == 4){
			for (int i = 0; i < 4; i++){
				if (hero[i].Dieflg() == FALSE)cnt++;
			}
			for (int i = 0; i < 4; i++){
				rcv = hero[re_select].GetMagic(HEAL, cnt);
				if (hero[i].Dieflg() == TRUE)continue;
				hero[i].UpHP(rcv);
			}
		}
		else{
			if (re_select1 == 1){
				rcv = hero[re_select].GetMagic(HEAL, 1);
				if (hero[re_select2].Dieflg() == TRUE)break;
				hero[re_select2].UpHP(rcv);
			}
			if (re_select1 == 2){
				rcv = hero[re_select].GetMagic(RECOV, 1);
				if (hero[re_select2].Dieflg() == TRUE)hero[re_select2].Dieflg(FALSE);
				hero[re_select2].UpHP(rcv);
			}
		}
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		if (re_select2 != -1){
			re_select2 = -1;
			break;
		}
		if (re_select1 != -1){
			re_select1 = -1;
			break;
		}
		menu_select = MAIN;
		break;
	}
}

void StateMenu::Pp_Menu(Hero *hero, Directionkey direction){

	St_create();
	Re_create();

	char str[30];
	for (int i = 0; i < 4; i++){
		sprintf(str, "囚人№ %d", i);
		dx->text(str, 10, 10 + 20 * i, TRUE, 0xffffffff);
	}
	sprintf(str, "囚人№ %d", pp_select);
	dx->text(str, 10, 10 + 20 * pp_select, TRUE, 0xff00ffff);

	if (pp_select1 != -1){
		sprintf(str, "囚人№ %d", pp_select);
		dx->text(str, 120, 10, FALSE, 0xffffffff);
		sprintf(str, "LV            %d", hero[pp_select].s_LV());
		dx->text(str, 120, 35, TRUE, 0xffffffff);
		sprintf(str, "力            %d", hero[pp_select].s_ATT());
		dx->text(str, 120, 55, TRUE, 0xffffffff);
		sprintf(str, "魔力          %d", hero[pp_select].s_MAG());
		dx->text(str, 120, 75, TRUE, 0xffffffff);
		sprintf(str, "素早さ        %d", hero[pp_select].s_AG());
		dx->text(str, 120, 95, TRUE, 0xffffffff);
		sprintf(str, "体力          %d", hero[pp_select].s_VIT());
		dx->text(str, 120, 115, TRUE, 0xffffffff);
		sprintf(str, "PP            %d", hero[pp_select].s_ParameterPoint());
		dx->text(str, 120, 145, TRUE, 0xffffffff);

		switch (pp_select1){
		case 0:
			sprintf(str, "力            %d", hero[pp_select].s_ATT());
			dx->text(str, 120, 55, TRUE, 0xff00ffff);
			break;
		case 1:
			sprintf(str, "魔力          %d", hero[pp_select].s_MAG());
			dx->text(str, 120, 75, TRUE, 0xff00ffff);
			break;
		case 2:
			sprintf(str, "素早さ        %d", hero[pp_select].s_AG());
			dx->text(str, 120, 95, TRUE, 0xff00ffff);
			break;
		case 3:
			sprintf(str, "体力          %d", hero[pp_select].s_VIT());
			dx->text(str, 120, 115, TRUE, 0xff00ffff);
			break;
		}
	}

	switch (direction){
	case LEFT:
		break;
	case RIGHT:
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (pp_select1 != -1){
			if (pp_select1 == 0)break;
			pp_select1--;
			break;
		}
		if (pp_select == 0)break;
		pp_select--;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (pp_select1 != -1){
			if (pp_select1 == 3)break;
			pp_select1++;
			break;
		}
		if (pp_select == 3)break;
		pp_select++;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		if (pp_select1 != -1){
			if (hero[pp_select].s_ParameterPoint(-1) == FALSE)break;//PP足りない処理
			switch (pp_select1){
			case 0:
				hero[pp_select].s_ATT(TRUE);
				break;
			case 1:
				hero[pp_select].s_MAG(TRUE);
				break;
			case 2:
				hero[pp_select].s_AG(TRUE);
				break;
			case 3:
				hero[pp_select].s_VIT(TRUE);
				break;
			}
			break;
		}
		pp_select1 = 0;
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		if (pp_select1 != -1){
			pp_select1 = -1;
			break;
		}
		menu_select = MAIN;
		break;
	}
}