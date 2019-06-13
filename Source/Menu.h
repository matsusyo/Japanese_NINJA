#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"SoundManager.h"

const int ICON_NUM = 2;		//	各アイコン数

class Menu
{
private:
	int Cnt;					//	カウンター
	int Font_Handle[FONT_MAX];	//	フォントハンドル
	int Mission_Gr[ICON_NUM];	//	ステージ選択画面アイコン
	int Manual_Gr[ICON_NUM];	//	遊び方説明画面アイコン
	int Title_Gr[ICON_NUM];		//	タイトル画面アイコン
	int Exit_Gr[ICON_NUM];		//	ゲーム終了アイコン

public:

	Menu();					//	コンストラクタ
	~Menu();				//	デストラクタ
	int Update();			//	遊び方説明画面中に呼ばれる
	void Load_Font();		//	フォント読み込み
	void Load_Graph();		//	画像読み込み
	void Delete_Font();		//	フォント削除
	void Delete_Graph();	//	画像削除	
	void Draw_Title();		//	見出し描画
	void Draw_Icon();		//	アイコン描画
	void Draw_Icon_Text();	//	アイコン下のテキスト描画
	int Draw_Target_Icon(float, float);	//	マウス位置のアイコンを描画
};