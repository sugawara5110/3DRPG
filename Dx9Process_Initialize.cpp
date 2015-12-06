//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx9Process�N���X                                  **//
//**                                    Initialize�֐�                                   **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "Dx9Process.h"
#include <string.h>
#include <stdio.h>

// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g�̐ݒ�
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define MY_VERTEX_FVF2 ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

Dx9Process *Dx9Process::dx = NULL;

void Dx9Process::InstanceCreate(){

	if (dx == NULL)dx = new Dx9Process();
}

Dx9Process *Dx9Process::GetInstance(){

	if (dx != NULL)return dx;
	return NULL;
}

void Dx9Process::DeleteInstance(){

	if (dx != NULL){
		delete dx;
		dx = NULL;
	}
}

Dx9Process::Dx9Process(){}
Dx9Process::Dx9Process(const Dx9Process &obj) {}      // �R�s�[�R���X�g���N�^�֎~
void Dx9Process::operator=(const Dx9Process& obj) {} // ������Z�q�֎~

HRESULT Dx9Process::Initialize(HWND hWnd){//DirectX9������

	// Direct3D9�I�u�W�F�N�g�̍쐬
	if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == 0){
		return E_FAIL;	// �擾���s
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��������
	ZeroMemory(&D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	D3DPP.BackBufferCount = 1;
	D3DPP.Windowed = TRUE;				         // �E�C���h�E���\���̎w��
	D3DPP.BackBufferFormat = d3ddm.Format;	// �J���[���[�h�̎w��
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;// 
	D3DPP.EnableAutoDepthStencil = TRUE;	  //Z�o�b�t�@
	D3DPP.AutoDepthStencilFormat = D3DFMT_D16;//Z�o�b�t�@�̃t�H�[�}�b�g

	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&D3DPP, &pD3DDevice))) {
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&D3DPP, &pD3DDevice))) {
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&D3DPP, &pD3DDevice))) {
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �t�H���g�̐���1
	int fontsize = 15;
	D3DXFONT_DESC lf = { fontsize, 0, 0, 1, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, ("�l�r �S�V�b�N") };
	// ID3DXFont�R���|�[�l���g����1
	if (FAILED(D3DXCreateFontIndirect(pD3DDevice, &lf, &pD3DFont_s))){
		pD3DDevice->Release();  pD3D->Release();
		return E_FAIL;
	}
	// �t�H���g�̐���2
	fontsize = 25;
	lf = { fontsize, 0, 0, 1, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, ("�l�r �S�V�b�N") };
	// ID3DXFont�R���|�[�l���g����2
	if (FAILED(D3DXCreateFontIndirect(pD3DDevice, &lf, &pD3DFont_l))){
		pD3DDevice->Release();  pD3D->Release();
		return E_FAIL;
	}

	//�����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	//���ʕ`�惂�[�h�̎w��
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//�y��r���s�Ȃ�(������Ȃ��ƕ\�����Ԃ��������Ȃ�)
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//�f�B�U�����O���s�Ȃ��i���i���`��j
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	//�A���t�@�u�����f�B���O
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//�]�����̃A���t�@����Z
	//pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//�]�����̃A���t�@�𔽓]�������̂���Z
	//�A���t�@�e�X�g�̗L����(����������\�����Ȃ�)
	pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);//�A���t�@�l��̃s�N�Z���̎��L���̋���,�V�����s�N�Z���l���A���݂̃s�N�Z���l�ȏ�̂Ƃ��ɉ�����B
	//�s�����ɂ���l�̐ݒ�
	pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x66);
	//�e�N�X�`���X�e�[�W�̐ݒ�
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); //�X�e�[�W1,�e�N�X�`���̃J���[
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);//�X�e�[�W2,�f�B�t�F�[�Y�F	
	//�_�T�C�Y
	float PointSize = 1.0f;//�L���X�g���Ďg��
	pD3DDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&PointSize));

	//���C�g�\����0�N���A
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	//�}�e���A���\����0�N���A
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	//�}�e���A���F�ݒ�
	material.Diffuse.b = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.r = 1.0f;
	material.Diffuse.a = 1.0f;
	material.Ambient.b = 0.4f;
	material.Ambient.g = 0.4f;
	material.Ambient.r = 0.4f;
	material.Ambient.a = 0.4f;
	//�}�e���A���Z�b�g
	pD3DDevice->SetMaterial(&material);

	//�e�}�g���b�N�X������
	D3DXMatrixIdentity(&m_view);

	D3DXMatrixIdentity(&m_proj);

	D3DXMatrixIdentity(&m_world);

	// �r���[�|�[�g�̎擾
	pD3DDevice->GetViewport(&vp);

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// �ˉe�}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian(55.0f),	// �J�����̉�p
		aspect,				   // �A�X�y�N�g��
		1.0f,		          // near�v���[��
		10000.0f);			 // far�v���[��

	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);
	pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9) * 100);
	binary_ch = (char**)malloc(sizeof(char*) * 100);
	binary_size = (int*)malloc(sizeof(int) * 100);
	for (int i = 0; i < 100; i++){
		pTexture[i] = NULL;
		binary_ch[i] = NULL;
		binary_size[i] = 0;
	}
	return S_OK;
}

