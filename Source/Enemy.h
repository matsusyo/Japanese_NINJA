//	�G�P��

#pragma once
#include<math.h>
#include"DxLib.h"
#include"Literal.h"
#include"Player.h"
#include"Map.h"
#include"QtrCalc.h"
#include"Clone.h"
#include"Gauge.h"
#include"State.h"
#include"QtrDraw.h"
#include"EffectManager.h"
#include"SoundManager.h"
#include"TimeManager.h"

//	�G�̎��
const enum ENEMY_TYPE {
	ENEMY_BOSS,
	ENEMY_BLUE,
};

const int ENY_SIZE_X = 32;				//	�G�̉摜�T�C�Y
const int ENY_SIZE_Y = 32;
const int ENY_BLUE_HP_MAX = 50;			//	�G���G�̍ő�Hp
const int ENY_BOSS_HP_MAX = 50;		//	�{�X�̍ő�Hp
const float ENEMY_SPEED = 5.5f;			//	�ړ����x
const int ENEMY_GR_NUM = 24;			//	�O���t�B�b�N��
const int CHANGE_ANGLE_TIMING = 4 * 60;	//	�G��������ς���^�C�~���O
const int MOVE_TIMING = 5 * 60;			//	�G���ړ�����^�C�~���O
const int MOVE_TIME = (int)(0.1 * 60);	//	�ړ�����^�C�~���O�ɂȂ�������������
const float ABS_ANGLE_BASE = 25;		//	���E�p�x�̐�Βl�@�G�̊p�x����+-���Ď��E�����
const int ABS_ANGLE_ADD = 10;			//	���E�p�x��G�ɂ���ĕς���
const float VISION_BASE = 260;			//	���F�ł��鋗���̊�l
const int VISION_ADD = 60;				//	��l�Ƀ����_����60�ȉ��̐��l��+-���ēG�ɂ���Ď��F������ς���


class Enemy
{
protected:

	float Top_X, Top_Y;			//	�g�b�v�r���[���W
	float Top_Angle;			//	�g�b�v�r���[�p�x
	float Top_Radian;			//	�g�b�v�r���[�ł̃��W�A��
	float Qtr_X, Qtr_Y;			//	�N�H�[�^�[�r���[���W
	float Qtr_Angle;			//	�N�H�[�^�[�p�x
	float Camera_X, Camera_Y;	//	�J�������W                     
	float Speed;				//	�X�s�[�h
	float Size_X, Size_Y;		//	�L�����T�C�Y
	float H_Size_X, H_Size_Y;	//	�����̃T�C�Y
	
	int Eny_Gr[24];	//	�G�O���t�B�b�N24��
	int	Eny_Gr_Num;	//	�O���t�B�b�N�ԍ�
	int	Gauge_Gr[2];//	HP,�O�g
	int Damage_Cnt;	//	�_���[�W���󂯂����̖��G����
	int Dying_Cnt;	//	���ʂ܂ł̎���

	bool Hp_Flg;	//	HP�����邩�Ȃ���
	bool Find_Flg;	//	�v���C���[����������true

public:
	Gauge* Gauge_Bar[2];//	�Q�[�W�I�u�W�F�N�g
	State Status;		//	�X�e�[�^�X�I�u�W�F�N�g

	Enemy();
	~Enemy();
	
	//	�I�[�o�[���C�h
	virtual void Move();			//	�G�̋���
	virtual void Draw_Gauge();		//	HP��`��
	virtual void Draw_Ray(){}		//	�G�̎��E��`��
	virtual void Status_Update();	//	Status��ԊǗ�
	virtual bool Judge_Move(RECT, float*, float*, float, float);	//	�ړ��ł��邩���肷��@�ړ������� true ��Ԃ�
	
	//	�N�H�[�^�[�r����ł̊p�x�ɂ���ēG�摜��ς���
	virtual int Change_Gr(float qtrangle) {	
		if (qtrangle < 175 && qtrangle>95)return 21;			//	���΂߉�
		else if (qtrangle <= 95 && qtrangle >= 85)return 18;	//	����
		else if (qtrangle < 85 && qtrangle>5)return 15;			//	�E�΂߉�
		else if (qtrangle >= 175 || qtrangle <= -175)return 12;	//	����
		else if (qtrangle <= 5 && qtrangle >= -5)return 6;		//	�E��
		else if (qtrangle > -175 && qtrangle < -95)return 9;	//	���΂ߏ�
		else if (qtrangle >= -95 && qtrangle <= -85)return 0;	//	������
		else if (qtrangle > -85 && qtrangle < -5)return 3;		//	�E�΂ߏ�
		return 0;
	}

