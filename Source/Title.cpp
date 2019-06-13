#include"Title.h"

//	オブジェクトのポインタ
extern Input_Controll *Inp_Ctrl;	//	キー入力
extern Sound_Manager *My_Sound;		//	音

//	コンストラクタ
Title::Title()
{
	Cnt = 0;		//	カウンター初期化
	Load_Font();	//	フォント読み込み
	Load_Graph();	//	グラフィック読み込み
}

//	デストラクタ
Title::~Title()
{
	Delete_Font();	//	フォント削除
	Delete_Graph();	//	グラフィック削除
}

int Title::Update()
{
	Draw_Back_Gr();	//	背景描画
	Draw_Title();	//	見出しを描画

	//	文字列を点滅描画
	if (Cnt % 100 < 70)
		DrawStringToHandle(TEXT_X, TEXT_Y, "Click  Window to Start", COLOR[WHITE], Font_Handle[MS_GOTHIC]);

	Cnt++;
	//	一定時間経過するまでクリックを受け付けない
	if (Cnt <= CLICK_TIME)return TITLE; 

	//	ウィンドウ内で左クリックされたらメニュー画面に移行
	if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		My_Sound->Play_Se(SE_DECISION);	//	決定音鳴らす
		Cnt = 0;	//	初期化
		return MENU_INIT;	//	メニュー画面へ
	}

	return TITLE;	//	クリックされない限りタイトル画面
}

//	フォント読み込み
void Title::Load_Font() 
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MSゴシック", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("メイリオ", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	フォント削除
void Title::Delete_Font() { for (int i = 0; i < FONT_MAX; i++)  DeleteFontToHandle(Font_Handle[i]); }

//	グラフィック読み込み
void Title::Load_Graph() { Back_Gr = LoadGraph("Image/Title/Title1.png"); }

//	グラフィック削除
void Title::Delete_Graph() { DeleteGraph(Back_Gr); }

//	背景描画
void Title::Draw_Back_Gr() 
{
	DrawExtendGraph(0, 0, WINDOW_X, WINDOW_Y, Back_Gr, true);
}

//	見出し描画
void Title::Draw_Title() 
{
	DrawStringToHandle(TITLE_X, TITLE_Y, "Japanese NINJA No1", COLOR[YELLOW], Font_Handle[MV_BOLI]);
}