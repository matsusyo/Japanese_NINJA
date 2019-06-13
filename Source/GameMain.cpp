//	Japanese NINJA No.1
//	DX���C�u�����̌l����
//	�E�҂��ނƂ����Q�[��
//	�������1/10�`5/10

//	�v���v���Z�b�T
#include<time.h>			//	���ԏ��擾
#include"Dxlib.h"			//	DX���C�u����	
#include"Literal.h"			//	�萔���e����
#include"InputCtrl.h"		//	�L�[,�}�E�X���̓N���X
#include"QtrCalc.h"			//	�N�H�[�^���W�ϊ��N���X
#include"Player.h"			//	�v���C���[�N���X
#include"EnemyManager.h"	//	�G�N���X
#include"Map.h"				//	�}�b�v�N���X
#include"DartManager.h"		//	�藠���N���X
#include"BombManager.h"		//	���e�N���X
#include"Clone.h"			//	���g�N���X
#include"RankingData.h"		//	�����L���O�N���X
#include"TimeManager.h"		//	���ԊǗ��N���X
#include"QtrDraw.h"			//	�I�u�W�F�N�g�`��N���X
#include"EffectManager.h"	//	�G�t�F�N�g�N���X
#include"SoundManager.h"	//	���N���X
#include"Title.h"			//	�^�C�g����ʃN���X
#include"Manual.h"			//	�V�ѕ������N���X
#include"Menu.h"			//	���j���[��ʃN���X
#include"Stage.h"			//	�X�e�[�W�I����ʃN���X
#include"Clear.h"			//	�Q�[���N���A��ʃN���X
#include"GameOver.h"		//	�Q�[���I�[�o�[��ʃN���X

//	�I�u�W�F�N�g�̃|�C���^
Input_Controll *Inp_Ctrl;		//	�L�[����
Quarter_Calculation *Qtr_Calc;	//	���W�v�Z
Player *Ply;					//	�v���C���[�Ǘ�
Enemy_Manager *Eny;				//	�G�Ǘ�
Map *My_Map;					//	�}�b�v
Dart_Manager *Ply_Dart;			//	�藠��
Bomb_Manager *Ply_Bomb;			//	���e
Clone *Ply_Clone;				//	���g
RankingData *My_RankingData;	//	�����L���O�Ǘ�
Time_Manager *My_Time;			//	�N���A�^�C���A�o�ߎ��ԂȂǂ��Ǘ�
Qtr_Draw_Manager *My_Draw;		//	�I�u�W�F�N�g�̕`����܂Ƃ߂ĊǗ�
Effect_Manager *My_Effect;		//	�G�t�F�N�g�@�藠���┚�e���Փ�
Sound_Manager *My_Sound;		//	���@BGM��SE��
Title *My_Title;				//	�^�C�g�����
Manual *My_Manual;				//	�V�ѕ�����
Menu *My_Menu;					//	���j���[���
Stage *My_Stage;				//	�X�e�[�W�I�����
Clear *My_Clear;				//	�Q�[���N���A���
Game_Over *My_Over;				//	�Q�[���I�[�o�[���

//	�֐��v���g�^�C�v
void Mode_Ctrl();	// �Q�[�����[�h�Ǘ�
int Prepare();		// �Q�[���{�҂ɕK�v�ȃI�u�W�F�N�g�𐶐�
int Game_Main();	// �Q�[���{�҃v���C��
void Rel_Obj();		// �Q�[���{�҂Ŏg�����I�u�W�F�N�g��j��
int Judge();		// �S�Ă̓����蔻��

//	�O���֐��̃v���g�^�C�v
extern int Ranking();	// �����L���O���

//	�O���[�o��
int Game_Mode = TITLE_INIT;
char PlayerName[256];				//	���[�U�[���̉��z

