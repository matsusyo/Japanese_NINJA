//	��ʏ�ɑ��݂��锚�e���Ǘ�

#pragma once
#include"Bomb.h"		//	���e1��1�̋���
#include"InputCtrl.h"	//	�L�[�}�E�X����
#include"EnemyManager.h"//	�G

class Bomb_Manager
{
private:

	Bomb Bomb_Tbl[BOMB_NUM];	//	���e�e�[�u��

	int Gr[11];	// ���e�̃O���t�B�b�N

public:
	Bomb_Manager();		//	�R���X�g���N�^
	~Bomb_Manager();	//	�f�X�g���N�^
	void Register(float, float);	//	�e�[�u���ɓo�^����
	void Move();					//	���t���[�����Ƃ̔��e�̋���
	void Judge_Enemy();				//	�G�Ƃ̓����蔻��
	bool Judge_Player(RECT rc);		//	�v���C���[�Ƃ̓����蔻��
};
