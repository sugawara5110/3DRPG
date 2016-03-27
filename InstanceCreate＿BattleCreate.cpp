//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	 InstanceCreateクラス                                    **//
//**                               BattleCreate関数                                      **//
//*****************************************************************************************//

#include "InstanceCreate.h"

HANDLE *InstanceCreate::resource_loading_h = NULL;

HANDLE *InstanceCreate::battle_loading_h = NULL;
Position::E_Pos *InstanceCreate::e_po = NULL;
Position::H_Pos *InstanceCreate::h_po = NULL;
Encount InstanceCreate::encount_d = NOENCOUNT;
int InstanceCreate::no_d = -1;
int InstanceCreate::e_nu_d = -1;
Battle *InstanceCreate::battle = NULL;

HANDLE *InstanceCreate::map_loading_h = NULL;
Position::H_Pos *InstanceCreate::h_p = NULL;
Map *InstanceCreate::map = NULL;
Map *InstanceCreate::map_t = NULL;

void InstanceCreate::CreateThread_R(){
	resource_loading_h = (HANDLE*)_beginthreadex(NULL, 0, ResourceLoading, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_B(){
	battle_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingBattle, NULL, 0, NULL);
}

void InstanceCreate::CreateThread_M(){
	map_loading_h = (HANDLE*)_beginthreadex(NULL, 0, InstanceLoadingMap, NULL, 0, NULL);
}

void InstanceCreate::DeleteThread_R(){
	WaitForSingleObject(resource_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(resource_loading_h);//ハンドルを閉じる
	resource_loading_h = NULL;
}

void InstanceCreate::DeleteThread_B(){
	WaitForSingleObject(battle_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(battle_loading_h);                 //ハンドルを閉じる
	battle_loading_h = NULL;
}

void InstanceCreate::DeleteThread_M(){
	WaitForSingleObject(map_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(map_loading_h);                 //ハンドルを閉じる
	map_loading_h = NULL;
}

void InstanceCreate::ResourceLoad(){
	Dx11Process::GetInstance()->TextureBinaryDecodeAll();
	MovieSoundManager::ObjInit();
}

bool InstanceCreate::Resource_load_f(){
	DWORD th_end;
	GetExitCodeThread(resource_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return FALSE;
	return TRUE;
}

HANDLE *InstanceCreate::GetHANDLE_B(){
	return battle_loading_h;
}

void InstanceCreate::SetInstanceParameter_B(Position::E_Pos *e_pos, Position::H_Pos *h_pos, Encount encount, int no, int e_nu){
	e_po = e_pos;
	h_po = h_pos;
	encount_d = encount;
	no_d = no;
	e_nu_d = e_nu;
}

void InstanceCreate::BattleCreate(){
	if (battle == NULL){
		battle = new Battle(e_po, h_po, encount_d, no_d, e_nu_d);
	}
}

void InstanceCreate::BattleDelete(){
	if (battle != NULL){
		delete battle;
		battle = NULL;
	}
}

bool InstanceCreate::BattleCreate_f(){
	DWORD th_end;
	GetExitCodeThread(battle_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return FALSE;
	return TRUE;
}

Battle *InstanceCreate::GetInstance_B(){
	return battle;
}

HANDLE *InstanceCreate::GetHANDLE_M(){
	return map_loading_h;
}

void InstanceCreate::SetInstanceParameter_M(Position::H_Pos *h_pos){
	h_p = h_pos;
}

void InstanceCreate::MapCreate(){
	if (map_t != NULL){
		delete map_t;
		map_t = NULL;
	}
	map_t = new Map(h_p);
	MovieSoundManager::ObjCreate_map(Map::GetMapNo());
}

void InstanceCreate::MapObjSet(){
	if (map != NULL){
		delete map;
		map = NULL;
	}
	MovieSoundManager::ObjChange_map();
	map = map_t;
	map_t = NULL;
}

void InstanceCreate::MapDelete(){
	if (map != NULL){
		delete map;
		map = NULL;
	}
	if (map_t != NULL){
		delete map_t;
		map_t = NULL;
	}
}

bool InstanceCreate::MapCreate_f(){
	DWORD th_end;
	GetExitCodeThread(map_loading_h, &th_end);
	if (th_end == STILL_ACTIVE)return FALSE;
	return TRUE;
}

Map *InstanceCreate::GetInstance_M(){
	return map;
}

unsigned __stdcall ResourceLoading(void *){
	InstanceCreate::ResourceLoad();
	return 0;
}

unsigned __stdcall InstanceLoadingBattle(void *){
	InstanceCreate::BattleCreate();
	return 0;
}

unsigned __stdcall InstanceLoadingMap(void *){
	InstanceCreate::MapCreate();
	return 0;
}