HRESULT Dx9Process::GetVBarray(PrimitiveType type, PolygonData *pd, int pieces){//���_�o�b�t�@,�z��m��

	int ver;
	int verI;
	if (type == SQUARE){
		ver = pieces * 4;//pieces==�l�p�`�̌�
		verI = pieces * 6;
	}
	if (type == POINt){
		ver = pieces;//pieces==�_�̌�
		verI = pieces;
	}
	if (type == LINE_S || type == LINE_L){
		ver = pieces * 2;//pieces==���̌�
		verI = pieces * 2;
	}
	if (FAILED(pD3DDevice->CreateVertexBuffer(
		sizeof(MY_VERTEX) * ver,// ���_�o�b�t�@�S�̂̃T�C�Y
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g����
		MY_VERTEX_FVF,		   // ���_�f�[�^��FVF�t�H�[�}�b�g
		D3DPOOL_MANAGED, 	  // �V�X�e���̒��_�o�b�t�@������
		&pd->pMyVB, 	     // ���_�o�b�t�@���i�[����ϐ�
		NULL)))             // ���NULL�Ɏw��
	{
		return E_FAIL;
	}

	if (FAILED(pD3DDevice->CreateIndexBuffer(
		sizeof(DWORD) * verI,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&pd->pMyVBI,
		NULL)))
	{
		return E_FAIL;
	}

	pd->d3varray = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * ver);//���_�z��
	pd->d3varrayI = (DWORD*)malloc(sizeof(DWORD) * verI);     //���_�C���f�b�N�X�z��
	return S_OK;
}

HRESULT Dx9Process::Sclear(){//�X�N���[���N���A

	// �r���[�|�[�g�Ɛ[�x�o�b�t�@�̃N���A�ƃX�e���V���o�b�t�@�̍폜
	if (FAILED(pD3DDevice->Clear(0, NULL,   // �N���A����̈�͑S��
		D3DCLEAR_TARGET |			// �o�b�N�o�b�t�@���w��
		D3DCLEAR_ZBUFFER, 		   // �[�x�o�b�t�@�iZ�o�b�t�@�j���w��
		D3DCOLOR_XRGB(0, 0, 255), // �N���A����F
		1.0f, 					 // �N���A����[�x�o�b�t�@�iZ�o�b�t�@�j�̒l
		0)))				    // �N���A����X�e���V���o�b�t�@�̒l
	{
		return E_FAIL;
	}
	return S_OK;
}

void Dx9Process::Cameraset(float cax1, float cax2, float cay1, float cay2, float caz){//�J�����Z�b�g

	// �J�����̈ʒu�ƕ�����ݒ�
	D3DXMatrixLookAtLH(&m_view,
		&D3DXVECTOR3(cax1, cay1, caz),   //�J�����̈ʒu
		&D3DXVECTOR3(cax2, cay2, caz),  //�J�����̕�����������_
		&D3DXVECTOR3(0.0f, 0.0f, 1.0f)); //�J�����̏�̕���(�ʏ펋�_�ł̏������1.0f�ɂ���)
}

