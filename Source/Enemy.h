//	敵単体

#pragma once
#include<math.h>
#include"DxLib.h"
#include"Literal.h"
#include"Player.h"
#include"Map.h"
#include"QtrCalc.h"
#include"Clone.h"
#include"Gauge.h"
#include"State.h"
#include"QtrDraw.h"
#include"EffectManager.h"
#include"SoundManager.h"
#include"TimeManager.h"

//	敵の種類
const enum ENEMY_TYPE {
	ENEMY_BOSS,
	ENEMY_BLUE,
};

const int ENY_SIZE_X = 32;				//	敵の画像サイズ
const int ENY_SIZE_Y = 32;
const int ENY_BLUE_HP_MAX = 50;			//	雑魚敵の最大Hp
const int ENY_BOSS_HP_MAX = 50;		//	ボスの最大Hp
const float ENEMY_SPEED = 5.5f;			//	移動速度
const int ENEMY_GR_NUM = 24;			//	グラフィック数
const int CHANGE_ANGLE_TIMING = 4 * 60;	//	敵が向きを変えるタイミング
const int MOVE_TIMING = 5 * 60;			//	敵が移動するタイミング
const int MOVE_TIME = (int)(0.1 * 60);	//	移動するタイミングになった時動く時間
const float ABS_ANGLE_BASE = 25;		//	視界角度の絶対値　敵の角度から+-して視界を作る
const int ABS_ANGLE_ADD = 10;			//	視界角度を敵によって変える
const float VISION_BASE = 260;			//	視認できる距離の基準値
const int VISION_ADD = 60;				//	基準値にランダムに60以下の数値を+-して敵によって視認距離を変える


class Enemy
{
protected:

	float Top_X, Top_Y;			//	トップビュー座標
	float Top_Angle;			//	トップビュー角度
	float Top_Radian;			//	トップビューでのラジアン
	float Qtr_X, Qtr_Y;			//	クォータービュー座標
	float Qtr_Angle;			//	クォーター角度
	float Camera_X, Camera_Y;	//	カメラ座標                     
	float Speed;				//	スピード
	float Size_X, Size_Y;		//	キャラサイズ
	float H_Size_X, H_Size_Y;	//	半分のサイズ
	
	int Eny_Gr[24];	//	敵グラフィック24個
	int	Eny_Gr_Num;	//	グラフィック番号
	int	Gauge_Gr[2];//	HP,外枠
	int Damage_Cnt;	//	ダメージを受けた時の無敵時間
	int Dying_Cnt;	//	死ぬまでの時間

	bool Hp_Flg;	//	HPがあるかないか
	bool Find_Flg;	//	プレイヤーを見つけたらtrue

public:
	Gauge* Gauge_Bar[2];//	ゲージオブジェクト
	State Status;		//	ステータスオブジェクト

	Enemy();
	~Enemy();
	
	//	オーバーライド
	virtual void Move();			//	敵の挙動
	virtual void Draw_Gauge();		//	HPを描画
	virtual void Draw_Ray(){}		//	敵の視界を描画
	virtual void Status_Update();	//	Status状態管理
	virtual bool Judge_Move(RECT, float*, float*, float, float);	//	移動できるか判定する　移動したら true を返す
	
	//	クォータービュ上での角度によって敵画像を変える
	virtual int Change_Gr(float qtrangle) {	
		if (qtrangle < 175 && qtrangle>95)return 21;			//	左斜め下
		else if (qtrangle <= 95 && qtrangle >= 85)return 18;	//	正面
		else if (qtrangle < 85 && qtrangle>5)return 15;			//	右斜め下
		else if (qtrangle >= 175 || qtrangle <= -175)return 12;	//	左横
		else if (qtrangle <= 5 && qtrangle >= -5)return 6;		//	右横
		else if (qtrangle > -175 && qtrangle < -95)return 9;	//	左斜め上
		else if (qtrangle >= -95 && qtrangle <= -85)return 0;	//	後ろ向き
		else if (qtrangle > -85 && qtrangle < -5)return 3;		//	右斜め上
		return 0;
	}

