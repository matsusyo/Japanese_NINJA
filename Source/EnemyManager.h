//	全て敵を管理	

#pragma once
#include"Enemy.h"

const int ENY_NUM = 100;	//	敵の最大数
const int ENEMY_BLUE_GR_NUM = 24;	//	雑魚敵のグラフィック数
const int ENEMY_BOSS_GR_NUM = 24;	//	ボスのグラフィック

const float DART_TARGET_DISTANCE = 50;

class Enemy_Manager
{
private:
	Enemy* Eny_Tbl[ENY_NUM];	//	最大数分の敵テーブル
	int Enemy_Blue_Graph[ENEMY_BLUE_GR_NUM];	//	雑魚敵の画像
	int Enemy_Boss_Graph[ENEMY_BOSS_GR_NUM];	//	ボスの画像
	int Blue_Gauge_Graph[2];	//	雑魚のHp画像
	int Boss_Gauge_Graph[2];	//	ボスのHp画像

public:
	Enemy_Manager();
	~Enemy_Manager();

	bool Judge_Player_Attack(RECT rc, float dmg, int type);	//	敵全てのダメージ計算
	bool Judge_Player(RECT rc);	//	敵と何かが接触しているか返す
	
	//	毎フレーム呼ぶ
	void Move();			//	挙動
	void Status_Update();	//	ステータスの更新 生きてるか死んでるかとか
	void Draw_Ray();		//	視界を描画
	void Draw_Gauge();		//	HPを描画

	//	敵を生成する　エリアごとに分けて呼ぶ
	void Register_Enemy();	
	//	敵をホーミングする時に使用
	Enemy* Get_Target(float, float);	//	手裏剣のターゲットをもらう
};