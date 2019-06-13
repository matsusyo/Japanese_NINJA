#pragma once
#include "TimeManager.h"
#include"Map.h"

const int RANKING_NUM = 10;	// ランキングの数
						
// ランキングクラス
class RankingData {

	// ランキングデータマスタ
	Time_Manager Ranking_List[RANKING_NUM];	// 10番目は追加スコア用に使用。表示は0～9の10位まで
	int Select_Stage;

public:

	// コンストラクタ
	RankingData() {
		// 起動時にロード
		Data_Load();
	}

	// コンストラクタ こっちを使う
	RankingData(int stage) {
		Select_Stage = stage;
		// 起動時にロード
		Data_Load();
	}

	// デストラクタ
	~RankingData() {

		// 終了時にセーブ
		Data_Save();
	}

	// データロード
	errno_t Data_Load() {
		FILE *Fp;	// ファイルポインタ
		errno_t Err;	// エラーコード
		switch (Select_Stage)
		{//	ステージ別の専用ランキングファイルを開く
		default:
		case AAA:
			Err = fopen_s(&Fp, "Data/Ranking1.txt", "rb");
			break;
		case BBB:
			Err = fopen_s(&Fp, "Data/Ranking2.txt", "rb");
			break;
		case CCC:
			Err = fopen_s(&Fp, "Data/Ranking3.txt", "rb");
			break;
		case DDD:
			Err = fopen_s(&Fp, "Data/Ranking4.txt", "rb");
			break;
		case EEE:
			Err = fopen_s(&Fp, "Data/Ranking5.txt", "rb");
			break;
		case FFF:
			Err = fopen_s(&Fp, "Data/Ranking6.txt", "rb");
			break;
		}
		if (Err != 0) return Err;	// オープンエラーならクローズしないで終了
		fread(Ranking_List, sizeof(Time_Manager), RANKING_NUM, Fp);	// ブロックリード
		fclose(Fp);	// ファイルをクローズ
		return Err;	// ロード成功
	}

	// データセーブ
	errno_t Data_Save() {
		// セーブする
		FILE *Fp;	// ファイルポインタ
		errno_t Err;	// エラーコード
		switch (Select_Stage)
		{//	ステージ別の専用ランキングファイルを開く
		default:
		case AAA:
			Err = fopen_s(&Fp, "Data/Ranking1.txt", "wb");
			break;
		case BBB:
			Err = fopen_s(&Fp, "Data/Ranking2.txt", "wb");
			break;
		case CCC:
			Err = fopen_s(&Fp, "Data/Ranking3.txt", "wb");
			break;
		case DDD:
			Err = fopen_s(&Fp, "Data/Ranking4.txt", "wb");
			break;
		case EEE:
			Err = fopen_s(&Fp, "Data/Ranking5.txt", "wb");
			break;
		case FFF:
			Err = fopen_s(&Fp, "Data/Ranking6.txt", "wb");
			break;
		}
		if (Err != 0) return Err;	// オープンエラーならクローズしないで終了
		fwrite(Ranking_List, sizeof(Time_Manager), RANKING_NUM,Fp);	// ブロックライト
		fclose(Fp);	// ファイルをクローズ
		return Err;	// セーブ成功
	}

	// ランキングにタイムを追加
	void Add_Time(Time_Manager *argscore) {
		Ranking_List[RANKING_NUM - 1] = *argscore;	// ランキングテーブルの一番最後に追加して
		Ranking_Sort();		// ソートをかける
	}

	// タイムでソート
	void Ranking_Sort() {
			//	ランキングのクリアタイムが0秒以下だったらNULL
		for (int i = 0; i < RANKING_NUM - 1; i++)
			if (Ranking_List[i].Get_Clear_Time() <= 0)
				Ranking_List[i].Reset_Clear_Time();
		
		// バブルソート 小さい順にソート
		bool Flg = true;
		while (Flg) {	// スワップがある限りループ
			Flg = false;	// フラグ落としておく
			Time_Manager Tmp;
			for (int i = 0; i < RANKING_NUM - 1; i++) {

				//	スワップするタイムがNULLでなかったらスワップ
				if (Ranking_List[i].Get_Clear_Time() > Ranking_List[i + 1].Get_Clear_Time() && (Ranking_List[i + 1].Get_Clear_Time() != NULL)) {
					Tmp = Ranking_List[i];
					Ranking_List[i] = Ranking_List[i + 1];
					Ranking_List[i + 1] = Tmp;
					Flg = true;		// スワップしたよ
				}

				//	タイムがNULL
				else if (Ranking_List[i].Get_Clear_Time() == NULL && Ranking_List[i + 1].Get_Clear_Time() > 0) {
					Tmp = Ranking_List[i];
					Ranking_List[i] = Ranking_List[i + 1];
					Ranking_List[i + 1] = Tmp;
					Flg = true;		// スワップしたよ
				}
			}
		}
	}

	// カレントフラグを落とす
	void Set_Flg_Off_All() { for (int i = 0; i < RANKING_NUM; i++) Ranking_List[i].Set_Flg(false); }

	// 指定ランクのフラグを得る
	bool Is_Flg(int arg) { return Ranking_List[arg].Is_Flg(); }

	// アクセスメソッド
	void Get_String(int arg, char* argstr) {		// 指定順位のスコア文字列をゲット
		char Buf[256];
		Ranking_List[arg].Get_Total_Score_Str(Buf);
		strcpy_s(argstr, 256, Buf);
	}
	float Get_Clear_Time(int arg) { return Ranking_List[arg].Get_Clear_Time();  }	// 指定順位のタイムのゲット
};