void Dx9Process::LightPosSet(int Idx, float x, float y, float z, float range, float attenuation){

	light.Type = D3DLIGHT_POINT;
	//���C�g�F�ݒ�
	light.Diffuse.b = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.r = 1.0f;
	light.Ambient.b = 0.4f;
	light.Ambient.g = 0.4f;
	light.Ambient.r = 0.4f;

	//���C�g�̋��x
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = attenuation;
	light.Attenuation2 = 0.0f;
	//���C�g�̃|�W�V����
	light.Position.x = x;
	light.Position.y = y;
	light.Position.z = z;
	//���C�g�����W
	light.Range = range;
	pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x20202020);
	pD3DDevice->SetLight(Idx, &light);//���������C�g�ԍ�
	pD3DDevice->LightEnable(Idx, TRUE);//���������C�g�ԍ�
}

void Dx9Process::TextureInit(PolygonData *pd, int width, int height){

	//����e�N�X�`���g�p���J���e�N�X�`������
	pD3DDevice->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pd->pTexture,
		NULL
		);
}

void Dx9Process::TextureBinaryDecode(char *Bpass, int i){

	//�Í������R�[�h��
	/*
	FILE *fp;
	FILE *fp2;
	char *binary;
	char *binary_change;
	int size;
	char fname[64];
	char fname_out[64];
	int i1;
	int len;
	int enemy_no = 0;
	int boss_no = 0;
	int bk = 0;

	while (1){
	scanf_s("%s", fname, 64);
	len = strlen(fname);
	if (len == 0 || len > 64)break;
	i1 = 0;
	do{
	strncpy(&fname_out[i1], &fname[i1], 1);
	i1++;
	} while (fname[i1] != '.');
	strncpy(&fname_out[i1++], ".", 1);
	strncpy(&fname_out[i1++], "d", 1);
	strncpy(&fname_out[i1++], "a", 1);
	strncpy(&fname_out[i1], "\0", 1);

	i1 = 0;
	if (strncmp(fname, "enemy.", 6) == 0)enemy_no = 1;
	if (strncmp(fname, "boss.", 5) == 0)boss_no = 1;

	do{
	if (enemy_no >= 1){
	sprintf(fname, "enemy%d.png", enemy_no);
	sprintf(fname_out, "enemy%d.da", enemy_no);
	}
	if (boss_no >= 1){
	sprintf(fname, "boss%d.png", boss_no);
	sprintf(fname_out, "boss%d.da", boss_no);
	}
	fp = NULL;
	fp2 = NULL;
	binary = NULL;
	binary_change = NULL;
	size = 0;

	fp = fopen(fname, "rb");
	if (fp == NULL){
	bk = 1; break;
	}

	while (!feof(fp)){
	size++; fgetc(fp);
	}
	size++;//�I�[�����܂߂���

	//�|�C���^��擪�ɖ߂�
	fseek(fp, 0, SEEK_SET);

	binary = (char*)malloc(sizeof(char) * size);
	binary_change = (char*)malloc(sizeof(char) * size);

	for (int i = 0; i < size; i++){
	binary[i] = fgetc(fp);//�I�[�����܂œǂݍ���
	}

	for (int i = 0; i < 100; i++){
	strncpy(&binary_change[i], &binary[99 - i], 1);
	}
	for (int i = 100; i < size; i++){
	strncpy(&binary_change[i], &binary[i], 1);
	}

	fp2 = fopen(fname_out, "wb");

	fwrite(binary_change, size, 1, fp2);

	free(binary);
	free(binary_change);
	fclose(fp);
	fclose(fp2);
	if (enemy_no >= 1)enemy_no++;
	if (boss_no >= 1)boss_no++;
	} while (enemy_no >= 1 || boss_no >= 1);
	enemy_no = boss_no = 0;
	if (bk == 1)break;
	}
	*/

	//�����R�[�h��
	FILE *fp;
	fp = fopen(Bpass, "rb");

	while (!feof(fp)){
		binary_size[i]++; fgetc(fp);
	}
	binary_size[i]++;//�I�[�����܂߂���

	//�|�C���^��擪�ɖ߂�
	fseek(fp, 0, SEEK_SET);

	binary_ch[i] = (char*)malloc(sizeof(char) * binary_size[i]);

	for (int j = 99; j >= 0; j--){
		binary_ch[i][j] = fgetc(fp);
	}

	for (int j = 100; j < binary_size[i]; j++){
		binary_ch[i][j] = fgetc(fp);
	}

	fclose(fp);
}

