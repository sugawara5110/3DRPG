//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　      Endingクラス                                    **//
//**                                     StaffRoll関数                                   **//
//*****************************************************************************************//

#include "Ending.h"

Ending::Ending(){

	dx = Dx9Process::GetInstance();
	str_y = 600;
	MovieSoundManager::ObjCreate_ending();
	MovieSoundManager::Ending_sound(FALSE);
}

void Ending::StaffRoll(){

	MovieSoundManager::Ending_sound(TRUE);
	dx->text("   タイトル,エンディングBGM,効果音 提供", 150, str_y, FALSE, 0xffffffff);
	dx->text("                魔王魂様", 150, str_y + 30, FALSE, 0xffffffff);
	dx->text("          戦闘,マップBGM 提供", 150, str_y + 200, FALSE, 0xffffffff);
	dx->text("           Wingless Seraph様", 150, str_y + 230, FALSE, 0xffffffff);
	dx->text("              敵画像 提供", 150, str_y + 400, FALSE, 0xffffffff);
	dx->text("               HI-TIME様", 150, str_y + 430, FALSE, 0xffffffff);
	dx->text("              Folce-zero様", 150, str_y + 460, FALSE, 0xffffffff);
	dx->text("           エフェクト画像 提供", 150, str_y + 600, FALSE, 0xffffffff);
	dx->text("                ぴぽや様", 150, str_y + 630, FALSE, 0xffffffff);
	dx->text("            メインプログラマー", 150, str_y + 1200, FALSE, 0xffffffff);
	dx->text("                   俺", 150, str_y + 1230, FALSE, 0xffffffff);
	dx->text("       エグゼクティブ・プロデューサー", 150, str_y + 1600, FALSE, 0xffffffff);
	dx->text("                   俺", 150, str_y + 1630, FALSE, 0xffffffff);
	dx->text("                THE END", 150, str_y + 2500, FALSE, 0xffffffff);
	if (str_y > -2200)str_y--;
}

Ending::~Ending(){

	MovieSoundManager::ObjDelete_ending();
}

