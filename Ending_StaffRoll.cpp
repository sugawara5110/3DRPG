//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@      Ending�N���X                                    **//
//**                                     StaffRoll�֐�                                   **//
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
	dx->text("   �^�C�g��,�G���f�B���OBGM,���ʉ� ��", 150, str_y, FALSE, 0xffffffff);
	dx->text("                �������l", 150, str_y + 30, FALSE, 0xffffffff);
	dx->text("          �퓬,�}�b�vBGM ��", 150, str_y + 200, FALSE, 0xffffffff);
	dx->text("           Wingless Seraph�l", 150, str_y + 230, FALSE, 0xffffffff);
	dx->text("              �G�摜 ��", 150, str_y + 400, FALSE, 0xffffffff);
	dx->text("               HI-TIME�l", 150, str_y + 430, FALSE, 0xffffffff);
	dx->text("              Folce-zero�l", 150, str_y + 460, FALSE, 0xffffffff);
	dx->text("           �G�t�F�N�g�摜 ��", 150, str_y + 600, FALSE, 0xffffffff);
	dx->text("                �҂ۂ�l", 150, str_y + 630, FALSE, 0xffffffff);
	dx->text("            ���C���v���O���}�[", 150, str_y + 1200, FALSE, 0xffffffff);
	dx->text("                   ��", 150, str_y + 1230, FALSE, 0xffffffff);
	dx->text("       �G�O�[�N�e�B�u�E�v���f���[�T�[", 150, str_y + 1600, FALSE, 0xffffffff);
	dx->text("                   ��", 150, str_y + 1630, FALSE, 0xffffffff);
	dx->text("                THE END", 150, str_y + 2500, FALSE, 0xffffffff);
	if (str_y > -2200)str_y--;
}

Ending::~Ending(){

	MovieSoundManager::ObjDelete_ending();
}

