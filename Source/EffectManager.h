#pragma once

#include<vector>
#include "DxLib.h"		// DXライブラリの本体
#include"Player.h"
#include "SoundManager.h"	// 音

extern Sound_Manager *My_Sound;		//	音

using namespace std;	//	vectorを使用に必要

const enum EFFECT {
	EFFECT_DART,
	EFFECT_BOMB,
	EFFECT_WALL,
	EFFECT_COLLISION,
	EFFECT_ALERT,
};

const int DART_FRAME_PER = 3;	//	手裏剣の描画フレーム数
const int BOMB_FRAME_PER = 1;	//	爆弾の描画フレーム数
const int WALL_FRAME_PER = 3;	//	壁と衝突時の描画フレーム数
const int COLLISION_FRAME_PER = 1;	//	敵と衝突時の描画フレーム数
const int ALERT_FRAME_PER = 9;	//	プレイヤー発見時の描画フレーム数

const int DART_MOTION_NUM = 5;	//	手裏剣衝突時のモーション数
const int BOMB_MOTION_NUM = 7;	//	爆弾のモーション数
const int WALL_MOTION_NUM = 8;	//	壁と衝突時のモーション数
const int COLLISION_MOTION_NUM = 10;	//	敵と衝突時のモーション数
const int ALERT_MOTION_NUM = 1;	//	プレイヤー発見時のモーション数

const float DART_EFFECT_SIZE = 0.6f;		//	手裏剣のエフェクトの大きさ
const float BOMB_EFFECT_SIZE = 0.5f;		//	爆弾のエフェクトの大きさ
const float WALL_EFFECT_SIZE = 0.2f;		//	壁衝突時のエフェクトの大きさ
const float COLLISION_EFFECT_SIZE = 0.2f;	//	敵と衝突時のエフェクトの大きさ
const float ALERT_EFFECT_SIZE = 1.0f;	//	プレイヤー発見時のエフェクトの大きさ

// エフェクトひとつ分のクラス
class Effect {
private:
	float Pos_X;	//	座標
	float Pos_Y;
	int Cnt;		//	モーション用のカウンタ
	int *Grpt;		//	参照するグラフィックハンドラへの参照ポインタ
	int Type;		//	エフェクトのタイプ
	double Angle;	//	描画の角度に使う

public:
	// Effectのコンストラクタ
	Effect() {}	// 使わないけど必要 エラー対策
	Effect(float argx, float argy, int *arggr, int type) {	// こっちを使う
		Pos_X = argx;	// 座標情報
		Pos_Y = argy;
		Grpt = arggr;	// グラフィックハンドラ
		Type = type;
		Cnt = 0;		// モーションカウンタのクリア
		
		//	角度設定
		switch (Type) {
		case EFFECT_DART:	//	手裏剣が当たった時
		case EFFECT_BOMB:	//	爆弾が当たった時
		case EFFECT_WALL:	//	壁とぶつかった時
		case EFFECT_COLLISION://	敵とプレイヤーまたは分身が当たった時
			Angle = GetRand(360) / 180.0 * DX_PI;	// 描画の角度を決める
				break;
		case EFFECT_ALERT:
			Angle = 0;
			break;
		default:
			break;
		}
	}

	// 描画
	bool Draw(float camerax, float cameray) {
		Cnt++;		// モーションカウンタのインクリメント

		int X = (int)(Pos_X - camerax);
		int Y = (int)(Pos_Y - cameray);
		double EffectSize;

		//	タイプによって当たった時エフェクトを変える
		switch (Type) {
		case EFFECT_DART:	//	手裏剣が当たった時
			if (Cnt / DART_FRAME_PER >= DART_MOTION_NUM) return false;		// モーションが終わってたらfalse返す
			EffectSize = DART_EFFECT_SIZE;	//	エフェクトの大きさを設定
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[Cnt / DART_FRAME_PER], true);	//	斬撃描画
			break;
		case EFFECT_BOMB:	//	爆弾が当たった時
			if (Cnt / BOMB_FRAME_PER >= BOMB_MOTION_NUM) return false;		// モーションが終わってたらfalse返す
			EffectSize = BOMB_EFFECT_SIZE;	//	エフェクトの大きさを設定
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[Cnt / BOMB_FRAME_PER], true);	//	爆弾描画
			break;
		case EFFECT_WALL:	//	壁とぶつかった時
			if (Cnt / WALL_FRAME_PER >= WALL_MOTION_NUM) return false;		// モーションが終わってたらfalse返す
			EffectSize = WALL_EFFECT_SIZE;	//	エフェクトの大きさを設定
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[Cnt / WALL_FRAME_PER], true);	//	衝撃描画
			break;
		case EFFECT_COLLISION://	敵とプレイヤーまたは分身が当たった時
			if (Cnt / COLLISION_FRAME_PER >= COLLISION_MOTION_NUM) return false;		// モーションが終わってたらfalse返す
			EffectSize = COLLISION_EFFECT_SIZE;	//	エフェクトの大きさを設定
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[COLLISION_MOTION_NUM - Cnt / COLLISION_FRAME_PER], true);	//	衝撃描画
			break;
		case EFFECT_ALERT:
			if (Cnt / ALERT_FRAME_PER >= ALERT_MOTION_NUM) return false;		// モーションが終わってたらfalse返す
			EffectSize = COLLISION_EFFECT_SIZE;	//	エフェクトの大きさを設定
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[0], true);	//	！描画
			break;
		default:
			break;
		}
		return true;	// まだモーションがあるならtrueを返す
	}
};

