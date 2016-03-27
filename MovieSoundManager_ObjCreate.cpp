//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　    MovieSoundManagerクラス                                 **//
//**                                ObjCreate関数                                        **//
//*****************************************************************************************//

#include "MovieSoundManager.h"

MovieSoundManager::Movie *MovieSoundManager::mo;
MovieSoundManager::Movie *MovieSoundManager::f_wall;
MovieSoundManager::Sound_ *MovieSoundManager::dungeon_so[5];
MovieSoundManager::Sound_ *MovieSoundManager::rain_so;
MovieSoundManager::Sound_ *MovieSoundManager::enemy_so;
MovieSoundManager::Sound_ *MovieSoundManager::title_so;
MovieSoundManager::Sound_ *MovieSoundManager::die_so;
MovieSoundManager::Sound_ *MovieSoundManager::att_so;
MovieSoundManager::Sound_ *MovieSoundManager::flame_so;
MovieSoundManager::Sound_ *MovieSoundManager::heal_so;
MovieSoundManager::Sound_ *MovieSoundManager::magic_so;
MovieSoundManager::Sound_ *MovieSoundManager::select_so;
MovieSoundManager::Sound_ *MovieSoundManager::enter_so;
MovieSoundManager::Sound_ *MovieSoundManager::ending_so;
MovieSoundManager::Sound_ *MovieSoundManager::bosslost_so;
int MovieSoundManager::map_n;

MovieSoundManager::MovieSoundManager(){}

void MovieSoundManager::ObjInit(){

	mo = new Movie(0);
	f_wall = new Movie(1);
	for (int i = 0; i < 5; i++)dungeon_so[i] = NULL;
	rain_so = NULL;
	enemy_so = NULL;
	bosslost_so = NULL;
	title_so = NULL;
	die_so = NULL;
	ending_so = NULL;
	att_so = new Sound_(0);
	flame_so = new Sound_(1);
	heal_so = new Sound_(2);
	magic_so = new Sound_(3);
	select_so = new Sound_(4);
	enter_so = new Sound_(5);
	map_n = 0;
}

void MovieSoundManager::ObjCreate_title(){

	if (title_so == NULL)title_so = new Sound_(20);
}

void MovieSoundManager::ObjDelete_title(){

	if (title_so != NULL){
		delete title_so; title_so = NULL;
	}
}

void MovieSoundManager::ObjCreate_map(int map_no){
	map_n = map_no;
	switch (map_no){
	case 0:
		if (dungeon_so[0] == NULL)dungeon_so[0] = new Sound_(21);
		break;
	case 1:
		if (dungeon_so[1] == NULL)dungeon_so[1] = new Sound_(22);
		if (rain_so == NULL)rain_so = new Sound_(23);
		break;
	case 2:
		if (dungeon_so[2] == NULL)dungeon_so[2] = new Sound_(24);
		break;
	case 3:
		if (dungeon_so[3] == NULL)dungeon_so[3] = new Sound_(25);
		break;
	case 4:
		if (dungeon_so[4] == NULL)dungeon_so[4] = new Sound_(26);
		break;
	}
}

void MovieSoundManager::ObjChange_map(){
	for (int i = 0; i < 5; i++){
		if (map_n != i){
			delete dungeon_so[i];
			dungeon_so[i] = NULL;
		}
	}
	if (map_n != 1 && rain_so != NULL){
		delete rain_so;
		rain_so = NULL;
	}
}

void MovieSoundManager::ObjDelete_map(){
	for (int i = 0; i < 5; i++){
		if (dungeon_so[i] != NULL){
			delete dungeon_so[i];
			dungeon_so[i] = NULL;
		}
	}
	if (rain_so != NULL){
		delete rain_so;
		rain_so = NULL;
	}
}

