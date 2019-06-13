//	���͊Ǘ�

#pragma once
#include"DxLib.h"
#include"Literal.h"

////////////	���͊֘A
const int INPUT_KEY_NUM = 8;		// �L�[��
const int KEY_LOG_LENGTH = 100000;	// �L�[���O�̒���
const enum INPUT_NUMBER {
	MOUSE_MIDDLE, MOUSE_LEFT, MOUSE_RIGHT,
	KEY_Z, KEY_X, KEY_C, KEY_LSHIFT,KEY_SPEACE
};
const enum KEY_TYPE {
	TYPE_LCLICK, TYPE_LSHIFT, TYPE_SPACE
};


class Input_Controll {

	//	���̓}�X�^�[
	bool Input_Table[INPUT_KEY_NUM];			//	�L�[�X�L�����e�[�u��
	bool Input_Table_Once[INPUT_KEY_NUM];		//	�g���K�[�e�[�u��

	//	�L�[�{�[�h�̓��̓o�b�t�@
	bool Input_Key[INPUT_KEY_NUM];
	bool Input_Key_Once[INPUT_KEY_NUM];
	char Key_Buf_Main[256];	//	���̃L�[���͏��̃o�b�t�@
	char Key_Buf_Post[256];	//	�O�̃t���[���̃L�[���̓o�b�t�@
	char Mouse_Buf_Main[3];
	char Mouse_Buf_Post[3];

	BYTE Key_Log[KEY_LOG_LENGTH];		//	�L�[�{�[�h�L���v�`���p�̃e�[�u��
	BYTE Key_Log_Once[KEY_LOG_LENGTH];	//	�L�[�{�[�h�L���v�`���p�̃e�[�u��Once
	int Key_Log_Index;	//	�L�[���O�̃C���f�b�N�X
	int Key_Type;		//	�L�[����̃^�C�v �^�C�v�ő�����@��ς���

public:

	//	�R���X�g���N�^
	Input_Controll()
	{
		//	�t���O�𗎂Ƃ��Ƃ�
		for (int i = 0; i < INPUT_KEY_NUM; i++)
			Input_Table[i] = Input_Table_Once[i] = false;
		for (int i = 0; i < 256; i++)
			Key_Buf_Post[i] = Key_Buf_Main[i] = 0;
		Key_Log_Index = 0;
		Key_Type = TYPE_LCLICK;

		//	�L�[���O�̓ǂݍ���
		FILE *KeyLogFp;
		errno_t err;
		err = fopen_s(&KeyLogFp, "Data/KeyLog.dat", "rb");
		if (err != 0) return;
		ZeroMemory(Key_Log, KEY_LOG_LENGTH);
		fread(Key_Log, sizeof(BYTE), KEY_LOG_LENGTH, KeyLogFp);
		fclose(KeyLogFp);

		//	Once
		err = fopen_s(&KeyLogFp, "Data/KeyLogOnce.dat", "rb");
		if (err != 0) return;
		ZeroMemory(Key_Log_Once, KEY_LOG_LENGTH);
		fread(Key_Log_Once, sizeof(BYTE), KEY_LOG_LENGTH, KeyLogFp);
		fclose(KeyLogFp);
	}


	//	�f�X�g���N�^
~Input_Controll()
	{

		//	�L�[���O�̕ۑ�
		FILE *KeyLogFp;
		errno_t err;
		err = fopen_s(&KeyLogFp, "Data/KeyLog.dat", "wb");
		if (err != 0) return;
		fwrite(Key_Log, sizeof(BYTE), KEY_LOG_LENGTH, KeyLogFp);
		fclose(KeyLogFp);

		//	Once
		err = fopen_s(&KeyLogFp, "Data/KeyLogOnce.dat", "wb");
		if (err != 0) return;
		fwrite(Key_Log_Once, sizeof(BYTE), KEY_LOG_LENGTH, KeyLogFp);
		fclose(KeyLogFp);
	}


	//	�X�V
	void Update(int GAME_MODE)
	{
		Scan_Key_board();

		// ���̓}�X�^�[�e�[�u���փ}�[�W 
		for (int i = 0; i < INPUT_KEY_NUM; i++) {
			Input_Table[i] = (Input_Key[i]) ? true : false;
			Input_Table_Once[i] = (Input_Key_Once[i]) ? true : false;
		}

		// �v���C�̋L�^������ăt�@�C���o�͂���Ƃ������L���ɂ���
		if (GAME_MODE == PLAY) Record_Key_Log();
	}


	//	�L�[���O���Ƃ�
	void Record_Key_Log()
	{

		BYTE KeyBuf = NULL;	// �L�[�̉�����Ԃ�����1�o�C�g�ɋL�^����
		if (Input_Table[MOUSE_MIDDLE])	KeyBuf |= 0x01;	// 1�r�b�g��
		if (Input_Table[MOUSE_LEFT])KeyBuf |= 0x02;
		if (Input_Table[MOUSE_RIGHT])KeyBuf |= 0x04;
		if (Input_Table[KEY_Z])KeyBuf |= 0x08;
		if (Input_Table[KEY_X])KeyBuf |= 0x10;
		if (Input_Table[KEY_C])KeyBuf |= 0x20;
		if (Input_Table[KEY_LSHIFT])KeyBuf |= 0x40;
		if (Input_Table[KEY_SPEACE])KeyBuf |= 0x80;

		// �e�[�u���ɋL�^
		if (Key_Log_Index < KEY_LOG_LENGTH) Key_Log[Key_Log_Index] = KeyBuf;


		// Once
		KeyBuf = NULL;
		if (Input_Table_Once[MOUSE_MIDDLE])	KeyBuf |= 0x01;	// 1�r�b�g��
		if (Input_Table_Once[MOUSE_LEFT])KeyBuf |= 0x02;
		if (Input_Table_Once[MOUSE_RIGHT])KeyBuf |= 0x04;
		if (Input_Table_Once[KEY_Z])KeyBuf |= 0x08;
		if (Input_Table_Once[KEY_X])KeyBuf |= 0x10;
		if (Input_Table_Once[KEY_C])KeyBuf |= 0x20;
		if (Input_Table_Once[KEY_LSHIFT])KeyBuf |= 0x40;
		if (Input_Table_Once[KEY_SPEACE])KeyBuf |= 0x80;

		// �e�[�u���ɋL�^
		if (Key_Log_Index < KEY_LOG_LENGTH) Key_Log_Once[Key_Log_Index] = KeyBuf;

		Key_Log_Index++;
	}


