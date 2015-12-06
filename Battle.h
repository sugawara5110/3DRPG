//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Battle�N���X                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Battle_Header
#define Class_Battle_Header

#include "MovieSoundManager.h"
#include "Position.h"
#define METER_MAX 25000

//�O���錾
class Map;
class Parameter;
class Hero;  
class Enemy; 
class EnemySide; 
class EnemyBoss; 
//�O���錾

class Battle{

private:
	Dx9Process *dx;
	Dx9Process::MY_VERTEX2 *command, *h_select;//�R�}���h�I���E�C���h�E, �񕜑I���J�[�\��
	Dx9Process::PolygonData E_select;//�G�I���J�[�\��
	Enemy *enemy;
	EnemySide *enemyside;
	EnemyBoss *enemyboss;
	int e_num;            //�G�o����
	bool command_run_first_flg;//�R�}���h�I�������̏�� == FALSE
	bool time_stop_flg;       //�o�g�������ԃX�g�b�v�t���O
	int Escape_f;            //�G�X�P�[�v�\���t���O,���W
	bool Escape_s;          //�G�X�P�[�v����

	CommandSelect com_select;   //�R�}���h����
	CommandSelect E_com_select;//�G�p
	MagicSelect MAG_select;   //�I�������}�W�b�N
	MagicSelect E_MAG_select;//�G�p
	int select_obj;           //�I��Ώ۔ԍ��G,�v���C���[����,�G�t�F�N�g�I��(4==�S��)
	int E_select_obj;        //�G�p(�G�t�F�N�g�I��p)(4==�S��)

	typedef struct{
		float AGmeter;        //���[�^�[
		Action action;
		MagicSelect Magrun;//�I���}�W�b�N
		int RCVdrawY;       //�񕜕\���t���O,�\�����W
		int RCVdata;       //�񕜐��ۊ�
		int DMdrawY;      //�_���[�W�\���t���O,�\�����W
		int DMdata;      //�_���[�W���ۊ�
		int draw_x;
		bool command_run;  //�R�}���h�A�N�Z�X�����,�U���X�^�[�g�t���O
		bool LOST_fin;    //LOST�A�N�V�����I���t���O(�G�̂�)
		MenuSelect manu;//���I�����j���[(�ȉ��R�}���h�I���֘A)
		int M_select;    //���C�����j���[�J�[�\���ʒu
		int A_select;   //�U���ΏۃJ�[�\���ʒu
		int MA_select; //�}�W�b�N�ΏۃJ�[�\���ʒu
		int R_select; //�񕜑ΏۃJ�[�\���ʒu 
	}Draw;
	Draw *h_draw, *e_draw;
	Position::E_Pos *e_pos;
	Position::H_Pos *h_pos;

	void Debug(Enemy *enemy);//�f�o�b�N�p
	Battle();
	void Menucreate();
	void Cursor_h(int no);
	void Cursor_e(int select);
	void SelectPermissionMove(Hero *hero);
	CommandSelect E_AT_select(Hero *hero);
	CommandSelect Menu_main(Hero *hero, int i, Directionkey direction);
	CommandSelect Menu_AT(int i, Directionkey direction);
	CommandSelect Menu_MAG_AT(int i, Directionkey direction);
	CommandSelect Menu_RCV(Hero *hero, int i, Directionkey direction);
	CommandSelect H_AT_select(Hero *hero, int i, Directionkey direction);
	bool Escapedraw();

	template<typename T_dm, typename T_att>
	void ATprocess(T_dm *dm, T_att *att, Draw *d, Draw *at){
		if (dm->Dieflg() == FALSE){
			at->action = ATTACK;
			d->DMdata = att->GetAttack();
			time_stop_flg = TRUE;
		}
		at->AGmeter = 0;
	}

