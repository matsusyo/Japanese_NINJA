#pragma once

#include<vector>
#include "DxLib.h"		// DX���C�u�����̖{��
#include"Player.h"
#include "SoundManager.h"	// ��

extern Sound_Manager *My_Sound;		//	��

using namespace std;	//	vector���g�p�ɕK�v

const enum EFFECT {
	EFFECT_DART,
	EFFECT_BOMB,
	EFFECT_WALL,
	EFFECT_COLLISION,
	EFFECT_ALERT,
};

const int DART_FRAME_PER = 3;	//	�藠���̕`��t���[����
const int BOMB_FRAME_PER = 1;	//	���e�̕`��t���[����
const int WALL_FRAME_PER = 3;	//	�ǂƏՓˎ��̕`��t���[����
const int COLLISION_FRAME_PER = 1;	//	�G�ƏՓˎ��̕`��t���[����
const int ALERT_FRAME_PER = 9;	//	�v���C���[�������̕`��t���[����

const int DART_MOTION_NUM = 5;	//	�藠���Փˎ��̃��[�V������
const int BOMB_MOTION_NUM = 7;	//	���e�̃��[�V������
const int WALL_MOTION_NUM = 8;	//	�ǂƏՓˎ��̃��[�V������
const int COLLISION_MOTION_NUM = 10;	//	�G�ƏՓˎ��̃��[�V������
const int ALERT_MOTION_NUM = 1;	//	�v���C���[�������̃��[�V������

const float DART_EFFECT_SIZE = 0.6f;		//	�藠���̃G�t�F�N�g�̑傫��
const float BOMB_EFFECT_SIZE = 0.5f;		//	���e�̃G�t�F�N�g�̑傫��
const float WALL_EFFECT_SIZE = 0.2f;		//	�ǏՓˎ��̃G�t�F�N�g�̑傫��
const float COLLISION_EFFECT_SIZE = 0.2f;	//	�G�ƏՓˎ��̃G�t�F�N�g�̑傫��
const float ALERT_EFFECT_SIZE = 1.0f;	//	�v���C���[�������̃G�t�F�N�g�̑傫��

// �G�t�F�N�g�ЂƂ��̃N���X
class Effect {
private:
	float Pos_X;	//	���W
	float Pos_Y;
	int Cnt;		//	���[�V�����p�̃J�E���^
	int *Grpt;		//	�Q�Ƃ���O���t�B�b�N�n���h���ւ̎Q�ƃ|�C���^
	int Type;		//	�G�t�F�N�g�̃^�C�v
	double Angle;	//	�`��̊p�x�Ɏg��

public:
	// Effect�̃R���X�g���N�^
	Effect() {}	// �g��Ȃ����ǕK�v �G���[�΍�
	Effect(float argx, float argy, int *arggr, int type) {	// ���������g��
		Pos_X = argx;	// ���W���
		Pos_Y = argy;
		Grpt = arggr;	// �O���t�B�b�N�n���h��
		Type = type;
		Cnt = 0;		// ���[�V�����J�E���^�̃N���A
		
		//	�p�x�ݒ�
		switch (Type) {
		case EFFECT_DART:	//	�藠��������������
		case EFFECT_BOMB:	//	���e������������
		case EFFECT_WALL:	//	�ǂƂԂ�������
		case EFFECT_COLLISION://	�G�ƃv���C���[�܂��͕��g������������
			Angle = GetRand(360) / 180.0 * DX_PI;	// �`��̊p�x�����߂�
				break;
		case EFFECT_ALERT:
			Angle = 0;
			break;
		default:
			break;
		}
	}

