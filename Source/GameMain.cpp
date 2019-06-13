//	Japanese NINJA No.1
//	DXライブラリの個人制作
//	忍者を題材としたゲーム
//	制作期間1/10～5/10

//	プリプロセッサ
#include<time.h>			//	時間情報取得
#include"Dxlib.h"			//	DXライブラリ	
#include"Literal.h"			//	定数リテラル
#include"InputCtrl.h"		//	キー,マウス入力クラス
#include"QtrCalc.h"			//	クォータ座標変換クラス
#include"Player.h"			//	プレイヤークラス
#include"EnemyManager.h"	//	敵クラス
#include"Map.h"				//	マップクラス
#include"DartManager.h"		//	手裏剣クラス
#include"BombManager.h"		//	爆弾クラス
#include"Clone.h"			//	分身クラス
#include"RankingData.h"		//	ランキングクラス
#include"TimeManager.h"		//	時間管理クラス
#include"QtrDraw.h"			//	オブジェクト描画クラス
#include"EffectManager.h"	//	エフェクトクラス
#include"SoundManager.h"	//	音クラス
#include"Title.h"			//	タイトル画面クラス
#include"Manual.h"			//	遊び方説明クラス
#include"Menu.h"			//	メニュー画面クラス
#include"Stage.h"			//	ステージ選択画面クラス
#include"Clear.h"			//	ゲームクリア画面クラス
#include"GameOver.h"		//	ゲームオーバー画面クラス

//	オブジェクトのポインタ
Input_Controll *Inp_Ctrl;		//	キー入力
Quarter_Calculation *Qtr_Calc;	//	座標計算
Player *Ply;					//	プレイヤー管理
Enemy_Manager *Eny;				//	敵管理
Map *My_Map;					//	マップ
Dart_Manager *Ply_Dart;			//	手裏剣
Bomb_Manager *Ply_Bomb;			//	爆弾
Clone *Ply_Clone;				//	分身
RankingData *My_RankingData;	//	ランキング管理
Time_Manager *My_Time;			//	クリアタイム、経過時間などを管理
Qtr_Draw_Manager *My_Draw;		//	オブジェクトの描画もまとめて管理
Effect_Manager *My_Effect;		//	エフェクト　手裏剣や爆弾が衝突
Sound_Manager *My_Sound;		//	音　BGMやSE音
Title *My_Title;				//	タイトル画面
Manual *My_Manual;				//	遊び方説明
Menu *My_Menu;					//	メニュー画面
Stage *My_Stage;				//	ステージ選択画面
Clear *My_Clear;				//	ゲームクリア画面
Game_Over *My_Over;				//	ゲームオーバー画面

//	関数プロトタイプ
void Mode_Ctrl();	// ゲームモード管理
int Prepare();		// ゲーム本編に必要なオブジェクトを生成
int Game_Main();	// ゲーム本編プレイ中
void Rel_Obj();		// ゲーム本編で使ったオブジェクトを破棄
int Judge();		// 全ての当たり判定

//	外部関数のプロトタイプ
extern int Ranking();	// ランキング画面

//	グローバル
int Game_Mode = TITLE_INIT;
char PlayerName[256];				//	ユーザー名の仮配