void Dx9Process::TextureBinaryDecodeAll(){
	//�}�b�v0
	TextureBinaryDecode("./dat/texture/map/wall1.da", 0);
	TextureBinaryDecode("./dat/texture/map/ground1.da", 1);
	TextureBinaryDecode("./dat/texture/map/ceiling1.da", 2);
	TextureBinaryDecode("./dat/texture/map/EXIT1.da", 3);
	//�}�b�v1
	TextureBinaryDecode("./dat/texture/map/wall2.da", 4);
	TextureBinaryDecode("./dat/texture/map/ground2.da", 5);
	TextureBinaryDecode("./dat/texture/map/ceiling2.da", 6);
	TextureBinaryDecode("./dat/texture/map/EXIT2.da", 7);
	TextureBinaryDecode("./dat/texture/map/wall2-1.da", 8);
	TextureBinaryDecode("./dat/texture/map/ENTER2.da", 9);
	TextureBinaryDecode("./dat/texture/map/background.da", 10);
	//�}�b�v2
	TextureBinaryDecode("./dat/texture/map/ceiling3_wall3.da", 11);
	TextureBinaryDecode("./dat/texture/map/ground3.da", 12);
	TextureBinaryDecode("./dat/texture/map/ENTER3.da", 13);
	TextureBinaryDecode("./dat/texture/map/EXIT3.da", 14);
	//�}�b�v3
	TextureBinaryDecode("./dat/texture/map/ceiling4_ground4.da", 15);
	TextureBinaryDecode("./dat/texture/map/ENTER4.da", 16);
	//�}�b�v4
	TextureBinaryDecode("./dat/texture/map/wall5.da", 26);
	TextureBinaryDecode("./dat/texture/map/ground5.da", 27);
	TextureBinaryDecode("./dat/texture/map/ceiling5.da", 28);
	TextureBinaryDecode("./dat/texture/map/ENTER5.da", 29);
	//�ʏ�G
	TextureBinaryDecode("./dat/texture/enemy/enemy1.da", 30);
	TextureBinaryDecode("./dat/texture/enemy/enemy2.da", 31);
	TextureBinaryDecode("./dat/texture/enemy/enemy3.da", 32);
	TextureBinaryDecode("./dat/texture/enemy/enemy4.da", 33);
	TextureBinaryDecode("./dat/texture/enemy/enemy5.da", 34);
	TextureBinaryDecode("./dat/texture/enemy/enemy6.da", 35);
	TextureBinaryDecode("./dat/texture/enemy/enemy7.da", 36);
	TextureBinaryDecode("./dat/texture/enemy/enemy8.da", 37);
	TextureBinaryDecode("./dat/texture/enemy/enemy9.da", 38);
	TextureBinaryDecode("./dat/texture/enemy/enemy10.da", 39);
	TextureBinaryDecode("./dat/texture/enemy/enemy11.da", 40);
	TextureBinaryDecode("./dat/texture/enemy/enemy12.da", 41);
	TextureBinaryDecode("./dat/texture/enemy/enemy13.da", 42);
	TextureBinaryDecode("./dat/texture/enemy/enemy14.da", 43);
	TextureBinaryDecode("./dat/texture/enemy/enemy15.da", 44);
	TextureBinaryDecode("./dat/texture/enemy/enemy16.da", 45);
	TextureBinaryDecode("./dat/texture/enemy/enemy17.da", 46);
	TextureBinaryDecode("./dat/texture/enemy/enemy18.da", 47);
	TextureBinaryDecode("./dat/texture/enemy/enemy19.da", 48);
	TextureBinaryDecode("./dat/texture/enemy/enemy20.da", 49);
	//�{�X
	TextureBinaryDecode("./dat/texture/enemy/boss1.da", 50);
	TextureBinaryDecode("./dat/texture/enemy/boss2.da", 51);
	TextureBinaryDecode("./dat/texture/enemy/boss3.da", 52);
	TextureBinaryDecode("./dat/texture/enemy/boss4.da", 53);
	//���X�g�{�X
	TextureBinaryDecode("./dat/texture/enemy/lastboss.da", 59);
	//�����w�ʏ�
	TextureBinaryDecode("./dat/texture/magic/side_magic.da", 60);
	//�����w�{�X
	TextureBinaryDecode("./dat/texture/magic/boss_magic.da", 61);
	//�񕜃|�C���g
	TextureBinaryDecode("./dat/texture/magic/recover.da", 70);
	//�G�t�F�N�g
	TextureBinaryDecode("./dat/texture/effect/e_att.da", 80);
	TextureBinaryDecode("./dat/texture/effect/h_att.da", 81);
	TextureBinaryDecode("./dat/texture/effect/flame.da", 82);
	TextureBinaryDecode("./dat/texture/effect/healing.da", 83);
	TextureBinaryDecode("./dat/texture/effect/recov.da", 84);
}

