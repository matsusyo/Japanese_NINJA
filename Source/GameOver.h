#pragma once
#include<math.h>
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"SoundManager.h"


class Game_Over
{
private:
	int BackGr;				//	背景画像
	int Cnt;					//	カウンター
	int Font_Handle[FONT_MAX];	//	フォント数

public:
	Game_Over();		//	コンストラクタ
	~Game_Over();		//	デストラクタ
	int Update();		//	ゲームオーバー画面中呼ぶ
	void Load_Font();	//	フォント読み込み
	void Delete_Font();	//	フォント削除
	void Load_Graph();	//	画像読み込み
	void Delete_Graph();//	画像削除
	void Draw_BackGr();//	背景描画
	void Draw_Title();	//	見出し描画
	void Draw_Text();	//	テキスト描画
};
