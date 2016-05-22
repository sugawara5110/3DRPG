//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@    3DRPG_Main                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dx11Process.h"
#include "DxText.h"
#include "Map.h"
#include "Control.h"
#include "InstanceCreate.h"
#include "Hero.h"
#include "StateMenu.h"
#include <Process.h>
#include "Ending.h"
#pragma comment(lib,"winmm.lib")

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
//		hPrevInstance : �ȑO�̃C���X�^���X�̃n���h��(win16�ł̕ʃC���X�^���X�m�F�pwin32�ł͏��NULL)
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
	try{
		//DirectX������
		dx->Initialize(hWnd);
	}
	catch (char *E_mes){
		ErrorMessage(E_mes);
		Dx11Process::DeleteInstance();
		return E_FAIL;
	}
	//DxText�I�u�W�F�N�g����
	DxText::InstanceCreate();
	//�v���C���[
	Hero *hero = NULL;
	//�X���b�h����
	InstanceCreate::CreateThread_R();
	InstanceCreate::CreateThread_H();
	double i = 300.0;
	bool down = TRUE;
	bool res_f = FALSE;
	try{
		while (1){
			if (hero == NULL)hero = InstanceCreate::HeroCreate_f();
			if (res_f == FALSE)res_f = InstanceCreate::Resource_load_f();
			if (res_f == TRUE && hero != NULL){
				InstanceCreate::DeleteThread_R();
				dx->GetTexture(); break;
			}
			dx->Sclear();
			DxText::GetInstance()->Drawtext(L"�m�����k������������", 215.0f, (float)i, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
			dx->Drawscreen();
			if (down == TRUE)i += 0.01;
			if (down == FALSE)i -= 0.01;
			if (i > 320.0)down = FALSE;
			if (i < 280.0)down = TRUE;
		}
	}
	catch (char *E_mes){
		ErrorMessage(E_mes);
		DxText::DeleteInstance();
		Dx11Process::DeleteInstance();
		return E_FAIL;
	}

	bool battle_flg[3] = { FALSE, FALSE, FALSE };
	float view_rev = 0.0f;
	Control control;
	int map_no = 0;
	InstanceCreate::MapCreate();//�^�C�g���ɏo�͂���}�b�v
	InstanceCreate::MapObjSet();
	StateMenu statemenu;
	Encount encount = NOENCOUNT;
	bool menu = FALSE;
	bool title = TRUE;
	bool title_in = TRUE;
	bool heroInput = FALSE;
	Result result = WIN;
	MapState mapstate = NORMAL_MAP;
	Ending *ending = NULL;
	bool endingflg = FALSE;
	int  rnd;
	T_float tfloat;
	//FPS�v�Z�p
	DWORD time = 0;
	int frame = 0;
	char str[50];

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
		dx->Sclear();

		//FPS�v��
		frame++;
		sprintf(str, "     Ctrl:����  Delete:�L�����Z��  fps=%d", frame);
		if (timeGetTime() - time > 1000)
		{
			time = timeGetTime();
			frame = 0;
			char Name[100] = { 0 };
			GetClassNameA(hWnd, Name, sizeof(Name));
			strcat(Name, str);
			SetWindowTextA(hWnd, Name);
		}

		if (title == TRUE){
			if (title_in == TRUE){
				title_in = statemenu.TitleMenu(control.Direction());
			}
			if (title_in == FALSE){
				if (heroInput == NULL){
					for (int i = 0; i < 4; i++)hero[i].P_DataInput(statemenu.SetP_Data(i));//�p�����[�^�[�C���v�b�g
					heroInput = TRUE;
					Map::SetMapNo(statemenu.SetMap_No());
					for (int i = 0; i < 5; i++)Map::SetBossKilled(i, statemenu.Set_boss_kil(i));
				}
				title = InstanceCreate::CreateMapIns(statemenu.SetH_Pos(), &hero[0], &map_no);
			}
		}

		encount = InstanceCreate::GetInstance_M()->Mapdraw(&mapstate, control.Direction(TRUE), encount, menu, title, endingflg);

		if (mapstate == CHANGE_MAP){
			bool m_flg = InstanceCreate::CreateMapIns(NULL, &hero[0], &map_no);
			if (!m_flg)mapstate = NORMAL_MAP;
		}

		if (mapstate == RECOV_MAP){
			for (int i = 0; i < 4; i++){
				hero[i].all_recover();
			}
			mapstate = NORMAL_MAP;
		}

		if (endingflg == FALSE && title == FALSE && encount == NOENCOUNT && menu == FALSE && control.Direction() == ENTER)menu = TRUE;

		if (endingflg == FALSE && title == FALSE && encount != NOENCOUNT && menu == FALSE){
			if (battle_flg[2] == FALSE){
				if (encount == SIDE){
					int LV = (hero[0].s_LV() + hero[1].s_LV() + hero[2].s_LV() + hero[3].s_LV()) / 4;
					//���x���ɂ���ēG�o��������
					rnd = 0;
					if ((map_no + 1) * 3 < LV)rnd = rand() % 2;
					if ((map_no + 1) * 5 < LV)rnd = rand() % 3;
					if ((map_no + 1) * 7 < LV)rnd = rand() % 4;
				}
				else rnd = 0;
				Position::H_Pos *h_pos = InstanceCreate::GetInstance_M()->Getposition();
				float cx = h_pos->cx1 + h_pos->cx1 - h_pos->cx2;//���_
				float cy = h_pos->cy1 + h_pos->cy1 - h_pos->cy2;//���_
				float view_x = 0.0f;
				float view_y = 0.0f;
				switch ((int)h_pos->theta){
				case 360:
				case 0:
					view_y = -view_rev;
					break;
				case 90:
					view_x = view_rev;
					break;
				case 180:
					view_y = view_rev;
					break;
				case 270:
					view_x = -view_rev;
					break;
				}
				dx->Cameraset(cx + view_x, h_pos->cx2 + view_x, cy + view_y, h_pos->cy2 + view_y, (float)h_pos->pz * 100.0f + 35.0f);
				hero[0].OBJWalkDraw(h_pos->cx1, h_pos->cy1, h_pos->pz*100.0f, 0, 0, 0, h_pos->theta, -1);
				battle_flg[0] = InstanceCreate::CreateBattleIns(hero, encount, map_no, rnd);
				if (battle_flg[0] == TRUE)battle_flg[1] = TRUE;
				if ((view_rev += tfloat.Add(0.2f)) >= 80.0f && battle_flg[1] == TRUE){
					battle_flg[1] = FALSE;
					battle_flg[2] = TRUE;
					view_rev = 0.0f;
				}
				battle_flg[0] = FALSE;
			}
			if (battle_flg[2] != FALSE)result = InstanceCreate::GetInstance_B()->Fight(hero, control.Direction(), result);
			if (result == WIN && battle_flg[2] != FALSE){
				InstanceCreate::BattleDelete();
				battle_flg[2] = FALSE;
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

		if (mapstate == NORMAL_MAP && endingflg == FALSE && title == FALSE && encount == NOENCOUNT && menu == TRUE){
			menu = statemenu.Menudraw(InstanceCreate::GetInstance_M()->Getposition(),
				map_no, Map::GetBossKilled(), hero, control.Direction());
		}

		T_float::GetTime();
		dx->Drawscreen();
	}

	MovieSoundManager::ObjDelete();
	InstanceCreate::BattleDelete();
	InstanceCreate::MapDelete();
	ARR_DELETE(hero);
	S_DELETE(ending);
	DxText::DeleteInstance();
	Dx11Process::DeleteInstance();
	return (int)msg.wParam;
}