//	���C���G���g���[�|�C���g
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	SetMainWindowText(GameTitle);			//	���sexe�����Q�[���^�C�g�����ɕύX
	ChangeWindowMode(TRUE);					//	�E�B���h�E���[�h���t���X�N���[������ύX
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);	//	800*600�̃E�B���h�E�ɂ���
	SetMouseDispFlag(TRUE);					//	�}�E�X�J�[�\����\��
	if( DxLib_Init() == -1 ) return -1;		//	DX���C�u���������������@�G���[���N������I��
    SetDrawScreen( DX_SCREEN_BACK ) ;		// �`����ʂ��Z�J���_���T�[�t�F�X�ɂ���	

	Inp_Ctrl = new Input_Controll;			//	�L�[�ƃ}�E�X���͂̃I�u�W�F�N�g
	My_Time = new Time_Manager;				//	�v���C���Ԃƃ^�C���A�^�b�N�̃^�C���v��
	My_Sound = new Sound_Manager;			//	�� BGM�Ƃ�SE
	My_Sound->Change_Bgm(BGM_TITLE, true);	//	�^�C�g����ʉ����Z�b�g

	// �����L���O�Ŏg�����[�U�[�l�[���𓾂�
	DWORD dwSize = sizeof(PlayerName);	//	�T�C�Y��DWORD�^��
	GetUserName(PlayerName, &dwSize);	//	���[�U�[�������炤
	My_Time->Set_Player_Name(PlayerName);	//	���[�U�[���̃Z�b�g


	//	���C�����[�v
	while (true) {
		if (ProcessMessage() == -1)break; //	���b�Z�[�W���[�v����������@�G���[���N������I��
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0 || Game_Mode == EXIT)break; //	�G�X�P�[�v�L�[�������ꂽ��I��

		ClearDrawScreen();			//	��ʂ����������ăN���A�ɂ���
		Inp_Ctrl->Update(Game_Mode);//	�L�[��}�E�X���͏��̍X�V
		Mode_Ctrl();				//	�Q�[�����[�h�̊Ǘ�
		My_Sound->Update();			//	���̏���
		ScreenFlip();				//	�Z�J���_���T�[�t�F�X��ʂ̓��e���v���C�}���T�[�t�F�X��

	}

	//	�I�u�W�F�N�g�̌�n��
	Rel_Obj();	//	�Q�[���{�҂Ŏg���I�u�W�F�N�g��j��

	//	����ȊO�̃I�u�W�F�N�g���j��
	
	if (My_Title != NULL) { delete My_Title; My_Title = NULL; }			//	�^�C�g��
	if (My_Manual != NULL) { delete My_Manual; My_Manual = NULL; }		//	�V�ѕ�
	if (Inp_Ctrl != NULL) {delete Inp_Ctrl; Inp_Ctrl = NULL; }			//	����
	if (My_RankingData != NULL) {delete My_RankingData; My_RankingData = NULL; }	//	�����L���O
	if (My_Time != NULL) { delete  My_Time; My_Time = NULL; }			//	�^�C��
	if (My_Sound != NULL) { delete My_Sound; My_Sound = NULL; }			//	��
	DxLib_End();		//	DX���C�u�����g�p�̏I������	
	
	return 0;	//	�\�t�g�I��
}

