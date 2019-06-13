//	プレイヤに必要な物を宣言

#pragma once
#include<math.h>
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"QtrCalc.h"
#include"Map.h"
#include"Gauge.h"
#include"State.h"
#include"QtrDraw.h"
#include"SoundManager.h"
#include"EffectManager.h"

//	キャラサイズ
const int PLY_SIZE_X = 32;
const int PLY_SIZE_Y = 32;

//	移動スピード
const float PLY_DEF_SPEED = 5.4f;

class Player {
private:

	float Top_X, Top_Y;			//	トップビュー座標
	float Qtr_X, Qtr_Y;			//	クォータービュー座標
	float Top_MX, Top_MY;		//	トップマウス座標
	float Qtr_MX, Qtr_MY;		//	クォーターマウス座標
	float Camera_X, Camera_Y;	//	カメラ座標
	float Top_Angle, Qtr_Angle;	//	トップ、クォータの角度
	float Speed;				//	移動スピード
	float Size_X, Size_Y;		//	プレイヤーの画像サイズ
	float H_Size_X, H_Size_Y;	//	プレイヤー画像の半分のサイズ

	int	Ply_Gr[24];		//	プレイヤーのグラフィック　24個
	int	Ply_Gr_Num;		//	描画するグラフィックナンバー
	int	Gauge_Gr[3];	//	HP 忍術ゲージ 外枠のグラフィック
	int	Damage_Cnt;		//	被弾時の無敵時間
	int	Dying_Cnt;		//	死ぬまでの時間

	bool Mp_Flg;		//	Mpが０になったらtrue、再びMpがMaxに戻ったらfalse
	bool Clear_Flg;		//	ステージクリアしたらtrueにする

public:
	Gauge* Gauge_Bar[3];//	HPとMPゲージゲージ
	State Status;		//	被弾、瀕死、死亡などプレイヤーの状態

	Player();
	~Player();
	void Move();		//	移動処理
	void Draw_Gauge();	//	HP、MP描画

	//	Get～ 各変数を返す
	float Get_Top_X() { return Top_X; }			//	トップ座標
	float Get_Top_Y() { return Top_Y; }
	float Get_Qtr_X() { return Qtr_X; }			//	クォーター座標
	float Get_Qtr_Y() { return Qtr_Y; }
	float Get_Top_MX() { return Top_MX; }		//	トップマウス座標
	float Get_Top_MY() { return Top_MY; }
	float Get_Qtr_MX() { return Qtr_MX; }		//	クォーターマウス座標
	float Get_Qtr_MY() { return Qtr_MY; }
	float Get_Camera_X() { return Camera_X; }	//	カメラ座標
	float Get_Camera_Y() { return Camera_Y; }
	float Get_Top_Angle() { return Top_Angle; }	//	トップ角度
	float Get_Qtr_Angle() { return Qtr_Angle; }	//	クォーター角度
	float Get_Speed() { return Speed; }			//	スピード
	float Get_H_Size_X() { return H_Size_X; }	//	プレイヤーの半分のサイズ
	float Get_H_Size_Y() { return H_Size_Y; }
	bool Is_Mp_Flg() { return Mp_Flg; }		//	Mp_Flg
	bool Is_Clear_Flg() { return Clear_Flg; }	//	Clear_Flg
	
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

	//	Set～ 各変数に引数を代入
	void Set_Top_X(float arg) { Top_X = arg; }			//	トップ座標
	void Set_Top_Y(float arg) { Top_Y = arg; }
	void Set_Top_Angle(float arg) { Top_Angle = arg; }	//	トップ角度
	void Set_Qtr_Angle(float arg) { Qtr_Angle = arg; }	//	クォーター角度
	void Set_Speed(float arg) { Speed = arg; }			//	スピード
	void Set_Damage_Cnt(int arg) { Damage_Cnt = arg; }	//	ダメージをうけた時の無敵時間
	void Set_Dying_Cnt(int arg) { Dying_Cnt = arg; }	//	死ぬまでにかかる時間
	void Set_Mp_Flg(bool arg) { Mp_Flg = arg; }			//	Mp_Flg
	void Set_Clear_Flg(bool arg) { Clear_Flg = arg; }	//	Clear_Flg
	void Hp_Proc(float);	//	Hp計算
	void Mp_Proc(float);	//	Mp計算
	void Status_Update();	//	ステータスを更新

};