	template<typename T_dm, typename T_att>
	void MAGprocess(T_dm *p_dm, T_att *p_att, T_att *att, Draw *at, Draw *p_at, Draw *p_d, int *select_ob, MagicSelect *select_M, TemplateType type){
		int att_n;
		int dm_n;
		if (type == E_ATT){
			att_n = e_num;
			dm_n = 4;
		}
		if (type == H_ATT){
			att_n = 4;
			dm_n = e_num;
		}
		int cnt = 0;
		at->action = MAGIC;
		//MP����
		if (att->DownMP(*select_M) == FALSE){
			//MP����Ȃ��ꍇ�̏���
			at->AGmeter = 0;
			at->Magrun = NOSEL;
			time_stop_flg = TRUE;
			return;
		}
		//�}�W�b�N�U������
		switch (*select_M){
		case FLAME:
			att->s_Fp(1);
			//�S�̍U��
			if (*select_ob == 4){
				for (int i = 0; i < dm_n; i++){
					if (p_dm[i].Dieflg() == FALSE)cnt++;
				}
				for (int i = 0; i < dm_n; i++){
					if (p_dm[i].Dieflg() == TRUE)continue;
					p_d[i].DMdata = att->GetMagic(FLAME, cnt);
				}
			}
			else{
				//�P�̍U��
				if (p_dm[*select_ob].Dieflg() == TRUE){
					//�I����U�����O�ɑΏ�LOST�̏ꍇ�̏���
					at->action = NORMAL; at->AGmeter = 0; return;
				}
				p_d[*select_ob].DMdata = att->GetMagic(FLAME, 1);
			}
			break;
		case HEAL:
			att->s_Hp(1);
			//�S�̍U��
			if (*select_ob == 4){
				for (int i = 0; i < att_n; i++){
					if (p_att[i].Dieflg() == FALSE)cnt++;
				}
				for (int i = 0; i < att_n; i++){
					if (p_att[i].Dieflg() == TRUE)continue;
					p_at[i].RCVdata = att->GetMagic(HEAL, cnt);
				}
			}
			else{
				//�P�̍U��
				if (p_att[*select_ob].Dieflg() == TRUE){
					//�I����񕜒��O�ɑΏ�LOST�̏ꍇ�̏���
					at->action = NORMAL;  at->AGmeter = 0; return;
				}
				p_at[*select_ob].RCVdata = att->GetMagic(HEAL, 1);
			}
			break;
		case RECOV:
			att->s_Rp(1);
			//�퓬�s�\�ґI��:����  ����ȊO�I��:����
			p_at[*select_ob].RCVdata = att->GetMagic(RECOV, 1);
			if (p_att[*select_ob].Dieflg() == TRUE){
				p_att[*select_ob].Dieflg(FALSE);
				p_at[*select_ob].action = RECOVER;
				p_at[*select_ob].LOST_fin = FALSE;
			}
			break;
		}
		at->AGmeter = 0;
		time_stop_flg = TRUE;
	}

	template<typename T_rcv>
	void RCVdraw(T_rcv *rcv, Draw *at, int DMdrawYMAX){
		char str[30];
		if (at->RCVdrawY != 0){
			if (at->RCVdrawY++ < DMdrawYMAX){
				sprintf(str, "%d", at->RCVdata);
				dx->text(str, at->draw_x, at->RCVdrawY, FALSE, 0xff00ffff);
			}
			else{
				if (at->RCVdrawY >= DMdrawYMAX && at->RCVdrawY < DMdrawYMAX + 10){
					sprintf(str, "%d", at->RCVdata);
					dx->text(str, at->draw_x, DMdrawYMAX, FALSE, 0xff00ffff);
				}
				else {
					rcv->UpHP(at->RCVdata);
					at->RCVdrawY = 0;//�����ŏ��������Ȃ��Ɛ��l�\��������ςȂ�
					time_stop_flg = FALSE;
				}
			}
		}
	}

	template<typename T_dm>
	void DMdraw(T_dm *dm, Draw *d, int DMdrawYMAX){
		char str[30];
		if (d->DMdrawY != 0){
			if (d->DMdrawY++ < DMdrawYMAX){
				sprintf(str, "%d", d->DMdata);
				dx->text(str, d->draw_x, d->DMdrawY, FALSE, 0xffffffff);
			}
			else{
				if (d->DMdrawY >= DMdrawYMAX && d->DMdrawY < DMdrawYMAX + 10){
					sprintf(str, "%d", d->DMdata);
					dx->text(str, d->draw_x, DMdrawYMAX, FALSE, 0xffffffff);
				}
				else {
					dm->DownHP(d->DMdata);
					d->DMdrawY = 0;//�����ŏ��������Ȃ��Ɛ��l�\��������ςȂ�
					time_stop_flg = FALSE;
				}
			}
		}
	}

public:
	Battle(Position::E_Pos *e_pos, Position::H_Pos *h_pos, Encount encount, int no, int e_nu);
	Result Fight(Hero *he, Directionkey direction, Result result);
	bool GetH_DM(int element);
	bool GetH_RCV(int element);
	bool GetE_DM(int element);
	bool GetE_RCV(int element);
	~Battle();
};

#endif