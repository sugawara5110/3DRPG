//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Battle�N���X                                    **//
//**                                    Menu_main�֐�                                    **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Dx9Process.h"
#include <new>     //placement new
#include <stdlib.h>
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"

CommandSelect Battle::Menu_main(Hero *hero, int i, Directionkey direction){

	char str[30];
	dx->D2primitive(1, command);
	dx->text("  �U��", 10, 10, TRUE, 0xffffffff);
	if (hero[i].GetFlameATT_LV() >= 1){
		sprintf(str, "�t���C�� LV%d", hero[i].GetFlameATT_LV());
		dx->text(str, 10, 40, TRUE, 0xffffffff);
	}
	if (hero[i].GetHealing_LV() >= 1){
		sprintf(str, "�q�[�����O LV%d", hero[i].GetHealing_LV());
		dx->text(str, 10, 70, TRUE, 0xffffffff);
	}
	if (hero[i].GetRecover_LV() >= 1){
		sprintf(str, "���J�o�� LV%d", hero[i].GetRecover_LV());
		dx->text(str, 10, 100, TRUE, 0xffffffff);
	}
	dx->text("  ������", 10, 130, TRUE, 0xffffffff);
	dx->text("���X�L�b�v", 10, 180, TRUE, 0xffffffff);
	switch (h_draw[i].M_select){
	case 0:
		dx->text("  �U��", 10, 10, TRUE, 0xff00ffff);
		break;
	case 1:
		sprintf(str, "�t���C�� LV%d", hero[i].GetFlameATT_LV());
		dx->text(str, 10, 40, TRUE, 0xff00ffff);
		h_draw[i].Magrun = FLAME;
		break;
	case 2:
		sprintf(str, "�q�[�����O LV%d", hero[i].GetHealing_LV());
		dx->text(str, 10, 70, TRUE, 0xff00ffff);
		h_draw[i].Magrun = HEAL;
		break;
	case 3:
		sprintf(str, "���J�o�� LV%d", hero[i].GetRecover_LV());
		dx->text(str, 10, 100, TRUE, 0xff00ffff);
		h_draw[i].Magrun = RECOV;
		break;
	case 4:
		dx->text("  ������", 10, 130, TRUE, 0xff00ffff);
		break;
	}

	switch (direction){
	case LEFT:

		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		SelectPermissionMove(hero);
		return OTHER;//�I�����ړ��㑦���[�v�������Ȃ��Ƃ��̂܂ܑI�����ړ���̏����ɂȂ�̂Ő؂�ւ��Ȃ�
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].M_select == 0)break;
		h_draw[i].M_select--;
		if (hero[i].GetRecover_LV() <= 0 && h_draw[i].M_select == 3)h_draw[i].M_select = 2;
		if (hero[i].GetHealing_LV() <= 0 && h_draw[i].M_select == 2)h_draw[i].M_select = 1;
		if (hero[i].GetFlameATT_LV() <= 0 && h_draw[i].M_select == 1)h_draw[i].M_select = 0;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].M_select == 4)break;
		h_draw[i].M_select++;
		if (hero[i].GetFlameATT_LV() <= 0 && h_draw[i].M_select == 1)h_draw[i].M_select = 2;
		if (hero[i].GetHealing_LV() <= 0 && h_draw[i].M_select == 2)h_draw[i].M_select = 3;
		if (hero[i].GetRecover_LV() <= 0 && h_draw[i].M_select == 3)h_draw[i].M_select = 4;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		switch (h_draw[i].M_select){
		case 0:
			return ATT;
			break;
		case 1:
			MAG_select = FLAME;
			return MAG;
			break;
		case 2:
			MAG_select = HEAL;
			return MAG;
			break;
		case 3:
			MAG_select = RECOV;
			return MAG;
			break;
		case 4:
			return ESCAPE;
			break;
		}
		break;
	}
	return NOSELECT;
}

