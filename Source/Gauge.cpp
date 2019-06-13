
#include"Gauge.h"
#include<algorithm>	//	min,max���g��

using namespace std;

//	�R���X�g���N�^
Gauge::Gauge(float value1,float value2)
{
	Max_Value = value1;
	Now_Value = value1;
	Min_Value = value2;
}

float Gauge::Get_Max() { return Max_Value; }	//	�ő�l���擾
float Gauge::Get_Min() { return Min_Value; }	//	�ŏ��l���擾
float Gauge::Get_Now() { return Now_Value; }	//	���ݒl���擾

//	�ŏ��A�ő�A���ݒl���Z�b�g
void Gauge::Set_All(float value1, float value2, float value3)
{
	Set_Max(value1); 
	Set_Min(value2); 
	Set_Now(value3);
}

//	�ő�l���Z�b�g	
void Gauge::Set_Max(float value)
{
	Max_Value = max(Max_Value, value);	//	�ő�l���ŏ��l�ɂȂ�Ȃ��悤�ɂ���
	if (Get_Now() > Max_Value)	//	���ݒl���ő�l�𒴂��Ă�Ȃ�A�ő�l���X�V����
		Set_Now(Max_Value);
}

//	�ŏ��l���Z�b�g
void Gauge::Set_Min(float value)
{
	Min_Value = min(value, Max_Value);	//	�ŏ��l���ő�l�ɂȂ�Ȃ��悤�ɂ���
	if (Get_Now() < Min_Value)	//	���ݒl���ŏ��l�����ɂȂ�A�ŏ��l���X�V����
		Set_Now(Min_Value);
}

//	���ݒl���Z�b�g
void Gauge::Set_Now(float value)
{
	//	���ݒl���ŏ��l�ȏォ�ő�l�ȉ��ɂ���
	value = min(value, Max_Value);	
	value = max(value, Min_Value);
	Now_Value = value;
}

//	���ݒl��ǉ�
float Gauge::Add_Now(float value)
{
	Set_Now(Get_Now() + value);
	return Get_Now();
}

//	�ő�l��ǉ�
float Gauge::Add_Max(float value)
{
	Set_Max(Get_Max() + value);
	return Get_Max();
}

//	�ŏ��l��ǉ�
float Gauge::Add_Min(float value)
{
	Set_Min(Get_Min() + value);
	return Get_Min();
}

//	HP���󂩔���
bool Gauge::Is_Empty()
{
	return(Get_Now() <= Get_Min());	//	�ŏ��l�ȉ��Ȃ�true
}

//	HP�����^��������
bool Gauge::Is_Full()
{
	return (Get_Now() >= Get_Max());	//	�ő�l�ȏ�Ȃ�true
}

//	���ݒl�ƍő�l�̊������Z�o
float Gauge::Rate()
{
	return(Get_Now() / Get_Max());	//	���ݒl���ő�l
}

//	�Q�[�W������ۂɂ���
void Gauge::Empty()
{
	Now_Value = Get_Min();	//	�ŏ��l�����ݒl�ɑ��
}

//	�Q�[�W�𖞃^���ɂ���
void Gauge::Full()
{
	Now_Value = Get_Max();	//	�ő�l�����ݒl�ɑ��
}