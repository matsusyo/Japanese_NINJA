#pragma once
#include<iostream>
#include"BitFlag.h"

//	プレイヤーや敵の状態の種類
const int
NORMAL = 0x01,	//	00000001	ノーマル
DAMAGE = 0x02,	//	00000010	ダメージ
DYING = 0x04,	//	00000100	死んでいる 死んだ時のモーション中
DIE = 0x08,		//	00001000	死亡
ALL = 0xFF;		//	11111111	全て

//	ステータス管理をビットで行う
class State
{
private:
	BitFlag Flg;	//	状態を管理するフラグ(8bitなので8種類まで管理できる)

public:
	// コンストラクタでFlagを0に初期化(不定値対策)
	State() :Flg() {}
	~State() {}


			// メソッド
	void Reset() { Flg.Off(ALL); }						//	状態をリセット
	void On(int flg) { Flg.On(flg); }					//	指定された状態をON(有効)にする
	void Off(int flg) { Flg.Off(flg); }					//	指定された状態をOFF(無効)にする
	int Get() { return Flg.Get(); }						//	現在の状態を貰う
	bool Is(int flg) { return Flg.Is(flg); }			//	聞かれた状態と一致する？
	bool Is_Either(int flg) { return Flg.Is_Either(flg); }	//	聞かれた状態のいずれかが該当する？
	bool Is_Healthy() { return Flg.Is(0); }				//	ノーマル状態か

};