//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@    3DRPG_Main                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dx11Process.h"
#include "DxText.h"
#include <new>
#include "Map.h"
#include "Control.h"
#include "Battle.h"
#include "Hero.h"
#include "StateMenu.h"
#include <Process.h>
#include "NowLoading.h"
#include "Ending.h"

//-------------------------------------------------------------
// ���b�Z�[�W�����p�R�[���o�b�N�֐�
// ����
//		hWnd	: �E�B���h�E�n���h��
//		msg		: ���b�Z�[�W
//		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^(�����ꂽ�L�[�̓��蓙�Ɏg�p)
//		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^
// �߂�l
//		���b�Z�[�W��������
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch (msg) {
	case WM_CLOSE:			//�~�{�^��
		PostQuitMessage(0);//�A�v���P�[�V�����I������,���b�Z�[�W�L���[��WM_QUIT���|�X�g
		break;
	case WM_KEYDOWN:
		switch ((CHAR)wParam){
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_LEFT:
			Control::directionkey = LEFT;
			break;
		case VK_RIGHT:
			Control::directionkey = RIGHT;
			break;
		case VK_UP:
			Control::directionkey = UP;
			break;
		case VK_DOWN:
			Control::directionkey = DOWN;
			break;
		case VK_CONTROL:
			Control::directionkey = ENTER;
			break;
		case VK_DELETE:
			Control::directionkey = CANCEL;
			break;
		default:
			Control::directionkey = NOTPRESS;
			break;
		}
		break;
	default:
		Control::directionkey = NOTPRESS;
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-------------------------------------------------------------
// �A�v���P�[�V�����̃G���g���|�C���g
// ����
//		hInstance     : ���݂̃C���X�^���X�̃n���h��
//		hPrevInstance : �ȑO�̃C���X�^���X�̃n���h��
//		lpCmdLine	  : �R�}���h���C���p�����[�^
//		nCmdShow	  : �E�B���h�E�̕\�����
// �߂�l
//		����������0�ȊO�̒l
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	srand((unsigned)time(NULL));
	TCHAR clsName[] = L"3DRPG";// �E�B���h�E�N���X��

	HWND hWnd;//�E�B���h�E�n���h��
	MSG msg; //���b�Z�[�W
	//�E�C���h�E�N���X�̏�����
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); //���̍\���̂̃T�C�Y
	wcex.style = NULL;               //�E�C���h�E�X�^�C��(default)
	wcex.lpfnWndProc = WindowProc;  //���b�Z�[�W�����֐��̓o�^
	wcex.cbClsExtra = 0;       //�ʏ��0	                
	wcex.cbWndExtra = 0;      //�ʏ��0					
	wcex.hInstance = hInstance; //�C���X�^���X�ւ̃n���h��				
	wcex.hIcon = NULL;         //�A�C�R�� (����)				
	wcex.hCursor = NULL;      //�J�[�\���̌`				
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //�w�i				
	wcex.lpszMenuName = NULL;                       //���j���[����				
	wcex.lpszClassName = (TCHAR*)clsName;          //�N���X��               
	wcex.hIconSm = NULL;                          //���A�C�R��			   

	//�E�C���h�E�N���X�̓o�^(RegisterClassEx�֐�)
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	//�E�C���h�E�����E�C���h�E���[�h
	if (!(hWnd = CreateWindow(clsName, //�o�^�N���X��
		clsName,                      //�E�C���h�E��
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,//�E�C���h�E�X�^�C��
		CW_USEDEFAULT, //�E�C���h�E���ʒu
		0,            //�E�C���h�E�c�ʒu
		800,             //�E�C���h�E��
		600,            //�E�C���h�E����
		NULL,          //�e�E�C���h�E�n���h��
		NULL,         //���j���[,�q�E�C���h�E�n���h��
		hInstance,   //�A�v���P�[�V�����C���X�^���X�n���h��
		NULL)))     //�E�C���h�E�쐬�f�[�^
		return E_FAIL;

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
	ValidateRect(hWnd, 0);

	//Dx11Process�I�u�W�F�N�g����
	Dx11Process::InstanceCreate();
	//Dx11Process�I�u�W�F�N�g�擾
	Dx11Process *dx = Dx11Process::GetInstance();
	//DirectX������
	if (dx->Initialize(hWnd) == E_FAIL)return -1;
	//DxText�I�u�W�F�N�g����
	DxText::InstanceCreate();

	bool loop = TRUE;
	HANDLE now_loading_h = (HANDLE)_beginthreadex(NULL, 0, NowLoading, &loop, 0, NULL);
	dx->TextureBinaryDecodeAll();
	MovieSoundManager::ObjInit();
	loop = FALSE;
	WaitForSingleObject(now_loading_h, INFINITE);//�X���b�h���I������܂ő҂�
	CloseHandle(now_loading_h);//�n���h�������

	dx->GetTexture();
	Control control;
	Hero *hero = NULL;
	Battle *battle = NULL;
	Map *map = NULL;
	Map::SetBossKilled(-1, 0);//�{�X���j����������
	int map_no = 0;
	map = new Map(NULL);
	StateMenu statemenu;
	Encount encount = NOENCOUNT;
	bool menu = FALSE;
	bool title = TRUE;
	Result result = WIN;
	MapState mapstate = NORMAL_MAP;
	Ending *ending = NULL;
	bool endingflg = FALSE;
	int  rnd;

	while (1){//�A�v�����s�����[�v
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {	// PostQuitMessage()���Ă΂ꂽ
				break;	//���[�v�̏I��
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (FAILED(dx->Sclear()))break;

		if (title == TRUE){
			title = statemenu.TitleMenu(control.Direction());
			if (title == FALSE){
				hero = new Hero[4];
				for (int i = 0; i < 4; i++)new(hero + i) Hero(statemenu.SetP_Data(i), i);//�z���placement new���g���ď���������
				if (map != NULL){
					delete map;
					map = NULL;
				}
				Map::SetMapNo(statemenu.SetMap_No());
				for (int i = 0; i < 5; i++)Map::SetBossKilled(i, statemenu.Set_boss_kil(i));
				map = new Map(statemenu.SetH_Pos());
				map_no = Map::GetMapNo();
			}
		}

		encount = map->Mapdraw(&mapstate, control.Direction(TRUE), encount, menu, title, endingflg);

		if (mapstate == CHANGE_MAP){
			delete map;
			map = new Map(NULL);
			map_no = Map::GetMapNo();
			mapstate = NORMAL_MAP;
		}

		if (mapstate == RECOV_MAP){
			for (int i = 0; i < 4; i++){
				hero[i].all_recover();
			}
			mapstate = NORMAL_MAP;
		}

		if (endingflg == FALSE && title == FALSE && encount == NOENCOUNT && menu == FALSE && control.Direction() == ENTER)menu = TRUE;

		if (endingflg == FALSE && title == FALSE && encount != NOENCOUNT && menu == FALSE){
			if (battle == NULL){
				if (encount == SIDE){
					int LV = (hero[0].s_LV() + hero[1].s_LV() + hero[2].s_LV() + hero[3].s_LV()) / 4;
					//���x���ɂ���ēG�o��������
					rnd = 0;
					if ((map_no + 1) * 3 < LV)rnd = rand() % 2;
					if ((map_no + 1) * 5 < LV)rnd = rand() % 3;
					if ((map_no + 1) * 7 < LV)rnd = rand() % 4;
				}
				else rnd = 0;
				battle = new Battle(map->Getposition(rnd), map->Getposition(), encount, map_no, rnd + 1);
			}
			if (battle != NULL)result = battle->Fight(hero, control.Direction(), result);
			if (result == WIN && battle != NULL){
				delete battle;
				battle = NULL;
				if (encount == BOSS)Map::SetBossKilled(map_no, 1);//�{�X���j�����X�V
				encount = NOENCOUNT;
			}
			if (result == DIE){
				DxText::GetInstance()->Drawtext(L"�f�`�l�d�n�u�d�q", 280.0f, 300.0f, 35.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}

		if (endingflg == FALSE && Map::GetBossKilled(4) == 1){
			endingflg = TRUE;
			ending = new Ending();
		}
		if (endingflg == TRUE)ending->StaffRoll();

		if (endingflg == FALSE && title == FALSE && encount == NOENCOUNT && menu == TRUE){
			menu = statemenu.Menudraw(map->Getposition(), map_no, Map::GetBossKilled(), hero, control.Direction());
		}

		T_float::GetTime();
		dx->Drawscreen();
	}

	MovieSoundManager::ObjDelete();

	if (battle != NULL){
		delete battle;
		battle = NULL;
	}
	if (map != NULL){
		delete map;
		map = NULL;
	}
	if (hero != NULL){
		delete[] hero;
		hero = NULL;
	}
	if (ending != NULL){
		delete ending;
		ending = NULL;
	}
	DxText::DeleteInstance();
	Dx11Process::DeleteInstance();
	return (int)msg.wParam;
}