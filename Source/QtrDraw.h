#pragma once
#include "DxLib.h"
#include<vector>
#include<iostream>
#include<math.h>
#include"Literal.h"
#include"QtrCalc.h"

using namespace std;

const enum OBJECT_NAME {
	PLAYER, ENEMY, MAP, DART, BOMB, CLONE
};

class Qtr_Draw {
private:
	float Qtr_X, Qtr_Y;
	float H_Size_X, H_Size_Y;
	int Type, Graph;

public:
	Qtr_Draw() {}
	//	クォータ画面上での座標とオブジェクトの種類とグラフィックを受け取る
	Qtr_Draw(float qtrx, float qtry,float hsizex,float hsizey, int type, int graph) {
		Qtr_X = qtrx;
		Qtr_Y = qtry;
		H_Size_X = hsizex; 
		H_Size_Y = hsizey;
		Type = type;
		Graph = graph;
	}

	//	クォータ画面上での座標とオブジェクトの種類とグラフィックを受け取る
	Qtr_Draw(float qtrx, float qtry, int type, int graph) {
		Qtr_X = qtrx; 
		Qtr_Y = qtry;
		Type = type;
		Graph = graph;
	}

	//	オブジェクトを描画
	void Draw() {
		int left, top, right, bottom;
		//	特殊な描画があればcaseに分けて記述
		switch (Type) {
		case PLAYER:
		case ENEMY:
		case MAP:
		case BOMB:
		default:
			left = (int)(Qtr_X - H_Size_X);
			top = (int)(Qtr_Y - H_Size_Y);
			right = (int)(Qtr_X + H_Size_X);
			bottom = (int)(Qtr_Y + H_Size_Y);
			DrawExtendGraph(left, top, right, bottom, Graph, TRUE);	//	描画
			break;
		case DART:
			//	手裏剣は回転しているようにみせる
			double angle = DX_PI / 180;
			angle *= rand() % 180;
			DrawRotaGraph2((int)Qtr_X, (int)Qtr_Y, 12, 12, 1.0f, angle, Graph, TRUE, TRUE);
			break;
		}
	}

	float Get_Qtr_X() { return Qtr_X; }
	float Get_Qtr_Y() { return Qtr_Y; }
	int Get_Type() { return Type; }
	int GetGraph() { return Graph; }
};

class Qtr_Draw_Manager {
private:
	//	描画テーブル毎フレーム登録される数が違うのでvectorで作る
	vector<Qtr_Draw*> Qtr_Draw_Tbl;
public:
	Qtr_Draw_Manager() {}

	//	描画テーブルに登録してある物をすべて描画
	void Draw() {
		for (int i = 0; i < (int)Qtr_Draw_Tbl.size() - 1; i++) {
			Qtr_Draw_Tbl[i]->Draw();
		}
	}

	//	テーブルに登録させる
	void Register(float qtrx, float qtry, float hsizex, float hsizey, int type, int gr) {
		//	テーブルの最後尾に登録
		Qtr_Draw_Tbl.push_back(new Qtr_Draw(qtrx, qtry, hsizex, hsizey, type, gr));
	}

		//	テーブルをすべて空にする
	void Clear_Table(){
		Qtr_Draw_Tbl.erase(Qtr_Draw_Tbl.begin(), Qtr_Draw_Tbl.end());
		Qtr_Draw_Tbl.shrink_to_fit();
	}

	// 基本挿入法によるソート
	void insertionSort() {

		Qtr_Draw *Tmp;
		for (int j = 0; j < (int)Qtr_Draw_Tbl.size() - 1; j++) {	//	外側ループ
			for (int i = j; i >= 0; i--) {	//	内側ループ
				// Y座標が高い順にソートする
				if (Qtr_Draw_Tbl[i]->Get_Qtr_Y() > Qtr_Draw_Tbl[i + 1]->Get_Qtr_Y()) {
					//	スワップ
					Tmp = Qtr_Draw_Tbl[i];
					Qtr_Draw_Tbl[i] = Qtr_Draw_Tbl[i + 1];
					Qtr_Draw_Tbl[i + 1] = Tmp;
				}
				else break;	// スワップの必要がなくなった時点で内側のループを抜ける
			}
		}
	}
};
