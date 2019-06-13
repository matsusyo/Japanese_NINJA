//	手裏剣1つ1つの処理

#include"Dart.h"

extern Player *Ply;						//	プレイヤー
extern Enemy_Manager *Eny;				//	敵
extern Map *My_Map;						//	マップ
extern Quarter_Calculation *Qtr_Calc;	//	座標計算
extern Qtr_Draw_Manager *My_Draw;		//	描画テーブル
extern Sound_Manager *My_Sound;			//	音

//	コンストラクタ
Dart::Dart() {}
//	デストラクタ
Dart::~Dart(){}

//	手裏剣の動き
void Dart::Move() {
	if (!Flg) return;	//	フラグの立っていなければ何もしない
	
	//	 発射後の動き 追尾する
	Target = Eny->Get_Target(Top_X, Top_Y);	//	ターゲットをもらう
	
	//	ターゲットがいたら向かって旋回する
	if (Target!=NULL) {
		float bx = cosf(Top_Angle);		//	ショットの進んでいる X 方向
		float by = sinf(Top_Angle);		//	ショットの進んでいる Y 方向
		float ax = Target->Get_Top_X() - Top_X;	//	進むべき X 方向
		float ay = Target->Get_Top_Y() - Top_Y;	//	進むべき Y 方向
		float Radius = DART_RADIUS;		//	旋回率 (フレームごとのラジアン)

		//	外積を利用しターゲット側に旋回する
		Top_Angle += (ax * by - ay * bx < 0.0) ? +Radius : -Radius;
	}

	//	手裏剣の向きに移動させる
	Top_X += cosf(Top_Angle) * Speed;	//	x移動
	Top_Y += sinf(Top_Angle) * Speed;	//	y移動

	//	画面外へ出たらを消す
	if (Top_Y < 0 || Top_Y > MAP_Y*CHIP_SIZE_Y || Top_X < 0 || Top_X > MAP_X*CHIP_SIZE_X) {
		Flg = false;
	}

	//	壁に当たっても消す
	if (My_Map->Map_Get_Chip(Top_X, Top_Y) >= 7) {
		Flg = false;
	}

	Count--;	//	存在時間を減らす
	if (Count <= 0) Flg = false;	//	0になったら消す

	//	クォータ座標計算
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	クォータ座標計算
	float X = (float)(Qtr_X + QTR_OFFSET_X - Ply->Get_Camera_X());	//	描画位置計算
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Ply->Get_Camera_Y());
	My_Draw->Register(X, Y, H_Size_X, H_Size_Y, DART, Graph);		//	描画テーブルに登録
}


//	敵との当たり判定
void Dart::Judge_Enemy()
{
	//	敵に当たっているか判定し当たったら手裏剣は消える
	RECT rc = Get_Rect();
	if (Eny->Judge_Player_Attack(rc, DART_DAMAGE, DART)) {
		//	初期化
		Top_X = Top_Y = NULL;
		Flg = false;
	}
}