// エフェクト管理クラス
class Effect_Manager {

private:
	int Dart_Gr[DART_MOTION_NUM];	//	手裏剣のモーション分のグラフィック
	int Bomb_Gr[BOMB_MOTION_NUM];	//	爆弾のモーション分のグラフィック
	int Collision_Gr[COLLISION_MOTION_NUM];	//	敵と衝突時
	int Wall_Gr[WALL_MOTION_NUM];	//	壁衝突時
	int Alert_Gr[ALERT_MOTION_NUM];	//	プレイヤー発見時
	vector<Effect*>Effect_Tbl;

public:

	// Effect_Managerクラスのコンストラクタ
	Effect_Manager() {
		//	各画像を分割読み込み
		LoadDivGraph("image/Effect/DartEffect.png", DART_MOTION_NUM, DART_MOTION_NUM, 1, 120, 120, Dart_Gr);
		LoadDivGraph("image/Effect/BombEffect.png", BOMB_MOTION_NUM, BOMB_MOTION_NUM, 1, 120, 120, Bomb_Gr);
		LoadDivGraph("image/Effect/WallEffect.png", WALL_MOTION_NUM, WALL_MOTION_NUM, 1, 120, 120, Wall_Gr);
		LoadDivGraph("image/Effect/CollisionEffect.png", COLLISION_MOTION_NUM, COLLISION_MOTION_NUM / 2, 2, 192, 192, Collision_Gr);
		Alert_Gr[0] = LoadGraph("image/Effect/AlertEffect.png", true);
	}

	// テーブルの最後尾に登録
	void Register(float argx, float argy, int type) {
		switch (type)
		{
		case EFFECT_DART:	//	手裏剣の用のエフェクトを登録
			Effect_Tbl.push_back(new Effect(argx, argy, Dart_Gr, type));
			break;
		case EFFECT_BOMB:	//	爆弾のエフェクトを登録
			Effect_Tbl.push_back(new Effect(argx, argy, Bomb_Gr, type));
			break;
		case EFFECT_WALL:	//	壁衝突した時用のエフェクトを登録
			Effect_Tbl.push_back(new Effect(argx, argy, Wall_Gr, type));
			break;
		case EFFECT_COLLISION:	//	敵と衝突した時用のエフェクトを登録
			Effect_Tbl.push_back(new Effect(argx, argy, Collision_Gr, type));
			break;
		case EFFECT_ALERT:	//	敵と衝突した時用のエフェクトを登録
			Effect_Tbl.push_back(new Effect(argx, argy, Alert_Gr, type));
		default:	//	typeが不明なら登録しない
			break;
		}
	}

	// 描画
	void Draw(float camerax, float cameray) {
		// テーブルをすべて参照して
		for (int i = 0; i < (int)Effect_Tbl.size(); i++) {
			// 描画して
			if (Effect_Tbl[i]->Draw(camerax, cameray));
			else {
				// 爆発モーションが終わってたらあける
				Effect_Tbl.erase(Effect_Tbl.begin() + i);
				//	コンテナのendが返ってきたら終了
				if (Effect_Tbl.begin() + i == Effect_Tbl.end())break;
			}
		}
		//	描画したらテーブル内の長さを正す
		Effect_Tbl.shrink_to_fit();
	}
};