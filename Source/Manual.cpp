#include"Manual.h"

//	�I�u�W�F�N�g�̃|�C���^
extern Input_Controll *Inp_Ctrl;	//	�L�[����
extern Sound_Manager *My_Sound;		//	��

//	���j���[��ʌ��o����X,Y���W
const int TITLE_X = 35;
const int TITLE_Y = 40;
const int TEXT_X = TITLE_X * 10;
const int TEXT_Y = (int)(TITLE_Y * 1.6);

//	�����摜��X���W��Y���W
const int MANUAL_X = 70;
const int MANUAL_Y = 128;

//	�ڎ��ƂȂ遠�̍��W�Ɣ͈�
const int CONTENTS_X_LONG = 50;	//	���̉��̒���
const int CONTENTS_Y_LONG = 50;	//	�c�̒���
const int CONTENTS_Y = 500;
const int CONTENTS_1X = 205;
const int CONTENTS_2X = 285;
const int CONTENTS_3X = 365;
const int CONTENTS_4X = 445;
const int CONTENTS_5X = 525;

//	�N���b�N����ƃ��j���[��ʂɖ߂遠��X,Y���W�A�͈�
const int MENU_X = 610;
const int MENU_Y = 35;
const int MENU_X_LONG = 170;
const int MENU_Y_LONG = 70;

//	�R���X�g���N�^
Manual::Manual()
{
	Set_Page(0);	//	�y�[�W�ԍ�������
	Set_Cnt(0);		//	�J�E���g������
	Load_Font();	//	�t�H���g�ǂݍ���
	Load_Graph();	//	�O���t�B�b�N�ǂݍ���
}

//	�f�X�g���N�^
Manual::~Manual()
{
	Delete_Font();		//	�t�H���g�폜
	Delete_Graph();		//	�O���t�B�b�N�폜
}

//	�}�j���A����ʒ��Ă΂��
int Manual::Update()
{
	Draw_BackGr();			//	�w�i��`��
	Draw_Title();			//	�V�[���̌��o����`��
	Draw_TeachGr();			//	�V�[�����̐����摜��`��
	Draw_Contents_Select();	//	���݂̃V�[���ڎ�����Ԃŕ`��
	Draw_Contents_Out();	//	�V�[���ڎ��́��̊O�g��`��

	//	�}�E�X���W���l��
	float MouseX, MouseY;
	Inp_Ctrl->Get_Mouse_Pointer(&MouseX, &MouseY);

	Draw_Menu_Back(MouseX,MouseY);		//	���j���[�ɖ߂�A�C�R����`��

	//	�N���b�N�A�łł̌�I����h�~���� 50�J�E���g�ڂ܂ł͂����ŏ������I����
	if (Cnt <= CLICK_TIME) {
		Cnt++;
		return MANUAL;
	}

	//	MENU���N���b�N����ƃ��j���[��ʂɖ߂�悤�ɂ���
	if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT) && MouseX >= MENU_X && MouseX <= MENU_X + MENU_X_LONG && MouseY >= MENU_Y && MouseY <= MENU_Y + MENU_Y_LONG) {
		My_Sound->Play_Se(SE_DECISION);	//	���艹
		Set_Page(0);	//	�y�[�W������
		return MENU;	//	���j���[��ʂ�
	}

	//	MENU���N���b�N����Ă��Ȃ��ā��̖ڎ���Y���W�͈͊O�Ȃ�}�j���A����ʂ��ێ�
	if (!(MouseY >= CONTENTS_Y && MouseY <= CONTENTS_Y + CONTENTS_Y_LONG)) {
		return MANUAL;	//	�}�j���A����ʈێ�
	}
	
	Page_tmp = Page;	//	���݂̐�����ʔԍ������ԍ��ɑ��
	Draw_Contents_Target(MouseX, MouseY);	//	�}�E�X�̈ʒu�ɂ��遠�𔒐F�ŕ`��

	//	�Ⴄ�V�[�����N���b�N���ꂽ��V�[����ς���
	if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT) && Page != Page_tmp) {
		My_Sound->Play_Se(SE_SELECTION);	//	�y�[�W�ύX��
		Page = Page_tmp;	//	�N���b�N�������̃y�[�W�ԍ��Ɉړ�
	}

	//	�}�j���A����ʂ��ێ�
	return MANUAL;
}

//	�t�H���g�ǂݍ���
void Manual::Load_Font()
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MS�S�V�b�N", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 52, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("���C���I", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	�摜�ǂݍ���
void Manual::Load_Graph()
{
	Back_Gr = LoadGraph("Image/Manual/Back.png");
	Manual_Gr[0] = LoadGraph("Image/Manual/aaa.png");
	Manual_Gr[1] = LoadGraph("Image/Manual/bbb.png");
	Manual_Gr[2] = LoadGraph("Image/Manual/ccc.png");
	Manual_Gr[3] = LoadGraph("Image/Manual/ddd.png");
	Manual_Gr[4] = LoadGraph("Image/Manual/eee.png");
}

//	�t�H���g�폜
void Manual::Delete_Font() { for (int i = 0; i < FONT_MAX; i++)  DeleteFontToHandle(Font_Handle[i]); }

//	�O���t�B�b�N�폜
void Manual::Delete_Graph()
{
	DeleteGraph(Back_Gr);
	for (int i = 0; i < MANUAL_GR_NUM; i++) { DeleteGraph(Manual_Gr[i]); }
}

