#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"Player.h"
#include"QtrCalc.h"

//	ステージ数
const int STAGE_NUM = 6;
const int CHIP_SIZE_X = 64;
const int CHIP_SIZE_Y = 64;
//
//const char* STAGE_LIST[STAGE_NUM + 1]={
//	"",
//	"Data/map1_1.csv",
//	"Data/map1_2.csv",
//	"Data/map1_3.csv",
//	"Data/map1_4.csv",
//	"Data/map1_5.csv",
//	"Data/map1_6.csv"
//};

const enum STAGE_NAME {
	NO_STAGE, AAA, BBB, CCC, DDD, EEE, FFF
};


class Map {
		
	int Map_Data[MAP_Y][MAP_X];	//	マップデータ
	int	Chip_X, Chip_Y;			//	マップチップ番号 X,Y
	int Chip[16];				//	マップチップ数 １６個
	int	Chip_Num;				//	マップチップの変数

	float Qtr_X, Qtr_Y;			//	クォータ座標
	float Size_X, Size_Y;		//	マップサイズ
	float H_Size_X, H_Size_Y;	//	マップサイズの半分
	float Camera_X, Camera_Y;	//	カメラ位置

public:
	
	Map();		//	コンストラクタ
	Map(int stage);	//	こっちのコンストラクタを使用
	~Map();		//	デストラクタ

	//	マップの床だけ描画
	void Tile_Draw();

	//	マップの通れない部分を描画クラスに登録
	void Map_Register();		

	//	マップチップの値を取得する関数
	int Map_Get_Chip(const float &topx,const float &topy);
};