//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         StateMenuクラス                                 **//
//**                                     Menucreate関数                                  **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Hero.h"
#include "StateMenu.h"

StateMenu::StateMenu(){

	dx = Dx11Process::GetInstance();
	text = DxText::GetInstance();
	MovieSoundManager::ObjCreate_title();
	MovieSoundManager::Title_sound(FALSE);
	dx->GetVBarray2D(&state, 1);
	dx->GetVBarray2D(&s_state, 1);
	dx->GetVBarray2D(&r_state, 1);
	dx->GetVBarray2D(&r_state1, 1);
	menu_select = MAIN;
	main_select = 0;
	st_select = 0;
	re_select = 0;
	re_select1 = -1;
	re_select2 = -1;
	pp_select = 0;
	pp_select1 = -1;
	Encryptionkey = 0xf0f0f0;
	Decryptionkey = 0x0f0f0f;
}

void StateMenu::Menucreate(){

	D3DXVECTOR4 clr = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	state.d3varray[0].x = 0.0f;
	state.d3varray[0].y = 0.0f;
	state.d3varray[0].z = 0.9f;
	state.d3varray[0].color = clr;

	state.d3varray[1].x = 0.0f;
	state.d3varray[1].y = 430.0f;
	state.d3varray[1].z = 0.9f;
	state.d3varray[1].color = clr;

	state.d3varray[2].x = 500.0f;
	state.d3varray[2].y = 0.0f;
	state.d3varray[2].z = 0.9f;
	state.d3varray[2].color = clr;

	state.d3varray[3].x = 500.0f;
	state.d3varray[3].y = 430.0f;
	state.d3varray[3].z = 0.9f;
	state.d3varray[3].color = clr;

	dx->D2primitive(&state, 1, TRUE);

	clr = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	state.d3varray[0].x = 5.0f;
	state.d3varray[0].y = 5.0f;
	state.d3varray[0].z = 0.8f;
	state.d3varray[0].color = clr;

	state.d3varray[1].x = 5.0f;
	state.d3varray[1].y = 425.0f;
	state.d3varray[1].z = 0.8f;
	state.d3varray[1].color = clr;

	state.d3varray[2].x = 495.0f;
	state.d3varray[2].y = 5.0f;
	state.d3varray[2].z = 0.8f;
	state.d3varray[2].color = clr;

	state.d3varray[3].x = 495.0f;
	state.d3varray[3].y = 425.0f;
	state.d3varray[3].z = 0.8f;
	state.d3varray[3].color = clr;

	dx->D2primitive(&state, 1, TRUE);
}

void StateMenu::St_create(){

	D3DXVECTOR4 clr = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	s_state.d3varray[0].x = 110.0f;
	s_state.d3varray[0].y = 0.0f;
	s_state.d3varray[0].z = 0.7f;
	s_state.d3varray[0].color = clr;

	s_state.d3varray[1].x = 110.0f;
	s_state.d3varray[1].y = 430.0f;
	s_state.d3varray[1].z = 0.7f;
	s_state.d3varray[1].color = clr;

	s_state.d3varray[2].x = 500.0f;
	s_state.d3varray[2].y = 0.0f;
	s_state.d3varray[2].z = 0.7f;
	s_state.d3varray[2].color = clr;

	s_state.d3varray[3].x = 500.0f;
	s_state.d3varray[3].y = 430.0f;
	s_state.d3varray[3].z = 0.7f;
	s_state.d3varray[3].color = clr;

	dx->D2primitive(&s_state, 1, TRUE);

	clr = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	s_state.d3varray[0].x = 115.0f;
	s_state.d3varray[0].y = 5.0f;
	s_state.d3varray[0].z = 0.6f;
	s_state.d3varray[0].color = clr;

	s_state.d3varray[1].x = 115.0f;
	s_state.d3varray[1].y = 425.0f;
	s_state.d3varray[1].z = 0.6f;
	s_state.d3varray[1].color = clr;

	s_state.d3varray[2].x = 495.0f;
	s_state.d3varray[2].y = 5.0f;
	s_state.d3varray[2].z = 0.6f;
	s_state.d3varray[2].color = clr;

	s_state.d3varray[3].x = 495.0f;
	s_state.d3varray[3].y = 425.0f;
	s_state.d3varray[3].z = 0.6f;
	s_state.d3varray[3].color = clr;

	dx->D2primitive(&s_state, 1, TRUE);
}