void Dx9Process::GetTexture(){
	for (int i = 0; i < 100; i++){
		if (binary_size[i] == 0)continue;
		D3DXCreateTextureFromFileInMemory(pD3DDevice, binary_ch[i], binary_size[i], &pTexture[i]);
	}
	for (int i = 0; i < 100; i++){
		if (binary_ch[i] == NULL)continue;
		free(binary_ch[i]);
		binary_ch[i] = NULL;
	}
	free(binary_ch);
	free(binary_size);
}

void Dx9Process::GetTexture(PolygonData *pd, int no){

	pd->load_tex_no[pd->tex_no] = no;
}

void Dx9Process::GetTexturePixelArray(PolygonData *pd, T_xyz *p_arr, int size_x, int size_y, int z){

	D3DSURFACE_DESC desc;//�T�[�t�F�C�X
	D3DLOCKED_RECT  rect;//�T�[�t�F�C�X�̃s�b�`,�r�b�g�ւ̃|�C���^ 

	//�摜���̎擾
	pTexture[pd->load_tex_no[pd->tex_no]]->GetLevelDesc(0, &desc);
	//�e�N�X�`�������b�N����
	pTexture[pd->load_tex_no[pd->tex_no]]->LockRect(0, &rect, NULL, 0);
	//�e�N�X�`���̉��̃s�N�Z�����̌v�Z
	int width = rect.Pitch / 4;
	//�e�N�X�`���̏c�̃s�N�Z����
	int height = desc.Height;

	BYTE *pPixel;
	//���摜�𒲐�size�~size�ɂ�z������z�R�s�[
	for (int k = 0; k < z; k++){
		for (int j = 0; j < size_y; j++){
			for (int i = 0; i < size_x; i++){
				pPixel = ((BYTE*)rect.pBits) + ((int)(i * (float)width / size_x) * 4) + ((int)(j * (float)height / size_y) * rect.Pitch);
				int ind = k * size_x * size_y + j * size_x + i;
				p_arr[ind].color = ((int)(*(pPixel + 0)) << 24) + ((int)(*(pPixel + 1)) << 16) + (int)(*(pPixel + 2) << 8) + (int)(*(pPixel + 3));
				p_arr[ind].x = (float)i / 5;
				p_arr[ind].y = (float)j / 5;
				p_arr[ind].z = (float)k / 5;
			}
		}
	}
	pTexture[pd->load_tex_no[pd->tex_no]]->UnlockRect(0);
}

void Dx9Process::SetTextureMPixel(PolygonData *pd, int **m_pix, BYTE r, BYTE g, BYTE b, int a){

	D3DSURFACE_DESC desc;//�T�[�t�F�C�X
	D3DLOCKED_RECT  rect;//�T�[�t�F�C�X�̃s�b�`,�r�b�g�ւ̃|�C���^ 

	//�摜���̎擾
	pd->pTexture->GetLevelDesc(0, &desc);
	//�e�N�X�`�������b�N����
	pd->pTexture->LockRect(0, &rect, NULL, 0);
	//�e�N�X�`���̉��̃s�N�Z�����̌v�Z
	int width = rect.Pitch / 4;
	//�e�N�X�`���̏c�̃s�N�Z����
	int height = desc.Height;

	BYTE *pPixel;
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			pPixel = ((BYTE*)rect.pBits) + (i * 4) + (j * rect.Pitch);
			*(pPixel + 0) = m_pix[j][i] >> 16 & r;
			*(pPixel + 1) = m_pix[j][i] >> 8 & g;
			*(pPixel + 2) = m_pix[j][i] & b;

			if ((m_pix[j][i] >> 16 & b) < 20 && (m_pix[j][i] >> 8 & g) < 20 && (m_pix[j][i] & r) < 20){
				*(pPixel + 3) = 0;
			}
			else{
				*(pPixel + 3) = a;
			}
		}
	}
	pd->pTexture->UnlockRect(0);
}

