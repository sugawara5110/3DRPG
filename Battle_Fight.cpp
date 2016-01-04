//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Battle�N���X                                    **//
//**                                      Fight�֐�                                      **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Dx11Process.h"
#include <new>     //placement new
#include <stdlib.h>
#include <time.h>
#include "Enemy.h"
#include "EnemySide.h"
#include "EnemyBoss.h"
#include "Battle.h"
#include "Hero.h"

Result Battle::Fight(Hero *hero, Directionkey direction, Result result){

	float H_DMdrawYMAX = 470;
	float E_DMdrawYMAX = 300;
	int draw_count;
	Act_fin_flg act;

	//�G�퓬�s�\�A�N�V�����t���O
	//LOST�͔��������炸���ƂȂ̂Œ���
	for (int i = 0; i < e_num; i++)if (enemy[i].Dieflg() == TRUE)e_draw[i].action = LOST;

	//�G�\��
	for (int i = 0; i < e_num; i++){
		if (e_draw[i].LOST_fin == FALSE){
			act = enemy[i].Enemydraw(this, &E_select_obj, e_draw[i].action, e_draw[i].Magrun);
			switch (act){
			case AT_FIN:
				draw_count = 0;
				for (int i1 = 0; i1 < 4; i1++){
					if (h_draw[i1].DMdata >= 0){
						h_draw[i1].action = DAMAGE;//�_���[�W�󂯂��̂ł��̌�_���[�W����
						h_draw[i1].DMdrawY = 460;//�p�����[�^�[�ԈႦ��Ɛ����\������Ȃ��̂Œ���
						draw_count++;
					}
				}
				for (int i1 = 0; i1 < e_num; i1++){
					if (e_draw[i1].RCVdata >= 0){
						e_draw[i1].action = enemy[i1].Normal_act_get();//�񕜎󂯂��̂ł��̌�񕜏���
						e_draw[i1].RCVdrawY = 290;//�p�����[�^�[�ԈႦ��Ɛ����\������Ȃ��̂Œ���
						draw_count++;
					}
				}
				if (draw_count == 0)time_stop_flg = FALSE;
				break;
			case LOST_FIN:
				e_draw[i].LOST_fin = TRUE;
				break;
			}
		}
		e_draw[i].action = enemy[i].Normal_act_get();//��action�͈�x������OK�ȍ\���Ȃ̂œ��ꂽ�瑦����������B������Ȃ��Ƃ�����action�����ςȂ��B
	}

	//�v���C���[�X�e�[�^�X�\��
	for (int i = 0; i < 4; i++){
		act = hero[i].Statedraw(this, &select_obj, h_pos, e_pos, h_draw[i].AGmeter / METER_MAX, h_draw[i].command_run, h_draw[i].action, h_draw[i].Magrun);
		if (act == AT_FIN){
			draw_count = 0;
			for (int i1 = 0; i1 < e_num; i1++){
				if (e_draw[i1].DMdata >= 0){
					e_draw[i1].action = DAMAGE;//�_���[�W�󂯂��̂ł��̌�_���[�W����
					e_draw[i1].DMdrawY = 290;
					draw_count++;
				}
			}
			for (int i1 = 0; i1 < 4; i1++){
				if (h_draw[i1].RCVdata >= 0){
					h_draw[i1].action = NORMAL;//�񕜎󂯂��̂ł��̌�񕜏���
					h_draw[i1].RCVdrawY = 460;//�p�����[�^�[�ԈႦ��Ɛ����\������Ȃ��̂Œ���
					draw_count++;
				}
			}
			if (draw_count == 0)time_stop_flg = FALSE;
		}
		h_draw[i].action = NORMAL;
	}

	//GAMEOVER�����[�v�X�L�b�v
	if (result == DIE){
		MovieSoundManager::Die_sound(TRUE);
		return DIE;
	}

	MovieSoundManager::Enemy_sound(TRUE);

	//�G�U���p�^�[������
	E_com_select = E_AT_select(hero);

	//�G�U��
	if (time_stop_flg == FALSE && E_com_select != NOSELECT && E_com_select != OTHER){
		//���l�ۊǏ�����
		for (int i = 0; i < 4; i++){
			h_draw[i].DMdata = -1;
		}
		for (int i = 0; i < e_num; i++){
			e_draw[i].RCVdata = -1;
		}

		if (E_com_select == ATT){
			for (int i = 0; i < e_num; i++){
				if (enemy[i].Dieflg() == FALSE && e_draw[i].command_run == TRUE){
					ATprocess<Hero, Enemy>(&hero[E_select_obj], &enemy[i], &h_draw[E_select_obj], &e_draw[i]);
					break;
				}
			}
		}
		if (E_com_select == MAG){
			for (int i = 0; i < e_num; i++){
				if (enemy[i].Dieflg() == FALSE && e_draw[i].command_run == TRUE){
					MAGprocess<Hero, Enemy>(hero, enemy, &enemy[i], &e_draw[i], e_draw, h_draw, &E_select_obj, &E_MAG_select, E_ATT);
					break;
				}
			}
		}
		E_com_select = NOSELECT;
	}
	//�G��
	for (int i = 0; i < e_num; i++)RCVdraw<Enemy>(&enemy[i], &e_draw[i], E_DMdrawYMAX);
	//�v���C���[�_���[�W
	for (int i = 0; i < 4; i++)DMdraw<Hero>(&hero[i], &h_draw[i], H_DMdrawYMAX);

	//�v���C���[�S��
	if (hero[0].Dieflg() == TRUE && hero[1].Dieflg() == TRUE && hero[2].Dieflg() == TRUE && hero[3].Dieflg() == TRUE){
		MovieSoundManager::Enemy_soundoff();
		MovieSoundManager::Die_sound(FALSE);
		SelectPermissionMove(hero); return DIE;
	}

	//�퓬�s�\���R�}���h�I�����j��,�A�N�V����������
	for (int i = 0; i < 4; i++){
		if (hero[i].Dieflg() == TRUE){
			if (h_draw[i].command_run == TRUE){
				SelectPermissionMove(hero);
				com_select = NOSELECT;
			}
			hero[i].Act_f_init();
		}
	}

	//�R�}���h����
	if (com_select == NOSELECT || com_select == OTHER){
		for (int i = 0; i < 4; i++){
			com_select = H_AT_select(hero, i, direction);
			if (com_select != NOSELECT)break;
		}
	}

	//�v���C���[�U��
	if (time_stop_flg == FALSE && com_select != NOSELECT && com_select != OTHER){
		//���l�ۊǏ�����
		for (int i = 0; i < e_num; i++){
			e_draw[i].DMdata = -1;
		}
		for (int i = 0; i < 4; i++){
			h_draw[i].RCVdata = -1;
		}

		if (com_select == ATT){
			for (int i = 0; i < 4; i++){
				if (hero[i].Dieflg() == FALSE && h_draw[i].command_run == TRUE){
					ATprocess<Enemy, Hero>(&enemy[select_obj], &hero[i], &e_draw[select_obj], &h_draw[i]);
					//�R�}���h���͊���I�����ړ�
					SelectPermissionMove(hero);
					break;
				}
			}
		}
		if (com_select == MAG){
			for (int i = 0; i < 4; i++){
				if (hero[i].Dieflg() == FALSE && h_draw[i].command_run == TRUE){
					MAGprocess<Enemy, Hero>(enemy, hero, &hero[i], &h_draw[i], h_draw, e_draw, &select_obj, &MAG_select, H_ATT);
					//�R�}���h���͊���I�����ړ�
					SelectPermissionMove(hero);
					break;
				}
			}
		}
		if (com_select == ESCAPE){
			int h_esc = 0;
			int e_esc = 0;
			for (int i = 0; i < 4; i++){
				if (hero[i].Dieflg() == TRUE)continue;
				h_esc += hero[i].s_AG();
			}
			for (int i = 0; i < e_num; i++){
				if (enemy[i].Dieflg() == TRUE)continue;
				e_esc += enemy[i].s_AG();
			}
			//�G�X�P�[�v�ۊm�F
			bool es = TRUE;
			for (int i = 0; i < e_num; i++){
				if (enemy[i].Dieflg() == TRUE)continue;
				if (enemy[i].s_esc() == FALSE){
					es = FALSE;
					break;
				}
			}
			if (es == TRUE && h_esc > e_esc)Escape_s = TRUE;
			else Escape_s = FALSE;
			Escape_f = 300;
			SelectPermissionMove(hero);
			for (int i = 0; i < 4; i++)h_draw[i].AGmeter = 0;
		}
		com_select = NOSELECT;
	}
	//�v���C���[��
	for (int i = 0; i < 4; i++)RCVdraw<Hero>(&hero[i], &h_draw[i], H_DMdrawYMAX);
	//�G�_���[�W
	for (int i = 0; i < e_num; i++)DMdraw<Enemy>(&enemy[i], &e_draw[i], E_DMdrawYMAX);
	//�G�X�P�[�v�\��
	if (Escapedraw() == FALSE)if (Escape_s == TRUE)return WIN;

	//�v���C���[�S�����[�^�[���܂��ĂȂ���Ԃł̑I�����j��
	if (h_draw[0].AGmeter <= METER_MAX && h_draw[1].AGmeter <= METER_MAX && h_draw[2].AGmeter <= METER_MAX && h_draw[3].AGmeter <= METER_MAX){
		SelectPermissionMove(hero);
	}

	//�G�S�Ł�Exp����
	if (e_draw[0].LOST_fin == TRUE &&
		(e_pos[1].element == FALSE || e_draw[1].LOST_fin == TRUE) &&
		(e_pos[2].element == FALSE || e_draw[2].LOST_fin == TRUE) &&
		(e_pos[3].element == FALSE || e_draw[3].LOST_fin == TRUE)){
		int GetExp = 0;
		for (int i = 0; i < e_num; i++){
			if (e_pos[i].element == TRUE)GetExp += enemy[i].s_Exp();
		}
		for (int i = 0; i < 4; i++){
			if (hero[i].Dieflg() == TRUE)continue;
			hero[i].s_Exp(GetExp);
		}
		for (int i = 0; i < 4; i++){
			hero[i].Act_f_init();
		}
		return WIN;
	}
	Debug(enemy);//�f�o�b�N�p
	return IN_BATTLE;
}

