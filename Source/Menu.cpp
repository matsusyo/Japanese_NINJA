#include"Menu.h"

//	�I�u�W�F�N�g�̃|�C���^
extern Input_Controll *Inp_Ctrl;	//	�L�[,�}�E�X����
extern Sound_Manager *My_Sound;		//	��

const int TITLE_X = 35;		//	���j���[��ʌ��o����X,Y���W
const int TITLE_Y = 40;
const int TEXT_X = TITLE_X * 10;		//	���������ʂ�������e�L�X�g���W
const int TEXT_Y = (int)(TITLE_Y * 1.6);

const int ICON_1X_LINE = 55;		//	�X�e�[�W�I���Ɉڍs����A�C�R���̋N�_X���W
const int ICON_2X_LINE = 295;		//	�V�ѕ��A�C�R����X���W
const int ICON_3X_LINE = 535;		//	�ݒ�A�C�R����X���W
const int ICON_ALL_Y_LINE = 185;	//	�A�C�R����Y���W �A�C�R���͕��s�ɕ��ׂ�
const int ICON_X_LONG = 210;		//	�A�C�R���̉���
const int ICON_Y_LONG = ICON_X_LONG;//	�A�C�R���̏c�� �c���Ɖ����𑵂���

const int EXIT_X = 660;
const int EXIT_Y = 500;
const int EXIT_X_LONG = 100;
const int EXIT_Y_LONG = 80;

const int TEXT_1X_LINE = 117;		//	�X�e�[�W�I���A�C�R�����̃e�L�X�g��X���W
const int TEXT_2X_LINE = 343;		//	�V�ѕ��A�C�R�����̃e�L�X�gX���W
const int TEXT_3X_LINE = 565;		//	�ݒ�A�C�R�����̃e�L�X�gX���W
const int TEXT_ALL_Y_LINE = 405;	//	�A�C�R�����̃e�L�X�g��Y���W 

//	�R���X�g���N�^
Menu::Menu()
{
	Cnt = 0;		//	�J�E���^�[������
	Load_Font();	//	�t�H���g�ǂݍ���
	Load_Graph();	//	�摜�ǂݍ���
}

//	�f�X�g���N�^
Menu::~Menu()
{
	Delete_Font();	//	�t�H���g�폜
	Delete_Graph();	//	�摜�폜
}

int Menu::Update() 
{
	Draw_Title();		//	���o���`��
	Draw_Icon_Text();	//	�A�C�R�����̃e�L�X�g�`��

	//	�N���b�N�A�łŃV�[�����}�ɕς�邱�Ƃ��Ȃ��悤�ɂ���
	if (Cnt <= CLICK_TIME) {
		Cnt++;			//	�J�E���^��i�߂�
		Draw_Icon();	//	�A�C�R���`��
		return MENU;	//	���j���[��ʂ��ێ�
	}

	//	�}�E�X�J�[�\���̈ʒu�ŃA�C�R���摜��ς���
	float MouseX, MouseY;
	Inp_Ctrl->Get_Mouse_Pointer(&MouseX, &MouseY);

	//	�}�E�X�ʒu�̃A�C�R���̃��[�h����
	int Mode = Draw_Target_Icon(MouseX, MouseY);

	//	�A�C�R����ŃN���b�N���ꂽ�烂�[�h�`�F���W
	if (Mode != MENU && Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		My_Sound->Play_Se(SE_DECISION);	//	���艹��炷
		Cnt = 0;		//	�J�E���^���Z�b�g
		return Mode;	//	���[�h�ύX
	}

	//	�A�C�R����ŃN���b�N����Ȃ������ꍇ�̓��j���[��ʂ��ێ�
	return MENU;
}

//	�t�H���g�ǂݍ���
void Menu::Load_Font()
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MS�S�V�b�N", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("���C���I", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	�摜�ǂݍ���
void Menu::Load_Graph()
{
	Mission_Gr[0] = LoadGraph("Image/Menu/Target.png");
	Mission_Gr[1] = LoadGraph("Image/Menu/Target_Back.png");
	Manual_Gr[0] = LoadGraph("Image/Menu/Practice.png");
	Manual_Gr[1] = LoadGraph("Image/Menu/Practice_Back.png");
	Title_Gr[0] = LoadGraph("Image/Menu/Title.png");
	Title_Gr[1] = LoadGraph("Image/Menu/Title_Back.png");
	Exit_Gr[0] = LoadGraph("Image/Menu/Exit.png");
	Exit_Gr[1] = LoadGraph("Image/Menu/Exit_Back.png");
}

//	�t�H���g�폜
void Menu::Delete_Font() { for (int i = 0; i < FONT_MAX; i++) DeleteFontToHandle(Font_Handle[i]); }

