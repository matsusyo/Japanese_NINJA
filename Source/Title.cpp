#include"Title.h"

//	�I�u�W�F�N�g�̃|�C���^
extern Input_Controll *Inp_Ctrl;	//	�L�[����
extern Sound_Manager *My_Sound;		//	��

//	�R���X�g���N�^
Title::Title()
{
	Cnt = 0;		//	�J�E���^�[������
	Load_Font();	//	�t�H���g�ǂݍ���
	Load_Graph();	//	�O���t�B�b�N�ǂݍ���
}

//	�f�X�g���N�^
Title::~Title()
{
	Delete_Font();	//	�t�H���g�폜
	Delete_Graph();	//	�O���t�B�b�N�폜
}

int Title::Update()
{
	Draw_Back_Gr();	//	�w�i�`��
	Draw_Title();	//	���o����`��

	//	�������_�ŕ`��
	if (Cnt % 100 < 70)
		DrawStringToHandle(TEXT_X, TEXT_Y, "Click  Window to Start", COLOR[WHITE], Font_Handle[MS_GOTHIC]);

	Cnt++;
	//	��莞�Ԍo�߂���܂ŃN���b�N���󂯕t���Ȃ�
	if (Cnt <= CLICK_TIME)return TITLE; 

	//	�E�B���h�E���ō��N���b�N���ꂽ�烁�j���[��ʂɈڍs
	if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		My_Sound->Play_Se(SE_DECISION);	//	���艹�炷
		Cnt = 0;	//	������
		return MENU_INIT;	//	���j���[��ʂ�
	}

	return TITLE;	//	�N���b�N����Ȃ�����^�C�g�����
}

//	�t�H���g�ǂݍ���
void Title::Load_Font() 
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MS�S�V�b�N", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("���C���I", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	�t�H���g�폜
void Title::Delete_Font() { for (int i = 0; i < FONT_MAX; i++)  DeleteFontToHandle(Font_Handle[i]); }

//	�O���t�B�b�N�ǂݍ���
void Title::Load_Graph() { Back_Gr = LoadGraph("Image/Title/Title1.png"); }

//	�O���t�B�b�N�폜
void Title::Delete_Graph() { DeleteGraph(Back_Gr); }

//	�w�i�`��
void Title::Draw_Back_Gr() 
{
	DrawExtendGraph(0, 0, WINDOW_X, WINDOW_Y, Back_Gr, true);
}

//	���o���`��
void Title::Draw_Title() 
{
	DrawStringToHandle(TITLE_X, TITLE_Y, "Japanese NINJA No1", COLOR[YELLOW], Font_Handle[MV_BOLI]);
}