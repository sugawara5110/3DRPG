//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          DsProcessクラス                                   **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_DsProcess_Header
#define Class_DsProcess_Header

#include "Dx9Process.h"
#include <dshow.h>
#include <qedit.h>

class DsProcess{

protected:
	IGraphBuilder *pGraphBuilder;      //インターフェース,グラフへのフィルタの追加、2 つのピンの接続等
	IBaseFilter *pSampleGrabberFilter;//インターフェース,フィルタを制御(Movie使用時)
	ISampleGrabber *pSampleGrabber;  //インターフェース,フィルタ グラフ内を通る個々のメディア サンプルを取得(Movie使用時)
	IVideoWindow *pVideoWindow;     //インターフェース,ビデオ ウィンドウのプロパティを設定
	IMediaControl *pMediaControl;  //インターフェース,フィルタ グラフを通るデータ フローを制御
	IMediaPosition *pMediaPosition;  //インターフェース,ストリーム内の位置をシーク
	REFTIME time2;                   //動画の全再生時間
	REFTIME time1;                  //動画の現再生位置
	IBasicAudio *pBasicAudio;      //インターフェース,オーディオ ストリームのボリュームとバランスを制御

	DsProcess();

public:
	virtual ~DsProcess();
};

#endif