#pragma once
#include"DxLib.h"		//	DXライブラリ本体
#include"Literal.h"		//	定数
#include"Player.h"		//	プレイヤー
#include"EnemyManager.h"//	敵
#include"Map.h"			//	マップ
#include"QtrCalc.h"		//	トップ,クォータビュー座標相互変換
#include"QtrDraw.h"		//	描画テーブル

//		定数
const int BOMB_NUM = 10;	//	存在できる爆弾数 MPMAXから出せる量
const int BOMB_SIZE_X = 32;	//	サイズ
const int BOMB_SIZE_Y = 32;
const int BOMB_EXPLOSION_TIME = 5 * 60;	//	5フレームで爆発
const int BOMB_GRAPH = 11;				//	11個の画像を使う

const float BOMB_MP = -12.0f;	//	爆弾のMP消費量
const float BOMB_SPEED = 8.0f;	//	スピード
const float EXPLOSION_SPREAD_SIZE_X = 0.15f;	//	爆弾をすこしずつ大きくするためにSizeに追加する X
const float EXPLOSION_SPREAD_SIZE_Y = 0.15f;	//	Yの拡大値
const float BOMB_DAMAGE = -5.0f;	//	爆弾のダメージ

class Bomb
{
private:
	
	float Top_X, Top_Y;			//	トップビュー座標
	float Qtr_X, Qtr_Y;			//	クォーター座標
	float Size_X, Size_Y;		//	サイズ
	float H_Size_X, H_Size_Y;	//	半分のサイズ
	float Top_Angle;			//	進行角度
	float Qtr_Angle;			//	描画する弾の向き

	int Type;				//	タイプ　
	int	Count;				//　生存時間
	int	Graph[BOMB_GRAPH];	//	グラフィック

	bool Flg;			//	爆弾が出てるか出てないかのフラグ
	bool Explosion_Flg;	//	爆発中かどうか

public:

	// コンストラクタ
	Bomb();
	//	デストラクタ
	~Bomb();

	//	アクセスメソッド
	void Set_Top_XY(float argx, float argy) { Top_X = argx; Top_Y = argy; }	//	トップビューの座標をセット
	void Set_Qtr_XY(float argx, float argy) { Qtr_X = argx; Qtr_Y = argy; }	//	クォータービューの座標をセット
	float Get_Top_X() { return Top_X; }	//	トップビュー上での座標をもらう
	float Get_Top_Y() { return Top_Y; }	
	float Get_Qtr_X() { return Qtr_X; }	//	クォータービュー上での座標をもらう
	float Get_Qtr_Y() { return Qtr_Y; }	

	void Set_Top_Angle(float arg) { Top_Angle = arg; }	//	トップビューでの角度をセット
	void Set_Qtr_Angle(float arg) { Qtr_Angle = arg; }	//	クォータービューでの角度をセット

	int Get_Type() { return Type; }			//	ボムのタイプをもらう
	void Set_Type(int arg) { Type = arg; }	//	ボムのタイプをセットする

	void Set_Graph_Handle(int number,int arggr) { Graph[number] = arggr; }	//	添え字番号を渡してグラフィックをセットする

	//	サイズをセット　セットするとH_Sizeもセットされる
	void Set_Size(float x, float y) { Size_X =x; Size_Y = y; H_Size_X = x / 2; H_Size_Y = y / 2; }	//	XYを一気に
	void Set_Size_X(float arg) { Size_X = arg; H_Size_X = Size_X / 2;};	//	Xのサイズをセット
	void Set_Size_Y(float arg) { Size_Y = arg; H_Size_Y = Size_Y / 2; };//	Yのサイズをセット

	//	サイズを増加　爆弾を大きくしていく
	void Spread_Size_X(float arg) { Size_X += arg; H_Size_X = Size_X / 2; };	//	Xのサイズを大きくする
	void Spread_Size_Y(float arg) { Size_Y += arg; H_Size_Y = Size_Y / 2; };	//	Yのサイズを大きくする

	//	RECTをもらう RECTは少し大きめにとる
	RECT Get_Rect() {
		RECT MyRect = {
			(int)(Top_X - H_Size_X-20),	//	Left
			(int)(Top_Y - H_Size_Y-20),	//	Top
			(int)(Top_X + H_Size_X+20),	//	Right
			(int)(Top_Y + H_Size_Y+20)	//	Bottom
		};
		return MyRect;
	}
	
	void Set_Count(int arg) { Count = arg; }	//	カウントをセットする
	void Set_Flg(bool arg) { Flg = arg; }		//	爆弾が存在するかセット trueで存在する
	bool Is_Flg() { return Flg; }				//	爆弾が存在してるかどうか

	void Set_Explosion_Flg(bool arg) { Explosion_Flg = arg; }	//	爆発中かどうかセットする trueで爆発
	bool Is_Explosion_Flg() { return Explosion_Flg; }			//	爆発中かどうか

	void Move();		//	挙動
	void Judge_Enemy();	//	敵との当たり判定
	
	//	引数のRECTと当たったかどうかだけ返す
	bool Collision(RECT rc) {
		RECT BombRect = Get_Rect();	//	爆弾のRECT
		RECT Dst;	//	結果

		//	当たってたら true
		if (IntersectRect(&Dst, &rc, &BombRect))
			return  true;

		else return false;
	}
};