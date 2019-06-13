#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"Map.h"
#include"InputCtrl.h"
#include"SoundManager.h"

const int STAGE_GR_NUM = 6;	//	ステージ画像のグラフィック数

class Stage
{
private:
	int Cnt;					//	カウンター
	int Select_Stage;				//	ステージ名
	int StageGr[STAGE_GR_NUM];	//	各ステージのグラフィック名
	int FontHandle[FONT_MAX];	//	フォント

public:
	Stage();			//	コンストラクタ
	~Stage();			//	デストラクタ
	int Update();		//	ステージ画面中呼ぶ
	void Load_Font();	//	フォント読み込み
	void Load_Graph();	//	画像読み込み
	void Delete_Font();	//	フォント削除
	void Delete_Graph();//	画像削除
	void Draw_Title();	//	見出し描画
	void Draw_Stage();	//	各ステージ画像描画
	int Draw_Target_Stage(float, float);	//	マウス位置のステージ画像描画
	int Get_Select_Stage() { return Select_Stage; }	//	現在のステージをゲット
	void Set_Select_Stage(int arg) { Select_Stage = arg; }	//	ステージをセット
};