//	分身の処理 1体しか出れない

#include"Clone.h"

extern Quarter_Calculation *Qtr_Calc;	//	トップ,クォーター座標相互変換
extern Map *My_Map;						//	マップ
extern Player *Ply;						//	プレイヤー
extern Qtr_Draw_Manager *My_Draw;		//	描画テーブル

//	コンストラクタ
Clone::Clone() {};	//	こっちは使わない
Clone::Clone(float topX,float topY,float topAngle,float qtrAngle)
{
	//	プレイヤーが向いている方向に分身が走るようにする
	Top_X = topX;	//	プレイヤーの座標を登録
	Top_Y = topY;
	Top_Angle = topAngle;	//	トップビュー上での角度
	Qtr_Angle = qtrAngle;	//	クォータビュー上での角度
	Speed = CLONE_SPEED;	//	分身のスピード

	//	プレイヤーと同じサイズをもつ
	Size_X = PLY_SIZE_X; Size_Y = PLY_SIZE_Y;
	H_Size_X = Size_X / 2; H_Size_Y = Size_Y / 2;

	LoadDivGraph("Image/Player/Ninja.png", 24, 6, 4, 32, 50, Clone_Gr);	//	画像読み込み
	Clone_Gr_Num = 0;	//	初期化
	Clone_Flg = true;	//	分身存在フラグを立てる
}

//	デストラクタ
Clone::~Clone() {}

//	動き
void Clone::Move()
{
	Clone_Gr_Num = rand() % 3;	//	プレイヤー画像を動かす

	//	X,Yの移動量計算
	float MoveX = cosf(Top_Angle)*Speed;
	float MoveY = sinf(Top_Angle)*Speed;

	//	座標に加えて分身を動かす
	Top_X += MoveX;	
	Top_Y += MoveY;

	//	壁にぶつかったら消滅させる
	if (My_Map->Map_Get_Chip(Top_X, Top_Y) >= 7) {
		//	初期化
		Top_X = Top_Y = NULL;
		Clone_Flg = false;	//	分身存在フラグを落とす
	}

	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	クォーターキャラ座標計算

	//	カメラ位置を計算
	Camera_X = Ply->Get_Camera_X();
	Camera_Y = Ply->Get_Camera_Y();

	//	分身の角度によって分身の画像を変える
	if (Qtr_Angle < 175 && Qtr_Angle>95)Clone_Gr_Num += 3;				//	左斜め下
	else if (Qtr_Angle <= 95 && Qtr_Angle >= 85)Clone_Gr_Num += 0;		//	正面
	else if (Qtr_Angle < 85 && Qtr_Angle>5)Clone_Gr_Num += 9;			//	右斜め下
	else if (Qtr_Angle >= 175 || Qtr_Angle <= -175)Clone_Gr_Num += 6;	//	左横
	else if (Qtr_Angle <= 5 && Qtr_Angle >= -5)Clone_Gr_Num += 12;		//	右横
	else if (Qtr_Angle > -175 && Qtr_Angle < -95)Clone_Gr_Num += 15;	//	左斜め上
	else if (Qtr_Angle >= -95 && Qtr_Angle <= -85)Clone_Gr_Num += 18;	//	後ろ向き
	else if (Qtr_Angle > -85 && Qtr_Angle < -5)Clone_Gr_Num += 21;		//	右斜め上

	//	クォータービュー上での中心座標を描画テーブルに登録
	float X = (float)(Qtr_X + QTR_OFFSET_X - Camera_X);	//	中心座標を計算
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);
	My_Draw->Register(X, Y, H_Size_X, H_Size_Y, CLONE, Clone_Gr[Clone_Gr_Num]);	//	登録
}