	//	アクセスメソッド
	float Get_Top_X() { return Top_X; }
	float Get_Top_Y() { return Top_Y; }
	float Get_Qtr_X() { return Qtr_X; }
	float Get_Qtr_Y() { return Qtr_Y; }
	float Get_Camera_X() { return Camera_X; }
	float Get_Camera_Y() { return Camera_Y; }
	float Get_Top_Angle() { return Top_Angle; }
	float Get_Top_Radian() { return Top_Radian; }
	float Get_Qtr_Angle() { return Qtr_Angle; }
	float Get_Speed() { return Speed; }

	virtual int Get_Type() { return NULL; }	//	敵のタイプを返す
	bool Is_Hp_Flg() { return Hp_Flg; }	//	Hpがなければ true

	RECT Get_Rect() {	//	RECTを返す
		RECT MyRect = {
			(int)(Top_X - H_Size_X + 6),//	Left
			(int)(Top_Y - H_Size_Y + 6),//	Top
			(int)(Top_X + H_Size_X - 6),//	Right
			(int)(Top_Y + H_Size_Y - 6)	//	Bottom
		};
		return MyRect;
	}

	bool Collision(RECT rc) {	//	受け取ったRECTと接触してるか返す
		RECT EnyRect = Get_Rect();	//	敵のRECT
		RECT Dst;	//	結果

		//	当たってたら true
		if (IntersectRect(&Dst, &rc, &EnyRect))return  true;
		else return false;
	}

	void Set_Top_X(float arg) { Top_X = arg; }
	void Set_Top_Y(float arg) { Top_Y = arg; }
	void Set_Top_Angle(float arg) { Top_Angle = arg; }
	void Set_Top_Radian(float arg) { Top_Radian = arg; }
	void Set_Speed(float arg) { Speed = arg; }
	void Set_Damage_Count(int arg) { Damage_Cnt = arg; }
	void Set_Dying_Count(int arg) { Dying_Cnt = arg; }
	void Set_Hp_Flg(bool arg) { Hp_Flg = arg; }
};

//	敵兵士（仮）
class Enemy_Blue :public Enemy {
private:
	int Change_Angle_Cnt;		//	向きを変えるタイミングを計る
	int Walk_Cnt;				//	移動するタイミングをはかる
	int Target_Name;			//	ターゲットの名前を記録
	float Top_End_X, Top_End_Y;	//	トップビューでの視界の終点
	float Qtr_End_X, Qtr_End_Y;	//	クォータービューでの視界の終点
	float Target_Angle;			//	ターゲットへの角度
	float Target_Radian;		//	ターゲットへのラジアン
	float Target_Abs_Angle;		//	視認したターゲット角度の絶対値
	float Vision_Distance;		//	視認できる距離
	float Abs_Angle;			//	視界の絶対値　Top_Angleから+-して視界の範囲にする

public:
	Enemy_Blue() {};	//	コンストラクタ　こっちは使わない
	Enemy_Blue(float argx, float argy,int graph[],int gaugegr[]);	//	コンストラクタ
	~Enemy_Blue();	//	デストラクタ
	void Move();	//	継承した場合は固有の動きをつける
	bool Search_Player();	//	プレイヤーを探す
	float Calc_Distance(RECT);	//	プレイヤーまたは分身から敵への距離を求める
	float Calc_Abs_Angle(float,float);	//	基準角度からプレイヤー角度への絶対値を求める
	bool Judge_Wall(float, float, float, bool*);	//	プレイヤーが壁越しにいるかどうか
	void Draw_Ray();	//	視界を描画
	int Get_Type() { return ENEMY_BLUE; }	//	敵の種類を返す
	int Get_Target_Name() { return Target_Name; }
	void Set_Target_Name(int arg) { Target_Name = arg; }
};

//	敵
class Enemy_Boss :public Enemy {
private:
	int Cnt_Angle;	//	角度を変えるタイミング
	float Vision_X, Vision_Y;		//	プレイヤーを視認してるか
	bool Move_Flg_X, Move_Flg_Y;
	float Qtr_Vision_X, Qtr_Vision_Y;
public:
	Enemy_Boss() {};
	Enemy_Boss(float argx, float argy,int graph[],int gaugegr[]);
	~Enemy_Boss();

	void Move();
	void Status_Update();
	int Get_Type() { return ENEMY_BOSS; }
};