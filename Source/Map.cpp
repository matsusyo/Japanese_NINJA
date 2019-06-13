#include"Map.h"

//	オブジェクトのポインタ
extern Player *Ply;						//	プレイヤー
extern Quarter_Calculation *Qtr_Calc;	//	座標計算
extern Qtr_Draw_Manager *My_Draw;		//	描画テーブル


//	コンストラクタ
Map::Map() {};
Map::Map(int stage)
{
	Size_X = (float)CHIP_SIZE_X;
	Size_Y = (float)CHIP_SIZE_Y;
	H_Size_X = Size_X / 2;
	H_Size_Y = Size_Y / 2;
	FILE* Fp;	//	ファイルのポインタを宣言
	errno_t Err;

	//	マップをtxtファイルから読み込む
	switch (stage) {
	default:
	case AAA:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_1.csv", "r");//Fpを読み取り形式で開く
		break;
	case BBB:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_2.csv", "r");//Fpを読み取り形式で開く
		break;
	case CCC:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_3.csv", "r");//Fpを読み取り形式で開く
		break;
	case DDD:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_4.csv", "r");//Fpを読み取り形式で開く
		break;
	case EEE:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_5.csv", "r");//Fpを読み取り形式で開く
		break;
	case FFF:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_6.csv", "r");//Fpを読み取り形式で開く
		break;
	}

	if (Fp == NULL)DebugBreak();
	int c;			//	文字を格納する変数
	int y = 0;
	int x = 0;
	char buf[10];	//	文字列を格納する
	memset(buf, 0, sizeof(buf));
	bool Flg = false;
	while (1) {
		while (1) {
			c = fgetc(Fp);	//	文字読んでcに格納
			if (c == EOF) { Flg = true;	break; }	

			//	cがセルの区切りか改行でなければ、bufに連結する
			if (c != ',' && c != '\n') { strcat_s(buf, (const char*)&c); }
			
			//	区切りか改行ならループを抜ける
			else {
				Chip_Num = atoi(buf);	//	bufをint型にして、Chip_Numに代入
				Map_Data[y][x] = Chip_Num;
				memset(buf, 0, sizeof(buf));	//	bufをリセット
				break;
			}
		}

		//	1セル分のループを抜けた時
		if (Flg)break;					//	ファイルを全て読み込み終えたらbreak
		if (c == ',')x++;				//	読み込んだ文字がカンマなら1行セルを移動
		if (c == '\n') { y++; x = 0; }	//	改行ならセルを0行目にして1列セルを移動
	}
	fclose(Fp);
}

//	デストラクタ
Map::~Map(){}

//	マップを描画テーブルに登録
void Map::Map_Register()
{
	//	カメラ位置
	Camera_X = Ply->Get_Camera_X();
	Camera_Y = Ply->Get_Camera_Y();

	//	マップを描画テーブルに登録 X=MAP_X
	for (int y = 0; y < MAP_Y; y++)
		for (int x = MAP_X - 1; x >= 0; x--) {
			Qtr_X = (float)((x + y) * (CHIP_SIZE_X / 2));
			Qtr_Y = (float)((-x + y) * (CHIP_SIZE_Y / 4));
			float X = (float)(Qtr_X + H_Size_X - Camera_X);
			float Y = (float)(Qtr_Y + H_Size_Y - Camera_Y);
			//	ウィンドウにマップが入りきる分だけ登録
			if (X <= WINDOW_X + CHIP_SIZE_X * 2 && X >= -CHIP_SIZE_X * 2 && Y <= WINDOW_Y + CHIP_SIZE_Y * 2 && Y >= -CHIP_SIZE_Y * 2 && Map_Data[y][x] >= 7)
				My_Draw->Register(X, Y, H_Size_X, H_Size_Y, MAP, Chip[Map_Data[y][x]]);
		}
}

//	移動できるマップチップだけ描画する
void Map::Tile_Draw()
{
	//	カメラ位置
	Camera_X = Ply->Get_Camera_X();
	Camera_Y = Ply->Get_Camera_Y();
	//	クォーターマップ描画　奥から描画する X=MAP_X
	for (int y = 0; y < MAP_Y; y++)
		for (int x = MAP_X - 1; x >= 0; x--)
			if (Map_Data[y][x] < 7) {
				Qtr_X = (float)((x + y) * (CHIP_SIZE_X / 2));
				Qtr_Y = (float)((-x + y) * (CHIP_SIZE_Y / 4));
				float X = (float)(Qtr_X - Camera_X);
				float Y = (float)(Qtr_Y - Camera_Y);
				if (X <= WINDOW_X + CHIP_SIZE_X * 2 && X >= -CHIP_SIZE_X * 2 && Y <= WINDOW_Y + CHIP_SIZE_Y * 2 && Y >= -CHIP_SIZE_Y * 2)
					DrawGraph((int)(Qtr_X - Camera_X), (int)(Qtr_Y - Camera_Y), Chip[Map_Data[y][x]], TRUE);
			}
}

//	トップビュー座標からマップチップの値を取得する
int Map::Map_Get_Chip(const float &topx, const float &topy)
{	
	//	変換して返す
	Qtr_Calc->Top_TopChip(topx, topy, &Chip_X, &Chip_Y);
	return Map_Data[Chip_Y][Chip_X];
}