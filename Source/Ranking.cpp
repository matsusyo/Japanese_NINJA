#include "DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"RankingData.h"
#include"TimeManager.h"
#include"SoundManager.h"

const int TEXT_RANKOUT_X = 100;	//	�����N�A�E�g�̎��̃e�L�X�g���W
const int TEXT_RANKOUT_Y = 475;
const int TEXT_RANKIN_X = 250;	//	�����N�C���̎��̃e�L�X�g���W
const int TEXT_RANKIN_Y = 475;
const int TEXT_INPUT_X = 285;	//	���͂𑣂��e�L�X�g���W
const int TEXT_INPUT_Y = 520;

int RankingCnt = 0;	// �����L���O�J�E���^

extern Input_Controll *Inp_Ctrl;	// ����
extern RankingData *My_RankingData;	// �����L���O
extern Time_Manager *My_Time;	// �X�R�A
extern Sound_Manager *My_Sound;	//	��

// �����L���O
int Ranking()
{
	static int RankingFont;						// �����L���O�t�H���g
	static int RankingTitleFont;				// �����L���O�^�C�g���t�H���g
	static float RankingTargetPosY[RANKING_NUM];// �^�[�Q�b�g�ƂȂ�y���W�̔z��
	static float RankingMovePosY[RANKING_NUM];	// ���ۂ�y���W�̔z��
	static float RankingTilePosY;				// �^�C�g���ʒuy
	static int StarGr[7];						// ���p�̃O�� 7�F
	static int StarFont;						// ���t�H���g
	static bool RankIn = false;					// �����N�C���������ǂ����̃t���O
	static int MyRankNum = 0;					// ���ʂ������@
	char Buf[256];								// �X�R�A����������炤
	char BufLine[256];							// �ŏI�I�ȕ�����o�b�t�@

	// ���N���X
	class Star {
		int StarGrNum;		// �ق��ԍ�
		float Pos_X, Pos_Y;	// �ʒu
		float Angle;		// �p�x(�ړ��p)
		double AngleRot;	// �p�x(��]�p)
		double Size;		// �T�C�Y
		float Speed;		// �X�s�[�h(�ړ��p)
		float SpeedRota;	// �X�s�[�h(��]�p)
		float SpeedGravity;	// �d�͉����x
	public:
		// �R���X�g���N�^
		Star() {}
		Star(int ArgX, int ArgY) {
			Pos_X = (float)ArgX; Pos_Y = (float)ArgY;					// �ʒu
			Size = 0.2 + (double)(rand() % 100) / 100 / 2;				// �T�C�Y���Ă��Ƃ���
			Angle = 1.0f * (DX_PI_F);// �ړ��p�p�x�͂Ă��Ƃ��� 180�`360�x
			Speed = (float)(5 + rand() % 5);							// ���̃X�s�[�h��5�`10���炢
			AngleRot = 0;												// ��]�p�x
			SpeedRota = (float)(rand() % 100) / 1000.0f;				// ��]�X�s�[�h�Ă��Ƃ�
			StarGrNum = rand() % 7;										// 7�F�̂ǂꂩ
			SpeedGravity = 0.0f;										// �d�͉����x�̏�����
		}
		void Draw() {	// �`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);	// �A���t�@�u�����h
			DrawRotaGraph((int)Pos_X, (int)Pos_Y, Size, AngleRot, StarGr[StarGrNum], true);	//�`��
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ���ǂ�
		}
		void Move() {	// �ړ�
			Pos_X += cosf(Angle)*Speed;		// �p�x�Ɍ������Ĉړ�
			Pos_Y += sinf(Angle)*Speed;
			if (Speed > 0) Speed -= 0.1f;	// �������Ă���
			AngleRot += SpeedRota;			// ��]����
			Pos_Y += SpeedGravity;			// ���R�������Ă���
			SpeedGravity += 0.05f;			// �d�͉����x
		}
		bool Is_Range() {	// ��ʊO������
			if (Pos_X > WINDOW_X || Pos_X < 0 || Pos_Y > WINDOW_Y || Pos_Y < -100) return false;
			return true;
		}
	};

	const int StarNum = 256;		// ���̍ő�l
	static Star *MyStar[StarNum];	// ���̃e�[�u��

	// �����L���O�ɗ��čŏ��̈�񂾂��s������
	if (RankingCnt == 0) {
		StarFont = CreateFontToHandle(NULL, 128, 10);			//���U���g�t�H���g
		for (int i = 0; i < 7; i++) {
			StarGr[i] = MakeScreen(128, 128, true);				// ���p�̃O��������Ƃ��@7�F
			SetDrawScreen(StarGr[i]);							// ��ʂɕ`��
			DrawStringToHandle(0, 0, "��", COLOR[i], StarFont);	// ����`��
		}

		SetDrawScreen(DX_SCREEN_BACK);							// �`�����o�b�N�o�b�t�@�ɖ߂�
		for (int i = 0; i < StarNum; i++) MyStar[i] = NULL;		// �X�^�[�e�[�u�����N���A���Ă���
		RankingFont = CreateFontToHandle("Times", 25, 10);		// ���U���g�t�H���g
		RankingTitleFont = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);	// ���U���g�^�C�g���t�H���g

		for (int i = 0; i < RANKING_NUM - 1; i++) {
			RankingTargetPosY[i] = 120 + (float)i * 38;	// y�̒�ʒu�����߂Ă���
			RankingMovePosY[i] = WINDOW_Y + 100;		// �����ʒu��K���Ɍ��߂Ă���
		}

		// �}�C�X�R�A�������N�C���������ɂ����BGM�ς���
		My_Sound->Change_Bgm(BGM_RANKING_OUT, true);
		//	�����N�C�����Ă���N���ABGM�𗬂���炷
		for (int i = 0; i < RANKING_NUM - 1; i++)
			if (My_RankingData->Is_Flg(i)) {
				My_Sound->Change_Bgm(BGM_RANKING_IN, true);
				break;
			}
	}

	// �ق��Ɋւ��邱��
	if (RankIn) {		// ���߂ł��������o���̂̓����N�C���������ƃf���̂Ƃ�����

		// �ЂƂo�^�����烋�[�v������
		if (RankingCnt % (10 + MyRankNum * 3) == 0) {	// �����N�ɂ���ēo�^�p�x�����߂�
			for (int i = 0; i < StarNum; i++) {			// �e�[�u����T��
				if (MyStar[i] != NULL) continue;		// �g���Ă�Ƃ���̓X�L�b�v
				MyStar[i] = new Star(750, 50);			// �o�^
				break;
			}
		}
		for (int i = 0; i < StarNum; i++) {
			if (MyStar[i] == NULL) continue;	// �Ȃ���΃X�L�b�v

			MyStar[i]->Move();	// �ړ�
			MyStar[i]->Draw();	// �`��

			// �͈͊O�ɏo���肵����폜���ċ󂯂�
			if (!MyStar[i]->Is_Range()) { delete MyStar[i]; MyStar[i] = NULL; }
		}
	}

	// ���[�h�^�C�g���`��
	DrawStringToHandle(60, 30, "Ranking", COLOR[YELLOW], RankingTitleFont);

	// �����L���O�`��
	RankIn = false;
	for (int i = 0; i < RANKING_NUM - 1; i++) {	// 10�Ԗڂ̓J�����g�X�R�A�Ȃ̂Ŕ�\��
		My_RankingData->Get_String(i, Buf);	// �w�胉���N�̕���������炤
		if (My_RankingData->Get_Clear_Time(i) <= 0) Buf[0] = NULL;	// �X�R�A0�͕\�����Ȃ�
		sprintf_s(BufLine, 256, "%2d :%s", i + 1, Buf);	// �����L���O�\���́A1st����
		int StrColor = GetColor(220, 220, 220);	// �f�t�H���g�J���[
		if (My_RankingData->Is_Flg(i)) {		// �}�C�X�R�A��������
			StrColor = COLOR[rand() % 7];	// ��������������
			RankIn = true;	// �����N�C��������
			MyRankNum = i;	// �����N���L�^
		}
		const float FitSpd = 10.0f;		// �t�B�b�g���Ă����X�s�[�h
		if (i > 0) {
			if (RankingMovePosY[i - 1] < 100.0f) {	// �O�̃����N��������x��ʒu�ɂ�����
				RankingMovePosY[i] -= RankingMovePosY[i] / FitSpd;	// 0�ɋ߂Â�
			}
		}
		else RankingMovePosY[i] -= RankingMovePosY[i] / FitSpd;	// 0�Ԃ͖�������0�ɋ߂Â�
		DrawStringToHandle(100, (int)(RankingTargetPosY[i] + RankingMovePosY[i]), BufLine, StrColor, RankingFont);	// �`��
	}


	// ���ʂɑ΂���R�����g
	if (RankingMovePosY[9] < 90.0f) {	// �����N10��������x��ʒu�ɂ�����
		if (!RankIn) {
			// �����N�C�����Ă��Ȃ�������c�O�ł����E�B�E �ǂ̂��炢�y�΂Ȃ����������m�F
			My_Time->Get_Total_Score_Str(Buf);		// �����̃X�R�A����������炢
			sprintf_s(BufLine, 256, "Out of Rank : %s", Buf);	// ���`����
			DrawString(TEXT_RANKOUT_X, TEXT_RANKOUT_Y, BufLine, GetColor(255, 128, 255));	// ���̕��ɕ\��
		}
		else {
			// �����N�C�����Ă����炨�߂łƂ�!
			DrawString(TEXT_RANKIN_X, TEXT_RANKIN_Y, "Congratulations for Rank In!", GetColor(255, 32, 0));	// ���̕��ɕ\��
		}
	}

	// ���̓��b�Z�[�W
	if (RankingCnt > 200) {
		if (RankingCnt % 100 < 70) DrawString(TEXT_INPUT_X, TEXT_INPUT_Y, "Click Window to Menu", COLOR[WHITE]);	// ���b�Z�[�W�̕\��
		if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {	//	���N���b�N
			RankingCnt = 0;	// �J�E���^�N���A
			DeleteFontToHandle(RankingFont);	// �쐬�����t�H���g�f�[�^���폜����
			DeleteFontToHandle(RankingTitleFont);	// �쐬�����t�H���g�f�[�^���폜����
			for (int i = 0; i < 7; i++) DeleteGraph(StarGr[i]);	// �^�C�g���̃O���t�B�b�N��j������
			My_Sound->Play_Se(SE_DECISION);
			return MENU_INIT;
		}
	}

	RankingCnt++;	// �J�E���^�A�b�v

	return RANKING;
}