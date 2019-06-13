//	�藠���N���X�̐錾

#pragma once
#include<math.h>	//	�O�����̒藝�Ŏg�p
#include"DxLib.h"	//	DX���C�u�����{��
#include"Literal.h"	//	�萔
#include"Player.h"	//	�v���C���[
#include"EnemyManager.h"	//	�G
#include"Map.h"				//	�}�b�v
#include"QtrCalc.h"			//	���W�ϊ�
#include"QtrDraw.h"			//	�`��e�[�u��
#include"SoundManager.h"	//	��

//	�萔
const int DART_NUM = 10;	//	���݂ł���藠����
const int DART_SIZE_X = 24;	//	�T�C�Y
const int DART_SIZE_Y = 24;
const int DART_H_SIZE_X = DART_SIZE_X / 2;	//	�����̃T�C�Y
const int DART_H_SIZE_Y = DART_SIZE_Y / 2;
const int DART_COUNT = 2 * 60;	//	�藠�������݂ł��鎞��

const float DART_MP = -7.5f;		//	�藠����MP�����
const float DART_SPEED = 8.0f;		//	�藠���̃X�s�[�h
const float DART_RADIUS = 0.175f;	//	�藠���̐���

class Dart
{
private:
	
	float Top_X, Top_Y;	//	�g�b�v���W
	float Qtr_X, Qtr_Y;	//	�N�H�[�^�[���W
	float Size_X, Size_Y;	//	�藠���̃T�C�Y
	float H_Size_X, H_Size_Y;
	float Speed;		//	�ړ��X�s�[�h
	float Top_Angle;	//	�i�s�p�x
	float Qtr_Angle;	//	�`�悷��e�̌���
	
	int Type;		//	�藠���̎�ށ@����͂P��ނ̗\�� 
	int	Graph;		//	�O���t�B�b�N
	int	Count;		//	�J�E���g�@�e�̐�������
	int	Target_Num;	//	�^�[�Q�b�g�̔ԍ�

	bool Flg;	//	�藠�����o�Ă邩�o�ĂȂ����̃t���O

	Enemy *Target;	//	�K���ȓG���^�[�Q�b�g�ɂ���

public:

	// �R���X�g���N�^
	Dart();
	//	�f�X�g���N�^
	~Dart();

	//	�A�N�Z�X���\�b�h
	float Get_Top_X() { return Top_X; }	//	�g�b�v���W�Z�b�g
	float Get_Top_Y() { return Top_Y; }
	void Set_Top_XY(float argx, float argy) { Top_X = argx; Top_Y = argy; }	//	X,Y�����ɃZ�b�g

	float Get_Qtr_X() { return Qtr_X; }	//	�N�H�[�^�[���W�Z�b�g
	float Get_Qtr_Y() { return Qtr_Y; }
	void Set_Qtr_XY(float argx, float argy) { Qtr_X = argx; Qtr_Y = argy; }	//	X,Y�����ɃZ�b�g

	void Set_Speed(float arg) { Speed = arg; }	//	�X�s�[�h���Z�b�g
	void Set_Top_Angle(float arg) { Top_Angle = arg; }	//	�g�b�v�p�x���Z�b�g
	void Set_Qtr_Angle(float arg) { Qtr_Angle = arg; }	//	�N�H�[�^�[�p�x���Z�b�g

	int Get_Type() { return Type; }	//	�藠���̃^�C�v���Q�b�g	
	void Set_Type(int arg) { Type = arg; }	//	�藠���̃^�C�v���Z�b�g

	void Set_Graph_Handle(int Arg) { Graph = Arg; }	//	�O���t�B�b�N�n���h�����Z�b�g

	//	�T�C�Y,�n�[�t�T�C�Y��X,Y�܂Ƃ߂ăZ�b�g
	void Set_Size(float x, float y) { Size_X = x; Size_Y = y; H_Size_X = x / 2; H_Size_Y = y / 2; }
	
	//	RECT�̃Q�b�g
	RECT Get_Rect() {
		RECT MyRect = {
			//	RECT�͏����߂ɂƂ�
			(int)Top_X - DART_H_SIZE_X + DART_SIZE_X / 4,	//	Left
			(int)Top_Y - DART_H_SIZE_Y + DART_SIZE_Y / 4,	//	Top
			(int)Top_X + DART_H_SIZE_X - DART_SIZE_X / 4,	//	Right
			(int)Top_Y + DART_H_SIZE_Y - DART_SIZE_Y / 4	//	Bottom
		};
		return MyRect;
	}
	
	void Set_Count(int arg) { Count = arg; }	//	�藠���̑��ݎ��Ԃ��Z�b�g
	void Set_Flg(bool arg) { Flg = arg; }		//	�藠�����݃t���O���Z�b�g
	bool Is_Flg() { return Flg; }				//	�藠�������݂��Ă��邩�Ԃ�

	void SetTarget(Enemy* arg) { Target = arg; }	//	�^�[�Q�b�g���Z�b�g����
	Enemy* Get_Target() { return Target; }			//	�^�[�Q�b�g��Ԃ�
	void SetTargetNum(int arg) { Target_Num = arg; }//	�^�[�Q�b�g�̓Y������Ԃ�
	
	void Move();	//	����
	void Judge_Enemy();	//	�G�Ƃ̓����蔻��

	//	�󂯎����RECT�ƐڐG���Ă邩�Ԃ�
	bool Collision(RECT rc) {
		RECT EnyRect = Get_Rect();	//	�G��RECT
		RECT Dst;	//	����

		//	�������Ă��� true
		if (IntersectRect(&Dst, &rc, &EnyRect))
			return  true;

		else return false;
	}
};

