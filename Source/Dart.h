//	手裏剣クラスの宣言

#pragma once
#include<math.h>	//	三平方の定理で使用
#include"DxLib.h"	//	DXライブラリ本体
#include"Literal.h"	//	定数
#include"Player.h"	//	プレイヤー
#include"EnemyManager.h"	//	敵
#include"Map.h"				//	マップ
#include"QtrCalc.h"			//	座標変換
#include"QtrDraw.h"			//	描画テーブル
#include"SoundManager.h"	//	音

//	定数
const int DART_NUM = 10;	//	存在できる手裏剣数
const int DART_SIZE_X = 24;	//	サイズ
const int DART_SIZE_Y = 24;
const int DART_H_SIZE_X = DART_SIZE_X / 2;	//	半分のサイズ
const int DART_H_SIZE_Y = DART_SIZE_Y / 2;
const int DART_COUNT = 2 * 60;	//	手裏剣が存在できる時間

const float DART_MP = -7.5f;		//	手裏剣のMP消費量
const float DART_SPEED = 8.0f;		//	手裏剣のスピード
const float DART_RADIUS = 0.175f;	//	手裏剣の旋回率

class Dart
{
private:
	
	float Top_X, Top_Y;	//	トップ座標
	float Qtr_X, Qtr_Y;	//	クォーター座標
	float Size_X, Size_Y;	//	手裏剣のサイズ
	float H_Size_X, H_Size_Y;
	float Speed;		//	移動スピード
	float Top_Angle;	//	進行角度
	float Qtr_Angle;	//	描画する弾の向き
	
	int Type;		//	手裏剣の種類　今回は１種類の予定 
	int	Graph;		//	グラフィック
	int	Count;		//	カウント　弾の生存時間
	int	Target_Num;	//	ターゲットの番号

	bool Flg;	//	手裏剣が出てるか出てないかのフラグ

	Enemy *Target;	//	適当な敵をターゲットにする

public:

	// コンストラクタ
	Dart();
	//	デストラクタ
	~Dart();

	//	アクセスメソッド
	float Get_Top_X() { return Top_X; }	//	トップ座標セット
	float Get_Top_Y() { return Top_Y; }
	void Set_Top_XY(float argx, float argy) { Top_X = argx; Top_Y = argy; }	//	X,Y同時にセット

	float Get_Qtr_X() { return Qtr_X; }	//	クォーター座標セット
	float Get_Qtr_Y() { return Qtr_Y; }
	void Set_Qtr_XY(float argx, float argy) { Qtr_X = argx; Qtr_Y = argy; }	//	X,Y同時にセット

	void Set_Speed(float arg) { Speed = arg; }	//	スピードをセット
	void Set_Top_Angle(float arg) { Top_Angle = arg; }	//	トップ角度をセット
	void Set_Qtr_Angle(float arg) { Qtr_Angle = arg; }	//	クォーター角度をセット

	int Get_Type() { return Type; }	//	手裏剣のタイプをゲット	
	void Set_Type(int arg) { Type = arg; }	//	手裏剣のタイプをセット

	void Set_Graph_Handle(int Arg) { Graph = Arg; }	//	グラフィックハンドルをセット

	//	サイズ,ハーフサイズをX,Yまとめてセット
	void Set_Size(float x, float y) { Size_X = x; Size_Y = y; H_Size_X = x / 2; H_Size_Y = y / 2; }
	
	//	RECTのゲット
	RECT Get_Rect() {
		RECT MyRect = {
			//	RECTは小さめにとる
			(int)Top_X - DART_H_SIZE_X + DART_SIZE_X / 4,	//	Left
			(int)Top_Y - DART_H_SIZE_Y + DART_SIZE_Y / 4,	//	Top
			(int)Top_X + DART_H_SIZE_X - DART_SIZE_X / 4,	//	Right
			(int)Top_Y + DART_H_SIZE_Y - DART_SIZE_Y / 4	//	Bottom
		};
		return MyRect;
	}
	
	void Set_Count(int arg) { Count = arg; }	//	手裏剣の存在時間をセット
	void Set_Flg(bool arg) { Flg = arg; }		//	手裏剣存在フラグをセット
	bool Is_Flg() { return Flg; }				//	手裏剣が存在しているか返す

	void SetTarget(Enemy* arg) { Target = arg; }	//	ターゲットをセットする
	Enemy* Get_Target() { return Target; }			//	ターゲットを返す
	void SetTargetNum(int arg) { Target_Num = arg; }//	ターゲットの添え字を返す
	
	void Move();	//	挙動
	void Judge_Enemy();	//	敵との当たり判定

	//	受け取ったRECTと接触してるか返す
	bool Collision(RECT rc) {
		RECT EnyRect = Get_Rect();	//	敵のRECT
		RECT Dst;	//	結果

		//	当たってたら true
		if (IntersectRect(&Dst, &rc, &EnyRect))
			return  true;

		else return false;
	}
};

