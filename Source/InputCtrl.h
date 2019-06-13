//	入力管理

#pragma once
#include"DxLib.h"
#include"Literal.h"

////////////	入力関連
const int INPUT_KEY_NUM = 8;		// キー数
const int KEY_LOG_LENGTH = 100000;	// キーログの長さ
const enum INPUT_NUMBER {
	MOUSE_MIDDLE, MOUSE_LEFT, MOUSE_RIGHT,
	KEY_Z, KEY_X, KEY_C, KEY_LSHIFT,KEY_SPEACE
};
const enum KEY_TYPE {
	TYPE_LCLICK, TYPE_LSHIFT, TYPE_SPACE
};


class Input_Controll {

	//	入力マスター
	bool Input_Table[INPUT_KEY_NUM];			//	キースキャンテーブル
	bool Input_Table_Once[INPUT_KEY_NUM];		//	トリガーテーブル

	//	キーボードの入力バッファ
	bool Input_Key[INPUT_KEY_NUM];
	bool Input_Key_Once[INPUT_KEY_NUM];
	char Key_Buf_Main[256];	//	今のキー入力情報のバッファ
	char Key_Buf_Post[256];	//	前のフレームのキー入力バッファ
	char Mouse_Buf_Main[3];
	char Mouse_Buf_Post[3];

	BYTE Key_Log[KEY_LOG_LENGTH];		//	キーボードキャプチャ用のテーブル
	BYTE Key_Log_Once[KEY_LOG_LENGTH];	//	キーボードキャプチャ用のテーブルOnce
	int Key_Log_Index;	//	キーログのインデックス
	int Key_Type;		//	キー操作のタイプ タイプで操作方法を変える

public:

