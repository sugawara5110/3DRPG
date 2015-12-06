//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@           Movie�N���X                                      **//
//**                                    GetFrame�֐�                                     **//
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

	// SampleGrabber(Filter)�𐶐�
	CoCreateInstance(CLSID_SampleGrabber,
		NULL,
		CLSCTX_INPROC,
		IID_IBaseFilter,
		(LPVOID *)&pSampleGrabberFilter);

	// Filter����ISampleGrabber�C���^�[�t�F�[�X���擾���܂�
	pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber, (LPVOID *)&pSampleGrabber);

	// SampleGrabber��ڑ�����t�H�[�}�b�g���w��B
	// �����̎w��̎d���ɂ��SampleGrabber�̑}���ӏ���
	// ����ł��܂��B���̃T���v���̂悤�Ȏw��������
	// ��ʏo�͂̐��O�ŃT���v�����擾�ł��܂��B
	ZeroMemory(&am_media_type, sizeof(am_media_type));
	am_media_type.majortype = MEDIATYPE_Video;
	am_media_type.subtype = MEDIASUBTYPE_RGB24;
	am_media_type.formattype = FORMAT_VideoInfo;
	pSampleGrabber->SetMediaType(&am_media_type);

	// Graph��SampleGrabber Filter��ǉ�
	pGraphBuilder->AddFilter(pSampleGrabberFilter, L"Sample Grabber");

	char *fname = NULL;

	switch (no){
	case 0:
		fname = MovieBinaryDecode("./dat/movie/torch.da");//����
		break;

	case 1:
		fname = MovieBinaryDecode("./dat/movie/f_wall.da");//����
		break;
	}
	LPSTR lstr = fname;
	BSTR bstr;
	//BSTR�ɕK�v�ȃo�b�t�@�T�C�Y�����߂�(directshow�p)
	int bstrlen = (int)MultiByteToWideChar(
		CP_ACP,		 // �R�[�h�y�[�W ANSI �R�[�h�y�[�W
		0,			// �����̎�ނ��w�肷��t���O
		lstr,	   // �}�b�v��������̃A�h���X
		strlen(lstr), // �}�b�v��������̃o�C�g��
		NULL,		 // �}�b�v�惏�C�h�����������o�b�t�@�̃A�h���X
		0			// �o�b�t�@�̃T�C�Y
		);

	//�o�b�t�@���m�ۂ���
	bstr = SysAllocStringLen(NULL, bstrlen);

	//BSTR�ɕ���
	MultiByteToWideChar(
		CP_ACP,
		0,
		lstr,
		strlen(lstr),
		bstr,      //RenderFile�̈����Ɏg��
		bstrlen
		);

	// Graph�𐶐��B
	// ������SampleGrabber���܂�Graph�������I�ɍ쐬�����
	pMediaControl->RenderFile(bstr);

	// �ڑ����擾�B
	// ���̏�����RenderFile�ɂ��Graph���\�����ꂽ��Ɏ��s
	pSampleGrabber->GetConnectedMediaType(&am_media_type);
	pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;

	// Grab���sTRUE,���s���Ȃ�FALSE  
	// SetBufferSamples���s��Ȃ��ƃo�b�t�@����f�[�^���擾�ł��Ȃ��B
	pSampleGrabber->SetBufferSamples(TRUE);

	//�����\�����Ȃ��悤�ɂ���
	pVideoWindow->put_AutoShow(OAFALSE);

	// �Đ��J�n
	pMediaControl->Run();

	// �o�b�t�@��p��
	nBufferSize = am_media_type.lSampleSize;// �f�[�^�T�C�Y
	pBuffer = new BYTE[nBufferSize];

	//pBuffe x����1���C���T�C�Y�v�Z
	linesize = pVideoInfoHeader->bmiHeader.biWidth * 3;
	if (linesize % sizeof(LONG) != 0)
		linesize += sizeof(LONG) - (linesize % sizeof(LONG));

	//�X�g���[���̎��ԕ����擾(�ŏ���1��擾�����ok)
	pMediaPosition->get_Duration(&time2);

	remove(fname);//�t�@�C���폜�A�����ɍ폜����Ȃ����g�p�����ƃA�N�Z�X�ł��Ȃ��݂���
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
	size++;//�I�[�������܂񂾌�

	//�|�C���^��擪�ɖ߂�
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

	//�Đ����x1.0�W�� �L���͈�0.1�`4.0 
	pMediaPosition->put_Rate(1.0);

	//�X�g���[���̍��v���ԕ�����Ƃ���A���݂̈ʒu���擾����B
	pMediaPosition->get_CurrentPosition(&time1);

	//���ʒu�ƏI���ʒu�������ꍇ�X�^�[�g�ʒu�ɃZ�b�g
	if (time1 == time2)pMediaPosition->put_CurrentPosition(0);

	//�ŐV�摜�f�[�^��pBuffer�Ɋi�[
	pSampleGrabber->GetCurrentBuffer(&nBufferSize, (long *)(pBuffer));

	if (m_pix == NULL){
		m_pix = (int**)malloc(sizeof(int*) * hei);
		for (int i = 0; i < hei; i++){
			m_pix[i] = (int*)malloc(sizeof(int) * wid);
		}
	}

	//�t���[���T�C�Y
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
		free(pBuffer);//���������
		pBuffer = NULL;
	}
}