//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Map�N���X                                       **//
//**                                     Move�֐�                                        **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "Map.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Encount Map::Move(MapState *mapstate, Directionkey direction){

	int rnd;

	//��}�X���̈ړ��挈��
	if (elevator_UP == FALSE && elevator_DOWN == FALSE && moving == FALSE && direction != NOTPRESS &&
		direction != ENTER && direction != TWOPRESS && direction != CANCEL){
		direction_move = direction;
		if (direction_move == LEFT){
			if (src_theta == 0)src_theta = 360;
			m_theta = src_theta - 90;
		}
		if (direction_move == RIGHT){
			if (src_theta == 360)src_theta = 0;
			m_theta = src_theta + 90;
		}
		if (direction_move == UP){
			if (src_theta == 0 || src_theta == 360){
				stepx = cax1; stepy = cay1 - 100;
			}
			if (src_theta == 90){
				stepx = cax1 + 100; stepy = cay1;
			}
			if (src_theta == 180){
				stepx = cax1; stepy = cay1 + 100;
			}
			if (src_theta == 270){
				stepx = cax1 - 100; stepy = cay1;
			}
		}
		if (direction_move == DOWN){
			if (src_theta == 0 || src_theta == 360){
				stepx = cax1; stepy = cay1 + 100;
			}
			if (src_theta == 90){
				stepx = cax1 - 100; stepy = cay1;
			}
			if (src_theta == 180){
				stepx = cax1; stepy = cay1 - 100;
			}
			if (src_theta == 270){
				stepx = cax1 + 100; stepy = cay1;
			}
		}
		moving = TRUE;
	}

	//�G���x�[�^A�㓞�B
	if (elevator_UP == FALSE && mxy.m[POS_CE] == 65){
		elevator_UP = TRUE;
	}
	if (elevator_UP == TRUE){
		if ((elevator_step += 2) > 300.0f){
			posz += 3;
			elevator_step = 0.0f;
			elevator_UP = FALSE;
		}
		return NOENCOUNT;
	}
	//�G���x�[�^B�����B
	if (elevator_DOWN == FALSE && mxy.m[POS_CE] == 66){
		elevator_DOWN = TRUE;
	}
	if (elevator_DOWN == TRUE){
		if ((elevator_step -= 2) < -300.0f){
			posz -= 3;
			elevator_step = 0.0f;
			elevator_DOWN = FALSE;
		}
		return NOENCOUNT;
	}

	//�o��1�|�C���g���B
	if (mxy.m[POS_CE] == 54){
		*mapstate = CHANGE_MAP;
		switch (map_no){
		case 0:
			map_no_s = 1;
			MPos = POS_ST;
			break;
		case 1:
			map_no_s = 2;
			MPos = POS_ST;
			break;
		case 2:
			map_no_s = 3;
			MPos = POS_ST;
			break;
		}
		return NOENCOUNT;
	}

	//�o��2�|�C���g���B
	if (mxy.m[POS_CE] == 56){
		*mapstate = CHANGE_MAP;
		switch (map_no){
		case 0:
			break;
		case 1:
			map_no_s = 4;
			MPos = POS_ST;
			break;
		}
		return NOENCOUNT;
	}

	//�����|�C���g���B
	if (mxy.m[POS_CE] == 55){
		*mapstate = CHANGE_MAP;
		switch (map_no){
		case 0:
			break;
		case 1:
			map_no_s = 0;
			MPos = POS_EN1;
			break;
		case 2:
			map_no_s = 1;
			MPos = POS_EN1;
			break;
		case 3:
			map_no_s = 2;
			MPos = POS_EN1;
			break;
		case 4:
			map_no_s = 1;
			MPos = POS_EN2;
			break;
		}
		return NOENCOUNT;
	}

	//�񕜃|�C���g����
	if (mxy.m[POS_CE] == 50 && recover_p_f == FALSE){
		recover_p_f = TRUE;
		map_text_f = 300;
		_tcscpy_s(m_tx, L"�g�o�l�o�S�񕜁I�I");
		*mapstate = RECOV_MAP;
	}
	else if (mxy.m[POS_CE] != 50)recover_p_f = FALSE;

	//�����蔻��
	if (direction_move == UP){
		if (((src_theta == 0 || src_theta == 360) &&
			(posy == 0 || mxy.m[POSY_D1] == 49 ||
			mxy.m[POSY_D1] == 52 ||
			mxy.m[POSY_D1] == 53 ||
			mxy.m[POSY_D1] == 45 ||
			mxy.m[POSY_D1] == 73)) ||
			(src_theta == 90 &&
			(posx == mxy.x - 1 || mxy.m[POSX_U1] == 49 ||
			mxy.m[POSX_U1] == 52 ||
			mxy.m[POSX_U1] == 53 ||
			mxy.m[POSX_U1] == 45 ||
			mxy.m[POSX_U1] == 73)) ||
			(src_theta == 180 &&
			(posy == mxy.y - 1 || mxy.m[POSY_U1] == 49 ||
			mxy.m[POSY_U1] == 52 ||
			mxy.m[POSY_U1] == 53 ||
			mxy.m[POSY_U1] == 45 ||
			mxy.m[POSY_U1] == 73)) ||
			(src_theta == 270 &&
			(posx == 0 || mxy.m[POSX_D1] == 49 ||
			mxy.m[POSX_D1] == 52 ||
			mxy.m[POSX_D1] == 53 ||
			mxy.m[POSX_D1] == 45 ||
			mxy.m[POSX_D1] == 73))){
			moving = FALSE; return NOENCOUNT;
		}
	}
	if (direction_move == DOWN){
		if (((src_theta == 0 || src_theta == 360) &&
			(posy == mxy.y - 1 || mxy.m[POSY_U1] == 49 ||
			mxy.m[POSY_U1] == 52 ||
			mxy.m[POSY_U1] == 53 ||
			mxy.m[POSY_U1] == 56 ||
			mxy.m[POSY_U1] == 45 ||
			mxy.m[POSY_U1] == 73 ||
			mxy.m[POSY_U1] == 51 ||
			mxy.m[POSY_U1] == 54 ||
			mxy.m[POSY_U1] == 55)) ||
			(src_theta == 90 &&
			(posx == 0 || mxy.m[POSX_D1] == 49 ||
			mxy.m[POSX_D1] == 52 ||
			mxy.m[POSX_D1] == 53 ||
			mxy.m[POSX_D1] == 56 ||
			mxy.m[POSX_D1] == 45 ||
			mxy.m[POSX_D1] == 73 ||
			mxy.m[POSX_D1] == 51 ||
			mxy.m[POSX_D1] == 54 ||
			mxy.m[POSX_D1] == 55)) ||
			(src_theta == 180 &&
			(posy == 0 || mxy.m[POSY_D1] == 49 ||
			mxy.m[POSY_D1] == 52 ||
			mxy.m[POSY_D1] == 53 ||
			mxy.m[POSY_D1] == 56 ||
			mxy.m[POSY_D1] == 45 ||
			mxy.m[POSY_D1] == 73 ||
			mxy.m[POSY_D1] == 51 ||
			mxy.m[POSY_D1] == 54 ||
			mxy.m[POSY_D1] == 55)) ||
			(src_theta == 270 &&
			(posx == mxy.x - 1 || mxy.m[POSX_U1] == 49 ||
			mxy.m[POSX_U1] == 52 ||
			mxy.m[POSX_U1] == 53 ||
			mxy.m[POSX_U1] == 56 ||
			mxy.m[POSX_U1] == 45 ||
			mxy.m[POSX_U1] == 73 ||
			mxy.m[POSX_U1] == 51 ||
			mxy.m[POSX_U1] == 54 ||
			mxy.m[POSX_U1] == 55))){
			moving = FALSE; return NOENCOUNT;
		}
	}

	//�ړ�����
	bool movf;
	float m = tfloat.Add(0.3f);
	switch (direction_move){

	case LEFT:
		src_theta = src_theta - m;
		cay2 = cay1 - (int)(cos(src_theta * 3.14f / 180.0f) * 50.0f);
		cax2 = cax1 + (int)(sin(src_theta * 3.14f / 180.0f) * 50.0f);
		if (src_theta <= m_theta){
			src_theta = m_theta;
			moving = FALSE;
			direction_move = NOTPRESS;
		}
		break;

	case RIGHT:
		src_theta = src_theta + m;
		cay2 = cay1 - (int)(cos(src_theta * 3.14f / 180.0f) * 50.0f);
		cax2 = cax1 + (int)(sin(src_theta * 3.14f / 180.0f) * 50.0f);
		if (src_theta >= m_theta){
			src_theta = m_theta;
			moving = FALSE;
			direction_move = NOTPRESS;
		}
		break;

	case UP:
		movf = FALSE;
		if (src_theta == 0 || src_theta == 360){
			cay1 -= m; cay2 -= m;
			if (stepy >= cay1){
				cay1 = stepy;
				cay2 = cay1 - 100.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 90){
			cax1 += m; cax2 += m;
			if (stepx <= cax1){
				cax1 = stepx;
				cax2 = cax1 + 100.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 180){
			cay1 += m; cay2 += m;
			if (stepy <= cay1){
				cay1 = stepy;
				cay2 = cay1 + 100.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 270){
			cax1 -= m; cax2 -= m;
			if (stepx >= cax1){
				cax1 = stepx;
				cax2 = cax1 - 100.0f;
				movf = TRUE;
			}
		}
		if (movf == TRUE){
			if (src_theta == 0 || src_theta == 360)posy -= 1;
			if (src_theta == 90)posx += 1;
			if (src_theta == 180)posy += 1;
			if (src_theta == 270)posx -= 1;
			moving = FALSE;
			direction_move = NOTPRESS;
			//�{�X�G���J�E���g
			if (mxy.m[POS_CE] == 51 && boss_p_f == FALSE){
				if (((src_theta == 0 || src_theta == 360) && mxy.m[POSY_D1] == 48) ||//�A�X�L�[�R�[�h48 = 0
					(src_theta == 90 && mxy.m[POSX_U1] == 48) ||
					(src_theta == 180 && mxy.m[POSY_U1] == 48) ||
					(src_theta == 270 && mxy.m[POSX_D1] == 48)){
					boss_p_f = TRUE;
					return BOSS;
				}
			}
			else if (mxy.m[POS_CE] != 51)boss_p_f = FALSE;
			//�ʏ�G���J�E���g
			rnd = rand() % 10;
			if (rnd == 1){
				if (mxy.m[POS_CE] != 50 && mxy.m[POS_CE] != 51 && mxy.m[POS_CE] != 65 && mxy.m[POS_CE] != 66){
					if (((src_theta == 0 || src_theta == 360) && mxy.m[POSY_D1] == 48) ||//�A�X�L�[�R�[�h48 = 0
						(src_theta == 90 && mxy.m[POSX_U1] == 48) ||
						(src_theta == 180 && mxy.m[POSY_U1] == 48) ||
						(src_theta == 270 && mxy.m[POSX_D1] == 48))return SIDE;
				}
			}
		}
		break;

	case DOWN:
		movf = FALSE;
		if (src_theta == 0 || src_theta == 360){
			cay1 += m; cay2 += m;
			if (stepy <= cay1){
				cay1 = stepy;
				cay2 = cay1 - 100.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 90){
			cax1 -= m; cax2 -= m;
			if (stepx >= cax1){
				cax1 = stepx;
				cax2 = cax1 + 100.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 180){
			cay1 -= m; cay2 -= m;
			if (stepy >= cay1){
				cay1 = stepy;
				cay2 = cay1 + 100.0f;
				movf = TRUE;
			}
		}
		if (src_theta == 270){
			cax1 += m; cax2 += m;
			if (stepx <= cax1){
				cax1 = stepx;
				cax2 = cax1 - 100.0f;
				movf = TRUE;
			}
		}
		if (movf == TRUE){
			if (src_theta == 0 || src_theta == 360)posy += 1;
			if (src_theta == 90)posx -= 1;
			if (src_theta == 180)posy -= 1;
			if (src_theta == 270)posx += 1;
			moving = FALSE;
			direction_move = NOTPRESS;
			//�ʏ�G���J�E���g
			rnd = rand() % 3;
			if (rnd == 1){
				if (mxy.m[POS_CE] != 50 && mxy.m[POS_CE] != 51 && mxy.m[POS_CE] != 65 && mxy.m[POS_CE] != 66){
					if (((src_theta == 0 || src_theta == 360) && mxy.m[POSY_D1] == 48) ||
						(src_theta == 90 && mxy.m[POSX_U1] == 48) ||
						(src_theta == 180 && mxy.m[POSY_U1] == 48) ||
						(src_theta == 270 && mxy.m[POSX_D1] == 48))return SIDE;
				}
			}
		}
		break;
	}
	return NOENCOUNT;
}

void Map::MapText(TCHAR str[30]){

	if (map_text_f != 0){
		if ((map_text_f += tfloat.Add(0.1f)) < 340){
			text->Drawtext(str, 300.0f, 300.0f, 25, { 0.0f, 1.0f, 1.0f, 1.0f });
		}
		else {
			map_text_f = 0;
		}
	}
}