//	メインエントリーポイント
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	SetMainWindowText(GameTitle);			//	実行exe名をゲームタイトル名に変更
	ChangeWindowMode(TRUE);					//	ウィンドウモードをフルスクリーンから変更
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);	//	800*600のウィンドウにする
	SetMouseDispFlag(TRUE);					//	マウスカーソルを表示
	if( DxLib_Init() == -1 ) return -1;		//	DXライブラリ初期化処理　エラーが起きたら終了
    SetDrawScreen( DX_SCREEN_BACK ) ;		// 描画先画面をセカンダリサーフェスにする	

	Inp_Ctrl = new Input_Controll;			//	キーとマウス入力のオブジェクト
	My_Time = new Time_Manager;				//	プレイ時間とタイムアタックのタイム計測
	My_Sound = new Sound_Manager;			//	音 BGMとかSE
	My_Sound->Change_Bgm(BGM_TITLE, true);	//	タイトル画面音をセット

	// ランキングで使うユーザーネームを得る
	DWORD dwSize = sizeof(PlayerName);	//	サイズをDWORD型へ
	GetUserName(PlayerName, &dwSize);	//	ユーザー名をもらう
	My_Time->Set_Player_Name(PlayerName);	//	ユーザー名のセット


	//	メインループ
	while (true) {
		if (ProcessMessage() == -1)break; //	メッセージループを処理する　エラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) != 0 || Game_Mode == EXIT)break; //	エスケープキーが押されたら終了

		ClearDrawScreen();			//	画面を初期化してクリアにする
		Inp_Ctrl->Update(Game_Mode);//	キーやマウス入力情報の更新
		Mode_Ctrl();				//	ゲームモードの管理
		My_Sound->Update();			//	音の処理
		ScreenFlip();				//	セカンダリサーフェス画面の内容をプライマリサーフェスへ

	}

	//	オブジェクトの後始末
	Rel_Obj();	//	ゲーム本編で使うオブジェクトを破棄

	//	それ以外のオブジェクトも破棄
	
	if (My_Title != NULL) { delete My_Title; My_Title = NULL; }			//	タイトル
	if (My_Manual != NULL) { delete My_Manual; My_Manual = NULL; }		//	遊び方
	if (Inp_Ctrl != NULL) {delete Inp_Ctrl; Inp_Ctrl = NULL; }			//	入力
	if (My_RankingData != NULL) {delete My_RankingData; My_RankingData = NULL; }	//	ランキング
	if (My_Time != NULL) { delete  My_Time; My_Time = NULL; }			//	タイム
	if (My_Sound != NULL) { delete My_Sound; My_Sound = NULL; }			//	音
	DxLib_End();		//	DXライブラリ使用の終了処理	
	
	return 0;	//	ソフト終了
}