//	�Q�[�����[�h�̃R���g���[��
void Mode_Ctrl()
{
	//	�֐��̕Ԃ�l�ŃQ�[�����[�h��ς���
	switch (Game_Mode) {

		//	�^�C�g���̑O�ɏ�����
	case TITLE_INIT:
		if (My_Title == NULL) My_Title = new Title;
		Game_Mode = TITLE;
		break;

		//	�^�C�g�����
	case TITLE:
		switch (My_Title->Update()) {
		case MENU_INIT:	//	MENU���Ԃ�l�Ȃ烁�j���[��ʂֈڍs
			Game_Mode = MENU_INIT;
			My_Sound->Change_Bgm(BGM_MENU, true);	//	���j���[��ʉ����Z�b�g
			break;
		default:	//	����ȊO�Ȃ�^�C�g����ʂ��ێ�����
			break;
		}
		break;


		//	���j���[�̑O�ɏ�����
	case MENU_INIT:
		if (My_Manual == NULL) My_Manual = new Manual;
		if (My_Menu == NULL)My_Menu = new Menu;
		Game_Mode = MENU;
		break;


		//	���j���[���
	case MENU:
		switch (My_Menu->Update()) {
		case STAGE:		//	STAGE���Ԃ�l�Ȃ�X�e�[�W�I����ʂֈڍs
			if (My_Stage == NULL)My_Stage = new Stage;
			Game_Mode = STAGE;
			break;
		case MANUAL:	//	MANUAL�Ȃ�V�ѕ�������ʂ�
			Game_Mode = MANUAL;
			break;
		case TITLE_INIT:		//	TITLE�Ȃ�^�C�g����ʂ�
			Game_Mode = TITLE_INIT;
			My_Sound->Change_Bgm(BGM_TITLE, true);	//	���j���[��ʉ����Z�b�g
			break;
		case EXIT:	//	EXIT�Ȃ�Q�[���I��
			Game_Mode = EXIT;
			break;
		default:		//	����ȊO�Ȃ烁�j���[��ʈێ�
			break;
		}
		break;


		//	�X�e�[�W�I�����
	case STAGE:
		switch (My_Stage->Update()) {
		case PREPARE:	//	PREPARE�Ȃ�Q�[���{�҂ɕK�v�ȃI�u�W�F�N�g��������
			Game_Mode = PREPARE;
			break;
		default:		//	����ȊO�Ȃ�ێ�
			break;
		}
		break;


		//	�V�ѕ��������
	case MANUAL:
		switch (My_Manual->Update()) {
		case MENU:	//	MENU�Ȃ烁�j���[��ʂ�
			Game_Mode = MENU;
			break;
		default:	//	����ȊO�Ȃ�ێ�
			break;
		}
		break;


		//	�Q�[���{�ҏ�����
	case PREPARE:
		switch (Prepare()) {
		case PLAY:	//	PLAY�Ȃ�Q�[���{�҃v���C��ʂ�
			Game_Mode = PLAY;
			if (My_Title != NULL) { delete My_Title; My_Title = NULL; }
			if (My_Menu != NULL) { delete My_Menu; My_Menu = NULL; }
			if (My_Manual != NULL) { delete My_Manual; My_Manual = NULL; }
			My_Sound->Change_Bgm(BGM_PLAY, true);	//	�v���C��ʉ����Z�b�g
			break;
		default:	//	����ȊO�Ȃ�ێ�
			break;
		}
		break;


		//	�Q�[���{�҃v���C���
	case PLAY:

		//	�Ԃ�l�ŏ�����U�蕪��
		switch (Game_Main()) {

			//	�v���C��
		case PLAY:
			break;

			//	�Q�[���I�[�o�[
		case OVER:
			if (My_Over == NULL)My_Over = new Game_Over;
			Game_Mode = OVER;	//	�Q�[���I�[�o�[��ʂɈڍs	
			My_Sound->Change_Bgm(BGM_OVER, true);	//	�Q�[���I�[�o�[��ʉ����Z�b�g
			break;

			//	�Q�[���N���A
		case CLEAR:
			//	�����ŃN���A�^�C���ƃN���A���̓��t���L�^
			My_Time->SetClear_Time();		//	���݂̎��ԁ[�Q�[���v���C�J�n����
			My_Time->Set_Date();			//	�N���A���̓��t���L�^
			My_RankingData->Add_Time(My_Time);	//	�����L���O�ɒǉ�
			if (My_Clear == NULL)My_Clear = new Clear;
			Game_Mode = CLEAR;	//	�Q�[���N���A��ʂɈڍs
			My_Sound->Change_Bgm(BGM_CLEAR, true);	//	�Q�[���N���A��ʉ����Z�b�g
			break;

		default:	//	����ȊO�Ȃ�ێ�
			break;
		}
		break;


		//	�Q�[���I�[�o�[���
	case OVER:
		switch (My_Over->Update()) {
		case MENU_INIT:	//	MENU���Ԃ�l�Ȃ烁�j���[��ʂ�
			if (My_Over != NULL) { delete My_Over; My_Over = NULL; }
			Game_Mode = MENU_INIT;
			My_Time->Clear();	//	���Ԃ����Z�b�g
			My_Sound->Change_Bgm(BGM_MENU, true);
			Rel_Obj();			//	�Q�[���{�҂Ŏg�����I�u�W�F�N�g�̂ݔj������	
			break;
		default:	//	����ȊO�Ȃ�ێ�
			break;
		}
		break;


		//	�Q�[���N���A���
	case CLEAR:
		switch (My_Clear->Update()) {
		case RANKING:	//	RANKING���Ԃ�l�Ȃ烉���L���O��ʂ�
			if (My_Clear != NULL) { delete My_Clear; My_Clear = NULL; }
			Game_Mode = RANKING;
			Rel_Obj();			//	�Q�[���{�҂Ŏg�����I�u�W�F�N�g�̂ݔj������
			break;
		default:		//	����ȊO�Ȃ�ێ�
			break;
		}
		break;


		//	�����L���O���
	case RANKING:
		switch (Ranking()) {
		case MENU_INIT:	//	TITLE���Ԃ�l�Ȃ�^�C�g����ʂ�
			Game_Mode = MENU_INIT; 
			My_RankingData->Data_Save();	// �����L���O�f�[�^���Z�[�u
			My_Time->Clear();	//	���Ԃ����Z�b�g
			My_Sound->Change_Bgm(BGM_MENU, true);
			break;
		default:	//	����ȊO�Ȃ�ێ�
			break;
		}
		break;


		//	�\�����ʕԂ�l�Ȃ�I��
	default:
		Game_Mode = EXIT;
		break;
	}
}


