//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　  Sound_クラス                                        **//
//**                                 sound関数                                           **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "MovieSoundManager.h"

MovieSoundManager::Sound_::Sound_(){}

MovieSoundManager::Sound_::Sound_(int no){

	char *fname = NULL;

	//復号処理
	switch (no){
		//効果音
	case 0:
		fname = SoundBinaryDecode("./dat/movie/att.da");
		break;
	case 1:
		fname = SoundBinaryDecode("./dat/movie/flame.da");
		break;
	case 2:
		fname = SoundBinaryDecode("./dat/movie/heal.da");
		break;
	case 3:
		fname = SoundBinaryDecode("./dat/movie/magic.da");
		break;
	case 4:
		fname = SoundBinaryDecode("./dat/movie/select.da");
		break;
	case 5:
		fname = SoundBinaryDecode("./dat/movie/enter.da");
		break;
		//タイトル
	case 20:
		fname = SoundBinaryDecode("./dat/movie/title.da");
		break;
		//ダンジョン
	case 21:
		fname = SoundBinaryDecode("./dat/movie/dungeon0.da");
		break;
	case 22:
		fname = SoundBinaryDecode("./dat/movie/dungeon1.da");
		break;
	case 23:
		fname = SoundBinaryDecode("./dat/movie/rain.da");
		break;
	case 24:
		fname = SoundBinaryDecode("./dat/movie/dungeon2.da");
		break;
	case 25:
		fname = SoundBinaryDecode("./dat/movie/dungeon3.da");
		break;
	case 26:
		fname = SoundBinaryDecode("./dat/movie/dungeon4.da");
		break;
		//戦闘
	case 30:
		fname = SoundBinaryDecode("./dat/movie/die.da");
		break;
	case 31:
		fname = SoundBinaryDecode("./dat/movie/side_enemy.da");
		break;
	case 32:
		fname = SoundBinaryDecode("./dat/movie/boss_enemy.da");
		break;
	case 33:
		fname = SoundBinaryDecode("./dat/movie/boss_enemy2.da");
		break;
	case 34:
		fname = SoundBinaryDecode("./dat/movie/lastboss.da");
		break;
	case 35:
		fname = SoundBinaryDecode("./dat/movie/ending.da");
		break;
	case 36:
		fname = SoundBinaryDecode("./dat/movie/bosslost.da");
		break;
	}

	LPSTR lstr = fname;
	BSTR bstr;
	//BSTRに必要なバッファサイズを求める(directshow用)
	int bstrlen = (int)MultiByteToWideChar(
		CP_ACP,		 // コードページ ANSI コードページ
		0,			// 文字の種類を指定するフラグ
		lstr,	   // マップ元文字列のアドレス
		strlen(lstr), // マップ元文字列のバイト数
		NULL,		 // マップ先ワイド文字列を入れるバッファのアドレス
		0			// バッファのサイズ
		);

	//バッファを確保する
	bstr = SysAllocStringLen(NULL, bstrlen);

	//BSTRに複製
	MultiByteToWideChar(
		CP_ACP,
		0,
		lstr,
		strlen(lstr),
		bstr,      //RenderFileの引数に使う
		bstrlen
		);

	// Graphを生成
	pMediaControl->RenderFile(bstr);

	pVideoWindow->put_AutoShow(OAFALSE);  //自動表示しないようにする

	// 再生開始
	pMediaControl->Run();

	pBasicAudio->put_Volume(-10000);//音量調整(-10000 ～ 0)

	//ストリームの時間幅を取得(最初に1回取得すればok)
	pMediaPosition->get_Duration(&time2);

	remove(fname);//ファイル削除、すぐに削除されないが使用中だとアクセスできないみたい
}

char *MovieSoundManager::Sound_::SoundBinaryDecode(char *bpass){

	FILE *fp;
	FILE *fp2;
	static char decfname[64];
	int size = 0;
	char *binary = NULL;

	int i1 = 0;
	do{
		strncpy(&decfname[i1], &bpass[i1], 1);
		i1++;
	} while (bpass[i1] != '.');
	strncpy(&decfname[i1++], ".", 1);
	strncpy(&decfname[i1++], "m", 1);
	strncpy(&decfname[i1++], "p", 1);
	strncpy(&decfname[i1++], "3", 1);
	strncpy(&decfname[i1], "\0", 1);

	fp = fopen(bpass, "rb");

	while (!feof(fp)){
		size++; fgetc(fp);
	}
	size++;//終端文字も含んだ個数

	//ポインタを先頭に戻す
	fseek(fp, 0, SEEK_SET);

	binary = (char*)malloc(sizeof(char) * size);

	for (int i = 99; i >= 0; i--){
		binary[i] = fgetc(fp);
	}
	for (int i = 100; i < size; i++){
		binary[i] = fgetc(fp);
	}

	fclose(fp);
	fp2 = fopen(decfname, "wb");

	for (int i = 0; i < size + 1; i++){
		fputc(binary[i], fp2);
	}

	fclose(fp2);
	free(binary);
	binary = NULL;

	return decfname;
}

void MovieSoundManager::Sound_::sound(bool repeat, long volume){

	pBasicAudio->put_Volume(volume);//音声ON

	if (repeat == FALSE){
		pMediaPosition->put_CurrentPosition(0);//再生位置をスタート位置にセット
	}
	else{
		//ストリームの合計時間幅を基準とする、現在の位置を取得する。
		pMediaPosition->get_CurrentPosition(&time1);

		//現位置と終了位置が同じ場合スタート位置にセット
		if (time1 == time2)pMediaPosition->put_CurrentPosition(0);
	}
}

void MovieSoundManager::Sound_::soundoff(){
	pBasicAudio->put_Volume(-10000);//音声OFF
}

void MovieSoundManager::Sound_::soundloop(bool repeat, long volume, REFTIME start, REFTIME end){

	REFTIME s = time2 * start / 100.0;
	REFTIME e = time2 * end / 100.0;

	pBasicAudio->put_Volume(volume);//音声ON

	if (repeat == FALSE){
		pMediaPosition->put_CurrentPosition(s);//再生位置をスタート位置にセット
	}
	else{
		//ストリームの合計時間幅を基準とする、現在の位置を取得する。
		pMediaPosition->get_CurrentPosition(&time1);

		//現位置と終了位置が同じ場合スタート位置にセット
		if (time1 >= e)pMediaPosition->put_CurrentPosition(s);
	}
}