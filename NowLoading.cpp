//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　    NowLoading                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include "Dx9Process.h"

unsigned __stdcall NowLoading(void *loop){

	Dx9Process *dx = Dx9Process::GetInstance();
	bool *th_loop = (bool*)loop;
	int i = 300;
	bool down = TRUE;

	while (*th_loop == TRUE){
		dx->Sclear();
		dx->text("Now Loading", 300, i, FALSE, 0xffffffff);
		dx->Drawscreen();
		if (down == TRUE)i++;
		if (down == FALSE)i--;
		if (i > 320)down = FALSE;
		if (i < 280)down = TRUE;
	}
	return 0;
}