enum SCREEN_MODE {  // �X�N���[�����[�h
	NO_FADEOUT, FADEOUT_SCREEN, FADEOUT_FONT
};
//	�Q�[���{�҂Ŏg���I�u�W�F�N�g�𐶐�
int Prepare()
{
	static int FadeOutMode = NO_FADEOUT;  // �t�F�[�h�A�E�g�Ȃ�
	static float Alpha = 255.0f;   // �����x
	static float AlphaSpeed = 2.5f;  // ���������x
	static int FontHandle[3];	//	�t�H���g

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)Alpha);  // �������`��
	if (FadeOutMode == NO_FADEOUT) {
		FadeOutMode = FADEOUT_SCREEN;

		//	�t�H���g�ǂݍ���
		FontHandle[0] = CreateFontToHandle("MS�S�V�b�N", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
		FontHandle[1] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
		FontHandle[2] = CreateFontToHandle("���C���I", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	
		Qtr_Calc = new Quarter_Calculation;	//	�N�H�[�^���W�ϊ�
		My_Map = new Map(My_Stage->Get_Select_Stage());	//	�G�̏������W�����߂���悤�ɁA�G��葁���I�u�W�F�N�g�����@
		Ply = new Player;					//	�v���C���[
		Eny = new Enemy_Manager;			//	�G����
		My_Draw = new  Qtr_Draw_Manager;	//	�E�B���h�E���ɕ`�悷�镨�̃e�[�u��
		My_Effect = new Effect_Manager;		//	����
		Ply_Dart = new Dart_Manager;		//	�藠��
		Ply_Bomb = new Bomb_Manager;		//	���e
		My_Effect = new Effect_Manager;		//	�G�t�F�N�g
		My_RankingData = new RankingData(My_Stage->Get_Select_Stage());	//	�����L���O

		Ply_Clone = NULL;	//	���g�͈�̂����o���Ȃ��̂ŕ��g���g������new����

		My_RankingData->Set_Flg_Off_All();	//	�����L���O�̃J�����g�v���C���[�t���O��S���I�t�ɂ���

		My_Time->Set_Flg(true);		//	�J�����g�v���C���[�̂��邵���Ă�
		My_Time->Set_Player_Name(PlayerName);
	}

	// �w�i�t�F�[�h�A�E�g
	if (FadeOutMode == FADEOUT_SCREEN) {
		Alpha -= AlphaSpeed;  // ���񂾂񔖂�
		
		if (Alpha < 0) {  // �w�i����������
			FadeOutMode = FADEOUT_FONT;  // �����̃t�F�[�h�A�E�g��
			AlphaSpeed = 0.3f;  // ���������x(�����͂������)
		}
	}
	DrawStringToHandle(500, 450, "Loading", COLOR[YELLOW], FontHandle[1]);

	// �����t�F�[�h�C��
	if (FadeOutMode == FADEOUT_FONT) {
		Alpha += AlphaSpeed;  // ���񂾂�Z��
		AlphaSpeed += 0.005f;  // ���������x(���񂾂񑁂�)
		
		if (Alpha > 200) {  // ���������S�ɕ\�������
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);  // �ʏ�`��
			FadeOutMode = NO_FADEOUT;
			return PLAY;  // �v���C��������
		}
	}
	return PREPARE;	//	�Q�[���{�҂�
}


