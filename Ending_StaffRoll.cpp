//*****************************************************************************************//
//**                                                                                     **//
//**                   @@@@@@      EndingƒNƒ‰ƒX                                    **//
//**                                     StaffRollŠÖ”                                   **//
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
	text->Drawtext(L"ƒ^ƒCƒgƒ‹AƒGƒ“ƒfƒBƒ“ƒO‚a‚f‚lAŒø‰Ê‰¹’ñ‹Ÿ", 110.0f, str_y, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"–‚‰¤°—l", 340.0f, str_y + 30.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"í“¬Aƒ}ƒbƒv‚a‚f‚l’ñ‹Ÿ", 250.0f, str_y + 200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"‚v‚‰‚Ž‚‡‚Œ‚…‚“‚“‚r‚…‚’‚‚‚ˆ—l", 170.0f, str_y + 230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"“G‰æ‘œ’ñ‹Ÿ", 320.0f, str_y + 400.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"‚g‚h|‚s‚h‚l‚d—l", 290.0f, str_y + 430.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"‚e‚‚Œ‚ƒ‚…|‚š‚…‚’‚…—l", 250.0f, str_y + 460.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"ƒGƒtƒFƒNƒg‰æ‘œ’ñ‹Ÿ", 260.0f, str_y + 600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"‚Ò‚Û‚â—l", 340.0f, str_y + 630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"ƒƒCƒ“ƒvƒƒOƒ‰ƒ}[", 260.0f, str_y + 1200.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"›Œ´‘", 350.0f, str_y + 1230.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"ƒGƒOƒ[ƒNƒeƒBƒuEƒvƒƒfƒ…[ƒT[", 160.0f, str_y + 1600.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"›Œ´‘", 350.0f, str_y + 1630.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	text->Drawtext(L"‚s‚g‚d‚d‚m‚c", 310.0f, str_y + 2500.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (str_y > -2200)str_y -= 0.4f;
}

Ending::~Ending(){

	MovieSoundManager::ObjDelete_ending();
}