	//	�L�[�ƃ}�E�X���͂̃X�L����
	void Scan_Key_board()
	{
		//	�}�E�X�̓��͏��
		Mouse_Buf_Main[0] = (GetMouseInput() & MOUSE_INPUT_MIDDLE);
		Mouse_Buf_Main[1] = (GetMouseInput() & MOUSE_INPUT_LEFT);
		Mouse_Buf_Main[2] = (GetMouseInput() & MOUSE_INPUT_RIGHT);

		//	�N���b�N���̏��
		Input_Key[MOUSE_MIDDLE] = (Mouse_Buf_Main[MOUSE_INPUT_MIDDLE] != 0) ? true : false;
		Input_Key[MOUSE_LEFT] = (Mouse_Buf_Main[MOUSE_INPUT_LEFT] != 0) ? true : false;
		Input_Key[MOUSE_RIGHT] = (Mouse_Buf_Main[MOUSE_INPUT_RIGHT] != 0) ? true : false;

		//	��x�݂̂̃N���b�N
		Input_Key_Once[MOUSE_MIDDLE] = (Mouse_Buf_Main[MOUSE_INPUT_MIDDLE] != 0 && Mouse_Buf_Post[MOUSE_INPUT_MIDDLE] == 0) ? true : false;
		Input_Key_Once[MOUSE_LEFT] = (Mouse_Buf_Main[MOUSE_INPUT_LEFT] != 0 && Mouse_Buf_Post[MOUSE_INPUT_LEFT] == 0) ? true : false;
		Input_Key_Once[MOUSE_RIGHT] = (Mouse_Buf_Main[MOUSE_INPUT_RIGHT] != 0 && Mouse_Buf_Post[MOUSE_INPUT_RIGHT] == 0) ? true : false;

		// �L�[�̓��͏��
		GetHitKeyStateAll(Key_Buf_Main);
		Input_Key[KEY_Z] = (Key_Buf_Main[KEY_INPUT_Z] != 0) ? true : false;
		Input_Key[KEY_X] = (Key_Buf_Main[KEY_INPUT_X] != 0) ? true : false;
		Input_Key[KEY_C] = (Key_Buf_Main[KEY_INPUT_C] != 0) ? true : false;
		Input_Key[KEY_LSHIFT] = (Key_Buf_Main[KEY_INPUT_LSHIFT] != 0) ? true : false;
		Input_Key[KEY_SPEACE] = (Key_Buf_Main[KEY_INPUT_SPACE] != 0) ? true : false;

		//	��x�݂̂̃L�[����
		Input_Key_Once[KEY_Z] = (Key_Buf_Main[KEY_INPUT_Z] != 0 && Key_Buf_Post[KEY_INPUT_Z] == 0) ? true : false;
		Input_Key_Once[KEY_X] = (Key_Buf_Main[KEY_INPUT_X] != 0 && Key_Buf_Post[KEY_INPUT_X] == 0) ? true : false;
		Input_Key_Once[KEY_C] = (Key_Buf_Main[KEY_INPUT_C] != 0 && Key_Buf_Post[KEY_INPUT_C] == 0) ? true : false;
		Input_Key_Once[KEY_LSHIFT] = (Key_Buf_Main[KEY_INPUT_LSHIFT] != 0 && Key_Buf_Post[KEY_INPUT_LSHIFT] == 0) ? true : false;
		Input_Key_Once[KEY_SPEACE] = (Key_Buf_Main[KEY_INPUT_SPACE] != 0 && Key_Buf_Post[KEY_INPUT_SPACE] == 0) ? true : false;

		// ���̃L�[��Ԃ�Post�ɂƂ��Ă���
		for (int i = 0; i < 3; i++) Mouse_Buf_Post[i] = Mouse_Buf_Main[i];
		for (int i = 0; i < 256; i++) Key_Buf_Post[i] = Key_Buf_Main[i];
	}

	//	�L�[���O�̃C���f�b�N�X���N���A
	void Clear_KeyLog_Counter() { Key_Log_Index = 0; }

	//	�L�[�X�L�����f�[�^�̃Q�b�g
	bool Is_Input(int arg) { return Input_Table[arg]; }

	//	�L�[�g���K�[�f�[�^�̃Q�b�g
	bool Is_Input_Once(int arg) { return Input_Table_Once[arg]; }

	///	�}�E�X�ʒu���Q�b�g
	void Get_Mouse_Pointer(float* mousex, float* mousey)
	{
		int x, y;
		GetMousePoint(&x, &y);

		*mousex = (float)x;
		*mousey = (float)y;
	}
};