void Dx9Process::SetFog(bool on, float StartPos, float EndPos, DWORD r, DWORD g, DWORD b){

	pD3DDevice->SetRenderState(D3DRS_FOGENABLE, on); //�t�H�O�FON
	if (on == FALSE)return;
	pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0x00, r, g, b)); //�t�H�O�F
	pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);      //���_���[�h
	pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);     //�e�[�u�����[�h
	pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos)); //�J�n�ʒu
	pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));     //�I���ʒu
}

void Dx9Process::D3primitive(PrimitiveType type, PolygonData *pd, int pieces, float x, float y, float z, float r, bool a, bool lock,bool light_f){//3D�`��

	D3DPRIMITIVETYPE primitive;
	D3DXMATRIX mov;
	D3DXMATRIX rot;

	int ver;
	int verI;
	int triangle;
	if (type == SQUARE){
		primitive = D3DPT_TRIANGLELIST;
		ver = pieces * 4;
		verI = pieces * 6;
		triangle = pieces * 2;//�O�p�`�̐�
	}
	if (type == POINt){
		primitive = D3DPT_POINTLIST;
		ver = pieces;
		verI = pieces;
		triangle = pieces;//���_�̐�
	}
	if (type == LINE_S){
		primitive = D3DPT_LINESTRIP;
		ver = pieces * 2;
		verI = pieces * 2;
		triangle = pieces * 2;//���_�̐�
	}
	if (type == LINE_L){
		primitive = D3DPT_LINELIST;
		ver = pieces * 2;
		verI = pieces * 2;
		triangle = pieces * 2;//���_�̐�
	}
	//���_�f�[�^�ݒ�(lock==FALSE:�ݒ�s�v,pd->lock==FALSE�̏ꍇ�͂܂����ݒ�̈׎��s����)
	if (lock == TRUE || pd->lock == FALSE){
		MY_VERTEX *d3vcopy;
		DWORD     *d3vcopyI;
		pd->pMyVB->Lock(0, 0, (void**)&d3vcopy, 0);
		memcpy(d3vcopy, pd->d3varray, sizeof(MY_VERTEX) * ver);
		pd->pMyVB->Unlock();
		pd->pMyVBI->Lock(0, 0, (void**)&d3vcopyI, 0);
		memcpy(d3vcopyI, pd->d3varrayI, sizeof(DWORD) * verI);
		pd->pMyVBI->Unlock();
		pd->lock = TRUE;
	}

	pD3DDevice->BeginScene();// �`��J�n�錾

	// ���_�o�b�t�@�[�������_�����O�p�C�v���C���ɐݒ�
	pD3DDevice->SetStreamSource(0, pd->pMyVB, 0, sizeof(MY_VERTEX));
	pD3DDevice->SetIndices(pd->pMyVBI);

	//�\���ʒu
	D3DXMatrixRotationZ(&rot, D3DXToRadian(r));
	D3DXMatrixTranslation(&mov, x, y, z);

	D3DXMatrixMultiply(&m_world, &rot, &mov);

	//�}�g���b�N�X�������_�����O�p�C�v���C���ɐݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

	//�A���t�@�u�����f�B���O
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, a);

	//�A���t�@�e�X�g�̗L����(����������\�����Ȃ�)
	pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, a);

	//���C�e�B���O���[�h 
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, light_f);                     

	switch (a){
	case TRUE:
		//�e�N�X�`���[�Z�b�g
		if (pd->pTexture == NULL){
			//�Î~��e�N�X�`���g�p
			pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//�X�e�[�W1,2�̊|�����킹
			pD3DDevice->SetTexture(0, pTexture[pd->load_tex_no[pd->tex_no]]);
		}
		else{
			//����e�N�X�`���g�p
			pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);//�X�e�[�W1�����̂܂܏o��
			pD3DDevice->SetTexture(0, pd->pTexture);
		}
		break;

	case FALSE:
		//�|���S���F�̂�
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);//�X�e�[�W2�����̂܂܏o��
		break;
	}
	//���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	pD3DDevice->SetFVF(MY_VERTEX_FVF);

	//�`��
	pD3DDevice->DrawIndexedPrimitive(primitive, 0, 0, verI, 0, triangle);

	pD3DDevice->EndScene();//�`��I���錾
}

