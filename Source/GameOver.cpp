//	�Q�[���I�[�o���[�h���̏���
#include"GameOver.h"

const int TITLE_X = 200;	//	�Q�[�����[�h���\�����W
const int TITLE_Y = 110;
const int TEXT_INPUT_X = 250;	//	���͂𑣂��e�L�X�g�̍��W
const int TEXT_INPUT_Y = 380;

//	�I�u�W�F�N�g�̃|�C���^
extern Input_Controll *Inp_Ctrl;	//	�L�[����
extern Sound_Manager *My_Sound;		//	��

//	�R���X�g���N�^
Game_Over::Game_Over()
{
	Cnt = 0;		//	�J�E���^�[������
	Load_Font();	//	�t�H���g�ǂݍ���
	Load_Graph();	//	�O���t�B�b�N�ǂݍ���
}

//	�f�X�g���N�^
Game_Over::~Game_Over()
{
	Delete_Font();	//	�t�H���g�폜
	Delete_Graph();	//	�O���t�B�b�N�폜
}

//	�Q�[���I�[�o�[��ʒ��Ă΂��
int Game_Over::Update()
{
	Cnt++;
	Draw_BackGr();	//	�w�i�`��
	Draw_Title();	//	���o���`��

	//	�������_�ŕ`��
	if (Cnt % 100 < 70)	Draw_Text();
		
	//	�J�E���g���i��ł����獶�N���b�N�Ń^�C�g����
	if (Cnt > CLICK_TIME && Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		Cnt = 0;	//	�J�E���g�����Z�b�g���Ȃ��Ǝ��ɗ����Ƃ��G���[���N����
		My_Sound->Play_Se(SE_DECISION);	//	���艹
		return MENU_INIT;	//	���j���[��ʂ�
	}
	return OVER;
}

//	�t�H���g�ǂݍ���
void Game_Over::Load_Font()
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MS�S�V�b�N", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("���C���I", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	�t�H���g�폜
void Game_Over::Delete_Font() { for (int i = 0; i < FONT_MAX; i++)  DeleteFontToHandle(Font_Handle[i]); }

//	�O���t�B�b�N�ǂݍ���
void Game_Over::Load_Graph() { BackGr = LoadGraph("image/Over/GameOver.png"); }

//	�O���t�B�b�N�폜
void Game_Over::Delete_Graph() { DeleteGraph(BackGr); }

//	�w�i�`��
void Game_Over::Draw_BackGr() { DrawExtendGraph(0, 0, WINDOW_X, WINDOW_Y, BackGr, true); }

//	���o���`��
void Game_Over::Draw_Title() { DrawStringToHandle(TITLE_X, TITLE_Y, "Game Over ...", COLOR[YELLOW], Font_Handle[MV_BOLI]); }

//	�e�L�X�g�`��
void Game_Over::Draw_Text() { DrawStringToHandle(TEXT_INPUT_X, TEXT_INPUT_Y, "Click to Window", COLOR[WHITE], Font_Handle[MS_GOTHIC]); }