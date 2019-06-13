//	���g�N���X�̐錾

#pragma once
#include<math.h>	//	atan2f���g�p����
#include"DxLib.h"	//	DX���C�u�����{��
#include"Literal.h"	//	�萔
#include"Player.h"	//	�v���C���[
#include"Map.h"		//	�}�b�v
#include"QtrCalc.h"	//	���W�ϊ�
#include"QtrDraw.h"	//	�`��e�[�u��

const int CLONE_GR_NUM = 24;
const float CLONE_MP = -20.0f;	//	���g��MP�����
const float CLONE_SPEED = 5.7f;	//	���g�̃X�s�[�h

class Clone
{
private:

	float Top_X, Top_Y;	//	�g�b�v���W
	float Qtr_X, Qtr_Y;	//	�N�H�[�^�[���W
	float Camera_X, Camera_Y;	//	�J�������W
	float Top_Angle, Qtr_Angle;	//	�g�b�v,�N�H�[�^�[�p�x
	float Speed;				//	�X�s�[�h
	float Size_X, Size_Y;		//	�摜�T�C�Y
	float H_Size_X, H_Size_Y;	//	�摜�T�C�Y�̔����̃T�C�Y

	int	Clone_Gr[CLONE_GR_NUM];	//	���g�̃O���t�B�b�N�@24��
	int	Clone_Gr_Num;			//	�`�悷��O���t�B�b�N�i���o�[

	bool Clone_Flg;		//	���g�̏p���������Ȃ�true

public:

	//	�R���X�g���N�^
	Clone();	//	�������͎g��Ȃ�
	Clone(float, float, float, float);	//	�v���C���̍��W�Ɗp�x(Top��Qtr)��n��
	
	//	�f�X�g���N�^
	~Clone();

	//	���g�̏p�𔭓����Ă���Ԃ͌Ă�
	void Move();

	//	�A�N�Z�X���\�b�h
	float Get_Top_X() { return Top_X; }	//	�g�b�v���W�����炤
	float Get_Top_Y() { return Top_Y; }
	float Get_Qtr_X() { return Qtr_X; }	//	�N�H�[�^�[���W�����炤
	float Get_Qtr_Y() { return Qtr_Y; }
	float Get_Camera_X() { return Camera_X; }	//	�J���������炤
	float Get_Camera_Y() { return Camera_Y; }
	float Get_Top_Angle() { return Top_Angle; }	//	�g�b�v�r���[�ł̊p�x�����炤
	float Get_Qtr_Angle() { return Qtr_Angle; }	//	�N�H�[�^�[�r���[�ł̊p�x�����炤
	float Get_Speed() { return Speed; }	//	�X�s�[�h�����炤
	float Get_H_Size_X() { return H_Size_X; }	//	�摜�T�C�Y�̔����̃T�C�Y�����炤
	float Get_H_Size_Y() { return H_Size_Y; }
	
	bool Is_Clone_Flg() { return Clone_Flg; }	//	���g�����݂��Ă��邩�ǂ���

	//	RECT�͏������Ƃ� �����蔻�����萳�m�ɂ��邽��
	RECT Get_Rect() {
		RECT MyRect = {
			(int)(Top_X - H_Size_X + 6),//	Left
			(int)(Top_Y - H_Size_Y + 6),//	Top
			(int)(Top_X + H_Size_X - 6),//	Right
			(int)(Top_Y + H_Size_Y - 6)	//	Bottom
		};
		return MyRect;
	}

	//	�����蔻��@��Q����RECT�������ɓn��
	bool Collision(RECT rc) {
		RECT MyRect = Get_Rect();
		RECT Dst;	//	���ʂ������Ƃ�

		// �������Ă����true��Ԃ�
		if (IntersectRect(&Dst, &MyRect, &rc))
			return true;
		else
			return false;
	}

	void Set_Top_X(float arg) { Top_X = arg; }	//	�g�b�v���W���Z�b�g
	void Set_Top_Y(float arg) { Top_Y = arg; }
	void Set_Top_Angle(float arg) { Top_Angle = arg; }	//	�g�b�v�p�x���Z�b�g
	void Set_Speed(float arg) { Speed = arg; }			//	�X�s�[�h���Z�b�g
	void Set_Clone_Flg(bool arg) { Clone_Flg = arg; }	//	���g�����݂��Ă��邩���߂�t���O���Z�b�g
};