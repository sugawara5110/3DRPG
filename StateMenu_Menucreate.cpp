//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         StateMenuクラス                                 **//
//**                                     Menucreate関数                                  **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Dx9Process.h"
#include "Hero.h"
#include "StateMenu.h"

StateMenu::StateMenu(){

	dx = Dx9Process::GetInstance();
	MovieSoundManager::ObjCreate_title();
	MovieSoundManager::Title_sound(FALSE);
	state = (Dx9Process::MY_VERTEX2*)malloc(sizeof(Dx9Process::MY_VERTEX2) * 4);
	s_state = (Dx9Process::MY_VERTEX2*)malloc(sizeof(Dx9Process::MY_VERTEX2) * 4);
	r_state = (Dx9Process::MY_VERTEX2*)malloc(sizeof(Dx9Process::MY_VERTEX2) * 4);
	r_state1 = (Dx9Process::MY_VERTEX2*)malloc(sizeof(Dx9Process::MY_VERTEX2) * 4);
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

	state[0].x = 0.0f;
	state[0].y = 0.0f;
	state[0].z = 0.0f;
	state[0].rhw = 0.0f;
	state[0].color = (200 << 16) + (200 << 8) + 200;
	state[0].tu = 0.0f;
	state[0].tv = 0.0f;

	state[1].x = 0.0f;
	state[1].y = 430.0f;
	state[1].z = 0.0f;
	state[1].rhw = 0.0f;
	state[1].color = (200 << 16) + (200 << 8) + 200;
	state[1].tu = 0.0f;
	state[1].tv = 0.0f;

	state[2].x = 370.0f;
	state[2].y = 430.0f;
	state[2].z = 0.0f;
	state[2].rhw = 0.0f;
	state[2].color = (200 << 16) + (200 << 8) + 200;
	state[2].tu = 0.0f;
	state[2].tv = 0.0f;

	state[3].x = 370.0f;
	state[3].y = 0.0f;
	state[3].z = 0.0f;
	state[3].rhw = 0.0f;
	state[3].color = (200 << 16) + (200 << 8) + 200;
	state[3].tu = 0.0f;
	state[3].tv = 0.0f;
	dx->D2primitive(1, state);

	state[0].x = 5.0f;
	state[0].y = 5.0f;
	state[0].z = 0.0f;
	state[0].rhw = 0.0f;
	state[0].color = (0 << 16) + (0 << 8) + 200;
	state[0].tu = 0.0f;
	state[0].tv = 0.0f;

	state[1].x = 5.0f;
	state[1].y = 425.0f;
	state[1].z = 0.0f;
	state[1].rhw = 0.0f;
	state[1].color = (0 << 16) + (0 << 8) + 200;
	state[1].tu = 0.0f;
	state[1].tv = 0.0f;

	state[2].x = 365.0f;
	state[2].y = 425.0f;
	state[2].z = 0.0f;
	state[2].rhw = 0.0f;
	state[2].color = (0 << 16) + (0 << 8) + 200;
	state[2].tu = 0.0f;
	state[2].tv = 0.0f;

	state[3].x = 365.0f;
	state[3].y = 5.0f;
	state[3].z = 0.0f;
	state[3].rhw = 0.0f;
	state[3].color = (0 << 16) + (0 << 8) + 200;
	state[3].tu = 0.0f;
	state[3].tv = 0.0f;
	dx->D2primitive(1, state);
}

