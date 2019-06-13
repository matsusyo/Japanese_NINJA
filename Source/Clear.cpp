//	クリアモード時の処理
#include"Clear.h"

const int TITLE_X = 180;	//	見出しの座標
const int TITLE_Y = 110;
const int TEXT_X = 155;	//	テキストの座標
const int TEXT_Y = 380;

const double CLEARALPHA_SPEED = 1.0;  // CLEAR透明度の変化速度

//	オブジェクトのポインタ
extern Input_Controll *Inp_Ctrl;	//	入力
extern Sound_Manager *My_Sound;		//	音
extern Map *My_Map;					//	マップ
extern Qtr_Draw_Manager *My_Draw;	//	描画テーブル

//	コンストラクタ
Clear::Clear()
{
	Cnt = 0;		//	カウンター初期化
	ClearAlpha = 50.0;  // CLEAR画像透明度
	ClearAlphaType = true;  // 透明度上げるか下げるか
	Load_Font();	//	フォント読み込み
}

//	デストラクタ
Clear::~Clear()
{
	Delete_Font();	//	フォント削除
}

//	クリア画面中呼ぶ
int Clear::Update()
{
	Cnt++;
	Draw_BackGr();	//	背景を描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)ClearAlpha);	// 半透明の描画
	Draw_Title();	//	見だしを描画
	if (ClearAlphaType)ClearAlpha += CLEARALPHA_SPEED;  // だんだん薄く
	else ClearAlpha -= CLEARALPHA_SPEED;  // だんだん濃く
	if (ClearAlpha < 150)ClearAlphaType = true;  // 透明度が150になったら今度は薄く
	if (ClearAlpha > 250)ClearAlphaType = false;  // 透明度が250を超えたら濃く

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// 半透明描画をやめる

	//	点滅描画
	if (Cnt % 100 > 50 && Cnt >= 150)Draw_Text();	//	アイコン下のテキスト描画

	//	クリック連打でシーンが急に変わることがないようにする
	if (Cnt <= CLICK_TIME) {
		return CLEAR;	//	クリア画面を維持
	}

	//	アイコン上でクリックされたらモードチェンジ
	if (Inp_Ctrl->Is_Input_Once(MOUSE_LEFT)) {
		My_Sound->Play_Se(SE_DECISION);	//	決定音を鳴らす
		Cnt = 0;		//	カウンタリセット
		return RANKING;	//	モード変更
	}

	//	アイコン上でクリックされなかった場合はメニュー画面を維持
	return CLEAR;
}

//	フォント読み込み
void Clear::Load_Font()
{
	Font_Handle[MS_GOTHIC] = CreateFontToHandle("MSゴシック", 50, 7, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MV_BOLI] = CreateFontToHandle("MV Boli", 75, 8, DX_FONTTYPE_ANTIALIASING_8X8);
	Font_Handle[MEIRIO] = CreateFontToHandle("メイリオ", 20, 7, DX_FONTTYPE_ANTIALIASING_8X8);
}

//	背景描画
void Clear::Draw_BackGr()
{
	My_Map->Tile_Draw();		//	床だけ先に描画　
	My_Draw->Draw();			//	描画テーブル内のものを描画
}

//	フォント削除
void Clear::Delete_Font()
{
	for (int i = 0; i < FONT_MAX; i++) DeleteFontToHandle(Font_Handle[i]);
}

//	見出し描画
void Clear::Draw_Title()
{
	DrawStringToHandle(TITLE_X, TITLE_Y, "Stage Clear!!", COLOR[YELLOW], Font_Handle[MV_BOLI]);
}

//	説明するテキスト描画
void Clear::Draw_Text()
{
	DrawStringToHandle(TEXT_X, TEXT_Y, "Click Window to Next", COLOR[WHITE], Font_Handle[MS_GOTHIC]);
}