//	�w�i��`��
void Manual::Draw_BackGr() { DrawExtendGraph(0, 0, WINDOW_X, WINDOW_Y, Back_Gr, TRUE); }

//	���o����`��
void Manual::Draw_Title()
{
	//	�y�[�W�ԍ��ɂ���Č��o����ς���
	switch (Get_Page()) {
	default:
	case 0:
		DrawStringToHandle(TITLE_X, TITLE_Y, "���Q�[���ɂ��ā�", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	case 1:
		DrawStringToHandle(TITLE_X, TITLE_Y, "���Q�[�����[����", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	case 2:
		DrawStringToHandle(TITLE_X, TITLE_Y, "���ړ����@��", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	case 3:
		DrawStringToHandle(TITLE_X, TITLE_Y, "���E�p��", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	case 4:
		DrawStringToHandle(TITLE_X, TITLE_Y, "���E�p��", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	}
}

//	�����摜��`��
void Manual::Draw_TeachGr()
{
	//	�y�[�W�ԍ��ɂ���Đ����摜��ς���
	switch (Get_Page()) {
	default:
	case 0:
		DrawGraph(MANUAL_X, MANUAL_Y, Manual_Gr[0], TRUE);
		break;
	case 1:
		DrawGraph(MANUAL_X, MANUAL_Y, Manual_Gr[1], TRUE);
		break;
	case 2:
		DrawGraph(MANUAL_X - 1, MANUAL_Y, Manual_Gr[2], TRUE);
		break;
	case 3:
		DrawGraph(MANUAL_X, MANUAL_Y, Manual_Gr[3], TRUE);
		break;
	case 4:
		DrawGraph(MANUAL_X, MANUAL_Y, Manual_Gr[4], TRUE);
		break;
	}
}

//	���݂̃y�[�W�́���Ԃŕ`��
void Manual::Draw_Contents_Select()
{
	//	���݂̃y�[�W�ŃX�C�b�`����
	switch (Get_Page()) {
	default:
	case 0:
		DrawBox(CONTENTS_1X, CONTENTS_Y, CONTENTS_1X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	case 1:
		DrawBox(CONTENTS_2X, CONTENTS_Y, CONTENTS_2X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	case 2:
		DrawBox(CONTENTS_3X, CONTENTS_Y, CONTENTS_3X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	case 3:
		DrawBox(CONTENTS_4X, CONTENTS_Y, CONTENTS_4X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	case 4:
		DrawBox(CONTENTS_5X, CONTENTS_Y, CONTENTS_5X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	}
}

//	�N���b�N����ƃ��j���[��ʂɖ߂�͈͂�`��
void Manual::Draw_Menu_Back(float mousex, float mousey) 
{
	if (mousey >= MENU_Y && mousey <= MENU_Y + 70 && mousex >= MENU_X && mousex <= MENU_X + 170)
		DrawStringToHandle(MENU_X, MENU_Y, "MENU", COLOR[RED], Font_Handle[MV_BOLI]);

	else
		DrawStringToHandle(MENU_X, MENU_Y, "MENU", COLOR[LIGHTBLUE], Font_Handle[MV_BOLI]);
}


//	�y�[�W��\�����̊O�g���T�`��
void Manual::Draw_Contents_Out()
{
	DrawBox(CONTENTS_1X, CONTENTS_Y, CONTENTS_1X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
	DrawBox(CONTENTS_2X, CONTENTS_Y, CONTENTS_2X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
	DrawBox(CONTENTS_3X, CONTENTS_Y, CONTENTS_3X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
	DrawBox(CONTENTS_4X, CONTENTS_Y, CONTENTS_4X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
	DrawBox(CONTENTS_5X, CONTENTS_Y, CONTENTS_5X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
}

//	�}�E�X�̈ʒu�ɂ��遠�𔒐F�ŕ`��
void Manual::Draw_Contents_Target(float mousex, float mousey)
{
	//	�}�E�X�����͈͓̔��ɓ������牼�ԍ��Ɂ��̐����ԍ�����
	if (Page != 0 && mousex >= CONTENTS_1X && mousex <= CONTENTS_1X + CONTENTS_X_LONG) {
		Page_tmp = 0;
		DrawBox(CONTENTS_1X, CONTENTS_Y, CONTENTS_1X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
	if (Page != 1 && mousex >= CONTENTS_2X && mousex <= CONTENTS_2X + CONTENTS_X_LONG) {
		Page_tmp = 1;
		DrawBox(CONTENTS_2X, CONTENTS_Y, CONTENTS_2X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
	if (Page != 2 && mousex >= CONTENTS_3X && mousex <= CONTENTS_3X + CONTENTS_X_LONG) {
		Page_tmp = 2;
		DrawBox(CONTENTS_3X, CONTENTS_Y, CONTENTS_3X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
	if (Page_tmp != 3 && mousex >= CONTENTS_4X && mousex <= CONTENTS_4X + CONTENTS_X_LONG) {
		Page_tmp = 3;
		DrawBox(CONTENTS_4X, CONTENTS_Y, CONTENTS_4X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
	if (Page != 4 && mousex >= CONTENTS_5X && mousex <= CONTENTS_5X + CONTENTS_X_LONG) {
		Page_tmp = 4;
		DrawBox(CONTENTS_5X, CONTENTS_Y, CONTENTS_5X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
}