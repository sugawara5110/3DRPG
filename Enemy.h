//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	         Enemy�N���X                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Enemy_Header
#define Class_Enemy_Header

#include "Parameter.h"
#include "Position.h"

//�O���錾
class Battle;
//�O���錾

class Enemy :public Parameter{

protected:
	int o_no;//�I�u�W�F�N�g�i���o�[
	int e_no;//�G�i���o�[
	float pos_offset;//
	Position::H_Pos *h_pos;
	Position::E_Pos *e_pos;
	Dx9Process::PolygonData en, *mag, effect;
	Dx9Process::T_xyz *p_array;//�e�N�X�`���s�N�Z���J���[XYZ�z��(�{�X�p)
	int ver_pcs[50];            //���̎g�p����s�N�Z���̃O���[�v���̌�(���ȊO)(�{�X�p)
	float mv[50];              //Z�����̊e�O���[�v���W(�{�X�p)
	int mag_size;             //�{�X�p
	bool esc;//�G�X�P�[�v�� TRUE==�G�X�P�[�v��
	float mov_z;
	float mov_y;
	float mov_x;
	float size_x;
	float size_y;
	Action normal_action;//�ʏ펞�̃A�N�V����
	Action act_f;
	int M_run_flg_rnd;  //�}�W�b�N���s�t���O�����p�����_��
	bool up;
	bool zoom;
	int count;
	float theta_recov;
	bool effect_f;//�G�t�F�N�gON,OFF
	float tx, ty;//�G�t�F�N�g�p�e�N�X�`�����W

	void Enemycreate(float x, float y);
	virtual bool Magiccreate(float x, float y, float z);
	bool Effectdraw(Battle *battle, int *E_select_obj);
	void PosOffset(int o_no);

public:
	Enemy();
	Enemy(int t_no, int no);
	Act_fin_flg Enemydraw(Battle *battle, int *E_select_obj, Action action, MagicSelect E_Magrun);
	Action Normal_act_get();
	virtual bool M_run_flg();//�}�W�b�N���s�t���O
	virtual void M_select(int *r, int *r1);//�}�W�b�N�I��
	bool s_esc();
	virtual ~Enemy();
};

#endif
