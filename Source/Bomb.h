#pragma once
#include"DxLib.h"		//	DX���C�u�����{��
#include"Literal.h"		//	�萔
#include"Player.h"		//	�v���C���[
#include"EnemyManager.h"//	�G
#include"Map.h"			//	�}�b�v
#include"QtrCalc.h"		//	�g�b�v,�N�H�[�^�r���[���W���ݕϊ�
#include"QtrDraw.h"		//	�`��e�[�u��

//		�萔
const int BOMB_NUM = 10;	//	���݂ł��锚�e�� MPMAX����o�����
const int BOMB_SIZE_X = 32;	//	�T�C�Y
const int BOMB_SIZE_Y = 32;
const int BOMB_EXPLOSION_TIME = 5 * 60;	//	5�t���[���Ŕ���
const int BOMB_GRAPH = 11;				//	11�̉摜���g��

const float BOMB_MP = -12.0f;	//	���e��MP�����
const float BOMB_SPEED = 8.0f;	//	�X�s�[�h
const float EXPLOSION_SPREAD_SIZE_X = 0.15f;	//	���e�����������傫�����邽�߂�Size�ɒǉ����� X
const float EXPLOSION_SPREAD_SIZE_Y = 0.15f;	//	Y�̊g��l
const float BOMB_DAMAGE = -5.0f;	//	���e�̃_���[�W

class Bomb
{
private:
	
	float Top_X, Top_Y;			//	�g�b�v�r���[���W
	float Qtr_X, Qtr_Y;			//	�N�H�[�^�[���W
	float Size_X, Size_Y;		//	�T�C�Y
	float H_Size_X, H_Size_Y;	//	�����̃T�C�Y
	float Top_Angle;			//	�i�s�p�x
	float Qtr_Angle;			//	�`�悷��e�̌���

	int Type;				//	�^�C�v�@
	int	Count;				//�@��������
	int	Graph[BOMB_GRAPH];	//	�O���t�B�b�N

	bool Flg;			//	���e���o�Ă邩�o�ĂȂ����̃t���O
	bool Explosion_Flg;	//	���������ǂ���

public:

	// �R���X�g���N�^
	Bomb();
	//	�f�X�g���N�^
	~Bomb();

	//	�A�N�Z�X���\�b�h
	void Set_Top_XY(float argx, float argy) { Top_X = argx; Top_Y = argy; }	//	�g�b�v�r���[�̍��W���Z�b�g
	void Set_Qtr_XY(float argx, float argy) { Qtr_X = argx; Qtr_Y = argy; }	//	�N�H�[�^�[�r���[�̍��W���Z�b�g
	float Get_Top_X() { return Top_X; }	//	�g�b�v�r���[��ł̍��W�����炤
	float Get_Top_Y() { return Top_Y; }	
	float Get_Qtr_X() { return Qtr_X; }	//	�N�H�[�^�[�r���[��ł̍��W�����炤
	float Get_Qtr_Y() { return Qtr_Y; }	

	void Set_Top_Angle(float arg) { Top_Angle = arg; }	//	�g�b�v�r���[�ł̊p�x���Z�b�g
	void Set_Qtr_Angle(float arg) { Qtr_Angle = arg; }	//	�N�H�[�^�[�r���[�ł̊p�x���Z�b�g

	int Get_Type() { return Type; }			//	�{���̃^�C�v�����炤
	void Set_Type(int arg) { Type = arg; }	//	�{���̃^�C�v���Z�b�g����

	void Set_Graph_Handle(int number,int arggr) { Graph[number] = arggr; }	//	�Y�����ԍ���n���ăO���t�B�b�N���Z�b�g����

	//	�T�C�Y���Z�b�g�@�Z�b�g�����H_Size���Z�b�g�����
	void Set_Size(float x, float y) { Size_X =x; Size_Y = y; H_Size_X = x / 2; H_Size_Y = y / 2; }	//	XY����C��
	void Set_Size_X(float arg) { Size_X = arg; H_Size_X = Size_X / 2;};	//	X�̃T�C�Y���Z�b�g
	void Set_Size_Y(float arg) { Size_Y = arg; H_Size_Y = Size_Y / 2; };//	Y�̃T�C�Y���Z�b�g

	//	�T�C�Y�𑝉��@���e��傫�����Ă���
	void Spread_Size_X(float arg) { Size_X += arg; H_Size_X = Size_X / 2; };	//	X�̃T�C�Y��傫������
	void Spread_Size_Y(float arg) { Size_Y += arg; H_Size_Y = Size_Y / 2; };	//	Y�̃T�C�Y��傫������

	//	RECT�����炤 RECT�͏����傫�߂ɂƂ�
	RECT Get_Rect() {
		RECT MyRect = {
			(int)(Top_X - H_Size_X-20),	//	Left
			(int)(Top_Y - H_Size_Y-20),	//	Top
			(int)(Top_X + H_Size_X+20),	//	Right
			(int)(Top_Y + H_Size_Y+20)	//	Bottom
		};
		return MyRect;
	}
	
	void Set_Count(int arg) { Count = arg; }	//	�J�E���g���Z�b�g����
	void Set_Flg(bool arg) { Flg = arg; }		//	���e�����݂��邩�Z�b�g true�ő��݂���
	bool Is_Flg() { return Flg; }				//	���e�����݂��Ă邩�ǂ���

	void Set_Explosion_Flg(bool arg) { Explosion_Flg = arg; }	//	���������ǂ����Z�b�g���� true�Ŕ���
	bool Is_Explosion_Flg() { return Explosion_Flg; }			//	���������ǂ���

	void Move();		//	����
	void Judge_Enemy();	//	�G�Ƃ̓����蔻��
	
	//	������RECT�Ɠ����������ǂ��������Ԃ�
	bool Collision(RECT rc) {
		RECT BombRect = Get_Rect();	//	���e��RECT
		RECT Dst;	//	����

		//	�������Ă��� true
		if (IntersectRect(&Dst, &rc, &BombRect))
			return  true;

		else return false;
	}
};