	// �`��
	bool Draw(float camerax, float cameray) {
		Cnt++;		// ���[�V�����J�E���^�̃C���N�������g

		int X = (int)(Pos_X - camerax);
		int Y = (int)(Pos_Y - cameray);
		double EffectSize;

		//	�^�C�v�ɂ���ē����������G�t�F�N�g��ς���
		switch (Type) {
		case EFFECT_DART:	//	�藠��������������
			if (Cnt / DART_FRAME_PER >= DART_MOTION_NUM) return false;		// ���[�V�������I����Ă���false�Ԃ�
			EffectSize = DART_EFFECT_SIZE;	//	�G�t�F�N�g�̑傫����ݒ�
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[Cnt / DART_FRAME_PER], true);	//	�a���`��
			break;
		case EFFECT_BOMB:	//	���e������������
			if (Cnt / BOMB_FRAME_PER >= BOMB_MOTION_NUM) return false;		// ���[�V�������I����Ă���false�Ԃ�
			EffectSize = BOMB_EFFECT_SIZE;	//	�G�t�F�N�g�̑傫����ݒ�
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[Cnt / BOMB_FRAME_PER], true);	//	���e�`��
			break;
		case EFFECT_WALL:	//	�ǂƂԂ�������
			if (Cnt / WALL_FRAME_PER >= WALL_MOTION_NUM) return false;		// ���[�V�������I����Ă���false�Ԃ�
			EffectSize = WALL_EFFECT_SIZE;	//	�G�t�F�N�g�̑傫����ݒ�
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[Cnt / WALL_FRAME_PER], true);	//	�Ռ��`��
			break;
		case EFFECT_COLLISION://	�G�ƃv���C���[�܂��͕��g������������
			if (Cnt / COLLISION_FRAME_PER >= COLLISION_MOTION_NUM) return false;		// ���[�V�������I����Ă���false�Ԃ�
			EffectSize = COLLISION_EFFECT_SIZE;	//	�G�t�F�N�g�̑傫����ݒ�
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[COLLISION_MOTION_NUM - Cnt / COLLISION_FRAME_PER], true);	//	�Ռ��`��
			break;
		case EFFECT_ALERT:
			if (Cnt / ALERT_FRAME_PER >= ALERT_MOTION_NUM) return false;		// ���[�V�������I����Ă���false�Ԃ�
			EffectSize = COLLISION_EFFECT_SIZE;	//	�G�t�F�N�g�̑傫����ݒ�
			DrawRotaGraph(X, Y, EffectSize, Angle, Grpt[0], true);	//	�I�`��
			break;
		default:
			break;
		}
		return true;	// �܂����[�V����������Ȃ�true��Ԃ�
	}
};

// �G�t�F�N�g�Ǘ��N���X
class Effect_Manager {

private:
	int Dart_Gr[DART_MOTION_NUM];	//	�藠���̃��[�V�������̃O���t�B�b�N
	int Bomb_Gr[BOMB_MOTION_NUM];	//	���e�̃��[�V�������̃O���t�B�b�N
	int Collision_Gr[COLLISION_MOTION_NUM];	//	�G�ƏՓˎ�
	int Wall_Gr[WALL_MOTION_NUM];	//	�ǏՓˎ�
	int Alert_Gr[ALERT_MOTION_NUM];	//	�v���C���[������
	vector<Effect*>Effect_Tbl;

public:

	// Effect_Manager�N���X�̃R���X�g���N�^
	Effect_Manager() {
		//	�e�摜�𕪊��ǂݍ���
		LoadDivGraph("image/Effect/DartEffect.png", DART_MOTION_NUM, DART_MOTION_NUM, 1, 120, 120, Dart_Gr);
		LoadDivGraph("image/Effect/BombEffect.png", BOMB_MOTION_NUM, BOMB_MOTION_NUM, 1, 120, 120, Bomb_Gr);
		LoadDivGraph("image/Effect/WallEffect.png", WALL_MOTION_NUM, WALL_MOTION_NUM, 1, 120, 120, Wall_Gr);
		LoadDivGraph("image/Effect/CollisionEffect.png", COLLISION_MOTION_NUM, COLLISION_MOTION_NUM / 2, 2, 192, 192, Collision_Gr);
		Alert_Gr[0] = LoadGraph("image/Effect/AlertEffect.png", true);
	}

	// �e�[�u���̍Ō���ɓo�^
	void Register(float argx, float argy, int type) {
		switch (type)
		{
		case EFFECT_DART:	//	�藠���̗p�̃G�t�F�N�g��o�^
			Effect_Tbl.push_back(new Effect(argx, argy, Dart_Gr, type));
			break;
		case EFFECT_BOMB:	//	���e�̃G�t�F�N�g��o�^
			Effect_Tbl.push_back(new Effect(argx, argy, Bomb_Gr, type));
			break;
		case EFFECT_WALL:	//	�ǏՓ˂������p�̃G�t�F�N�g��o�^
			Effect_Tbl.push_back(new Effect(argx, argy, Wall_Gr, type));
			break;
		case EFFECT_COLLISION:	//	�G�ƏՓ˂������p�̃G�t�F�N�g��o�^
			Effect_Tbl.push_back(new Effect(argx, argy, Collision_Gr, type));
			break;
		case EFFECT_ALERT:	//	�G�ƏՓ˂������p�̃G�t�F�N�g��o�^
			Effect_Tbl.push_back(new Effect(argx, argy, Alert_Gr, type));
		default:	//	type���s���Ȃ�o�^���Ȃ�
			break;
		}
	}

	// �`��
	void Draw(float camerax, float cameray) {
		// �e�[�u�������ׂĎQ�Ƃ���
		for (int i = 0; i < (int)Effect_Tbl.size(); i++) {
			// �`�悵��
			if (Effect_Tbl[i]->Draw(camerax, cameray));
			else {
				// �������[�V�������I����Ă��炠����
				Effect_Tbl.erase(Effect_Tbl.begin() + i);
				//	�R���e�i��end���Ԃ��Ă�����I��
				if (Effect_Tbl.begin() + i == Effect_Tbl.end())break;
			}
		}
		//	�`�悵����e�[�u�����̒����𐳂�
		Effect_Tbl.shrink_to_fit();
	}
};