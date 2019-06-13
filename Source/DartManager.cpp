//	手裏剣を全て管理する

#include"DartManager.h"	//	すべての手裏剣管理

extern Input_Controll *Inp_Ctrl;		//	キー入力
extern Quarter_Calculation *Qtr_Calc;	//	座標計算

//	コンストラクタ
Dart_Manager::Dart_Manager() 
{
	//	手裏剣のグラフィックの読み込み
	Gr[0] = LoadGraph("image/Dart/Knife1.png");
	//	テーブル内のフラグを落としておく
	for (int i = 0; i < DART_NUM; i++) Dart_Tbl[i].Set_Flg(false);	
}

//	デストラクタ
Dart_Manager::~Dart_Manager(){}

//	手裏剣をテーブルに登録する
void Dart_Manager::Register(float Top_X, float Top_Y, float Angle, int Type)
{
	//	使用されているところはスキップ
	for (int i = 0; i < DART_NUM; i++) {

		if (Dart_Tbl[i].Is_Flg()) continue;

		//	手裏剣登録
		Dart_Tbl[i].Set_Flg(true);	//	存在フラグを立てる
		Dart_Tbl[i].Set_Type(Type);	//	手裏剣のタイプをセット
		Dart_Tbl[i].Set_Top_XY(Top_X, Top_Y);	//	座標をセット
		Dart_Tbl[i].Set_Speed(DART_SPEED);		//	スピードセット
		Dart_Tbl[i].Set_Top_Angle(Angle);		//	トップビューでの角度をセット
		Dart_Tbl[i].Set_Size((float)DART_SIZE_X, (float)DART_SIZE_Y);	//	サイズをセット
		Dart_Tbl[i].Set_Graph_Handle(Gr[Type]);	//	タイプによってグラフィックを決める
		Dart_Tbl[i].Set_Count(DART_COUNT);		//	存在できる時間をセット	

		break;	// ひとつ登録したら抜ける
	}
}

//	フレームごとの挙動
void Dart_Manager::Move() 
{
	//	手裏剣一個ごとの挙動
	for (int i = 0; i<DART_NUM; i++) {
		Dart_Tbl[i].Move();	
	}
}

//	当たり判定
void Dart_Manager::Judge_Enemy()
{
	//	手裏剣一個ごとに判定
	for (int i = 0; i < DART_NUM; i++) {
		Dart_Tbl[i].Judge_Enemy();
	}
}