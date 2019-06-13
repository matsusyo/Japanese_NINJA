//	分身クラスの宣言

#pragma once
#include<math.h>	//	atan2fを使用する
#include"DxLib.h"	//	DXライブラリ本体
#include"Literal.h"	//	定数
#include"Player.h"	//	プレイヤー
#include"Map.h"		//	マップ
#include"QtrCalc.h"	//	座標変換
#include"QtrDraw.h"	//	描画テーブル

const int CLONE_GR_NUM = 24;
const float CLONE_MP = -20.0f;	//	分身のMP消費量
const float CLONE_SPEED = 5.7f;	//	分身のスピード

class Clone
{
private:

	float Top_X, Top_Y;	//	トップ座標
	float Qtr_X, Qtr_Y;	//	クォーター座標
	float Camera_X, Camera_Y;	//	カメラ座標
	float Top_Angle, Qtr_Angle;	//	トップ,クォーター角度
	float Speed;				//	スピード
	float Size_X, Size_Y;		//	画像サイズ
	float H_Size_X, H_Size_Y;	//	画像サイズの半分のサイズ

	int	Clone_Gr[CLONE_GR_NUM];	//	分身のグラフィック　24個
	int	Clone_Gr_Num;			//	描画するグラフィックナンバー

	bool Clone_Flg;		//	分身の術が発動中ならtrue

public:

	//	コンストラクタ
	Clone();	//	こっちは使わない
	Clone(float, float, float, float);	//	プレイヤの座標と角度(TopとQtr)を渡す
	
	//	デストラクタ
	~Clone();

	//	分身の術を発動している間は呼ぶ
	void Move();

	//	アクセスメソッド
	float Get_Top_X() { return Top_X; }	//	トップ座標をもらう
	float Get_Top_Y() { return Top_Y; }
	float Get_Qtr_X() { return Qtr_X; }	//	クォーター座標をもらう
	float Get_Qtr_Y() { return Qtr_Y; }
	float Get_Camera_X() { return Camera_X; }	//	カメラをもらう
	float Get_Camera_Y() { return Camera_Y; }
	float Get_Top_Angle() { return Top_Angle; }	//	トップビューでの角度をもらう
	float Get_Qtr_Angle() { return Qtr_Angle; }	//	クォータービューでの角度をもらう
	float Get_Speed() { return Speed; }	//	スピードをもらう
	float Get_H_Size_X() { return H_Size_X; }	//	画像サイズの半分のサイズをもらう
	float Get_H_Size_Y() { return H_Size_Y; }
	
	bool Is_Clone_Flg() { return Clone_Flg; }	//	分身が存在しているかどうか

	//	RECTは小さくとる 当たり判定をより正確にするため
	RECT Get_Rect() {
		RECT MyRect = {
			(int)(Top_X - H_Size_X + 6),//	Left
			(int)(Top_Y - H_Size_Y + 6),//	Top
			(int)(Top_X + H_Size_X - 6),//	Right
			(int)(Top_Y + H_Size_Y - 6)	//	Bottom
		};
		return MyRect;
	}

	//	あたり判定　障害物のRECTを引数に渡す
	bool Collision(RECT rc) {
		RECT MyRect = Get_Rect();
		RECT Dst;	//	結果をうけとる

		// 当たっていればtrueを返す
		if (IntersectRect(&Dst, &MyRect, &rc))
			return true;
		else
			return false;
	}

	void Set_Top_X(float arg) { Top_X = arg; }	//	トップ座標をセット
	void Set_Top_Y(float arg) { Top_Y = arg; }
	void Set_Top_Angle(float arg) { Top_Angle = arg; }	//	トップ角度をセット
	void Set_Speed(float arg) { Speed = arg; }			//	スピードをセット
	void Set_Clone_Flg(bool arg) { Clone_Flg = arg; }	//	分身が存在しているか決めるフラグをセット
};