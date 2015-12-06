//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　           Movieクラス                                      **//
//**                                    GetFrame関数                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "MovieSoundManager.h"

MovieSoundManager::Movie::Movie(){}

MovieSoundManager::Movie::Movie(int no){

	pVideoInfoHeader = NULL;
	nBufferSize = NULL;
	pBuffer = NULL;
	linesize = NULL;
	m_pix = NULL;

	// SampleGrabber(Filter)を生成
	CoCreateInstance(CLSID_SampleGrabber,
		NULL,
		CLSCTX_INPROC,
		IID_IBaseFilter,
		(LPVOID *)&pSampleGrabberFilter);

	// FilterからISampleGrabberインターフェースを取得します
	pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber, (LPVOID *)&pSampleGrabber);

	// SampleGrabberを接続するフォーマットを指定。
	// ここの指定の仕方によりSampleGrabberの挿入箇所を
	// 決定できます。このサンプルのような指定をすると
	// 画面出力の寸前でサンプルを取得できます。
	ZeroMemory(&am_media_type, sizeof(am_media_type));
	am_media_type.majortype = MEDIATYPE_Video;
	am_media_type.subtype = MEDIASUBTYPE_RGB24;
	am_media_type.formattype = FORMAT_VideoInfo;
	pSampleGrabber->SetMediaType(&am_media_type);

	// GraphにSampleGrabber Filterを追加
	pGraphBuilder->AddFilter(pSampleGrabberFilter, L"Sample Grabber");

	char *fname = NULL;

	switch (no){
	case 0:
		fname = MovieBinaryDecode("./dat/movie/torch.da");//松明
		break;

	case 1:
		fname = MovieBinaryDecode("./dat/movie/f_wall.da");//炎壁
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

	// Graphを生成。
	// ここでSampleGrabberを含んだGraphが自動的に作成される
	pMediaControl->RenderFile(bstr);

	// 接続情報取得。
	// この処理はRenderFileによりGraphが構成された後に実行
	pSampleGrabber->GetConnectedMediaType(&am_media_type);
	pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;

	// Grab実行TRUE,実行しないFALSE  
	// SetBufferSamplesを行わないとバッファからデータを取得できない。
	pSampleGrabber->SetBufferSamples(TRUE);

	//自動表示しないようにする
	pVideoWindow->put_AutoShow(OAFALSE);

	// 再生開始
	pMediaControl->Run();

	// バッファを用意
	nBufferSize = am_media_type.lSampleSize;// データサイズ
	pBuffer = new BYTE[nBufferSize];

	//pBuffe x方向1ラインサイズ計算
	linesize = pVideoInfoHeader->bmiHeader.biWidth * 3;
	if (linesize % sizeof(LONG) != 0)
		linesize += sizeof(LONG) - (linesize % sizeof(LONG));

	//ストリームの時間幅を取得(最初に1回取得すればok)
	pMediaPosition->get_Duration(&time2);

	remove(fname);//ファイル削除、すぐに削除されないが使用中だとアクセスできないみたい
}

char *MovieSoundManager::Movie::MovieBinaryDecode(char *bpass){

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
	strncpy(&decfname[i1++], "g", 1);
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

int **MovieSoundManager::Movie::GetFrame(int width, int height){

	hei = height;
	wid = width;

	//再生速度1.0標準 有効範囲0.1～4.0 
	pMediaPosition->put_Rate(1.0);

	//ストリームの合計時間幅を基準とする、現在の位置を取得する。
	pMediaPosition->get_CurrentPosition(&time1);

	//現位置と終了位置が同じ場合スタート位置にセット
	if (time1 == time2)pMediaPosition->put_CurrentPosition(0);

	//最新画像データをpBufferに格納
	pSampleGrabber->GetCurrentBuffer(&nBufferSize, (long *)(pBuffer));

	if (m_pix == NULL){
		m_pix = (int**)malloc(sizeof(int*) * hei);
		for (int i = 0; i < hei; i++){
			m_pix[i] = (int*)malloc(sizeof(int) * wid);
		}
	}

	//フレームサイズ
	xs = pVideoInfoHeader->bmiHeader.biWidth;
	ys = pVideoInfoHeader->bmiHeader.biHeight;
	for (int j = 0; j < hei; j++){
		for (int i = 0; i < wid; i++){
			int offset = (j * ys / hei) * linesize + (i * xs / wid) * 3;

			m_pix[hei - j - 1][i] =
				(pBuffer[offset + 0] << 16) +
				(pBuffer[offset + 1] << 8) +
				pBuffer[offset + 2];
		}
	}
	return m_pix;
}

MovieSoundManager::Movie::~Movie(){

	if (m_pix != NULL){
		for (int i = 0; i < hei; i++)free(m_pix[i]);
		free(m_pix);
		m_pix = NULL;
	}

	if (pBuffer != NULL){
		free(pBuffer);//メモリ解放
		pBuffer = NULL;
	}
}