//	�Q�[�����̏������s���֐�
int Game_Main()
{
	//	�藠�����ˁ@
	if (Inp_Ctrl->Is_Input_Once(KEY_Z)) {

		//	MP������΁A�v���C���[�̍��W�Ɗp�x��o�^
		if (Ply->Gauge_Bar[MP]->Get_Now() >= -DART_MP) {
			Ply->Gauge_Bar[MP]->Add_Now(DART_MP);
			My_Sound->Play_Se(SE_DART_REGISTER);
			Ply_Dart->Register(Ply->Get_Top_X(), Ply->Get_Top_Y(), Ply->Get_Top_Angle(), 0);
		}
		//	MP�Ȃ��@�X�J��
		else {
			My_Sound->Play_Se(SE_MISS);
		}
	}

	//	���e����
	if (Inp_Ctrl->Is_Input_Once(KEY_X)) {

		//	MP���O�łȂ���Δ���
		if (Ply->Gauge_Bar[MP]->Get_Now() >= -BOMB_MP) {
			Ply->Gauge_Bar[MP]->Add_Now(BOMB_MP);
			My_Sound->Play_Se(SE_BOMB_REGISTER);
			RECT rc = Ply->Get_Rect();
			Ply_Bomb->Register((float)((rc.left + rc.right) / 2), (float)rc.bottom);
		}
		else {
			My_Sound->Play_Se(SE_MISS);
		}
	}

	//	���g�̏p�𔭓��A���g�͈�̂���
	if (Inp_Ctrl->Is_Input_Once(KEY_C) && Ply_Clone == NULL) {

		//	MP���O�łȂ����
		if (Ply->Gauge_Bar[MP]->Get_Now() >= -CLONE_MP) {
			Ply->Gauge_Bar[MP]->Add_Now(CLONE_MP);
			//	���W�Ɗp�x�A�N�H�[�^�r���[��ł̊p�x��o�^
			My_Sound->Play_Se(SE_CLONE_REGISTER);
			Ply_Clone = new Clone(Ply->Get_Top_X(), Ply->Get_Top_Y(), Ply->Get_Top_Angle(), Ply->Get_Qtr_Angle());
		}
		else {
			My_Sound->Play_Se(SE_MISS);
		}
	}

	//	�藠�����ړ������`��e�[�u���ɓo�^
	Ply_Dart->Move();

	//	���e���ړ������`��e�[�u���ɓo�^
	Ply_Bomb->Move();

	//	���g�̏p���������Ȃ�X�V����@���ĂȂ��Ȃ�NULL
	if (Ply_Clone != NULL) {
		
		//	���g�����݂��Ă��瓮����
		if (Ply_Clone->Is_Clone_Flg())
			Ply_Clone->Move();

		//	���g����������NULL����
		else
			Ply_Clone = NULL;
	}

	//	�ړ�
	Ply->Move();
	Eny->Move();

	//	�`��
	My_Map->Tile_Draw();		//	��������ɕ`��@
	My_Map->Map_Register();		//	�ʂ�Ȃ�������`��e�[�u���ɓo�^ ��
	My_Draw->insertionSort();	//	�`��e�[�u������Y���W���������Ƀ\�[�g
	My_Draw->Draw();			//	�`��e�[�u�����̂��̂�`��

	Eny->Draw_Ray();	//	�G�̎��E��`��
	Eny->Draw_Gauge();	//	HP��`��

	// �����̕`��
	My_Effect->Draw(Ply->Get_Camera_X(), Ply->Get_Camera_Y());

	//	HP,MP��`��
	Ply->Draw_Gauge();

	My_Time->SetClear_Time();	// ���݂̎��ԁ[�Q�[���v���C�J�n����
	My_Time->Add_Time_Limit(-1.0f / 60.0f);	//	1�b�Ԃ�60����

	//	�c�萧�����Ԃ�30�b�ȏ゠��Δ��F�Ŏc�莞�Ԃ�\������
	if (My_Time->Get_Time_Limit() >= 30.0f)
		DrawFormatString(15, 65, COLOR[WHITE], "�������� : %.2f", My_Time->Get_Time_Limit());

	//	30�b�ȉ��Ȃ�ԐF�ŕ\��
	else
		DrawFormatString(15, 65, COLOR[RED], "�������� : %.2f", My_Time->Get_Time_Limit());

	//	�S�Ă̔���
	int result = Judge();
	if (result == PLAY)My_Draw->Clear_Table();
	return result;
	
}


