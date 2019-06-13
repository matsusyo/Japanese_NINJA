#include"Stage.h"

//	オブジェクトのポインタ
extern Input_Controll *Inp_Ctrl;	//	キー入力
extern Sound_Manager *My_Sound;		//	音

const int TITLE_X = 35;	//	ステージ画面見出しのX,Y座標
const int TITLE_Y = 40;
const int TEXT_X = TITLE_X * 10;
const int TEXT_Y = (int)(TITLE_Y * 1.6);
const int STAGE_1X_LINE = 80;	//	6個のステージを1段に3つ配置するので3個のx座標
const int STAGE_2X_LINE = 310;
const int STAGE_3X_LINE = 540;
const int FIRST_Y_LINE = 180;	//	Y座標2段分
const int SECOND_Y_LINE = 370;	//	2段目
const int STAGE_X_LONG = 180;	//	ステージの横幅
const int STAGE_Y_LONG = 140;	//	縦幅
const int FONT_NUM = 3;	//	フォント数


Stage::Stage() 
{
	Cnt = 0;
	Select_Stage = NO_STAGE;	//	選択ステージ初期化
	Load_Font();
	Load_Graph();
}

Stage::~Stage()
{
	Delete_Font();
	Delete_Graph();
}

int Stage::Update()
{
	Draw_Title();
	Draw_Stage();
	//	クリック可能になるまでインクリメント
	if (Cnt <= CLICK_TIME)Cnt++;
	//クリック連打でシーンが急に変わることがないようにする
	if (Cnt < CLICK_TIME) { return STAGE; }
	
	float MouseX, MouseY;	//	マウス座標ゲット
	Inp_Ctrl->Get_Mouse_Pointer(&MouseX, &MouseY);
	Select_Stage = Draw_Target_Stage(MouseX, MouseY);
	
	//	ステージが選択されている状態でクリックされたら次のモードへ
	if (Select_Stage != NO_STAGE && Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		Cnt = 0;	//	リセット
		My_Sound->Play_Se(SE_DECISION);	//	決定音
		return PREPARE;	//	PREPAREモードに進む
	}

	//	クリックされなかったら選択中のステージをリセット
	if (!Inp_Ctrl->Is_Input_Once(MOUSE_LEFT))Select_Stage = NO_STAGE;

	//	ステージ選択画面維持
	return STAGE;
}

void Stage::Load_Font() 
{
	StageGr[0] = LoadGraph("Image/Stage/1.png");
	StageGr[1] = LoadGraph("Image/Stage/2.png");
	StageGr[2] = LoadGraph("Image/Stage/3.png");
	StageGr[3] = LoadGraph("Image/Stage/4.png");
	StageGr[4] = LoadGraph("Image/Stage/5.png");
	StageGr[5] = LoadGraph("Image/Stage/6.png");
}

void Stage::Load_Graph() 
{
	FontHandle[MS_GOTHIC] = CreateFontToHandle("MSゴシック", 38, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	FontHandle[MV_BOLI] = CreateFontToHandle("MV Boli", 65, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	FontHandle[MEIRIO] = CreateFontToHandle("メイリオ", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

void Stage::Delete_Font() {for (int i = 0; i < FONT_NUM; i++)DeleteFontToHandle(FontHandle[i]);}

void Stage::Delete_Graph() {for (int i = 0; i < STAGE_GR_NUM; i++) { DeleteGraph(StageGr[i]); }}

void Stage::Draw_Title() {DrawStringToHandle(TITLE_X, TITLE_Y, "STAGE", COLOR[YELLOW], FontHandle[MV_BOLI]);}

void Stage::Draw_Stage() 
{
	DrawExtendGraph(STAGE_1X_LINE, FIRST_Y_LINE, STAGE_1X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, StageGr[0], FALSE);
	DrawExtendGraph(STAGE_2X_LINE, FIRST_Y_LINE, STAGE_2X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, StageGr[1], FALSE);
	DrawExtendGraph(STAGE_3X_LINE, FIRST_Y_LINE, STAGE_3X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, StageGr[2], FALSE);
	DrawExtendGraph(STAGE_1X_LINE, SECOND_Y_LINE, STAGE_1X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, StageGr[3], FALSE);
	DrawExtendGraph(STAGE_2X_LINE, SECOND_Y_LINE, STAGE_2X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, StageGr[4], FALSE);
	DrawExtendGraph(STAGE_3X_LINE, SECOND_Y_LINE, STAGE_3X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, StageGr[5], FALSE);
}

int Stage::Draw_Target_Stage(float mousex,float mousey) 
{
	int stage = NO_STAGE;
	//	クリックされた画像によってステージを変える	ステージ１
	if ((mousex >= STAGE_1X_LINE && mousex <= STAGE_1X_LINE + STAGE_X_LONG) && (mousey >= FIRST_Y_LINE && mousey <= FIRST_Y_LINE + STAGE_Y_LONG)) {
		//	□を描画　□の中にステージのイメージ画像を描画
		DrawBox(STAGE_1X_LINE, FIRST_Y_LINE, STAGE_1X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = AAA;
	}
	//	ステージ２
	if ((mousex >= STAGE_2X_LINE && mousex <= STAGE_2X_LINE + STAGE_X_LONG) && (mousey >= FIRST_Y_LINE && mousey <= FIRST_Y_LINE + STAGE_Y_LONG)) {
		DrawBox(STAGE_2X_LINE, FIRST_Y_LINE, STAGE_2X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = BBB;
	}

	//	ステージ3
	if ((mousex >= STAGE_3X_LINE && mousex <= STAGE_3X_LINE + STAGE_X_LONG) && mousey >= FIRST_Y_LINE && mousey <= FIRST_Y_LINE + STAGE_Y_LONG) {
		DrawBox(STAGE_3X_LINE, FIRST_Y_LINE, STAGE_3X_LINE + STAGE_X_LONG, FIRST_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = CCC;
	}

	//	ステージ４
	if ((mousey >= STAGE_1X_LINE && mousex <= STAGE_1X_LINE + STAGE_X_LONG) && (mousey >= SECOND_Y_LINE && mousey <= SECOND_Y_LINE + STAGE_Y_LONG)) {
		DrawBox(STAGE_1X_LINE, SECOND_Y_LINE, STAGE_1X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = DDD;
	}

	//	ステージ５
	if ((mousex >= STAGE_2X_LINE && mousex <= STAGE_2X_LINE + STAGE_X_LONG) && (mousey >= SECOND_Y_LINE && mousey <= SECOND_Y_LINE + STAGE_Y_LONG)) {
		DrawBox(STAGE_2X_LINE, SECOND_Y_LINE, STAGE_2X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = EEE;
	}

	//	ステージ６
	if ((mousex >= STAGE_3X_LINE && mousex <= STAGE_3X_LINE + STAGE_X_LONG) && mousey >= SECOND_Y_LINE && mousey <= SECOND_Y_LINE + STAGE_Y_LONG) {
		DrawBox(STAGE_3X_LINE, SECOND_Y_LINE, STAGE_3X_LINE + STAGE_X_LONG, SECOND_Y_LINE + STAGE_Y_LONG, COLOR[WHITE], FALSE);
		stage = FFF;
	}
	return stage;
}