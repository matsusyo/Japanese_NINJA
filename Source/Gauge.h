
//	HP,MP�Ȃǂ̃Q�[�W���Ǘ�

#pragma once

class Gauge {
private:
	float Max_Value;	//	�ő�l
	float Min_Value;	//	�ŏ��l
	float Now_Value;	//	���ݒl

public:
	////////////	�֐�

	//	�R���X�g���N�^ 
	Gauge(float, float);	//	�ő�l�ƍŏ��l��ݒ�
	
	//	�l���擾
	float Get_Max();
	float Get_Min();
	float Get_Now();

	//	�l���Z�b�g
	void Set_All(float value1, float value2, float value3);
	void Set_Max(float value);	
	void Set_Min(float value);	
	void Set_Now(float value);		

	//	�l�ɒǉ�
	float Add_Now(float value);		
	float Add_Max(float value);
	float Add_Min(float value);

	bool Is_Empty();	//	�Q�[�W������ۂ�����
	bool Is_Full();	//	�Q�[�W�����^��������

	float Rate();	//	���ݒl�ƍő�l�̊������Z�o

	void Empty();	//	�Q�[�W�𖞃^���ɂ���
	void Full();	//	�Q�[�W������ۂɂ���

};
