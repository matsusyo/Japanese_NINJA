//	画面上に存在する爆弾を管理

#pragma once
#include"Bomb.h"		//	爆弾1つ1つの挙動
#include"InputCtrl.h"	//	キーマウス入力
#include"EnemyManager.h"//	敵

class Bomb_Manager
{
private:

	Bomb Bomb_Tbl[BOMB_NUM];	//	爆弾テーブル

	int Gr[11];	// 爆弾のグラフィック

public:
	Bomb_Manager();		//	コンストラクタ
	~Bomb_Manager();	//	デストラクタ
	void Register(float, float);	//	テーブルに登録する
	void Move();					//	毎フレームごとの爆弾の挙動
	void Judge_Enemy();				//	敵との当たり判定
	bool Judge_Player(RECT rc);		//	プレイヤーとの当たり判定
};
