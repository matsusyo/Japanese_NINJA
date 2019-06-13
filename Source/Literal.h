//	�萔

#pragma once
#include"DxLib.h"

const char GameTitle[] = "Japanese NINJA No1";

const double FPS = 240.0;

//	�E�B���h�E�T�C�Y
const int WINDOW_X = 800;
const int WINDOW_Y = 600;
const int H_WINDOW_X = WINDOW_X / 2;
const int H_WINDOW_Y = WINDOW_Y / 2;

//	�}�b�v�T�C�Y
const int MAP_X = 72;
const int MAP_Y = 72;

//	�`����W�I�t�Z�b�g �� �}�b�v�`�b�v�A�L�����T�C�Y�̈Ⴂ��N�H�[�^�r���[�ł̕`��Ȃ̂ŕK�v
const int
QTR_OFFSET_X = 0,
QTR_OFFSET_Y = 32;	//	31�`33	32�����I�Ƀx�X�g

//	�Q�[����Ԃ̖��O	
const enum GAME_MODE
{
	TITLE_INIT, TITLE, MENU_INIT, MENU, STAGE, STORY, MANUAL, OPTION, PREPARE, PLAY, OVER, CLEAR, RANKING, EXIT,
};

//	HP,MP,�O�g�Q�[�W
const enum GAUGE_TYPE { HP, FRAME, MP };

//	HP,MP�̍ő�l
const float
HP_MAX = 100,
MP_MAX = 100;

//	�_���[�W��
const float
DART_DAMAGE = -5.0f,	//	�藠�����^����_���[�W
COLLISION_DAMAGE = -10.0f;	//	�G���^����_���[�W

//	�J�E���g�^�C�� �P * �U�O = �P �b
const int
//	���G����
DAMAGE_COUNT = 1 * 60,
E_DAMAGE_COUNT = 0 * 60,
//	���ʂ܂ł̎���
DYING_COUNT = 1 * 60,
E_DYING_COUNT = 1 * 60,
BOSS_DYING_COUNT = 4 * 60;


//	�F�̖��O
const enum COLOR_NAME {
	WHITE, RED, GREEN, BLUE, YELLOW, PURPLE, LIGHTBLUE
};

//	�F���l��
const unsigned int COLOR[] = {
		GetColor(255,255,255),	// ����
		GetColor(255,0,0),		// ����
		GetColor(0,255,0),		// �݂ǂ�
		GetColor(0,0,255),		// ����
		GetColor(255,255,0),	// ������
		GetColor(255,0,255),	// �ނ炳��
		GetColor(0,255,255)		// �݂�����
};

//	�t�H���g�̖��O
const enum FONT_NAME {
	MS_GOTHIC,
	MV_BOLI,
	MEIRIO,
	FONT_MAX,
};

//	�N���b�N���󂯕t����悤�ɂȂ�܂ł̎���
const int CLICK_TIME = 50;	