#include"Manual.h"

//	オブジェクトのポインタ
extern Input_Controll *Inp_Ctrl;	//	キー入力
extern Sound_Manager *My_Sound;		//	音

//	メニュー画面見出しのX,Y座標
const int TITLE_X = 35;
const int TITLE_Y = 40;
const int TEXT_X = TITLE_X * 10;
const int TEXT_Y = (int)(TITLE_Y * 1.6);

//	説明画像のX座標とY座標
const int MANUAL_X = 70;
const int MANUAL_Y = 128;

//	目次となる□の座標と範囲
const int CONTENTS_X_LONG = 50;	//	□の横の長さ
const int CONTENTS_Y_LONG = 50;	//	縦の長さ
const int CONTENTS_Y = 500;
const int CONTENTS_1X = 205;
const int CONTENTS_2X = 285;
const int CONTENTS_3X = 365;
const int CONTENTS_4X = 445;
const int CONTENTS_5X = 525;

//	クリックするとメニュー画面に戻る□のX,Y座標、範囲
const int MENU_X = 610;
const int MENU_Y = 35;
const int MENU_X_LONG = 170;
const int MENU_Y_LONG = 70;

//	コンストラクタ
Manual::Manual()
{
	Set_Page(0);	//	ページ番号初期化
	Set_Cnt(0);		//	カウント初期化
	Load_Font();	//	フォント読み込み
	Load_Graph();	//	グラフィック読み込み
}

//	デストラクタ
Manual::~Manual()
{
	Delete_Font();		//	フォント削除
	Delete_Graph();		//	グラフィック削除
}

//	マニュアル画面中呼ばれる
int Manual::Update()
{
	Draw_BackGr();			//	背景を描画
	Draw_Title();			//	シーンの見出しを描画
	Draw_TeachGr();			//	シーン毎の説明画像を描画
	Draw_Contents_Select();	//	現在のシーン目次□を赤で描画
	Draw_Contents_Out();	//	シーン目次の□の外枠を描画

	//	マウス座標を獲得
	float MouseX, MouseY;
	Inp_Ctrl->Get_Mouse_Pointer(&MouseX, &MouseY);

	Draw_Menu_Back(MouseX,MouseY);		//	メニューに戻るアイコンを描画

	//	クリック連打での誤選択を防止する 50カウント目まではここで処理を終える
	if (Cnt <= CLICK_TIME) {
		Cnt++;
		return MANUAL;
	}

	//	MENUをクリックするとメニュー画面に戻るようにする
	if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT) && MouseX >= MENU_X && MouseX <= MENU_X + MENU_X_LONG && MouseY >= MENU_Y && MouseY <= MENU_Y + MENU_Y_LONG) {
		My_Sound->Play_Se(SE_DECISION);	//	決定音
		Set_Page(0);	//	ページ初期化
		return MENU;	//	メニュー画面へ
	}

	//	MENUをクリックされていなくて□の目次のY座標範囲外ならマニュアル画面を維持
	if (!(MouseY >= CONTENTS_Y && MouseY <= CONTENTS_Y + CONTENTS_Y_LONG)) {
		return MANUAL;	//	マニュアル画面維持
	}
	
	Page_tmp = Page;	//	現在の説明画面番号を仮番号に代入
	Draw_Contents_Target(MouseX, MouseY);	//	マウスの位置にある□を白色で描画

	//	違うシーンをクリックされたらシーンを変える
	if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT) && Page != Page_tmp) {
		My_Sound->Play_Se(SE_SELECTION);	//	ページ変更音
		Page = Page_tmp;	//	クリックした□のページ番号に移動
	}

	//	マニュアル画面を維持
	return MANUAL;
}

//	フォント読み込み
void Manual::Load_Font()
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MSゴシック", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 52, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("メイリオ", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	画像読み込み
void Manual::Load_Graph()
{
	Back_Gr = LoadGraph("Image/Manual/Back.png");
	Manual_Gr[0] = LoadGraph("Image/Manual/aaa.png");
	Manual_Gr[1] = LoadGraph("Image/Manual/bbb.png");
	Manual_Gr[2] = LoadGraph("Image/Manual/ccc.png");
	Manual_Gr[3] = LoadGraph("Image/Manual/ddd.png");
	Manual_Gr[4] = LoadGraph("Image/Manual/eee.png");
}

//	フォント削除
void Manual::Delete_Font() { for (int i = 0; i < FONT_MAX; i++)  DeleteFontToHandle(Font_Handle[i]); }

//	グラフィック削除
void Manual::Delete_Graph()
{
	DeleteGraph(Back_Gr);
	for (int i = 0; i < MANUAL_GR_NUM; i++) { DeleteGraph(Manual_Gr[i]); }
}

//	背景を描画
void Manual::Draw_BackGr() { DrawExtendGraph(0, 0, WINDOW_X, WINDOW_Y, Back_Gr, TRUE); }

