#include"Menu.h"

//	オブジェクトのポインタ
extern Input_Controll *Inp_Ctrl;	//	キー,マウス入力
extern Sound_Manager *My_Sound;		//	音

const int TITLE_X = 35;		//	メニュー画面見出しのX,Y座標
const int TITLE_Y = 40;
const int TEXT_X = TITLE_X * 10;		//	何をする画面か教えるテキスト座標
const int TEXT_Y = (int)(TITLE_Y * 1.6);

const int ICON_1X_LINE = 55;		//	ステージ選択に移行するアイコンの起点X座標
const int ICON_2X_LINE = 295;		//	遊び方アイコンのX座標
const int ICON_3X_LINE = 535;		//	設定アイコンのX座標
const int ICON_ALL_Y_LINE = 185;	//	アイコンのY座標 アイコンは平行に並べる
const int ICON_X_LONG = 210;		//	アイコンの横幅
const int ICON_Y_LONG = ICON_X_LONG;//	アイコンの縦幅 縦幅と横幅を揃える

const int EXIT_X = 660;
const int EXIT_Y = 500;
const int EXIT_X_LONG = 100;
const int EXIT_Y_LONG = 80;

const int TEXT_1X_LINE = 117;		//	ステージ選択アイコン下のテキストのX座標
const int TEXT_2X_LINE = 343;		//	遊び方アイコン下のテキストX座標
const int TEXT_3X_LINE = 565;		//	設定アイコン下のテキストX座標
const int TEXT_ALL_Y_LINE = 405;	//	アイコン下のテキストのY座標 

//	コンストラクタ
Menu::Menu()
{
	Cnt = 0;		//	カウンター初期化
	Load_Font();	//	フォント読み込み
	Load_Graph();	//	画像読み込み
}

//	デストラクタ
Menu::~Menu()
{
	Delete_Font();	//	フォント削除
	Delete_Graph();	//	画像削除
}

int Menu::Update() 
{
	Draw_Title();		//	見出し描画
	Draw_Icon_Text();	//	アイコン下のテキスト描画

	//	クリック連打でシーンが急に変わることがないようにする
	if (Cnt <= CLICK_TIME) {
		Cnt++;			//	カウンタを進めて
		Draw_Icon();	//	アイコン描画
		return MENU;	//	メニュー画面を維持
	}

	//	マウスカーソルの位置でアイコン画像を変える
	float MouseX, MouseY;
	Inp_Ctrl->Get_Mouse_Pointer(&MouseX, &MouseY);

	//	マウス位置のアイコンのモードを代入
	int Mode = Draw_Target_Icon(MouseX, MouseY);

	//	アイコン上でクリックされたらモードチェンジ
	if (Mode != MENU && Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		My_Sound->Play_Se(SE_DECISION);	//	決定音を鳴らす
		Cnt = 0;		//	カウンタリセット
		return Mode;	//	モード変更
	}

	//	アイコン上でクリックされなかった場合はメニュー画面を維持
	return MENU;
}

//	フォント読み込み
void Menu::Load_Font()
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MSゴシック", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("メイリオ", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	画像読み込み
void Menu::Load_Graph()
{
	Mission_Gr[0] = LoadGraph("Image/Menu/Target.png");
	Mission_Gr[1] = LoadGraph("Image/Menu/Target_Back.png");
	Manual_Gr[0] = LoadGraph("Image/Menu/Practice.png");
	Manual_Gr[1] = LoadGraph("Image/Menu/Practice_Back.png");
	Title_Gr[0] = LoadGraph("Image/Menu/Title.png");
	Title_Gr[1] = LoadGraph("Image/Menu/Title_Back.png");
	Exit_Gr[0] = LoadGraph("Image/Menu/Exit.png");
	Exit_Gr[1] = LoadGraph("Image/Menu/Exit_Back.png");
}

//	フォント削除
void Menu::Delete_Font() { for (int i = 0; i < FONT_MAX; i++) DeleteFontToHandle(Font_Handle[i]); }

//	画像削除
void Menu::Delete_Graph()
{
	for (int i = 0; i < ICON_NUM; i++) {
		DeleteGraph(Mission_Gr[i]);
		DeleteGraph(Manual_Gr[i]);
		DeleteGraph(Title_Gr[i]);
		DeleteGraph(Exit_Gr[i]);
	}
}

