#pragma once
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>

//	スコアクラス
//	ゲームクリアのリザルトデータを管理する

const int SCORE_STRLENGTH = 256;
const float TIME_LIMIT = 180.0f;		//	制限時間

class Time_Manager {

	bool Flg;						//	カレントプレイヤーのしるし
	char Score_Str[SCORE_STRLENGTH];	//	スコアを一行の文字列にまとめる
	char Player_Name[100];			//	プレイヤー名
	time_t Date;					//	クリアした時の日付を記録する
	float Clear_Time;				//	経過時間を代入する
	float Time_Limit = TIME_LIMIT;	//	残り制限時間を代入


public:

	// コンストラクタ
	Time_Manager() {		// ランキングで使用
		Score_Str[0] = NULL;	// スコア用の文字列
		Player_Name[0] = NULL;	// プレイヤー名
		Date = 0;				// クリア時の日付
		Clear_Time = 0;			//	クリア時間初期化
		Time_Limit = TIME_LIMIT;//	制限時間セット
	}

	~Time_Manager() {

	}

	// スコアのクリア
	void Clear() {
		Score_Str[0] = NULL;	// スコア用の文字列
		Player_Name[0] = NULL;	// プレイヤー名
		Date = 0;				// クリア時の日付
		Clear_Time = 0;			//	クリア時間初期化
		Time_Limit = TIME_LIMIT;//	制限時間セット
	}

	// スコアを一行の文字列にまとめて返す
	void Get_Total_Score_Str(char *arg){
		struct tm Tms;	// 時間の構造体へのポインタ
		localtime_s(&Tms, &Date);	// 記録タイムから日時データを構造体へ取り出す
		char TimeStmpBuf[100];		// 日時用の文字データの確保
		sprintf_s(TimeStmpBuf, 100, "%02d/%02d/%02d %02d:%02d:%02d", Tms.tm_year+1900, Tms.tm_mon+1, Tms.tm_mday, Tms.tm_hour, Tms.tm_min, Tms.tm_sec);
		sprintf_s(Score_Str, 256, "%10s  %09f  %s", Player_Name, Clear_Time, TimeStmpBuf);
		strcpy_s(arg, 256, Score_Str);	// セット
	}

	// アクセスメソッド
	void Set_Flg(bool arg) { Flg = arg; }	// カレントプレイヤーのしるしをセット
	bool Is_Flg() { return Flg; }	// カレントプレイヤーのしるしをゲット

	void SetClear_Time() { Clear_Time = (TIME_LIMIT - Time_Limit); }
	void Reset_Clear_Time() { Clear_Time = NULL; }

	float Get_Clear_Time() { return Clear_Time; }
	
	void Set_Time_Limit() { Time_Limit = TIME_LIMIT - Clear_Time; }
	float Get_Time_Limit() { return Time_Limit; }
	void Add_Time_Limit(float arg) { Time_Limit += arg; }

	void Set_Date() { Date = time(NULL); }		// 今の日付をセット

	void Set_Player_Name(char* arg) { strcpy_s(Player_Name, 100, arg); }	// プレイヤー名のセット
};