//	見出しを描画
void Manual::Draw_Title()
{
	//	ページ番号によって見出しを変える
	switch (Get_Page()) {
	default:
	case 0:
		DrawStringToHandle(TITLE_X, TITLE_Y, "＜ゲームについて＞", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	case 1:
		DrawStringToHandle(TITLE_X, TITLE_Y, "＜ゲームルール＞", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	case 2:
		DrawStringToHandle(TITLE_X, TITLE_Y, "＜移動方法＞", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	case 3:
		DrawStringToHandle(TITLE_X, TITLE_Y, "＜忍術＞", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	case 4:
		DrawStringToHandle(TITLE_X, TITLE_Y, "＜忍術＞", COLOR[YELLOW], Font_Handle[MS_GOTHIC]);
		break;
	}
}

//	説明画像を描画
void Manual::Draw_TeachGr()
{
	//	ページ番号によって説明画像を変える
	switch (Get_Page()) {
	default:
	case 0:
		DrawGraph(MANUAL_X, MANUAL_Y, Manual_Gr[0], TRUE);
		break;
	case 1:
		DrawGraph(MANUAL_X, MANUAL_Y, Manual_Gr[1], TRUE);
		break;
	case 2:
		DrawGraph(MANUAL_X - 1, MANUAL_Y, Manual_Gr[2], TRUE);
		break;
	case 3:
		DrawGraph(MANUAL_X, MANUAL_Y, Manual_Gr[3], TRUE);
		break;
	case 4:
		DrawGraph(MANUAL_X, MANUAL_Y, Manual_Gr[4], TRUE);
		break;
	}
}

//	現在のページの□を赤で描画
void Manual::Draw_Contents_Select()
{
	//	現在のページでスイッチする
	switch (Get_Page()) {
	default:
	case 0:
		DrawBox(CONTENTS_1X, CONTENTS_Y, CONTENTS_1X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	case 1:
		DrawBox(CONTENTS_2X, CONTENTS_Y, CONTENTS_2X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	case 2:
		DrawBox(CONTENTS_3X, CONTENTS_Y, CONTENTS_3X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	case 3:
		DrawBox(CONTENTS_4X, CONTENTS_Y, CONTENTS_4X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	case 4:
		DrawBox(CONTENTS_5X, CONTENTS_Y, CONTENTS_5X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[RED], TRUE);
		break;
	}
}

//	クリックするとメニュー画面に戻る範囲を描画
void Manual::Draw_Menu_Back(float mousex, float mousey) 
{
	if (mousey >= MENU_Y && mousey <= MENU_Y + 70 && mousex >= MENU_X && mousex <= MENU_X + 170)
		DrawStringToHandle(MENU_X, MENU_Y, "MENU", COLOR[RED], Font_Handle[MV_BOLI]);

	else
		DrawStringToHandle(MENU_X, MENU_Y, "MENU", COLOR[LIGHTBLUE], Font_Handle[MV_BOLI]);
}


//	ページを表す□の外枠を５つ描画
void Manual::Draw_Contents_Out()
{
	DrawBox(CONTENTS_1X, CONTENTS_Y, CONTENTS_1X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
	DrawBox(CONTENTS_2X, CONTENTS_Y, CONTENTS_2X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
	DrawBox(CONTENTS_3X, CONTENTS_Y, CONTENTS_3X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
	DrawBox(CONTENTS_4X, CONTENTS_Y, CONTENTS_4X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
	DrawBox(CONTENTS_5X, CONTENTS_Y, CONTENTS_5X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], FALSE);
}

//	マウスの位置にある□を白色で描画
void Manual::Draw_Contents_Target(float mousex, float mousey)
{
	//	マウスが□の範囲内に入ったら仮番号に□の説明番号を代入
	if (Page != 0 && mousex >= CONTENTS_1X && mousex <= CONTENTS_1X + CONTENTS_X_LONG) {
		Page_tmp = 0;
		DrawBox(CONTENTS_1X, CONTENTS_Y, CONTENTS_1X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
	if (Page != 1 && mousex >= CONTENTS_2X && mousex <= CONTENTS_2X + CONTENTS_X_LONG) {
		Page_tmp = 1;
		DrawBox(CONTENTS_2X, CONTENTS_Y, CONTENTS_2X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
	if (Page != 2 && mousex >= CONTENTS_3X && mousex <= CONTENTS_3X + CONTENTS_X_LONG) {
		Page_tmp = 2;
		DrawBox(CONTENTS_3X, CONTENTS_Y, CONTENTS_3X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
	if (Page_tmp != 3 && mousex >= CONTENTS_4X && mousex <= CONTENTS_4X + CONTENTS_X_LONG) {
		Page_tmp = 3;
		DrawBox(CONTENTS_4X, CONTENTS_Y, CONTENTS_4X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
	if (Page != 4 && mousex >= CONTENTS_5X && mousex <= CONTENTS_5X + CONTENTS_X_LONG) {
		Page_tmp = 4;
		DrawBox(CONTENTS_5X, CONTENTS_Y, CONTENTS_5X + CONTENTS_X_LONG, CONTENTS_Y + CONTENTS_Y_LONG, COLOR[WHITE], TRUE);
	}
}