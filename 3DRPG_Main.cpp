//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    3DRPG_Main                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dx11Process.h"
#include "DxText.h"
#include <new>
#include "Map.h"
#include "Control.h"
#include "InstanceCreate.h"
#include "Hero.h"
#include "StateMenu.h"
#include <Process.h>
#include "Ending.h"
#pragma comment(lib,"winmm.lib")

//-------------------------------------------------------------
// メッセージ処理用コールバック関数
// 引数
//		hWnd	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ(押されたキーの特定等に使用)
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch (msg) {
	case WM_CLOSE:			//×ボタン
		PostQuitMessage(0);//アプリケーション終了処理,メッセージキューにWM_QUITをポスト
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
// アプリケーションのエントリポイント
// 引数
//		hInstance     : 現在のインスタンスのハンドル
//		hPrevInstance : 以前のインスタンスのハンドル
//		lpCmdLine	  : コマンドラインパラメータ
//		nCmdShow	  : ウィンドウの表示状態
// 戻り値
//		成功したら0以外の値
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	srand((unsigned)time(NULL));
	TCHAR clsName[] = L"3DRPG";// ウィンドウクラス名

	HWND hWnd;//ウィンドウハンドル
	MSG msg; //メッセージ
	//ウインドウクラスの初期化
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); //この構造体のサイズ
	wcex.style = NULL;               //ウインドウスタイル(default)
	wcex.lpfnWndProc = WindowProc;  //メッセージ処理関数の登録
	wcex.cbClsExtra = 0;       //通常は0	                
	wcex.cbWndExtra = 0;      //通常は0					
	wcex.hInstance = hInstance; //インスタンスへのハンドル				
	wcex.hIcon = NULL;         //アイコン (無し)				
	wcex.hCursor = NULL;      //カーソルの形				
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //背景				
	wcex.lpszMenuName = NULL;                       //メニュー無し				
	wcex.lpszClassName = (TCHAR*)clsName;          //クラス名               
	wcex.hIconSm = NULL;                          //小アイコン			   

	//ウインドウクラスの登録(RegisterClassEx関数)
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	//ウインドウ生成ウインドウモード
	if (!(hWnd = CreateWindow(clsName, //登録クラス名
		clsName,                      //ウインドウ名
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,//ウインドウスタイル
		CW_USEDEFAULT, //ウインドウ横位置
		0,            //ウインドウ縦位置
		800,             //ウインドウ幅
		600,            //ウインドウ高さ
		NULL,          //親ウインドウハンドル
		NULL,         //メニュー,子ウインドウハンドル
		hInstance,   //アプリケーションインスタンスハンドル
		NULL)))     //ウインドウ作成データ
		return E_FAIL;

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	// WM_PAINTが呼ばれないようにする
	ValidateRect(hWnd, 0);

	//Dx11Processオブジェクト生成
	Dx11Process::InstanceCreate();
	//Dx11Processオブジェクト取得
	Dx11Process *dx = Dx11Process::GetInstance();
	//DirectX初期化
	if (dx->Initialize(hWnd) == E_FAIL)return -1;
	//DxTextオブジェクト生成
	DxText::InstanceCreate();
	//スレッド生成
	InstanceCreate::CreateThread_R();
	double i = 300.0;
	bool down = TRUE;
	while (1){
		if (InstanceCreate::Resource_load_f() == TRUE)break;
		dx->Sclear();
		DxText::GetInstance()->Drawtext(L"ＮｏｗＬｏａｄｉｎｇ", 215.0f, (float)i, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
		dx->Drawscreen();
		if (down == TRUE)i += 0.01;
		if (down == FALSE)i -= 0.01;
		if (i > 320.0)down = FALSE;
		if (i < 280.0)down = TRUE;
	}
	//スレッド解放
	InstanceCreate::DeleteThread_R();
	dx->GetTexture();
	bool battle_flg = FALSE;
	Control control;
	Hero *hero = NULL;
	Map::SetBossKilled(-1, 0);//ボス撃破履歴初期化
	int map_no = 0;
	InstanceCreate::MapCreate();
	InstanceCreate::MapObjSet();
	StateMenu statemenu;
	Encount encount = NOENCOUNT;
	bool menu = FALSE;
	bool title = TRUE;
	bool title_in = TRUE;
	Result result = WIN;
	MapState mapstate = NORMAL_MAP;
	Ending *ending = NULL;
	bool endingflg = FALSE;
	int  rnd;
	//FPS計算用
	DWORD time = 0;
	int frame = 0;
	char str[50];

	while (1){//アプリ実行中ループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {	// PostQuitMessage()が呼ばれた
				break;	//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (FAILED(dx->Sclear()))break;

		//FPS計測
		frame++;
		sprintf(str, "     Ctrl:決定  Delete:キャンセル  fps=%d", frame);
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
				if (hero == NULL){
					hero = new Hero[4];
					for (int i = 0; i < 4; i++)new(hero + i) Hero(statemenu.SetP_Data(i), i);//配列をplacement newを使って初期化する
				}
				if (InstanceCreate::GetHANDLE_M() == NULL){
					Map::SetMapNo(statemenu.SetMap_No());
					for (int i = 0; i < 5; i++)Map::SetBossKilled(i, statemenu.Set_boss_kil(i));
					InstanceCreate::SetInstanceParameter_M(statemenu.SetH_Pos());
					InstanceCreate::CreateThread_M();
					map_no = Map::GetMapNo();
				}
				if (InstanceCreate::GetHANDLE_M() != NULL && InstanceCreate::MapCreate_f() == TRUE){
					InstanceCreate::MapObjSet();
					InstanceCreate::DeleteThread_M();
					title = FALSE;
				}
			}
		}

		encount = InstanceCreate::GetInstance_M()->Mapdraw(&mapstate, control.Direction(TRUE), encount, menu, title, endingflg);

		if (mapstate == CHANGE_MAP){
			if (InstanceCreate::GetHANDLE_M() == NULL){
				InstanceCreate::SetInstanceParameter_M(NULL);
				InstanceCreate::CreateThread_M();
				map_no = Map::GetMapNo();
			}
			if (InstanceCreate::GetHANDLE_M() != NULL && InstanceCreate::MapCreate_f() == TRUE){
				InstanceCreate::MapObjSet();
				InstanceCreate::DeleteThread_M();
				mapstate = NORMAL_MAP;
			}
		}

		if (mapstate == RECOV_MAP){
			for (int i = 0; i < 4; i++){
				hero[i].all_recover();
			}
			mapstate = NORMAL_MAP;
		}

		if (endingflg == FALSE && title == FALSE && encount == NOENCOUNT && menu == FALSE && control.Direction() == ENTER)menu = TRUE;

		if (endingflg == FALSE && title == FALSE && encount != NOENCOUNT && menu == FALSE){
			if (battle_flg == FALSE){
				if (encount == SIDE){
					int LV = (hero[0].s_LV() + hero[1].s_LV() + hero[2].s_LV() + hero[3].s_LV()) / 4;
					//レベルによって敵出現数制限
					rnd = 0;
					if ((map_no + 1) * 3 < LV)rnd = rand() % 2;
					if ((map_no + 1) * 5 < LV)rnd = rand() % 3;
					if ((map_no + 1) * 7 < LV)rnd = rand() % 4;
				}
				else rnd = 0;

				if (InstanceCreate::GetHANDLE_B() == NULL){
					InstanceCreate::SetInstanceParameter_B(InstanceCreate::GetInstance_M()->Getposition(rnd),
						InstanceCreate::GetInstance_M()->Getposition(), encount, map_no, rnd + 1);
					InstanceCreate::CreateThread_B();
				}

				if (InstanceCreate::GetHANDLE_B() != NULL){
					bool bf = InstanceCreate::BattleCreate_f();
					if (bf == TRUE){
						battle_flg = TRUE;
						InstanceCreate::DeleteThread_B();
					}
				}

			}
			if (battle_flg != FALSE)result = InstanceCreate::GetInstance_B()->Fight(hero, control.Direction(), result);
			if (result == WIN && battle_flg != FALSE){
				InstanceCreate::BattleDelete();
				battle_flg = FALSE;
				if (encount == BOSS)Map::SetBossKilled(map_no, 1);//ボス撃破履歴更新
				encount = NOENCOUNT;
			}
			if (result == DIE){
				DxText::GetInstance()->Drawtext(L"ＧＡＭＥＯＶＥＲ", 280.0f, 300.0f, 35.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
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