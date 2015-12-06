//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@  Sound_�N���X                                        **//
//**                                 sound�֐�                                           **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "MovieSoundManager.h"

MovieSoundManager::Sound_::Sound_(){}

MovieSoundManager::Sound_::Sound_(int no){

	char *fname = NULL;

	//��������
	switch (no){
		//���ʉ�
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
		//�^�C�g��
	case 20:
		fname = SoundBinaryDecode("./dat/movie/title.da");
		break;
		//�_���W����
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
		//�퓬
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

	// Graph�𐶐�
	pMediaControl->RenderFile(bstr);

	pVideoWindow->put_AutoShow(OAFALSE);  //�����\�����Ȃ��悤�ɂ���

	// �Đ��J�n
	pMediaControl->Run();

	pBasicAudio->put_Volume(-10000);//���ʒ���(-10000 �` 0)

	//�X�g���[���̎��ԕ����擾(�ŏ���1��擾�����ok)
	pMediaPosition->get_Duration(&time2);

	remove(fname);//�t�@�C���폜�A�����ɍ폜����Ȃ����g�p�����ƃA�N�Z�X�ł��Ȃ��݂���
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

void MovieSoundManager::Sound_::sound(bool repeat, long volume){

	pBasicAudio->put_Volume(volume);//����ON

	if (repeat == FALSE){
		pMediaPosition->put_CurrentPosition(0);//�Đ��ʒu���X�^�[�g�ʒu�ɃZ�b�g
	}
	else{
		//�X�g���[���̍��v���ԕ�����Ƃ���A���݂̈ʒu���擾����B
		pMediaPosition->get_CurrentPosition(&time1);

		//���ʒu�ƏI���ʒu�������ꍇ�X�^�[�g�ʒu�ɃZ�b�g
		if (time1 == time2)pMediaPosition->put_CurrentPosition(0);
	}
}

void MovieSoundManager::Sound_::soundoff(){
	pBasicAudio->put_Volume(-10000);//����OFF
}

void MovieSoundManager::Sound_::soundloop(bool repeat, long volume, REFTIME start, REFTIME end){

	REFTIME s = time2 * start / 100.0;
	REFTIME e = time2 * end / 100.0;

	pBasicAudio->put_Volume(volume);//����ON

	if (repeat == FALSE){
		pMediaPosition->put_CurrentPosition(s);//�Đ��ʒu���X�^�[�g�ʒu�ɃZ�b�g
	}
	else{
		//�X�g���[���̍��v���ԕ�����Ƃ���A���݂̈ʒu���擾����B
		pMediaPosition->get_CurrentPosition(&time1);

		//���ʒu�ƏI���ʒu�������ꍇ�X�^�[�g�ʒu�ɃZ�b�g
		if (time1 >= e)pMediaPosition->put_CurrentPosition(s);
	}
}