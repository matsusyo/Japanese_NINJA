
#include"Gauge.h"
#include<algorithm>	//	min,maxを使う

using namespace std;

//	コンストラクタ
Gauge::Gauge(float value1,float value2)
{
	Max_Value = value1;
	Now_Value = value1;
	Min_Value = value2;
}

float Gauge::Get_Max() { return Max_Value; }	//	最大値を取得
float Gauge::Get_Min() { return Min_Value; }	//	最小値を取得
float Gauge::Get_Now() { return Now_Value; }	//	現在値を取得

//	最少、最大、現在値をセット
void Gauge::Set_All(float value1, float value2, float value3)
{
	Set_Max(value1); 
	Set_Min(value2); 
	Set_Now(value3);
}

//	最大値をセット	
void Gauge::Set_Max(float value)
{
	Max_Value = max(Max_Value, value);	//	最大値＜最小値にならないようにする
	if (Get_Now() > Max_Value)	//	現在値が最大値を超えてるなら、最大値を更新する
		Set_Now(Max_Value);
}

//	最小値をセット
void Gauge::Set_Min(float value)
{
	Min_Value = min(value, Max_Value);	//	最小値＞最大値にならないようにする
	if (Get_Now() < Min_Value)	//	現在値が最小値未満になら、最小値を更新する
		Set_Now(Min_Value);
}

//	現在値をセット
void Gauge::Set_Now(float value)
{
	//	現在値を最小値以上かつ最大値以下にする
	value = min(value, Max_Value);	
	value = max(value, Min_Value);
	Now_Value = value;
}

//	現在値を追加
float Gauge::Add_Now(float value)
{
	Set_Now(Get_Now() + value);
	return Get_Now();
}

//	最大値を追加
float Gauge::Add_Max(float value)
{
	Set_Max(Get_Max() + value);
	return Get_Max();
}

//	最小値を追加
float Gauge::Add_Min(float value)
{
	Set_Min(Get_Min() + value);
	return Get_Min();
}

//	HPが空か判定
bool Gauge::Is_Empty()
{
	return(Get_Now() <= Get_Min());	//	最小値以下ならtrue
}

//	HPが満タンか判定
bool Gauge::Is_Full()
{
	return (Get_Now() >= Get_Max());	//	最大値以上ならtrue
}

//	現在値と最大値の割合を算出
float Gauge::Rate()
{
	return(Get_Now() / Get_Max());	//	現在値÷最大値
}

//	ゲージを空っぽにする
void Gauge::Empty()
{
	Now_Value = Get_Min();	//	最小値を現在値に代入
}

//	ゲージを満タンにする
void Gauge::Full()
{
	Now_Value = Get_Max();	//	最大値を現在値に代入
}