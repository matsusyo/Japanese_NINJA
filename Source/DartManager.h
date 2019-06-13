//	画面上に存在する手裏剣を管理

#pragma once
#include"Dart.h"		//	手裏剣1つ1つ
#include"InputCtrl.h"	//	キーマウス入力
#include"EnemyManager.h"//	敵

class Dart_Manager
{
private:
	Dart Dart_Tbl[DART_NUM];//	ショットテーブル
	int Gr[1];	// ショットのグラフィック １個

public:
	Dart_Manager();		//	コンストラクタ
	~Dart_Manager();	//	デストラクタ
	void Register(float,float,float,int);	// 手裏剣をテーブルに登録する
	void Move();		//	毎フレーム呼ぶ
	void Judge_Enemy();	//	敵との当たり判定
};