//	定数

#pragma once
#include"DxLib.h"

const char GameTitle[] = "Japanese NINJA No1";

const double FPS = 240.0;

//	ウィンドウサイズ
const int WINDOW_X = 800;
const int WINDOW_Y = 600;
const int H_WINDOW_X = WINDOW_X / 2;
const int H_WINDOW_Y = WINDOW_Y / 2;

//	マップサイズ
const int MAP_X = 72;
const int MAP_Y = 72;

//	描画座標オフセット → マップチップ、キャラサイズの違いやクォータビューでの描画なので必要
const int
QTR_OFFSET_X = 0,
QTR_OFFSET_Y = 32;	//	31～33	32が私的にベスト

//	ゲーム状態の名前	
const enum GAME_MODE
{
	TITLE_INIT, TITLE, MENU_INIT, MENU, STAGE, STORY, MANUAL, OPTION, PREPARE, PLAY, OVER, CLEAR, RANKING, EXIT,
};

//	HP,MP,外枠ゲージ
const enum GAUGE_TYPE { HP, FRAME, MP };

//	HP,MPの最大値
const float
HP_MAX = 100,
MP_MAX = 100;

//	ダメージ量
const float
DART_DAMAGE = -5.0f,	//	手裏剣が与えるダメージ
COLLISION_DAMAGE = -10.0f;	//	敵が与えるダメージ

//	カウントタイム １ * ６０ = １ 秒
const int
//	無敵時間
DAMAGE_COUNT = 1 * 60,
E_DAMAGE_COUNT = 0 * 60,
//	死ぬまでの時間
DYING_COUNT = 1 * 60,
E_DYING_COUNT = 1 * 60,
BOSS_DYING_COUNT = 4 * 60;


//	色の名前
const enum COLOR_NAME {
	WHITE, RED, GREEN, BLUE, YELLOW, PURPLE, LIGHTBLUE
};

//	色を獲得
const unsigned int COLOR[] = {
		GetColor(255,255,255),	// しろ
		GetColor(255,0,0),		// あか
		GetColor(0,255,0),		// みどり
		GetColor(0,0,255),		// あお
		GetColor(255,255,0),	// きいろ
		GetColor(255,0,255),	// むらさき
		GetColor(0,255,255)		// みずいろ
};

//	フォントの名前
const enum FONT_NAME {
	MS_GOTHIC,
	MV_BOLI,
	MEIRIO,
	FONT_MAX,
};

//	クリックを受け付けるようになるまでの時間
const int CLICK_TIME = 50;	