	//	�A�N�Z�X���\�b�h
	float Get_Top_X() { return Top_X; }
	float Get_Top_Y() { return Top_Y; }
	float Get_Qtr_X() { return Qtr_X; }
	float Get_Qtr_Y() { return Qtr_Y; }
	float Get_Camera_X() { return Camera_X; }
	float Get_Camera_Y() { return Camera_Y; }
	float Get_Top_Angle() { return Top_Angle; }
	float Get_Top_Radian() { return Top_Radian; }
	float Get_Qtr_Angle() { return Qtr_Angle; }
	float Get_Speed() { return Speed; }

	virtual int Get_Type() { return NULL; }	//	�G�̃^�C�v��Ԃ�
	bool Is_Hp_Flg() { return Hp_Flg; }	//	Hp���Ȃ���� true

	RECT Get_Rect() {	//	RECT��Ԃ�
		RECT MyRect = {
			(int)(Top_X - H_Size_X + 6),//	Left
			(int)(Top_Y - H_Size_Y + 6),//	Top
			(int)(Top_X + H_Size_X - 6),//	Right
			(int)(Top_Y + H_Size_Y - 6)	//	Bottom
		};
		return MyRect;
	}

	bool Collision(RECT rc) {	//	�󂯎����RECT�ƐڐG���Ă邩�Ԃ�
		RECT EnyRect = Get_Rect();	//	�G��RECT
		RECT Dst;	//	����

		//	�������Ă��� true
		if (IntersectRect(&Dst, &rc, &EnyRect))return  true;
		else return false;
	}

	void Set_Top_X(float arg) { Top_X = arg; }
	void Set_Top_Y(float arg) { Top_Y = arg; }
	void Set_Top_Angle(float arg) { Top_Angle = arg; }
	void Set_Top_Radian(float arg) { Top_Radian = arg; }
	void Set_Speed(float arg) { Speed = arg; }
	void Set_Damage_Count(int arg) { Damage_Cnt = arg; }
	void Set_Dying_Count(int arg) { Dying_Cnt = arg; }
	void Set_Hp_Flg(bool arg) { Hp_Flg = arg; }
};

//	�G���m�i���j
class Enemy_Blue :public Enemy {
private:
	int Change_Angle_Cnt;		//	������ς���^�C�~���O���v��
	int Walk_Cnt;				//	�ړ�����^�C�~���O���͂���
	int Target_Name;			//	�^�[�Q�b�g�̖��O���L�^
	float Top_End_X, Top_End_Y;	//	�g�b�v�r���[�ł̎��E�̏I�_
	float Qtr_End_X, Qtr_End_Y;	//	�N�H�[�^�[�r���[�ł̎��E�̏I�_
	float Target_Angle;			//	�^�[�Q�b�g�ւ̊p�x
	float Target_Radian;		//	�^�[�Q�b�g�ւ̃��W�A��
	float Target_Abs_Angle;		//	���F�����^�[�Q�b�g�p�x�̐�Βl
	float Vision_Distance;		//	���F�ł��鋗��
	float Abs_Angle;			//	���E�̐�Βl�@Top_Angle����+-���Ď��E�͈̔͂ɂ���

public:
	Enemy_Blue() {};	//	�R���X�g���N�^�@�������͎g��Ȃ�
	Enemy_Blue(float argx, float argy,int graph[],int gaugegr[]);	//	�R���X�g���N�^
	~Enemy_Blue();	//	�f�X�g���N�^
	void Move();	//	�p�������ꍇ�͌ŗL�̓���������
	bool Search_Player();	//	�v���C���[��T��
	float Calc_Distance(RECT);	//	�v���C���[�܂��͕��g����G�ւ̋��������߂�
	float Calc_Abs_Angle(float,float);	//	��p�x����v���C���[�p�x�ւ̐�Βl�����߂�
	bool Judge_Wall(float, float, float, bool*);	//	�v���C���[���ǉz���ɂ��邩�ǂ���
	void Draw_Ray();	//	���E��`��
	int Get_Type() { return ENEMY_BLUE; }	//	�G�̎�ނ�Ԃ�
	int Get_Target_Name() { return Target_Name; }
	void Set_Target_Name(int arg) { Target_Name = arg; }
};

//	�G
class Enemy_Boss :public Enemy {
private:
	int Cnt_Angle;	//	�p�x��ς���^�C�~���O
	float Vision_X, Vision_Y;		//	�v���C���[�����F���Ă邩
	bool Move_Flg_X, Move_Flg_Y;
	float Qtr_Vision_X, Qtr_Vision_Y;
public:
	Enemy_Boss() {};
	Enemy_Boss(float argx, float argy,int graph[],int gaugegr[]);
	~Enemy_Boss();

	void Move();
	void Status_Update();
	int Get_Type() { return ENEMY_BOSS; }
};