	//	コンストラクタ
	Input_Controll()
	{
		//	フラグを落としとく
		for (int i = 0; i < INPUT_KEY_NUM; i++)
			Input_Table[i] = Input_Table_Once[i] = false;
		for (int i = 0; i < 256; i++)
			Key_Buf_Post[i] = Key_Buf_Main[i] = 0;
		Key_Log_Index = 0;
		Key_Type = TYPE_LCLICK;

		//	キーログの読み込み
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


	//	デストラクタ
~Input_Controll()
	{

		//	キーログの保存
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


	//	更新
	void Update(int GAME_MODE)
	{
		Scan_Key_board();

		// 入力マスターテーブルへマージ 
		for (int i = 0; i < INPUT_KEY_NUM; i++) {
			Input_Table[i] = (Input_Key[i]) ? true : false;
			Input_Table_Once[i] = (Input_Key_Once[i]) ? true : false;
		}

		// プレイの記録を取ってファイル出力するときだけ有効にする
		if (GAME_MODE == PLAY) Record_Key_Log();
	}


	//	キーログをとる
	void Record_Key_Log()
	{

		BYTE KeyBuf = NULL;	// キーの押下状態をこの1バイトに記録する
		if (Input_Table[MOUSE_MIDDLE])	KeyBuf |= 0x01;	// 1ビット目
		if (Input_Table[MOUSE_LEFT])KeyBuf |= 0x02;
		if (Input_Table[MOUSE_RIGHT])KeyBuf |= 0x04;
		if (Input_Table[KEY_Z])KeyBuf |= 0x08;
		if (Input_Table[KEY_X])KeyBuf |= 0x10;
		if (Input_Table[KEY_C])KeyBuf |= 0x20;
		if (Input_Table[KEY_LSHIFT])KeyBuf |= 0x40;
		if (Input_Table[KEY_SPEACE])KeyBuf |= 0x80;

		// テーブルに記録
		if (Key_Log_Index < KEY_LOG_LENGTH) Key_Log[Key_Log_Index] = KeyBuf;


		// Once
		KeyBuf = NULL;
		if (Input_Table_Once[MOUSE_MIDDLE])	KeyBuf |= 0x01;	// 1ビット目
		if (Input_Table_Once[MOUSE_LEFT])KeyBuf |= 0x02;
		if (Input_Table_Once[MOUSE_RIGHT])KeyBuf |= 0x04;
		if (Input_Table_Once[KEY_Z])KeyBuf |= 0x08;
		if (Input_Table_Once[KEY_X])KeyBuf |= 0x10;
		if (Input_Table_Once[KEY_C])KeyBuf |= 0x20;
		if (Input_Table_Once[KEY_LSHIFT])KeyBuf |= 0x40;
		if (Input_Table_Once[KEY_SPEACE])KeyBuf |= 0x80;

		// テーブルに記録
		if (Key_Log_Index < KEY_LOG_LENGTH) Key_Log_Once[Key_Log_Index] = KeyBuf;

		Key_Log_Index++;
	}


	//	キーとマウス入力のスキャン
	void Scan_Key_board()
	{
		//	マウスの入力情報
		Mouse_Buf_Main[0] = (GetMouseInput() & MOUSE_INPUT_MIDDLE);
		Mouse_Buf_Main[1] = (GetMouseInput() & MOUSE_INPUT_LEFT);
		Mouse_Buf_Main[2] = (GetMouseInput() & MOUSE_INPUT_RIGHT);

		//	クリック中の情報
		Input_Key[MOUSE_MIDDLE] = (Mouse_Buf_Main[MOUSE_INPUT_MIDDLE] != 0) ? true : false;
		Input_Key[MOUSE_LEFT] = (Mouse_Buf_Main[MOUSE_INPUT_LEFT] != 0) ? true : false;
		Input_Key[MOUSE_RIGHT] = (Mouse_Buf_Main[MOUSE_INPUT_RIGHT] != 0) ? true : false;

		//	一度のみのクリック
		Input_Key_Once[MOUSE_MIDDLE] = (Mouse_Buf_Main[MOUSE_INPUT_MIDDLE] != 0 && Mouse_Buf_Post[MOUSE_INPUT_MIDDLE] == 0) ? true : false;
		Input_Key_Once[MOUSE_LEFT] = (Mouse_Buf_Main[MOUSE_INPUT_LEFT] != 0 && Mouse_Buf_Post[MOUSE_INPUT_LEFT] == 0) ? true : false;
		Input_Key_Once[MOUSE_RIGHT] = (Mouse_Buf_Main[MOUSE_INPUT_RIGHT] != 0 && Mouse_Buf_Post[MOUSE_INPUT_RIGHT] == 0) ? true : false;

		// キーの入力情報
		GetHitKeyStateAll(Key_Buf_Main);
		Input_Key[KEY_Z] = (Key_Buf_Main[KEY_INPUT_Z] != 0) ? true : false;
		Input_Key[KEY_X] = (Key_Buf_Main[KEY_INPUT_X] != 0) ? true : false;
		Input_Key[KEY_C] = (Key_Buf_Main[KEY_INPUT_C] != 0) ? true : false;
		Input_Key[KEY_LSHIFT] = (Key_Buf_Main[KEY_INPUT_LSHIFT] != 0) ? true : false;
		Input_Key[KEY_SPEACE] = (Key_Buf_Main[KEY_INPUT_SPACE] != 0) ? true : false;

		//	一度のみのキー入力
		Input_Key_Once[KEY_Z] = (Key_Buf_Main[KEY_INPUT_Z] != 0 && Key_Buf_Post[KEY_INPUT_Z] == 0) ? true : false;
		Input_Key_Once[KEY_X] = (Key_Buf_Main[KEY_INPUT_X] != 0 && Key_Buf_Post[KEY_INPUT_X] == 0) ? true : false;
		Input_Key_Once[KEY_C] = (Key_Buf_Main[KEY_INPUT_C] != 0 && Key_Buf_Post[KEY_INPUT_C] == 0) ? true : false;
		Input_Key_Once[KEY_LSHIFT] = (Key_Buf_Main[KEY_INPUT_LSHIFT] != 0 && Key_Buf_Post[KEY_INPUT_LSHIFT] == 0) ? true : false;
		Input_Key_Once[KEY_SPEACE] = (Key_Buf_Main[KEY_INPUT_SPACE] != 0 && Key_Buf_Post[KEY_INPUT_SPACE] == 0) ? true : false;

		// 今のキー状態をPostにとっておく
		for (int i = 0; i < 3; i++) Mouse_Buf_Post[i] = Mouse_Buf_Main[i];
		for (int i = 0; i < 256; i++) Key_Buf_Post[i] = Key_Buf_Main[i];
	}

	//	キーログのインデックスをクリア
	void Clear_KeyLog_Counter() { Key_Log_Index = 0; }

	//	キースキャンデータのゲット
	bool Is_Input(int arg) { return Input_Table[arg]; }

	//	キートリガーデータのゲット
	bool Is_Input_Once(int arg) { return Input_Table_Once[arg]; }

	///	マウス位置をゲット
	void Get_Mouse_Pointer(float* mousex, float* mousey)
	{
		int x, y;
		GetMousePoint(&x, &y);

		*mousex = (float)x;
		*mousey = (float)y;
	}
};
