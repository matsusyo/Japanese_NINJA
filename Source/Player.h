//	�v���C���ɕK�v�ȕ���錾

#pragma once
#include<math.h>
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"QtrCalc.h"
#include"Map.h"
#include"Gauge.h"
#include"State.h"
#include"QtrDraw.h"
#include"SoundManager.h"
#include"EffectManager.h"

//	�L�����T�C�Y
const int PLY_SIZE_X = 32;
const int PLY_SIZE_Y = 32;

//	�ړ��X�s�[�h
const float PLY_DEF_SPEED = 5.4f;

class Player {
private:

	float Top_X, Top_Y;			//	�g�b�v�r���[���W
	float Qtr_X, Qtr_Y;			//	�N�H�[�^�[�r���[���W
	float Top_MX, Top_MY;		//	�g�b�v�}�E�X���W
	float Qtr_MX, Qtr_MY;		//	�N�H�[�^�[�}�E�X���W
	float Camera_X, Camera_Y;	//	�J�������W
	float Top_Angle, Qtr_Angle;	//	�g�b�v�A�N�H�[�^�̊p�x
	float Speed;				//	�ړ��X�s�[�h
	float Size_X, Size_Y;		//	�v���C���[�̉摜�T�C�Y
	float H_Size_X, H_Size_Y;	//	�v���C���[�摜�̔����̃T�C�Y

	int	Ply_Gr[24];		//	�v���C���[�̃O���t�B�b�N�@24��
	int	Ply_Gr_Num;		//	�`�悷��O���t�B�b�N�i���o�[
	int	Gauge_Gr[3];	//	HP �E�p�Q�[�W �O�g�̃O���t�B�b�N
	int	Damage_Cnt;		//	��e���̖��G����
	int	Dying_Cnt;		//	���ʂ܂ł̎���

	bool Mp_Flg;		//	Mp���O�ɂȂ�����true�A�Ă�Mp��Max�ɖ߂�����false
	bool Clear_Flg;		//	�X�e�[�W�N���A������true�ɂ���

public:
	Gauge* Gauge_Bar[3];//	HP��MP�Q�[�W�Q�[�W
	State Status;		//	��e�A�m���A���S�Ȃǃv���C���[�̏��

	Player();
	~Player();
	void Move();		//	�ړ�����
	void Draw_Gauge();	//	HP�AMP�`��

	//	Get�` �e�ϐ���Ԃ�
	float Get_Top_X() { return Top_X; }			//	�g�b�v���W
	float Get_Top_Y() { return Top_Y; }
	float Get_Qtr_X() { return Qtr_X; }			//	�N�H�[�^�[���W
	float Get_Qtr_Y() { return Qtr_Y; }
	float Get_Top_MX() { return Top_MX; }		//	�g�b�v�}�E�X���W
	float Get_Top_MY() { return Top_MY; }
	float Get_Qtr_MX() { return Qtr_MX; }		//	�N�H�[�^�[�}�E�X���W
	float Get_Qtr_MY() { return Qtr_MY; }
	float Get_Camera_X() { return Camera_X; }	//	�J�������W
	float Get_Camera_Y() { return Camera_Y; }
	float Get_Top_Angle() { return Top_Angle; }	//	�g�b�v�p�x
	float Get_Qtr_Angle() { return Qtr_Angle; }	//	�N�H�[�^�[�p�x
	float Get_Speed() { return Speed; }			//	�X�s�[�h
	float Get_H_Size_X() { return H_Size_X; }	//	�v���C���[�̔����̃T�C�Y
	float Get_H_Size_Y() { return H_Size_Y; }
	bool Is_Mp_Flg() { return Mp_Flg; }		//	Mp_Flg
	bool Is_Clear_Flg() { return Clear_Flg; }	//	Clear_Flg
	
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

	//	Set�` �e�ϐ��Ɉ�������
	void Set_Top_X(float arg) { Top_X = arg; }			//	�g�b�v���W
	void Set_Top_Y(float arg) { Top_Y = arg; }
	void Set_Top_Angle(float arg) { Top_Angle = arg; }	//	�g�b�v�p�x
	void Set_Qtr_Angle(float arg) { Qtr_Angle = arg; }	//	�N�H�[�^�[�p�x
	void Set_Speed(float arg) { Speed = arg; }			//	�X�s�[�h
	void Set_Damage_Cnt(int arg) { Damage_Cnt = arg; }	//	�_���[�W�����������̖��G����
	void Set_Dying_Cnt(int arg) { Dying_Cnt = arg; }	//	���ʂ܂łɂ����鎞��
	void Set_Mp_Flg(bool arg) { Mp_Flg = arg; }			//	Mp_Flg
	void Set_Clear_Flg(bool arg) { Clear_Flg = arg; }	//	Clear_Flg
	void Hp_Proc(float);	//	Hp�v�Z
	void Mp_Proc(float);	//	Mp�v�Z
	void Status_Update();	//	�X�e�[�^�X���X�V

};