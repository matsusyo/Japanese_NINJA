#pragma once
#include<math.h>
#include"DxLib.h"
#include"Literal.h"
#include "InputCtrl.h"
#include"SoundManager.h"

const int TITLE_X = 60;	//	見出しの座標
const int TITLE_Y = 110;
const int TEXT_X = 180;	//	テキストの座標
const int TEXT_Y = 380;

class Title
{
private:
	int Back_Gr;				//	背景画像
	int Cnt;					//	カウンター
	int Font_Handle[FONT_MAX];	//	フォント数

public:
	Title();			//	コンストラクタ
	~Title();			//	デストラクタ
	int Update();		//	タイトル画面中呼ぶ
	void Load_Font();	//	フォント読み込み
	void Delete_Font();	//	フォント削除
	void Load_Graph();	//	画像読み込み
	void Delete_Graph();//	画像削除
	void Draw_Back_Gr();//	背景描画
	void Draw_Title();	//	見出し描画
};
