#include "DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"RankingData.h"
#include"TimeManager.h"
#include"SoundManager.h"

const int TEXT_RANKOUT_X = 100;	//	ランクアウトの時のテキスト座標
const int TEXT_RANKOUT_Y = 475;
const int TEXT_RANKIN_X = 250;	//	ランクインの時のテキスト座標
const int TEXT_RANKIN_Y = 475;
const int TEXT_INPUT_X = 285;	//	入力を促すテキスト座標
const int TEXT_INPUT_Y = 520;

int RankingCnt = 0;	// ランキングカウンタ

extern Input_Controll *Inp_Ctrl;	// 入力
extern RankingData *My_RankingData;	// ランキング
extern Time_Manager *My_Time;	// スコア
extern Sound_Manager *My_Sound;	//	音

// ランキング
int Ranking()
{
	static int RankingFont;						// ランキングフォント
	static int RankingTitleFont;				// ランキングタイトルフォント
	static float RankingTargetPosY[RANKING_NUM];// ターゲットとなるy座標の配列
	static float RankingMovePosY[RANKING_NUM];	// 実際のy座標の配列
	static float RankingTilePosY;				// タイトル位置y
	static int StarGr[7];						// ☆用のグラ 7色
	static int StarFont;						// ☆フォント
	static bool RankIn = false;					// ランクインしたかどうかのフラグ
	static int MyRankNum = 0;					// 順位を示す　
	char Buf[256];								// スコア文字列をもらう
	char BufLine[256];							// 最終的な文字列バッファ

	// ☆クラス
	class Star {
		int StarGrNum;		// ほし番号
		float Pos_X, Pos_Y;	// 位置
		float Angle;		// 角度(移動用)
		double AngleRot;	// 角度(回転用)
		double Size;		// サイズ
		float Speed;		// スピード(移動用)
		float SpeedRota;	// スピード(回転用)
		float SpeedGravity;	// 重力加速度
	public:
		// コンストラクタ
		Star() {}
		Star(int ArgX, int ArgY) {
			Pos_X = (float)ArgX; Pos_Y = (float)ArgY;					// 位置
			Size = 0.2 + (double)(rand() % 100) / 100 / 2;				// サイズをてきとうに
			Angle = 1.0f * (DX_PI_F);// 移動用角度はてきとうに 180～360度
			Speed = (float)(5 + rand() % 5);							// 星のスピードは5～10くらい
			AngleRot = 0;												// 回転角度
			SpeedRota = (float)(rand() % 100) / 1000.0f;				// 回転スピードてきとう
			StarGrNum = rand() % 7;										// 7色のどれか
			SpeedGravity = 0.0f;										// 重力加速度の初期化
		}
		void Draw() {	// 描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);	// アルファブレンド
			DrawRotaGraph((int)Pos_X, (int)Pos_Y, Size, AngleRot, StarGr[StarGrNum], true);	//描画
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// もどす
		}
		void Move() {	// 移動
			Pos_X += cosf(Angle)*Speed;		// 角度に向かって移動
			Pos_Y += sinf(Angle)*Speed;
			if (Speed > 0) Speed -= 0.1f;	// 減速していく
			AngleRot += SpeedRota;			// 回転する
			Pos_Y += SpeedGravity;			// 自由落下していく
			SpeedGravity += 0.05f;			// 重力加速度
		}
		bool Is_Range() {	// 画面外か判定
			if (Pos_X > WINDOW_X || Pos_X < 0 || Pos_Y > WINDOW_Y || Pos_Y < -100) return false;
			return true;
		}
	};

	const int StarNum = 256;		// ☆の最大値
	static Star *MyStar[StarNum];	// ☆のテーブル

	// ランキングに来て最初の一回だけ行う処理
	if (RankingCnt == 0) {
		StarFont = CreateFontToHandle(NULL, 128, 10);			//リザルトフォント
		for (int i = 0; i < 7; i++) {
			StarGr[i] = MakeScreen(128, 128, true);				// ☆用のグラを作っとく　7色
			SetDrawScreen(StarGr[i]);							// 画面に描画
			DrawStringToHandle(0, 0, "★", COLOR[i], StarFont);	// ☆を描画
		}

		SetDrawScreen(DX_SCREEN_BACK);							// 描画先をバックバッファに戻す
		for (int i = 0; i < StarNum; i++) MyStar[i] = NULL;		// スターテーブルをクリアしておく
		RankingFont = CreateFontToHandle("Times", 25, 10);		// リザルトフォント
		RankingTitleFont = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);	// リザルトタイトルフォント

		for (int i = 0; i < RANKING_NUM - 1; i++) {
			RankingTargetPosY[i] = 120 + (float)i * 38;	// yの定位置を決めておく
			RankingMovePosY[i] = WINDOW_Y + 100;		// 初期位置を適当に決めておく
		}

		// マイスコアがランクインしたかによってBGM変える
		My_Sound->Change_Bgm(BGM_RANKING_OUT, true);
		//	ランクインしてたらクリアBGMを流すを鳴らす
		for (int i = 0; i < RANKING_NUM - 1; i++)
			if (My_RankingData->Is_Flg(i)) {
				My_Sound->Change_Bgm(BGM_RANKING_IN, true);
				break;
			}
	}

	// ほしに関すること
	if (RankIn) {		// おめでたい☆を出すのはランクインした時とデモのときだけ

		// ひとつ登録したらループ抜ける
		if (RankingCnt % (10 + MyRankNum * 3) == 0) {	// ランクによって登録頻度を決める
			for (int i = 0; i < StarNum; i++) {			// テーブルを探す
				if (MyStar[i] != NULL) continue;		// 使ってるところはスキップ
				MyStar[i] = new Star(750, 50);			// 登録
				break;
			}
		}
		for (int i = 0; i < StarNum; i++) {
			if (MyStar[i] == NULL) continue;	// なければスキップ

			MyStar[i]->Move();	// 移動
			MyStar[i]->Draw();	// 描画

			// 範囲外に出たりしたら削除して空ける
			if (!MyStar[i]->Is_Range()) { delete MyStar[i]; MyStar[i] = NULL; }
		}
	}

	// モードタイトル描画
	DrawStringToHandle(60, 30, "Ranking", COLOR[YELLOW], RankingTitleFont);

	// ランキング描画
	RankIn = false;
	for (int i = 0; i < RANKING_NUM - 1; i++) {	// 10番目はカレントスコアなので非表示
		My_RankingData->Get_String(i, Buf);	// 指定ランクの文字列をもらう
		if (My_RankingData->Get_Clear_Time(i) <= 0) Buf[0] = NULL;	// スコア0は表示しない
		sprintf_s(BufLine, 256, "%2d :%s", i + 1, Buf);	// ランキング表示は、1stから
		int StrColor = GetColor(220, 220, 220);	// デフォルトカラー
		if (My_RankingData->Is_Flg(i)) {		// マイスコアだったら
			StrColor = COLOR[rand() % 7];	// ちかちかさせる
			RankIn = true;	// ランクインしたよ
			MyRankNum = i;	// ランクを記録
		}
		const float FitSpd = 10.0f;		// フィットしていくスピード
		if (i > 0) {
			if (RankingMovePosY[i - 1] < 100.0f) {	// 前のランクがある程度定位置についたら
				RankingMovePosY[i] -= RankingMovePosY[i] / FitSpd;	// 0に近づく
			}
		}
		else RankingMovePosY[i] -= RankingMovePosY[i] / FitSpd;	// 0番は無条件で0に近づく
		DrawStringToHandle(100, (int)(RankingTargetPosY[i] + RankingMovePosY[i]), BufLine, StrColor, RankingFont);	// 描画
	}


	// 結果に対するコメント
	if (RankingMovePosY[9] < 90.0f) {	// ランク10がある程度定位置についたら
		if (!RankIn) {
			// ランクインしていなかったら残念でした・。・ どのくらい及ばなかったかを確認
			My_Time->Get_Total_Score_Str(Buf);		// 自分のスコア文字列をもらい
			sprintf_s(BufLine, 256, "Out of Rank : %s", Buf);	// 整形して
			DrawString(TEXT_RANKOUT_X, TEXT_RANKOUT_Y, BufLine, GetColor(255, 128, 255));	// 下の方に表示
		}
		else {
			// ランクインしていたらおめでとう!
			DrawString(TEXT_RANKIN_X, TEXT_RANKIN_Y, "Congratulations for Rank In!", GetColor(255, 32, 0));	// 下の方に表示
		}
	}

	// 入力メッセージ
	if (RankingCnt > 200) {
		if (RankingCnt % 100 < 70) DrawString(TEXT_INPUT_X, TEXT_INPUT_Y, "Click Window to Menu", COLOR[WHITE]);	// メッセージの表示
		if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {	//	左クリック
			RankingCnt = 0;	// カウンタクリア
			DeleteFontToHandle(RankingFont);	// 作成したフォントデータを削除する
			DeleteFontToHandle(RankingTitleFont);	// 作成したフォントデータを削除する
			for (int i = 0; i < 7; i++) DeleteGraph(StarGr[i]);	// タイトルのグラフィックを破棄する
			My_Sound->Play_Se(SE_DECISION);
			return MENU_INIT;
		}
	}

	RankingCnt++;	// カウンタアップ

	return RANKING;
}