//	ゲームモードのコントロール
void Mode_Ctrl()
{
	//	関数の返り値でゲームモードを変える
	switch (Game_Mode) {

		//	タイトルの前に初期化
	case TITLE_INIT:
		if (My_Title == NULL) My_Title = new Title;
		Game_Mode = TITLE;
		break;

		//	タイトル画面
	case TITLE:
		switch (My_Title->Update()) {
		case MENU_INIT:	//	MENUが返り値ならメニュー画面へ移行
			Game_Mode = MENU_INIT;
			My_Sound->Change_Bgm(BGM_MENU, true);	//	メニュー画面音をセット
			break;
		default:	//	それ以外ならタイトル画面を維持する
			break;
		}
		break;


		//	メニューの前に初期化
	case MENU_INIT:
		if (My_Manual == NULL) My_Manual = new Manual;
		if (My_Menu == NULL)My_Menu = new Menu;
		Game_Mode = MENU;
		break;


		//	メニュー画面
	case MENU:
		switch (My_Menu->Update()) {
		case STAGE:		//	STAGEが返り値ならステージ選択画面へ移行
			if (My_Stage == NULL)My_Stage = new Stage;
			Game_Mode = STAGE;
			break;
		case MANUAL:	//	MANUALなら遊び方説明画面へ
			Game_Mode = MANUAL;
			break;
		case TITLE_INIT:		//	TITLEならタイトル画面へ
			Game_Mode = TITLE_INIT;
			My_Sound->Change_Bgm(BGM_TITLE, true);	//	メニュー画面音をセット
			break;
		case EXIT:	//	EXITならゲーム終了
			Game_Mode = EXIT;
			break;
		default:		//	それ以外ならメニュー画面維持
			break;
		}
		break;


		//	ステージ選択画面
	case STAGE:
		switch (My_Stage->Update()) {
		case PREPARE:	//	PREPAREならゲーム本編に必要なオブジェクトを初期化
			Game_Mode = PREPARE;
			break;
		default:		//	それ以外なら維持
			break;
		}
		break;


		//	遊び方説明画面
	case MANUAL:
		switch (My_Manual->Update()) {
		case MENU:	//	MENUならメニュー画面へ
			Game_Mode = MENU;
			break;
		default:	//	それ以外なら維持
			break;
		}
		break;


		//	ゲーム本編初期化
	case PREPARE:
		switch (Prepare()) {
		case PLAY:	//	PLAYならゲーム本編プレイ画面へ
			Game_Mode = PLAY;
			if (My_Title != NULL) { delete My_Title; My_Title = NULL; }
			if (My_Menu != NULL) { delete My_Menu; My_Menu = NULL; }
			if (My_Manual != NULL) { delete My_Manual; My_Manual = NULL; }
			My_Sound->Change_Bgm(BGM_PLAY, true);	//	プレイ画面音をセット
			break;
		default:	//	それ以外なら維持
			break;
		}
		break;


		//	ゲーム本編プレイ画面
	case PLAY:

		//	返り値で処理を振り分け
		switch (Game_Main()) {

			//	プレイ中
		case PLAY:
			break;

			//	ゲームオーバー
		case OVER:
			if (My_Over == NULL)My_Over = new Game_Over;
			Game_Mode = OVER;	//	ゲームオーバー画面に移行	
			My_Sound->Change_Bgm(BGM_OVER, true);	//	ゲームオーバー画面音をセット
			break;

			//	ゲームクリア
		case CLEAR:
			//	ここでクリアタイムとクリア時の日付を記録
			My_Time->SetClear_Time();		//	現在の時間ーゲームプレイ開始時間
			My_Time->Set_Date();			//	クリア時の日付を記録
			My_RankingData->Add_Time(My_Time);	//	ランキングに追加
			if (My_Clear == NULL)My_Clear = new Clear;
			Game_Mode = CLEAR;	//	ゲームクリア画面に移行
			My_Sound->Change_Bgm(BGM_CLEAR, true);	//	ゲームクリア画面音をセット
			break;

		default:	//	それ以外なら維持
			break;
		}
		break;


		//	ゲームオーバー画面
	case OVER:
		switch (My_Over->Update()) {
		case MENU_INIT:	//	MENUが返り値ならメニュー画面へ
			if (My_Over != NULL) { delete My_Over; My_Over = NULL; }
			Game_Mode = MENU_INIT;
			My_Time->Clear();	//	時間をリセット
			My_Sound->Change_Bgm(BGM_MENU, true);
			Rel_Obj();			//	ゲーム本編で使ったオブジェクトのみ破棄する	
			break;
		default:	//	それ以外なら維持
			break;
		}
		break;


		//	ゲームクリア画面
	case CLEAR:
		switch (My_Clear->Update()) {
		case RANKING:	//	RANKINGが返り値ならランキング画面へ
			if (My_Clear != NULL) { delete My_Clear; My_Clear = NULL; }
			Game_Mode = RANKING;
			Rel_Obj();			//	ゲーム本編で使ったオブジェクトのみ破棄する
			break;
		default:		//	それ以外なら維持
			break;
		}
		break;


		//	ランキング画面
	case RANKING:
		switch (Ranking()) {
		case MENU_INIT:	//	TITLEが返り値ならタイトル画面へ
			Game_Mode = MENU_INIT; 
			My_RankingData->Data_Save();	// ランキングデータをセーブ
			My_Time->Clear();	//	時間をリセット
			My_Sound->Change_Bgm(BGM_MENU, true);
			break;
		default:	//	それ以外なら維持
			break;
		}
		break;


		//	予期せぬ返り値なら終了
	default:
		Game_Mode = EXIT;
		break;
	}
}


