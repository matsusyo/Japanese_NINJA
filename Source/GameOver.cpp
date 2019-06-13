//	ゲームオーバモード時の処理
#include"GameOver.h"

const int TITLE_X = 200;	//	ゲームモード名表示座標
const int TITLE_Y = 110;
const int TEXT_INPUT_X = 250;	//	入力を促すテキストの座標
const int TEXT_INPUT_Y = 380;

//	オブジェクトのポインタ
extern Input_Controll *Inp_Ctrl;	//	キー入力
extern Sound_Manager *My_Sound;		//	音

//	コンストラクタ
Game_Over::Game_Over()
{
	Cnt = 0;		//	カウンター初期化
	Load_Font();	//	フォント読み込み
	Load_Graph();	//	グラフィック読み込み
}

//	デストラクタ
Game_Over::~Game_Over()
{
	Delete_Font();	//	フォント削除
	Delete_Graph();	//	グラフィック削除
}

//	ゲームオーバー画面中呼ばれる
int Game_Over::Update()
{
	Cnt++;
	Draw_BackGr();	//	背景描画
	Draw_Title();	//	見出し描画

	//	文字列を点滅描画
	if (Cnt % 100 < 70)	Draw_Text();
		
	//	カウントが進んでいたら左クリックでタイトルへ
	if (Cnt > CLICK_TIME && Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		Cnt = 0;	//	カウントをリセットしないと次に来たときエラーが起きる
		My_Sound->Play_Se(SE_DECISION);	//	決定音
		return MENU_INIT;	//	メニュー画面へ
	}
	return OVER;
}

//	フォント読み込み
void Game_Over::Load_Font()
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MSゴシック", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("メイリオ", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	フォント削除
void Game_Over::Delete_Font() { for (int i = 0; i < FONT_MAX; i++)  DeleteFontToHandle(Font_Handle[i]); }

//	グラフィック読み込み
void Game_Over::Load_Graph() { BackGr = LoadGraph("image/Over/GameOver.png"); }

//	グラフィック削除
void Game_Over::Delete_Graph() { DeleteGraph(BackGr); }

//	背景描画
void Game_Over::Draw_BackGr() { DrawExtendGraph(0, 0, WINDOW_X, WINDOW_Y, BackGr, true); }

//	見出し描画
void Game_Over::Draw_Title() { DrawStringToHandle(TITLE_X, TITLE_Y, "Game Over ...", COLOR[YELLOW], Font_Handle[MV_BOLI]); }

//	テキスト描画
void Game_Over::Draw_Text() { DrawStringToHandle(TEXT_INPUT_X, TEXT_INPUT_Y, "Click to Window", COLOR[WHITE], Font_Handle[MS_GOTHIC]); }