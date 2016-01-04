//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　    NowLoading                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include "Dx11Process.h"
#include "DxText.h"

unsigned __stdcall NowLoading(void *loop){

	Dx11Process *dx = Dx11Process::GetInstance();
	bool *th_loop = (bool*)loop;
	double i = 300.0;
	bool down = TRUE;

	while (*th_loop == TRUE){
		dx->Sclear();
		DxText::GetInstance()->Drawtext(L"ＮｏｗＬｏａｄｉｎｇ", 215.0f, (float)i, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
		dx->Drawscreen();
		if (down == TRUE)i += 0.01;
		if (down == FALSE)i -= 0.01;
		if (i > 320.0)down = FALSE;
		if (i < 280.0)down = TRUE;
	}
	return 0;
}