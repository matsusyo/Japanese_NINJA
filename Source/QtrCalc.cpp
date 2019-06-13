//	座標変換クラス	クォーター、トップビュー座標、チップに相互変換	

#include"QtrCalc.h"

//	コンストラクタ
Quarter_Calculation::Quarter_Calculation() {};
//	デストラクタ
Quarter_Calculation::~Quarter_Calculation() {};

//	トップ → トップチップ
void Quarter_Calculation::Top_TopChip(const float &topx, const  float &topy, int *topchipx, int *topchipy)
{
	*topchipx = (int)(topx / CHIP_SIZE_X);

	*topchipy = (int)(topy / CHIP_SIZE_Y);
}

//	トップ → クォータ座標
void Quarter_Calculation::Top_Qtr(const float &topx, const float &topy, float *qtrx, float *qtry)
{
	*qtrx = (((topx / CHIP_SIZE_X) + (topy / CHIP_SIZE_Y))*(CHIP_SIZE_X / 2));

	*qtry = (((-topx / CHIP_SIZE_X) + (topy / CHIP_SIZE_Y))*(CHIP_SIZE_Y / 4));
}

//	クォータ → トップチップ
void Quarter_Calculation::Qtr_TopChip(const float &qtrx, const float &qtry, int *topchipx, int *topchipy)
{
	float X = ((qtrx)-(qtry * 2));
	*topchipx = (int)(X / CHIP_SIZE_X);

	float Y = ((qtrx)+(qtry * 2));
	*topchipy = (int)(Y / CHIP_SIZE_X);
}

//	クォータ →	トップ座標
void Quarter_Calculation::Qtr_Top(const float &qtrx, const float &qtry, float *topx, float *topy)
{
	*topx = ((qtrx)-(qtry * 2));
	*topx /= CHIP_SIZE_X;
	*topx *= CHIP_SIZE_X;

	*topy = ((qtrx)+(qtry * 2));
	*topy /= CHIP_SIZE_X;
	*topy *= CHIP_SIZE_Y;
}