void StateMenu::Re_create(){

	D3DXVECTOR4 clr = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	r_state.d3varray[0].x = 0.0f;
	r_state.d3varray[0].y = 0.0f;
	r_state.d3varray[0].z = 0.7f;
	r_state.d3varray[0].color = clr;

	r_state.d3varray[1].x = 0.0f;
	r_state.d3varray[1].y = 430.0f;
	r_state.d3varray[1].z = 0.7f;
	r_state.d3varray[1].color = clr;

	r_state.d3varray[2].x = 110.0f;
	r_state.d3varray[2].y = 0.0f;
	r_state.d3varray[2].z = 0.7f;
	r_state.d3varray[2].color = clr;

	r_state.d3varray[3].x = 110.0f;
	r_state.d3varray[3].y = 430.0f;
	r_state.d3varray[3].z = 0.7f;
	r_state.d3varray[3].color = clr;

	dx->D2primitive(&r_state, 1, TRUE);

	clr = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	r_state.d3varray[0].x = 5.0f;
	r_state.d3varray[0].y = 5.0f;
	r_state.d3varray[0].z = 0.6f;
	r_state.d3varray[0].color = clr;

	r_state.d3varray[1].x = 5.0f;
	r_state.d3varray[1].y = 425.0f;
	r_state.d3varray[1].z = 0.6f;
	r_state.d3varray[1].color = clr;

	r_state.d3varray[2].x = 105.0f;
	r_state.d3varray[2].y = 5.0f;
	r_state.d3varray[2].z = 0.6f;
	r_state.d3varray[2].color = clr;

	r_state.d3varray[3].x = 105.0f;
	r_state.d3varray[3].y = 425.0f;
	r_state.d3varray[3].z = 0.6f;
	r_state.d3varray[3].color = clr;

	dx->D2primitive(&r_state, 1, TRUE);
}

void StateMenu::Re_create1(){

	D3DXVECTOR4 clr = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	r_state1.d3varray[0].x = 0.0f;
	r_state1.d3varray[0].y = 150.0f;
	r_state1.d3varray[0].z = 0.5f;
	r_state1.d3varray[0].color = clr;

	r_state1.d3varray[1].x = 0.0f;
	r_state1.d3varray[1].y = 430.0f;
	r_state1.d3varray[1].z = 0.5f;
	r_state1.d3varray[1].color = clr;

	r_state1.d3varray[2].x = 135.0f;
	r_state1.d3varray[2].y = 150.0f;
	r_state1.d3varray[2].z = 0.5f;
	r_state1.d3varray[2].color = clr;

	r_state1.d3varray[3].x = 135.0f;
	r_state1.d3varray[3].y = 430.0f;
	r_state1.d3varray[3].z = 0.5f;
	r_state1.d3varray[3].color = clr;

	dx->D2primitive(&r_state1, 1, TRUE);

	clr = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	r_state1.d3varray[0].x = 5.0f;
	r_state1.d3varray[0].y = 155.0f;
	r_state1.d3varray[0].z = 0.4f;
	r_state1.d3varray[0].color = clr;

	r_state1.d3varray[1].x = 5.0f;
	r_state1.d3varray[1].y = 425.0f;
	r_state1.d3varray[1].z = 0.4f;
	r_state1.d3varray[1].color = clr;

	r_state1.d3varray[2].x = 130.0f;
	r_state1.d3varray[2].y = 155.0f;
	r_state1.d3varray[2].z = 0.4f;
	r_state1.d3varray[2].color = clr;

	r_state1.d3varray[3].x = 130.0f;
	r_state1.d3varray[3].y = 425.0f;
	r_state1.d3varray[3].z = 0.4f;
	r_state1.d3varray[3].color = clr;

	dx->D2primitive(&r_state1, 1, TRUE);
}

StateMenu::~StateMenu(){

	MovieSoundManager::ObjDelete_title();
}