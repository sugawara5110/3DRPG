//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@	 InstanceCreate�N���X                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_InstanceCreate_Header
#define Class_InstanceCreate_Header

#include "Battle.h"
#include "Map.h"
#include <Process.h>

class InstanceCreate{

private:
	//�������\�[�X�ǂݍ���
	static HANDLE *resource_loading_h;

	//Battle�����p�p�����[�^
	static HANDLE *battle_loading_h;
	static Position::E_Pos *e_po;
	static Position::H_Pos *h_po;
	static Encount encount_d;
	static int no_d;
	static int e_nu_d;
	static Battle *battle;

	//Map�����p�p�����[�^
	static HANDLE *map_loading_h;
	static Position::H_Pos *h_p;
	static Map *map, *map_t;

	InstanceCreate(){};//�C���X�^���X������

public:
	static void CreateThread_R();
	static void CreateThread_B();
	static void CreateThread_M();

	static void DeleteThread_R();
	static void DeleteThread_B();
	static void DeleteThread_M();

	static void ResourceLoad();
	static bool Resource_load_f();

	static HANDLE *GetHANDLE_B();
	static void SetInstanceParameter_B(Position::E_Pos *e_pos, Position::H_Pos *h_pos, Encount encount, int no, int e_nu);
	static void BattleCreate();//�}���`�X���b�h�ȊO�Ŏg�p�֎~
	static void BattleDelete();
	static bool BattleCreate_f();
	static Battle *GetInstance_B();//�}���`�X���b�h�Ő��������NULL�`�F�b�N�ł͖����I���t���OBattleCreate_f()�Ő����m�F����

	static HANDLE *GetHANDLE_M();
	static void SetInstanceParameter_M(Position::H_Pos *h_pos);
	static void MapCreate();//�}���`�X���b�h�ȊO�Ŏg�p�֎~(���C�����[�v�O�͉�)
	static void MapObjSet();
	static void MapDelete();
	static bool MapCreate_f();
	static Map *GetInstance_M();//�}���`�X���b�h�Ő��������NULL�`�F�b�N�ł͖����I���t���OMapCreate_f()�Ő����m�F����
};

#endif

unsigned __stdcall ResourceLoading(void *);
unsigned __stdcall InstanceLoadingBattle(void *);
unsigned __stdcall InstanceLoadingMap(void *);