enum SCREEN_MODE {  // スクリーンモード
	NO_FADEOUT, FADEOUT_SCREEN, FADEOUT_FONT
};
//	ゲーム本編で使うオブジェクトを生成
int Prepare()
{
	static int FadeOutMode = NO_FADEOUT;  // フェードアウトなし
	static float Alpha = 255.0f;   // 透明度
	static float AlphaSpeed = 2.5f;  // 透明化速度
	static int FontHandle[3];	//	フォント

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)Alpha);  // 半透明描画
	if (FadeOutMode == NO_FADEOUT) {
		FadeOutMode = FADEOUT_SCREEN;

		//	フォント読み込み
		FontHandle[0] = CreateFontToHandle("MSゴシック", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
		FontHandle[1] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
		FontHandle[2] = CreateFontToHandle("メイリオ", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	
		Qtr_Calc = new Quarter_Calculation;	//	クォータ座標変換
		My_Map = new Map(My_Stage->Get_Select_Stage());	//	敵の初期座標を求められるように、敵より早くオブジェクトを作る　
		Ply = new Player;					//	プレイヤー
		Eny = new Enemy_Manager;			//	敵たち
		My_Draw = new  Qtr_Draw_Manager;	//	ウィンドウ内に描画する物のテーブル
		My_Effect = new Effect_Manager;		//	爆発
		Ply_Dart = new Dart_Manager;		//	手裏剣
		Ply_Bomb = new Bomb_Manager;		//	爆弾
		My_Effect = new Effect_Manager;		//	エフェクト
		My_RankingData = new RankingData(My_Stage->Get_Select_Stage());	//	ランキング

		Ply_Clone = NULL;	//	分身は一体しか出せないので分身を使う時にnewする

		My_RankingData->Set_Flg_Off_All();	//	ランキングのカレントプレイヤーフラグを全部オフにする

		My_Time->Set_Flg(true);		//	カレントプレイヤーのしるし立てる
		My_Time->Set_Player_Name(PlayerName);
	}

	// 背景フェードアウト
	if (FadeOutMode == FADEOUT_SCREEN) {
		Alpha -= AlphaSpeed;  // だんだん薄く
		
		if (Alpha < 0) {  // 背景が消えたら
			FadeOutMode = FADEOUT_FONT;  // 文字のフェードアウトへ
			AlphaSpeed = 0.3f;  // 透明化速度(文字はゆっくり)
		}
	}
	DrawStringToHandle(500, 450, "Loading", COLOR[YELLOW], FontHandle[1]);

	// 文字フェードイン
	if (FadeOutMode == FADEOUT_FONT) {
		Alpha += AlphaSpeed;  // だんだん濃く
		AlphaSpeed += 0.005f;  // 透明化速度(だんだん早く)
		
		if (Alpha > 200) {  // 文字が完全に表示される
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);  // 通常描画
			FadeOutMode = NO_FADEOUT;
			return PLAY;  // プレイ初期化へ
		}
	}
	return PREPARE;	//	ゲーム本編へ
}