void Dx9Process::D2primitive(int square, MY_VERTEX2 *d2varray){//2D�`��

	int triangle = square * 2;
	pD3DDevice->BeginScene();// �`��J�n�錾

	//�m�[���C�e�B���O���[�h 	
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�u�����f�B���O
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//�A���t�@�e�X�g�̗L����(����������\�����Ȃ�)
	pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);//�X�e�[�W2�����̂܂܏o��

	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// �`��
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, triangle, d2varray, sizeof(MY_VERTEX2));
	pD3DDevice->EndScene();// �`��I���錾
}

void Dx9Process::text(char str[30], int x, int y, bool size_s, DWORD cl){//�e�L�X�g�`��

	pD3DDevice->BeginScene();
	RECT r = { x, y, x, y };
	if (size_s == TRUE)
		pD3DFont_s->DrawText(NULL, str, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, cl);
	else
		pD3DFont_l->DrawText(NULL, str, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, cl);
	pD3DDevice->EndScene();
}

void  Dx9Process::Drawscreen(){//�X�N���[���`��

	if (FAILED(pD3DDevice->Present(0, 0, 0, 0))) {//�`�挋�ʂ̓]��
		// �f�o�C�X�������畜�A
		pD3DDevice->Reset(&D3DPP);
	}
}

Dx9Process::~Dx9Process(){

	for (int i = 0; i < 100; i++){
		if (pTexture[i] == NULL)continue;
		pTexture[i]->Release();
		pTexture[i] = NULL;
	}

	pD3DFont_s->Release();     //ID3DXFont�R���|�[�l���g
	pD3DFont_s = NULL;
	pD3DFont_l->Release();   //ID3DXFont�R���|�[�l���g
	pD3DFont_l = NULL;
	pD3DDevice->Release(); //IDirect3DDevice9�C���^�[�t�F�C�X
	pD3DDevice = NULL;
	pD3D->Release();     //IDirect3D9�C���^�[�t�F�C�X
	pD3D = NULL;
}

Dx9Process::PolygonData::PolygonData(){
	tex_no = 0;
	pTexture = NULL;
	pMyVB = NULL;
	lock = FALSE;//���b�N,���_�ݒ�O
}

Dx9Process::PolygonData::~PolygonData(){
	
	if (pMyVB != NULL){
		pMyVB->Release();
		pMyVB = NULL;
		pMyVBI->Release();
		pMyVBI = NULL;
		free(d3varray);
		d3varray = NULL;
		free(d3varrayI);
		d3varrayI = NULL;
	}
	if (pTexture != NULL){
		pTexture->Release();
		pTexture = NULL;
	}
}

void Dx9Process::PolygonData::SetVertex(int I1, int I2, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	int r, int g, int b,
	float u, float v){

	d3varrayI[I1] = i;
	d3varrayI[I2] = i;
	d3varray[i].x = vx;
	d3varray[i].y = vy;
	d3varray[i].z = vz;
	d3varray[i].normal = D3DXVECTOR3(nx, ny, nz);
	d3varray[i].color = (r << 16) + (g << 8) + b;
	d3varray[i].tex = D3DXVECTOR2(u, v);
}

void Dx9Process::PolygonData::SetVertex(int I1, int i,
	float vx, float vy, float vz,
	float nx, float ny, float nz,
	int r, int g, int b,
	float u, float v){

	d3varrayI[I1] = i;
	d3varray[i].x = vx;
	d3varray[i].y = vy;
	d3varray[i].z = vz;
	d3varray[i].normal = D3DXVECTOR3(nx, ny, nz);
	d3varray[i].color = (r << 16) + (g << 8) + b;
	d3varray[i].tex = D3DXVECTOR2(u, v);
}