void MovieSoundManager::ObjCreate_battle(int n){

	if (die_so == NULL)die_so = new Sound_(30);
	if (n == 0 && enemy_so == NULL)enemy_so = new Sound_(31);
	if (n == 1 && enemy_so == NULL)enemy_so = new Sound_(32);
	if (n == 2 && enemy_so == NULL)enemy_so = new Sound_(33);
	if (n == 3 && enemy_so == NULL)enemy_so = new Sound_(34);
	if (n > 0 && bosslost_so == NULL)bosslost_so = new Sound_(36);
}

void MovieSoundManager::ObjDelete_battle(){

	if (die_so != NULL){
		delete die_so;
		die_so = NULL;
	}
	if (enemy_so != NULL){
		delete enemy_so;
		enemy_so = NULL;
	}
	if (bosslost_so != NULL){
		delete bosslost_so;
		bosslost_so = NULL;
	}
}

void MovieSoundManager::ObjCreate_ending(){

	if (ending_so == NULL)ending_so = new Sound_(35);
}

void MovieSoundManager::ObjDelete_ending(){

	if (ending_so != NULL){
		delete ending_so;
		ending_so = NULL;
	}
}

void MovieSoundManager::ObjDelete(){

	ObjDelete_title();
	ObjDelete_map();
	ObjDelete_battle();
	ObjDelete_ending();

	if (mo != NULL){
		delete mo; 
		mo = NULL;
	}
	if (f_wall != NULL){
		delete f_wall; 
		f_wall = NULL;
	}
	if (att_so != NULL){
		delete att_so;
		att_so = NULL;
	}
	if (flame_so != NULL){
		delete flame_so;
		flame_so = NULL;
	}
	if (heal_so != NULL){
		delete heal_so;
		heal_so = NULL;
	}
	if (magic_so != NULL){
		delete magic_so;
		magic_so = NULL;
	}
	if (select_so != NULL){
		delete select_so;
		select_so = NULL;
	}
	if (enter_so != NULL){
		delete enter_so;
		enter_so = NULL;
	}
}

int **MovieSoundManager::Torch_GetFrame(int width, int height){
	return mo->GetFrame(width, height);
}

int **MovieSoundManager::FireWall_GetFrame(int width, int height){
	return f_wall->GetFrame(width, height);
}

void MovieSoundManager::Dungeon_sound(bool repeat, int map){
	dungeon_so[map]->sound(repeat, -1000);
}

void MovieSoundManager::Dungeon_soundoff(int map){
	dungeon_so[map]->soundoff();
}

void MovieSoundManager::Rain_sound(bool repeat){
	rain_so->sound(repeat, -1200);
}

void MovieSoundManager::Rain_soundoff(){
	rain_so->soundoff();
}

void MovieSoundManager::Enemy_sound(bool repeat){
	enemy_so->sound(repeat, -1000);
}

void MovieSoundManager::Enemy_soundoff(){
	enemy_so->soundoff();
}

void MovieSoundManager::Bosslost_sound(bool repeat){
	bosslost_so->soundloop(repeat, -700, 5, 30);
}

void MovieSoundManager::Title_sound(bool repeat){
	title_so->sound(repeat, -1000);
}

void MovieSoundManager::Title_soundoff(){
	title_so->soundoff();
}

void MovieSoundManager::Ending_sound(bool repeat){
	ending_so->sound(repeat, -1000);
}

void MovieSoundManager::Die_sound(bool repeat){
	die_so->sound(repeat, -1000);
}

void MovieSoundManager::Die_soundoff(){
	die_so->soundoff();
}

void MovieSoundManager::Att_sound(bool repeat){
	att_so->sound(repeat, -700);
}

void MovieSoundManager::Flame_sound(bool repeat){
	flame_so->sound(repeat, -700);
}

void MovieSoundManager::Heal_sound(bool repeat){
	heal_so->sound(repeat, -700);
}

void MovieSoundManager::Magic_sound(bool repeat){
	magic_so->sound(repeat, -500);
}

void MovieSoundManager::Select_sound(bool repeat){
	select_so->sound(repeat, -500);
}

void MovieSoundManager::Enter_sound(bool repeat){
	enter_so->sound(repeat, -500);
}