//	ゲーム中の処理を行う関数
int Game_Main()
{
	//	手裏剣発射　
	if (Inp_Ctrl->Is_Input_Once(KEY_Z)) {

		//	MPがあれば、プレイヤーの座標と角度を登録
		if (Ply->Gauge_Bar[MP]->Get_Now() >= -DART_MP) {
			Ply->Gauge_Bar[MP]->Add_Now(DART_MP);
			My_Sound->Play_Se(SE_DART_REGISTER);
			Ply_Dart->Register(Ply->Get_Top_X(), Ply->Get_Top_Y(), Ply->Get_Top_Angle(), 0);
		}
		//	MPなし　スカ音
		else {
			My_Sound->Play_Se(SE_MISS);
		}
	}

	//	爆弾発射
	if (Inp_Ctrl->Is_Input_Once(KEY_X)) {

		//	MPが０でなければ発射
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

	//	分身の術を発動、分身は一体だけ
	if (Inp_Ctrl->Is_Input_Once(KEY_C) && Ply_Clone == NULL) {

		//	MPが０でなければ
		if (Ply->Gauge_Bar[MP]->Get_Now() >= -CLONE_MP) {
			Ply->Gauge_Bar[MP]->Add_Now(CLONE_MP);
			//	座標と角度、クォータビュー上での角度を登録
			My_Sound->Play_Se(SE_CLONE_REGISTER);
			Ply_Clone = new Clone(Ply->Get_Top_X(), Ply->Get_Top_Y(), Ply->Get_Top_Angle(), Ply->Get_Qtr_Angle());
		}
		else {
			My_Sound->Play_Se(SE_MISS);
		}
	}

	//	手裏剣を移動させ描画テーブルに登録
	Ply_Dart->Move();

	//	爆弾を移動させ描画テーブルに登録
	Ply_Bomb->Move();

	//	分身の術が発動中なら更新する　してないならNULL
	if (Ply_Clone != NULL) {
		
		//	分身が存在してたら動かす
		if (Ply_Clone->Is_Clone_Flg())
			Ply_Clone->Move();

		//	分身が消えたらNULLを代入
		else
			Ply_Clone = NULL;
	}

	//	移動
	Ply->Move();
	Eny->Move();

	//	描画
	My_Map->Tile_Draw();		//	床だけ先に描画　
	My_Map->Map_Register();		//	通れない部分を描画テーブルに登録 壁
	My_Draw->insertionSort();	//	描画テーブル内をY座標が高い順にソート
	My_Draw->Draw();			//	描画テーブル内のものを描画

	Eny->Draw_Ray();	//	敵の視界を描画
	Eny->Draw_Gauge();	//	HPを描画

	// 爆発の描画
	My_Effect->Draw(Ply->Get_Camera_X(), Ply->Get_Camera_Y());

	//	HP,MPを描画
	Ply->Draw_Gauge();

	My_Time->SetClear_Time();	// 現在の時間ーゲームプレイ開始時間
	My_Time->Add_Time_Limit(-1.0f / 60.0f);	//	1秒間に60回回る

	//	残り制限時間が30秒以上あれば白色で残り時間を表示する
	if (My_Time->Get_Time_Limit() >= 30.0f)
		DrawFormatString(15, 65, COLOR[WHITE], "制限時間 : %.2f", My_Time->Get_Time_Limit());

	//	30秒以下なら赤色で表示
	else
		DrawFormatString(15, 65, COLOR[RED], "制限時間 : %.2f", My_Time->Get_Time_Limit());

	//	全ての判定
	int result = Judge();
	if (result == PLAY)My_Draw->Clear_Table();
	return result;
	
}


//	全ての判定を行う
int Judge()
{
	//	手裏剣と敵	敵がダメージを受ける
	Ply_Dart->Judge_Enemy();

	//	爆弾と敵	敵がダメージを受ける
	Ply_Bomb->Judge_Enemy();

	//	爆弾とプレイヤー　プレイヤーがダメージを受ける
	if (Ply_Bomb->Judge_Player(Ply->Get_Rect())) {
		//	ノーマル状態ならHP計算へ
		if (Ply->Status.Is(NORMAL)) {
			Ply->Hp_Proc(BOMB_DAMAGE);
			float x = (float)(Ply->Get_Qtr_X() + QTR_OFFSET_X);	//	登録する座標を計算
			float y = (float)(Ply->Get_Qtr_Y() + QTR_OFFSET_Y);
			My_Effect->Register(x, y, EFFECT_BOMB);	//	エフェクト登録
			My_Sound->Play_Se(SE_BOMB_SPARK);		//	衝突音を鳴らす
		}
	}

	//	敵とプレイヤー	プレイヤーがダメージ受ける
	if (Eny->Judge_Player(Ply->Get_Rect())) {
		//	ノーマル状態ならHP計算へ
		if (Ply->Status.Is(NORMAL)) {
			Ply->Hp_Proc(COLLISION_DAMAGE);
			float x = (float)(Ply->Get_Qtr_X() + QTR_OFFSET_X);	//	登録する座標を計算
			float y = (float)(Ply->Get_Qtr_Y() + QTR_OFFSET_Y);
			My_Effect->Register(x, y, EFFECT_COLLISION);	//	エフェクト登録
			My_Sound->Play_Se(SE_COLLISION);	//	衝突音を鳴らす
		}
	}

	//	状態の更新	生きてるとか死んでるとか
	Ply->Status_Update();
	Eny->Status_Update();

	//	クリアフラグが立ってたらクリア
	if (Ply->Is_Clear_Flg()) {
		return CLEAR;
	}

	//	プレイヤーが死んでたらゲームオーバー
	if (Ply->Status.Is(DIE)) {
		return OVER;
	}

	//	制限時間をオーバーしたらゲームオーバー
	if (My_Time->Get_Time_Limit() < 0) {
		return OVER;
	}

	//	それ以外ならゲームを続ける
	return PLAY;
}


//	ゲーム本編で使用したオブジェクトのあとしまつ
void Rel_Obj()
{
	//	エラー対策でオブジェクトがNULLでない時 delete する
	if (Qtr_Calc != NULL) { delete Qtr_Calc; Qtr_Calc = NULL; }	//	クォーター座標計算
	if (Ply != NULL) { delete Ply; Ply = NULL; }				//	プレイヤー
	if (Eny != NULL) { delete Eny; Eny = NULL; }				//	敵
	if (My_Map != NULL) { delete My_Map; My_Map = NULL; }		//	マップ
	if (Ply_Dart != NULL) { delete Ply_Dart; Ply_Dart = NULL; }	//	手裏剣
	if (My_Draw != NULL) { delete My_Draw; My_Draw = NULL; }	//	描画テーブル
	if (My_Effect != NULL) { delete My_Effect; My_Effect = NULL; }	//	エフェクト
}