//*****************************************************************************************//
//**                                                                                     **//
//**                   @@@@@@      EndingNX                                    **//
//**                                     StaffRollÖ                                   **//
//*****************************************************************************************//

#include "Ending.h"

Ending::Ending(){

	dx = Dx11Process::GetInstance();
	text = DxText::GetInstance();
	str_y = 600;
	MovieSoundManager::ObjCreate_ending();
	MovieSoundManager::Ending_sound(FALSE);
}

void Ending::StaffRoll(){

	MovieSoundManager::Ending_sound(TRUE);
	text->Drawtext(L"^CgAGfBOaflAøÊ¹ñ", 110.0f, str_y, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"¤°l", 340.0f, str_y + 30.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"í¬A}bvaflñ", 250.0f, str_y + 200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"vrl", 170.0f, str_y + 230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"Gæñ", 320.0f, str_y + 400.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"gh|shldl", 290.0f, str_y + 430.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"e|l", 250.0f, str_y + 460.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"GtFNgæñ", 260.0f, str_y + 600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"ÒÛâl", 340.0f, str_y + 630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"CvO}[", 260.0f, str_y + 1200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"´", 350.0f, str_y + 1230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"GO[NeBuEvf[T[", 160.0f, str_y + 1600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"´", 350.0f, str_y + 1630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"sgddmc", 310.0f, str_y + 2500.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (str_y > -2200)str_y -= 0.4f;
}

Ending::~Ending(){

	MovieSoundManager::ObjDelete_ending();
}

