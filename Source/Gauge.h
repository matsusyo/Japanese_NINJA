
//	HP,MPなどのゲージを管理

#pragma once

class Gauge {
private:
	float Max_Value;	//	最大値
	float Min_Value;	//	最小値
	float Now_Value;	//	現在値

public:
	////////////	関数

	//	コンストラクタ 
	Gauge(float, float);	//	最大値と最小値を設定
	
	//	値を取得
	float Get_Max();
	float Get_Min();
	float Get_Now();

	//	値をセット
	void Set_All(float value1, float value2, float value3);
	void Set_Max(float value);	
	void Set_Min(float value);	
	void Set_Now(float value);		

	//	値に追加
	float Add_Now(float value);		
	float Add_Max(float value);
	float Add_Min(float value);

	bool Is_Empty();	//	ゲージが空っぽか判定
	bool Is_Full();	//	ゲージが満タンか判定

	float Rate();	//	現在値と最大値の割合を算出

	void Empty();	//	ゲージを満タンにする
	void Full();	//	ゲージを空っぽにする

};
