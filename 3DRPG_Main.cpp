//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　    3DRPG_Main                                          **//
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

	bool loop = TRUE;
	HANDLE now_loading_h = (HANDLE)_beginthreadex(NULL, 0, NowLoading, &loop, 0, NULL);
	dx->TextureBinaryDecodeAll();
	MovieSoundManager::ObjInit();
	loop = FALSE;
	WaitForSingleObject(now_loading_h, INFINITE);//スレッドが終了するまで待つ
	CloseHandle(now_loading_h);//ハンドルを閉じる

	dx->GetTexture();
	Control control;
	Hero *hero = NULL;
	Battle *battle = NULL;
	Map *map = NULL;
	Map::SetBossKilled(-1, 0);//ボス撃破履歴初期化
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

		if (title == TRUE){
			title = statemenu.TitleMenu(control.Direction());
			if (title == FALSE){
				hero = new Hero[4];
				for (int i = 0; i < 4; i++)new(hero + i) Hero(statemenu.SetP_Data(i), i);//配列をplacement newを使って初期化する
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
					//レベルによって敵出現数制限
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