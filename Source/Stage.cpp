#include"Stage.h"

//	�I�u�W�F�N�g�̃|�C���^
extern Input_Controll *Inp_Ctrl;	//	�L�[����
extern Sound_Manager *My_Sound;		//	��

const int TITLE_X = 35;	//	�X�e�[�W��ʌ��o����X,Y���W
const int TITLE_Y = 40;
const int TEXT_X = TITLE_X * 10;
const int TEXT_Y = (int)(TITLE_Y * 1.6);
const int STAGE_1X_LINE = 80;	//	6�̃X�e�[�W��1�i��3�z�u����̂�3��x���W
const int STAGE_2X_LINE = 310;
const int STAGE_3X_LINE = 540;
const int FIRST_Y_LINE = 180;	//	Y���W2�i��
const int SECOND_Y_LINE = 370;	//	2�i��
const int STAGE_X_LONG = 180;	//	�X�e�[�W�̉���
const int STAGE_Y_LONG = 140;	//	�c��
const int FONT_NUM = 3;	//	�t�H���g��


Stage::Stage() 
{
	Cnt = 0;
	Select_Stage = NO_STAGE;	//	�I���X�e�[�W������
	Load_Font();
	Load_Graph();
}

Stage::~Stage()
{
	Delete_Font();
	Delete_Graph();
}

int Stage::Update()
{
	Draw_Title();
	Draw_Stage();
	//	�N���b�N�\�ɂȂ�܂ŃC���N�������g
	if (Cnt <= CLICK_TIME)Cnt++;
	//�N���b�N�A�łŃV�[�����}�ɕς�邱�Ƃ��Ȃ��悤�ɂ���
	if (Cnt < CLICK_TIME) { return STAGE; }
	
	float MouseX, MouseY;	//	�}�E�X���W�Q�b�g
	Inp_Ctrl->Get_Mouse_Pointer(&MouseX, &MouseY);
	Select_Stage = Draw_Target_Stage(MouseX, MouseY);
	
	//	�X�e�[�W���I������Ă����ԂŃN���b�N���ꂽ�玟�̃��[�h��
	if (Select_Stage != NO_STAGE && Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		Cnt = 0;	//	���Z�b�g
		My_Sound->Play_Se(SE_DECISION);	//	���艹
		return PREPARE;	//	PREPARE���[�h�ɐi��
	}

	//	�N���b�N����Ȃ�������I�𒆂̃X�e�[�W�����Z�b�g
	if (!Inp_Ctrl->Is_Input_Once(MOUSE_LEFT))Select_Stage = NO_STAGE;

	//	�X�e�[�W�I����ʈێ�
	return STAGE;
}

void Stage::Load_Font() 
{
	StageGr[0] = LoadGraph("Image/Stage/1.png");
	StageGr[1] = LoadGraph("Image/Stage/2.png");
	StageGr[2] = LoadGraph("Image/Stage/3.png");
	StageGr[3] = LoadGraph("Image/Stage/4.png");
	StageGr[4] = LoadGraph("Image/Stage/5.png");
	StageGr[5] = LoadGraph("Image/Stage/6.png");
}

void Stage::Load_Graph() 
{
	FontHandle[MS_GOTHIC] = CreateFontToHandle("MS�S�V�b�N", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	FontHandle[MV_BOLI] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	FontHandle[MEIRIO] = CreateFontToHandle("���C���I", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

void Stage::Delete_Font() {for (int i = 0; i < FONT_NUM; i++)DeleteFontToHandle(FontHandle[i]);}

void Stage::Delete_Graph() {for (int i = 0; i < STAGE_GR_NUM; i++) { DeleteGraph(StageGr[i]); }}

void Stage::Draw_Title() {DrawStringToHandle(TITLE_X, TITLE_Y, "STAGE", COLOR[YELLOW], FontHandle[MV_BOLI]);}

void Stage::Draw_Stage() 
{
	DrawExtendGraph(STAGE_1X_LINE, FIRST_Y_LINE, STAGE_1X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, StageGr[0], FALSE);
	DrawExtendGraph(STAGE_2X_LINE, FIRST_Y_LINE, STAGE_2X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, StageGr[1], FALSE);
	DrawExtendGraph(STAGE_3X_LINE, FIRST_Y_LINE, STAGE_3X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, StageGr[2], FALSE);
	DrawExtendGraph(STAGE_1X_LINE, SECOND_Y_LINE, STAGE_1X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, StageGr[3], FALSE);
	DrawExtendGraph(STAGE_2X_LINE, SECOND_Y_LINE, STAGE_2X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, StageGr[4], FALSE);
	DrawExtendGraph(STAGE_3X_LINE, SECOND_Y_LINE, STAGE_3X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, StageGr[5], FALSE);
}

int Stage::Draw_Target_Stage(float mousex,float mousey) 
{
	int stage = NO_STAGE;
	//	�N���b�N���ꂽ�摜�ɂ���ăX�e�[�W��ς���	�X�e�[�W�P
	if ((mousex >= STAGE_1X_LINE && mousex <= STAGE_1X_LINE + STAGE_X_LONG) && (mousey >= FIRST_Y_LINE && mousey <= FIRST_Y_LINE + STAGE_Y_LONG)) {
		//	����`��@���̒��ɃX�e�[�W�̃C���[�W�摜��`��
		DrawBox(STAGE_1X_LINE, FIRST_Y_LINE, STAGE_1X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = AAA;
	}
	//	�X�e�[�W�Q
	if ((mousex >= STAGE_2X_LINE && mousex <= STAGE_2X_LINE + STAGE_X_LONG) && (mousey >= FIRST_Y_LINE && mousey <= FIRST_Y_LINE + STAGE_Y_LONG)) {
		DrawBox(STAGE_2X_LINE, FIRST_Y_LINE, STAGE_2X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = BBB;
	}

	//	�X�e�[�W3
	if ((mousex >= STAGE_3X_LINE && mousex <= STAGE_3X_LINE + STAGE_X_LONG) && mousey >= FIRST_Y_LINE && mousey <= FIRST_Y_LINE + STAGE_Y_LONG) {
		DrawBox(STAGE_3X_LINE, FIRST_Y_LINE, STAGE_3X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = CCC;
	}

	//	�X�e�[�W�S
	if ((mousey >= STAGE_1X_LINE && mousex <= STAGE_1X_LINE + STAGE_X_LONG) && (mousey >= SECOND_Y_LINE && mousey <= SECOND_Y_LINE + STAGE_Y_LONG)) {
		DrawBox(STAGE_1X_LINE, SECOND_Y_LINE, STAGE_1X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = DDD;
	}

	//	�X�e�[�W�T
	if ((mousex >= STAGE_2X_LINE && mousex <= STAGE_2X_LINE + STAGE_X_LONG) && (mousey >= SECOND_Y_LINE && mousey <= SECOND_Y_LINE + STAGE_Y_LONG)) {
		DrawBox(STAGE_2X_LINE, SECOND_Y_LINE, STAGE_2X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = EEE;
	}

	//	�X�e�[�W�U
	if ((mousex >= STAGE_3X_LINE && mousex <= STAGE_3X_LINE + STAGE_X_LONG) && mousey >= SECOND_Y_LINE && mousey <= SECOND_Y_LINE + STAGE_Y_LONG) {
		DrawBox(STAGE_3X_LINE, SECOND_Y_LINE, STAGE_3X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = FFF;
	}
	return stage;
}