//	見出し描画
void Menu::Draw_Title() { DrawStringToHandle(TITLE_X, TITLE_Y, "MENU", COLOR[YELLOW], Font_Handle[MV_BOLI]); }

//	アイコン描画
void Menu::Draw_Icon()
{
	DrawExtendGraph(ICON_1X_LINE, ICON_ALL_Y_LINE, ICON_1X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Mission_Gr[0], TRUE);
	DrawExtendGraph(ICON_2X_LINE, ICON_ALL_Y_LINE, ICON_2X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Manual_Gr[0], TRUE);
	DrawExtendGraph(ICON_3X_LINE, ICON_ALL_Y_LINE, ICON_3X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Title_Gr[0], TRUE);
	DrawExtendGraph(EXIT_X, EXIT_Y, EXIT_X + EXIT_X_LONG, EXIT_Y + EXIT_Y_LONG, Exit_Gr[0], TRUE);
}

//	アイコン下のテキスト描画
void Menu::Draw_Icon_Text()
{
	DrawStringToHandle(TEXT_1X_LINE, TEXT_ALL_Y_LINE, "任務", COLOR[WHITE], Font_Handle[MS_GOTHIC]);
	DrawStringToHandle(TEXT_2X_LINE, TEXT_ALL_Y_LINE, "遊び方", COLOR[WHITE], Font_Handle[MS_GOTHIC]);
	DrawStringToHandle(TEXT_3X_LINE, TEXT_ALL_Y_LINE, "タイトル", COLOR[WHITE], Font_Handle[MS_GOTHIC]);
}

//	マウス位置にあるアイコン描画
int Menu::Draw_Target_Icon(float mousex, float mousey)
{
	int Mode = MENU;
	//	アイコン上なら2個目のアイコンに変える
	if ((mousex >= ICON_1X_LINE && mousex <= ICON_1X_LINE + ICON_X_LONG) && (mousey >= ICON_ALL_Y_LINE && mousey <= ICON_ALL_Y_LINE + ICON_Y_LONG)) {
		DrawExtendGraph(ICON_1X_LINE, ICON_ALL_Y_LINE, ICON_1X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Mission_Gr[1], TRUE);
		Mode = STAGE;	//	ステージ選択画面を仮モードに代入
	}
	//	アイコン上でないのなら1個目の画像に変える
	else DrawExtendGraph(ICON_1X_LINE, ICON_ALL_Y_LINE, ICON_1X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Mission_Gr[0], TRUE);

	//	下も同じ
	if ((mousex >= ICON_2X_LINE && mousex <= ICON_2X_LINE + ICON_X_LONG) && (mousey >= ICON_ALL_Y_LINE && mousey <= ICON_ALL_Y_LINE + ICON_Y_LONG)) {
		DrawExtendGraph(ICON_2X_LINE, ICON_ALL_Y_LINE, ICON_2X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Manual_Gr[1], TRUE);
		Mode = MANUAL;	//	遊び方説明画面を仮モードに代入
	}
	else DrawExtendGraph(ICON_2X_LINE, ICON_ALL_Y_LINE, ICON_2X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Manual_Gr[0], TRUE);

	if (mousex >= ICON_3X_LINE && mousex <= ICON_3X_LINE + ICON_X_LONG && mousey >= ICON_ALL_Y_LINE && mousey <= ICON_ALL_Y_LINE + ICON_Y_LONG) {
		DrawExtendGraph(ICON_3X_LINE, ICON_ALL_Y_LINE, ICON_3X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Title_Gr[1], TRUE);
		Mode = TITLE_INIT;	//	設定画面を仮モードに代入
	}
	else DrawExtendGraph(ICON_3X_LINE, ICON_ALL_Y_LINE, ICON_3X_LINE + ICON_X_LONG, ICON_ALL_Y_LINE + ICON_Y_LONG, Title_Gr[0], TRUE);

	if (mousex >= EXIT_X && mousex <= EXIT_X + EXIT_X_LONG && mousey >= EXIT_Y && mousey <= EXIT_Y + EXIT_Y_LONG) {
		DrawExtendGraph(EXIT_X, EXIT_Y, EXIT_X + EXIT_X_LONG, EXIT_Y + EXIT_Y_LONG, Exit_Gr[1], TRUE);
		Mode = EXIT;
	}
	else DrawExtendGraph(EXIT_X, EXIT_Y, EXIT_X + EXIT_X_LONG, EXIT_Y + EXIT_Y_LONG, Exit_Gr[0], TRUE);

	return Mode;
}