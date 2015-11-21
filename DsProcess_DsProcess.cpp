//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          DsProcessクラス                                   **//
//**                                   DsProcess関数                                     **//
//*****************************************************************************************//

#include "DsProcess.h"

DsProcess::DsProcess(){

	pGraphBuilder = NULL;
	pSampleGrabberFilter = NULL;
	pSampleGrabber = NULL;
	pVideoWindow = NULL;
	pMediaControl = NULL;
	pMediaPosition = NULL;
	pBasicAudio = NULL;

	// COMを初期化
	CoInitialize(NULL);

	// FilterGraphを生成
	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&pGraphBuilder);

	// MediaControlインターフェース取得
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&pMediaControl);

	// IVideoWindowインターフェースを取得します
	pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);

	//MediaPositionインターフェース取得
	pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);

	//BasicAudioインターフェース取得
	pGraphBuilder->QueryInterface(IID_IBasicAudio,
		(LPVOID *)&pBasicAudio);
}

DsProcess::~DsProcess(){

	pBasicAudio->Release();
	pBasicAudio = NULL;
	pMediaPosition->Release();
	pMediaPosition = NULL;
	pMediaControl->Release();
	pMediaControl = NULL;
	pVideoWindow->Release();
	pVideoWindow = NULL;
	//解放の順番の関係でここに記述
	if (pSampleGrabber != NULL){
		pSampleGrabber->Release();
		pSampleGrabber = NULL;
		pSampleGrabberFilter->Release();
		pSampleGrabberFilter = NULL;
	}
	pGraphBuilder->Release();
	pGraphBuilder = NULL;

	// COM終了
	CoUninitialize();
}
