//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          DsProcess�N���X                                   **//
//**                                   DsProcess�֐�                                     **//
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

	// COM��������
	CoInitialize(NULL);

	// FilterGraph�𐶐�
	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&pGraphBuilder);

	// MediaControl�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&pMediaControl);

	// IVideoWindow�C���^�[�t�F�[�X���擾���܂�
	pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);

	//MediaPosition�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);

	//BasicAudio�C���^�[�t�F�[�X�擾
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
	//����̏��Ԃ̊֌W�ł����ɋL�q
	if (pSampleGrabber != NULL){
		pSampleGrabber->Release();
		pSampleGrabber = NULL;
		pSampleGrabberFilter->Release();
		pSampleGrabberFilter = NULL;
	}
	pGraphBuilder->Release();
	pGraphBuilder = NULL;

	// COM�I��
	CoUninitialize();
}
