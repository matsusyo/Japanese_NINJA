//	�N���A���[�h���̏���
#include"Clear.h"

const int TITLE_X = 180;	//	���o���̍��W
const int TITLE_Y = 110;
const int TEXT_X = 155;	//	�e�L�X�g�̍��W
const int TEXT_Y = 380;

const double CLEARALPHA_SPEED = 1.0;  // CLEAR�����x�̕ω����x

//	�I�u�W�F�N�g�̃|�C���^
extern Input_Controll *Inp_Ctrl;	//	����
extern Sound_Manager *My_Sound;		//	��
extern Map *My_Map;					//	�}�b�v
extern Qtr_Draw_Manager *My_Draw;	//	�`��e�[�u��

//	�R���X�g���N�^
Clear::Clear()
{
	Cnt = 0;		//	�J�E���^�[������
	ClearAlpha = 50.0;  // CLEAR�摜�����x
	ClearAlphaType = true;  // �����x�グ�邩�����邩
	Load_Font();	//	�t�H���g�ǂݍ���
}

//	�f�X�g���N�^
Clear::~Clear()
{
	Delete_Font();	//	�t�H���g�폜
}

//	�N���A��ʒ��Ă�
int Clear::Update()
{
	Cnt++;
	Draw_BackGr();	//	�w�i��`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)ClearAlpha);	// �������̕`��
	Draw_Title();	//	��������`��
	if (ClearAlphaType)ClearAlpha += CLEARALPHA_SPEED;  // ���񂾂񔖂�
	else ClearAlpha -= CLEARALPHA_SPEED;  // ���񂾂�Z��
	if (ClearAlpha < 150)ClearAlphaType = true;  // �����x��150�ɂȂ����獡�x�͔���
	if (ClearAlpha > 250)ClearAlphaType = false;  // �����x��250�𒴂�����Z��

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// �������`�����߂�

	//	�_�ŕ`��
	if (Cnt % 100 > 50 && Cnt >= 150)Draw_Text();	//	�A�C�R�����̃e�L�X�g�`��

	//	�N���b�N�A�łŃV�[�����}�ɕς�邱�Ƃ��Ȃ��悤�ɂ���
	if (Cnt <= CLICK_TIME) {
		return CLEAR;	//	�N���A��ʂ��ێ�
	}

	//	�A�C�R����ŃN���b�N���ꂽ�烂�[�h�`�F���W
	if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		My_Sound->Play_Se(SE_DECISION);	//	���艹��炷
		Cnt = 0;		//	�J�E���^���Z�b�g
		return RANKING;	//	���[�h�ύX
	}

	//	�A�C�R����ŃN���b�N����Ȃ������ꍇ�̓��j���[��ʂ��ێ�
	return CLEAR;
}

//	�t�H���g�ǂݍ���
void Clear::Load_Font()
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MS�S�V�b�N", 50, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 75, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("���C���I", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	�w�i�`��
void Clear::Draw_BackGr()
{
	My_Map->Tile_Draw();		//	��������ɕ`��@
	My_Draw->Draw();			//	�`��e�[�u�����̂��̂�`��
}

//	�t�H���g�폜
void Clear::Delete_Font()
{
	for (int i = 0; i < FONT_MAX; i++) DeleteFontToHandle(Font_Handle[i]);
}

//	���o���`��
void Clear::Draw_Title()
{
	DrawStringToHandle(TITLE_X, TITLE_Y, "Stage Clear!!", COLOR[YELLOW], Font_Handle[MV_BOLI]);
}

//	��������e�L�X�g�`��
void Clear::Draw_Text()
{
	DrawStringToHandle(TEXT_X, TEXT_Y, "Click Window to Next", COLOR[WHITE], Font_Handle[MS_GOTHIC]);
}