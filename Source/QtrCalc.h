//	座標変換クラス							
//	クォーター、トップビュー座標、チップに相互変換			
//	変換したい座標２つと変換した座標をいれる変数を渡す

#pragma once
#include"Literal.h"
#include"Map.h"

class Quarter_Calculation {

private:

public:

	Quarter_Calculation();
	~Quarter_Calculation();

	//	トップ → トップチップ
	void Top_TopChip(const float &topx, const  float &topy, int *topchipx, int *topchipy);

	//	トップ → クォータ座標
	void Top_Qtr(const float &topx, const float &topy, float *qtrx, float *qtry);
		
	//	クォータ → トップチップ
	void Qtr_TopChip(const float &qtrx, const float &qtry, int *topchipx, int *topchipy);
	
	//	クォータ →	トップ座標
	void Qtr_Top(const float &qtrx, const float &qtry, float *topx, float *topy);
};