//	�S�Ă̔�����s��
int Judge()
{
	//	�藠���ƓG	�G���_���[�W���󂯂�
	Ply_Dart->Judge_Enemy();

	//	���e�ƓG	�G���_���[�W���󂯂�
	Ply_Bomb->Judge_Enemy();

	//	���e�ƃv���C���[�@�v���C���[���_���[�W���󂯂�
	if (Ply_Bomb->Judge_Player(Ply->Get_Rect())) {
		//	�m�[�}����ԂȂ�HP�v�Z��
		if (Ply->Status.Is(NORMAL)) {
			Ply->Hp_Proc(BOMB_DAMAGE);
			float x = (float)(Ply->Get_Qtr_X() + QTR_OFFSET_X);	//	�o�^������W���v�Z
			float y = (float)(Ply->Get_Qtr_Y() + QTR_OFFSET_Y);
			My_Effect->Register(x, y, EFFECT_BOMB);	//	�G�t�F�N�g�o�^
			My_Sound->Play_Se(SE_BOMB_SPARK);		//	�Փˉ���炷
		}
	}

	//	�G�ƃv���C���[	�v���C���[���_���[�W�󂯂�
	if (Eny->Judge_Player(Ply->Get_Rect())) {
		//	�m�[�}����ԂȂ�HP�v�Z��
		if (Ply->Status.Is(NORMAL)) {
			Ply->Hp_Proc(COLLISION_DAMAGE);
			float x = (float)(Ply->Get_Qtr_X() + QTR_OFFSET_X);	//	�o�^������W���v�Z
			float y = (float)(Ply->Get_Qtr_Y() + QTR_OFFSET_Y);
			My_Effect->Register(x, y, EFFECT_COLLISION);	//	�G�t�F�N�g�o�^
			My_Sound->Play_Se(SE_COLLISION);	//	�Փˉ���炷
		}
	}

	//	��Ԃ̍X�V	�����Ă�Ƃ�����ł�Ƃ�
	Ply->Status_Update();
	Eny->Status_Update();

	//	�N���A�t���O�������Ă���N���A
	if (Ply->Is_Clear_Flg()) {
		return CLEAR;
	}

	//	�v���C���[������ł���Q�[���I�[�o�[
	if (Ply->Status.Is(DIE)) {
		return OVER;
	}

	//	�������Ԃ��I�[�o�[������Q�[���I�[�o�[
	if (My_Time->Get_Time_Limit() < 0) {
		return OVER;
	}

	//	����ȊO�Ȃ�Q�[���𑱂���
	return PLAY;
}


//	�Q�[���{�҂Ŏg�p�����I�u�W�F�N�g�̂��Ƃ��܂�
void Rel_Obj()
{
	//	�G���[�΍�ŃI�u�W�F�N�g��NULL�łȂ��� delete ����
	if (Qtr_Calc != NULL) { delete Qtr_Calc; Qtr_Calc = NULL; }	//	�N�H�[�^�[���W�v�Z
	if (Ply != NULL) { delete Ply; Ply = NULL; }				//	�v���C���[
	if (Eny != NULL) { delete Eny; Eny = NULL; }				//	�G
	if (My_Map != NULL) { delete My_Map; My_Map = NULL; }		//	�}�b�v
	if (Ply_Dart != NULL) { delete Ply_Dart; Ply_Dart = NULL; }	//	�藠��
	if (My_Draw != NULL) { delete My_Draw; My_Draw = NULL; }	//	�`��e�[�u��
	if (My_Effect != NULL) { delete My_Effect; My_Effect = NULL; }	//	�G�t�F�N�g
}