void StateMenu::St_create(){

	s_state[0].x = 110.0f;
	s_state[0].y = 0.0f;
	s_state[0].z = 0.0f;
	s_state[0].rhw = 0.0f;
	s_state[0].color = (200 << 16) + (200 << 8) + 200;
	s_state[0].tu = 0.0f;
	s_state[0].tv = 0.0f;

	s_state[1].x = 110.0f;
	s_state[1].y = 430.0f;
	s_state[1].z = 0.0f;
	s_state[1].rhw = 0.0f;
	s_state[1].color = (200 << 16) + (200 << 8) + 200;
	s_state[1].tu = 0.0f;
	s_state[1].tv = 0.0f;

	s_state[2].x = 370.0f;
	s_state[2].y = 430.0f;
	s_state[2].z = 0.0f;
	s_state[2].rhw = 0.0f;
	s_state[2].color = (200 << 16) + (200 << 8) + 200;
	s_state[2].tu = 0.0f;
	s_state[2].tv = 0.0f;

	s_state[3].x = 370.0f;
	s_state[3].y = 0.0f;
	s_state[3].z = 0.0f;
	s_state[3].rhw = 0.0f;
	s_state[3].color = (200 << 16) + (200 << 8) + 200;
	s_state[3].tu = 0.0f;
	s_state[3].tv = 0.0f;
	dx->D2primitive(1, s_state);

	s_state[0].x = 115.0f;
	s_state[0].y = 5.0f;
	s_state[0].z = 0.0f;
	s_state[0].rhw = 0.0f;
	s_state[0].color = (0 << 16) + (0 << 8) + 200;
	s_state[0].tu = 0.0f;
	s_state[0].tv = 0.0f;

	s_state[1].x = 115.0f;
	s_state[1].y = 425.0f;
	s_state[1].z = 0.0f;
	s_state[1].rhw = 0.0f;
	s_state[1].color = (0 << 16) + (0 << 8) + 200;
	s_state[1].tu = 0.0f;
	s_state[1].tv = 0.0f;

	s_state[2].x = 365.0f;
	s_state[2].y = 425.0f;
	s_state[2].z = 0.0f;
	s_state[2].rhw = 0.0f;
	s_state[2].color = (0 << 16) + (0 << 8) + 200;
	s_state[2].tu = 0.0f;
	s_state[2].tv = 0.0f;

	s_state[3].x = 365.0f;
	s_state[3].y = 5.0f;
	s_state[3].z = 0.0f;
	s_state[3].rhw = 0.0f;
	s_state[3].color = (0 << 16) + (0 << 8) + 200;
	s_state[3].tu = 0.0f;
	s_state[3].tv = 0.0f;
	dx->D2primitive(1, s_state);
}

void StateMenu::Re_create(){

	r_state[0].x = 0.0f;
	r_state[0].y = 0.0f;
	r_state[0].z = 0.0f;
	r_state[0].rhw = 0.0f;
	r_state[0].color = (200 << 16) + (200 << 8) + 200;
	r_state[0].tu = 0.0f;
	r_state[0].tv = 0.0f;

	r_state[1].x = 0.0f;
	r_state[1].y = 430.0f;
	r_state[1].z = 0.0f;
	r_state[1].rhw = 0.0f;
	r_state[1].color = (200 << 16) + (200 << 8) + 200;
	r_state[1].tu = 0.0f;
	r_state[1].tv = 0.0f;

	r_state[2].x = 110.0f;
	r_state[2].y = 430.0f;
	r_state[2].z = 0.0f;
	r_state[2].rhw = 0.0f;
	r_state[2].color = (200 << 16) + (200 << 8) + 200;
	r_state[2].tu = 0.0f;
	r_state[2].tv = 0.0f;

	r_state[3].x = 110.0f;
	r_state[3].y = 0.0f;
	r_state[3].z = 0.0f;
	r_state[3].rhw = 0.0f;
	r_state[3].color = (200 << 16) + (200 << 8) + 200;
	r_state[3].tu = 0.0f;
	r_state[3].tv = 0.0f;
	dx->D2primitive(1, r_state);

	r_state[0].x = 5.0f;
	r_state[0].y = 5.0f;
	r_state[0].z = 0.0f;
	r_state[0].rhw = 0.0f;
	r_state[0].color = (0 << 16) + (0 << 8) + 200;
	r_state[0].tu = 0.0f;
	r_state[0].tv = 0.0f;

	r_state[1].x = 5.0f;
	r_state[1].y = 425.0f;
	r_state[1].z = 0.0f;
	r_state[1].rhw = 0.0f;
	r_state[1].color = (0 << 16) + (0 << 8) + 200;
	r_state[1].tu = 0.0f;
	r_state[1].tv = 0.0f;

	r_state[2].x = 105.0f;
	r_state[2].y = 425.0f;
	r_state[2].z = 0.0f;
	r_state[2].rhw = 0.0f;
	r_state[2].color = (0 << 16) + (0 << 8) + 200;
	r_state[2].tu = 0.0f;
	r_state[2].tv = 0.0f;

	r_state[3].x = 105.0f;
	r_state[3].y = 5.0f;
	r_state[3].z = 0.0f;
	r_state[3].rhw = 0.0f;
	r_state[3].color = (0 << 16) + (0 << 8) + 200;
	r_state[3].tu = 0.0f;
	r_state[3].tv = 0.0f;
	dx->D2primitive(1, r_state);
}