//	�摜�폜
void Menu::Delete_Graph()
{
	for (int i = 0; i < ICON_NUM; i++) {
		DeleteGraph(Mission_Gr[i]);
		DeleteGraph(Manual_Gr[i]);
		DeleteGraph(Title_Gr[i]);
		DeleteGraph(Exit_Gr[i]);
	}
}

//	���o���`��
void Menu::Draw_Title() { DrawStringToHandle(TITLE_X, TITLE_Y, "MENU", COLOR[YELLOW], Font_Handle[MV_BOLI]); }

//	�A�C�R���`��
void Menu::Draw_Icon()
{
	DrawExtendGraph(ICON_1X_LINE, ICON_ALL_Y_LINE, ICON_1X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Mission_Gr[0], TRUE);
	DrawExtendGraph(ICON_2X_LINE, ICON_ALL_Y_LINE, ICON_2X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Manual_Gr[0], TRUE);
	DrawExtendGraph(ICON_3X_LINE, ICON_ALL_Y_LINE, ICON_3X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Title_Gr[0], TRUE);
	DrawExtendGraph(EXIT_X, EXIT_Y, EXIT_X + EXIT_X_LONG, EXIT_Y + EXIT_Y_LONG, Exit_Gr[0], TRUE);
}

//	�A�C�R�����̃e�L�X�g�`��
void Menu::Draw_Icon_Text()
{
	DrawStringToHandle(TEXT_1X_LINE, TEXT_ALL_Y_LINE, "�C��", COLOR[WHITE], Font_Handle[MS_GOTHIC]);
	DrawStringToHandle(TEXT_2X_LINE, TEXT_ALL_Y_LINE, "�V�ѕ�", COLOR[WHITE], Font_Handle[MS_GOTHIC]);
	DrawStringToHandle(TEXT_3X_LINE, TEXT_ALL_Y_LINE, "�^�C�g��", COLOR[WHITE], Font_Handle[MS_GOTHIC]);
}

//	�}�E�X�ʒu�ɂ���A�C�R���`��
int Menu::Draw_Target_Icon(float mousex, float mousey)
{
	int Mode = MENU;
	//	�A�C�R����Ȃ�2�ڂ̃A�C�R���ɕς���
	if ((mousex >= ICON_1X_LINE && mousex <= ICON_1X_LINE + ICON_X_LONG) && (mousey >= ICON_ALL_Y_LINE && mousey <= ICON_ALL_Y_LINE + ICON_Y_LONG)) {
		DrawExtendGraph(ICON_1X_LINE, ICON_ALL_Y_LINE, ICON_1X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Mission_Gr[1], TRUE);
		Mode = STAGE;	//	�X�e�[�W�I����ʂ������[�h�ɑ��
	}
	//	�A�C�R����łȂ��̂Ȃ�1�ڂ̉摜�ɕς���
	else DrawExtendGraph(ICON_1X_LINE, ICON_ALL_Y_LINE, ICON_1X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Mission_Gr[0], TRUE);

	//	��������
	if ((mousex >= ICON_2X_LINE && mousex <= ICON_2X_LINE + ICON_X_LONG) && (mousey >= ICON_ALL_Y_LINE && mousey <= ICON_ALL_Y_LINE + ICON_Y_LONG)) {
		DrawExtendGraph(ICON_2X_LINE, ICON_ALL_Y_LINE, ICON_2X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Manual_Gr[1], TRUE);
		Mode = MANUAL;	//	�V�ѕ�������ʂ������[�h�ɑ��
	}
	else DrawExtendGraph(ICON_2X_LINE, ICON_ALL_Y_LINE, ICON_2X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Manual_Gr[0], TRUE);

	if (mousex >= ICON_3X_LINE && mousex <= ICON_3X_LINE + ICON_X_LONG && mousey >= ICON_ALL_Y_LINE && mousey <= ICON_ALL_Y_LINE + ICON_Y_LONG) {
		DrawExtendGraph(ICON_3X_LINE, ICON_ALL_Y_LINE, ICON_3X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Title_Gr[1], TRUE);
		Mode = TITLE_INIT;	//	�ݒ��ʂ������[�h�ɑ��
	}
	else DrawExtendGraph(ICON_3X_LINE, ICON_ALL_Y_LINE, ICON_3X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Title_Gr[0], TRUE);

	if (mousex >= EXIT_X && mousex <= EXIT_X + EXIT_X_LONG && mousey >= EXIT_Y && mousey <= EXIT_Y + EXIT_Y_LONG) {
		DrawExtendGraph(EXIT_X, EXIT_Y, EXIT_X + EXIT_X_LONG, EXIT_Y + EXIT_Y_LONG, Exit_Gr[1], TRUE);
		Mode = EXIT;
	}
	else DrawExtendGraph(EXIT_X, EXIT_Y, EXIT_X + EXIT_X_LONG, EXIT_Y + EXIT_Y_LONG, Exit_Gr[0], TRUE);

	return Mode;
}