bool Battle::Escapedraw(){

	float m = tfloat.Add(0.1f);
	if (Escape_f != 0){
		if ((Escape_f += m) < 330){
			if (Escape_s == TRUE)text->Drawtext(L"�G�X�P�[�v����", 300.0f, 300.0f, 35.0f, { 0.0f, 0.7f, 1.0f, 1.0f });
			if (Escape_s == FALSE)text->Drawtext(L"�G�X�P�[�v���s", 300.0f, 300.0f, 35.0f, { 0.0f, 0.7f, 1.0f, 1.0f });
			return TRUE;
		}
		else {
			Escape_f = 0;
			return FALSE;
		}
	}
	return TRUE;
}

bool Battle::GetH_DM(int element){

	if (h_draw[element].DMdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetH_RCV(int element){

	if (h_draw[element].RCVdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetE_DM(int element){

	if (e_pos[element].element == FALSE)return FALSE;
	if (e_draw[element].DMdata >= 0)return TRUE;

	return FALSE;
}

bool Battle::GetE_RCV(int element){

	if (e_pos[element].element == FALSE)return FALSE;
	if (e_draw[element].RCVdata >= 0)return TRUE;

	return FALSE;
}

void Battle::Debug(Enemy *enemy){//�f�o�b�N�p

	char str[30];
	for (int i = 0; i < 4; i++){
		if (e_pos[i].element == FALSE)continue;
		sprintf(str, "ENEMY %d HP%d/MHP%d", i, enemy[i].s_HP(), enemy[i].s_MHP());
		//dx->text(str, 500, 10 + 90 * i, TRUE, 0xffffffff);
		sprintf(str, "ENEMY %d MP%d/MMP%d", i, enemy[i].s_MP(), enemy[i].s_MMP());
		//dx->text(str, 500, 25 + 90 * i, TRUE, 0xffffffff);
		sprintf(str, "ENEMY %d ���[�^�[ %f", i, e_draw[i].AGmeter);
		//dx->text(str, 500, 40 + 90 * i, TRUE, 0xffffffff);
		sprintf(str, "ENEMY %d x %f", i, e_pos[i].x);
		//dx->text(str, 500, 55 + 90 * i, TRUE, 0xffffffff);
		sprintf(str, "ENEMY %d y %f", i, e_pos[i].y);
		//dx->text(str, 500, 70 + 90 * i, TRUE, 0xffffffff);
		sprintf(str, "ENEMY %d z %f", i, e_pos[i].z);
		//dx->text(str, 500, 85 + 90 * i, TRUE, 0xffffffff);
	}
}