CommandSelect Battle::Menu_AT(int i, Directionkey direction){

	//���I�𒆂̓G��LOST�����ꍇ�̑I��Ώې؂�ւ�
	if (enemy[h_draw[i].A_select].Dieflg() == TRUE){
		for (int i1 = 0; i1 < e_num; i1++)if (enemy[i1].Dieflg() == FALSE){
			h_draw[i].A_select = i1; break;
		}
	}

	Cursor_e(h_draw[i].A_select);

	switch (direction){
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[1].element == TRUE && enemy[1].Dieflg() == FALSE)h_draw[i].A_select = 1;
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[3].element == TRUE && enemy[3].Dieflg() == FALSE)h_draw[i].A_select = 3;
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[2].element == TRUE && enemy[2].Dieflg() == FALSE)h_draw[i].A_select = 2;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (enemy[0].Dieflg() == FALSE)h_draw[i].A_select = 0;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		select_obj = h_draw[i].A_select;
		return ATT;
		break;
	case CANCEL:
		return CANCEL_M;
		break;
	}
	return NOSELECT;
}

CommandSelect Battle::Menu_MAG_AT(int i, Directionkey direction){

	//���I�𒆂̓G��LOST�����ꍇ�̑I��Ώې؂�ւ�(�S�̍U���̏ꍇ�͐؂�ւ����Ȃ�)
	if (h_draw[i].MA_select != 4 && enemy[h_draw[i].MA_select].Dieflg() == TRUE){
		for (int i1 = 0; i1 < e_num; i1++)if (enemy[i1].Dieflg() == FALSE){
			h_draw[i].MA_select = i1; break;
		}
	}

	if (h_draw[i].MA_select != 4)Cursor_e(h_draw[i].MA_select);//�P�̑I��
	else{
		for (int i1 = 0; i1 < e_num; i1++){
			if (enemy[i1].Dieflg() == TRUE)continue;
			Cursor_e(i1);//�S�̑I��(h_draw[i].A_select == 4)
		}
	}

	switch (direction){
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[1].element == TRUE && enemy[1].Dieflg() == FALSE)h_draw[i].MA_select = 1;
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[3].element == TRUE && enemy[3].Dieflg() == FALSE)h_draw[i].MA_select = 3;
		break;
	case UP:
		MovieSoundManager::Select_sound(FALSE);
		if (e_pos[2].element == TRUE && enemy[2].Dieflg() == FALSE)h_draw[i].MA_select = 2;
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].MA_select == 4){
			h_draw[i].MA_select = 0;
			break;
		}
		if (h_draw[i].MA_select == 0){
			h_draw[i].MA_select = 4;
			break;
		}
		if (enemy[0].Dieflg() == TRUE && h_draw[i].MA_select != 4){
			h_draw[i].MA_select = 4;
			break;
		}
		if (enemy[0].Dieflg() == FALSE)h_draw[i].MA_select = 0;
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		select_obj = h_draw[i].MA_select;
		return MAG;
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		return CANCEL_M;
		break;
	}
	return NOSELECT;
}

CommandSelect Battle::Menu_RCV(Hero *hero, int i, Directionkey direction){

	//RECOV�I�����̑S�̑I������
	if (MAG_select == RECOV && h_draw[i].MA_select == 4)h_draw[i].MA_select = 0;

	//�v���C���񕜑ΏۃJ�[�\��
	if (h_draw[i].MA_select == 4){
		for (int i1 = 0; i1 < 4; i1++){
			Cursor_h(i1);
		}
	}
	else Cursor_h(h_draw[i].MA_select);

	switch (direction){
	case LEFT:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].MA_select == 0)break;
		h_draw[i].MA_select--;
		break;
	case RIGHT:
		MovieSoundManager::Select_sound(FALSE);
		if (h_draw[i].MA_select == 3)break;
		if (h_draw[i].MA_select == 4){
			h_draw[i].MA_select = 0;
			break;
		}
		h_draw[i].MA_select++;
		break;
	case UP:
		break;
	case DOWN:
		MovieSoundManager::Select_sound(FALSE);
		if (MAG_select == HEAL){
			if (h_draw[i].MA_select == 4)h_draw[i].MA_select = 0;
			else h_draw[i].MA_select = 4;
		}
		break;
	case ENTER:
		MovieSoundManager::Enter_sound(FALSE);
		if (MAG_select == HEAL && h_draw[i].MA_select != 4 && hero[h_draw[i].MA_select].Dieflg() == TRUE)break;
		select_obj = h_draw[i].MA_select;
		return MAG;
		break;
	case CANCEL:
		MovieSoundManager::Select_sound(FALSE);
		return CANCEL_M;
		break;
	}
	return NOSELECT;
}
