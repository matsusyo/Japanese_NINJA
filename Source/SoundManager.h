#pragma once

#include "DxLib.h"		// DX���C�u�����̖{��

const int FADE_RATE = 5;	// �t�F�[�h�̃X�s�[�h
const int FADE_MAX = 100;

const int BGM_NUM = 19;	// BGM�̐�

// BackGroundMusic�̃i���o�[
const enum BGM {
	BGM_PLAY,			//	�v���C���ʏ펞
	BGM_PLAY_FOUND,		//	�v���C���G�ɔ�����
	BGM_OVER,			//	�Q�[���I�[�o�[
	BGM_CLEAR,			//	�Q�[���N���A
	BGM_RANKING_IN,		//	�����N�C��
	BGM_RANKING_OUT,	//	�����N�A�E�g
	BGM_TITLE,			//	�^�C�g��
	BGM_MENU            //	���j���[ �X�e�[�W�I����V�ѕ����܂�
};

// BGM�̃t�@�C�������X�g
static const char *BGM_FILE_NAME[BGM_NUM] = {
	"BGM_PLAY2.mp3",        // �ʏ�
	"BGM_PLAY3.mp3",		// ������
	"BGM_OVER.mp3",         // �Q�[���I�[�o�[ 
	"BGM_CLEAR.mp3",        // �Q�[���N���A
	"BGM_RANKING_IN.mp3",   // �����N�C��
	"BGM_RANKING_OUT.mp3",  // �����N�A�E�g
	"BGM_TITLE.mp3",        // �^�C�g��
	"BGM_MENU.mp3"          // ���j���[
};

const int SE_NUM = 15;	// SE�̐�

// SoundEffect�̃i���o�[
const enum SE {
	SE_SELECTION,		//	��ʑI��
	SE_DECISION,		//	��ʌ���
	SE_DART_REGISTER,	//	�藠���o�^
	SE_BOMB_REGISTER,	//	���e�o�^
	SE_CLONE_REGISTER,	//	���g�o�^
	SE_DART_SPARK,		//	�藠���Փ�
	SE_BOMB_SPARK,		//	���e�Փ�
	SE_COLLISION_WALL,	//	�ǏՓ�
	SE_COLLISION,       //	�G�ƃv���C���[�Փ�
	SE_ERASE_PLAYER,	//	�v���C���[���S
	SE_ERASE_ENEMY,		//	�G���S
	SE_ERASE_BOSS,      //	�{�X���S
	SE_ALERT,			//	����
	SE_DASH,			//	�_�b�V��
	SE_MISS,			//	�Q�[�W����̎�
};

// SE�̃t�@�C�������X�g
static const char *SE_FILE_NAME[SE_NUM] = {
	"SE_SELECT.mp3",			//	��ʑI��
	"SE_DECISION.mp3",          //	��ʌ���
	"SE_DART_REGISTER.mp3",     //	�藠���o�^
	"SE_BOMB_REGISTER.mp3",     //	���e�o�^
	"SE_CLONE_REGISTER.mp3",    //	���g�o�^
	"SE_DART_SPARK.mp3",	    //	�藠���Փ�
	"SE_BOMB_SPARK.mp3",		//	���e�Փ�
	"SE_COLLISION_WALL.mp3",	//	�ǏՓ�
	"SE_COLLISION.mp3",	        //	�G�ƃv���C���[�Փ�
	"SE_ERASE_PLAYER.mp3",      //	�v���C���[���S
	"SE_ERASE_ENEMY.mp3",       //	�G���S
	"SE_ERASE_BOSS.mp3",        //	�{�X���S
	"SE_ALERT.mp3",				//	����
	"SE_DASH.mp3",				//	�_�b�V��
	"SE_MISS.mp3",				//	�Q�[�W��
};

// �T�E���h�}�l�[�W���[�N���X
class Sound_Manager {

	int Bgm[BGM_NUM];	// BGM�̃n���h��
	int Se[SE_NUM];		// SE�̃n���h��

	int Fade;	// �t�F�[�h�l
	int Bgm_Current;	// �t�F�[�h�C�����Ă���
	int Bgm_Out;		// �t�F�[�h�A�E�g���Ē�~

public:

	// �R���X�g���N�^
	Sound_Manager() {

		char FullPath[256]; // Sound�t�@�C���̂���ꏊ�ւ̃t���p�X

		// BGM�e�[�u���̃N���A
		for (int i = 0; i < BGM_NUM; i++) Bgm[i] = 0;	// �ǂݍ���ł��Ȃ���Ԃɂ��Ă�����
		Bgm[BGM_PLAY] = LoadSoundMem("Sound/BGM_PLAY2.mp3");	// �e�ʂ̏d���Ȃ������炩���ߓǂ�ł���
		Bgm[BGM_PLAY_FOUND] = LoadSoundMem("Sound/BGM_PLAY3.mp3");	// �e�ʂ̏d���Ȃ������炩���ߓǂ�ł���

		// SE�ǂݍ���
		// SE�̓t�@�C���������������S�͏��Ȃ����낤����S���ǂݍ���ł���
		for (int i = 0; i < SE_NUM; i++) {
			sprintf_s(FullPath, "%s%s", "Sound/", SE_FILE_NAME[i]);	// �T�E���h�t�@�C���ւ̃p�X������
			Se[i] = LoadSoundMem(FullPath);	// �������֓ǂݍ���
			ChangeVolumeSoundMem(120, Se[i]);
		}

		Bgm_Current = BGM_TITLE;	// �N�����ɖ炷BGM�̃Z�b�g
		Fade = 0;	// �t�F�[�_�[�������Ă���
	}

	// �f�X�g���N�^
	~Sound_Manager() {
		InitSoundMem();	// �ǂݍ��񂾉���S����
	}

	// �X�V�@���t���[���ĂԂ���
	void Update() {

		// �N���X�t�F�[�h����
		ChangeVolumeSoundMem(Fade, Bgm_Current);	// �t�F�[�h�C��
		ChangeVolumeSoundMem(FADE_MAX - Fade, Bgm_Out);	// �t�F�[�h�A�E�g
		Fade += FADE_RATE;	// �t�F�[�h
		if (Fade > FADE_MAX) {	// �����Ȃ��悤��
			Fade = FADE_MAX;	// �ő�
			if (CheckSoundMem(Bgm_Out) == 1) {
				StopSoundMem(Bgm_Out);	// �t�F�[�h����������Ƃ߂�
				//DeleteSoundMem(Bgm_Out);	// ����Ȃ��̂Ŕj������
			}
		}
	}

	// BGM�̍Đ�
	void Change_Bgm(int bgmnum, bool loop, bool fadesw = true) {	// BGM�ԍ�, ���[�v���邩, �t�F�[�h�C�����邩

		if (CheckSoundMem(Bgm_Out) == 1) StopSoundMem(Bgm_Out);	// �t�F�[�h���ɕς���ꂽ����͎~�߂�

		// �Ȃ̃��[�h
		if (Bgm[bgmnum] == 0) {		// �����ǂݍ��܂�Ă��Ȃ�������
			char FullPath[256];		// Sound�t�@�C���̂���ꏊ�ւ̃t���p�X
			sprintf_s(FullPath, "%s%s", "Sound/", BGM_FILE_NAME[bgmnum]);	// �T�E���h�t�@�C���ւ̃p�X������
			Bgm[bgmnum] = LoadSoundMem(FullPath);	// �������֓ǂݍ���
		}

		Bgm_Out = Bgm_Current;	// �Đ����̉����t�F�[�h�A�E�g��
		Bgm_Current = Bgm[bgmnum];		// �w�肳�ꂽ�Ȃ��J�����g��
		int Loop = DX_PLAYTYPE_BACK;		// ���[�v�ݒ肵�Ȃ�
		if (loop) Loop = DX_PLAYTYPE_LOOP;	// ���[�v�ݒ肷��
		PlaySoundMem(Bgm_Current, Loop, true);	// �J�����g�Ȃ̍Đ�
		if (fadesw) {	// �N���X�t�F�[�h���邩
			// �N���X�t�F�[�h����ꍇ
			ChangeVolumeSoundMem(1, Bgm_Current);	// �t�F�[�h�C������ꍇ�̓{�����[���͍i���Ă���
			Fade = 0;	// �t�F�[�_�[�̏�����
		}
		else {
			// �t�F�[�h���Ȃ��ꍇ
			Fade = FADE_MAX;	// �t�F�[�_�[�͂����Ă���
		}
	}

	// SE�̍Đ�
	void Play_Se(int arg) {
		PlaySoundMem(Se[arg], DX_PLAYTYPE_BACK, true);
	}

};