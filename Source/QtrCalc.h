//	���W�ϊ��N���X							
//	�N�H�[�^�[�A�g�b�v�r���[���W�A�`�b�v�ɑ��ݕϊ�			
//	�ϊ����������W�Q�ƕϊ��������W�������ϐ���n��

#pragma once
#include"Literal.h"
#include"Map.h"

class Quarter_Calculation {

private:

public:

	Quarter_Calculation();
	~Quarter_Calculation();

	//	�g�b�v �� �g�b�v�`�b�v
	void Top_TopChip(const float &topx, const  float &topy, int *topchipx, int *topchipy);

	//	�g�b�v �� �N�H�[�^���W
	void Top_Qtr(const float &topx, const float &topy, float *qtrx, float *qtry);
		
	//	�N�H�[�^ �� �g�b�v�`�b�v
	void Qtr_TopChip(const float &qtrx, const float &qtry, int *topchipx, int *topchipy);
	
	//	�N�H�[�^ ��	�g�b�v���W
	void Qtr_Top(const float &qtrx, const float &qtry, float *topx, float *topy);
};