void StateMenu::Re_create1(){

	r_state1[0].x = 0.0f;
	r_state1[0].y = 150.0f;
	r_state1[0].z = 0.0f;
	r_state1[0].rhw = 0.0f;
	r_state1[0].color = (200 << 16) + (200 << 8) + 200;
	r_state1[0].tu = 0.0f;
	r_state1[0].tv = 0.0f;

	r_state1[1].x = 0.0f;
	r_state1[1].y = 430.0f;
	r_state1[1].z = 0.0f;
	r_state1[1].rhw = 0.0f;
	r_state1[1].color = (200 << 16) + (200 << 8) + 200;
	r_state1[1].tu = 0.0f;
	r_state1[1].tv = 0.0f;

	r_state1[2].x = 135.0f;
	r_state1[2].y = 430.0f;
	r_state1[2].z = 0.0f;
	r_state1[2].rhw = 0.0f;
	r_state1[2].color = (200 << 16) + (200 << 8) + 200;
	r_state1[2].tu = 0.0f;
	r_state1[2].tv = 0.0f;

	r_state1[3].x = 135.0f;
	r_state1[3].y = 150.0f;
	r_state1[3].z = 0.0f;
	r_state1[3].rhw = 0.0f;
	r_state1[3].color = (200 << 16) + (200 << 8) + 200;
	r_state1[3].tu = 0.0f;
	r_state1[3].tv = 0.0f;
	dx->D2primitive(1, r_state1);

	r_state1[0].x = 5.0f;
	r_state1[0].y = 155.0f;
	r_state1[0].z = 0.0f;
	r_state1[0].rhw = 0.0f;
	r_state1[0].color = (0 << 16) + (0 << 8) + 200;
	r_state1[0].tu = 0.0f;
	r_state1[0].tv = 0.0f;

	r_state1[1].x = 5.0f;
	r_state1[1].y = 425.0f;
	r_state1[1].z = 0.0f;
	r_state1[1].rhw = 0.0f;
	r_state1[1].color = (0 << 16) + (0 << 8) + 200;
	r_state1[1].tu = 0.0f;
	r_state1[1].tv = 0.0f;

	r_state1[2].x = 130.0f;
	r_state1[2].y = 425.0f;
	r_state1[2].z = 0.0f;
	r_state1[2].rhw = 0.0f;
	r_state1[2].color = (0 << 16) + (0 << 8) + 200;
	r_state1[2].tu = 0.0f;
	r_state1[2].tv = 0.0f;

	r_state1[3].x = 130.0f;
	r_state1[3].y = 155.0f;
	r_state1[3].z = 0.0f;
	r_state1[3].rhw = 0.0f;
	r_state1[3].color = (0 << 16) + (0 << 8) + 200;
	r_state1[3].tu = 0.0f;
	r_state1[3].tv = 0.0f;
	dx->D2primitive(1, r_state1);
}

StateMenu::~StateMenu(){

	MovieSoundManager::ObjDelete_title();

	free(state);
	state = NULL;
	free(s_state);
	s_state = NULL;
	free(r_state);
	r_state = NULL;
	free(r_state1);
	r_state1 = NULL;
}