//	��ʏ�ɑ��݂���藠�����Ǘ�

#pragma once
#include"Dart.h"		//	�藠��1��1��
#include"InputCtrl.h"	//	�L�[�}�E�X����
#include"EnemyManager.h"//	�G

class Dart_Manager
{
private:
	Dart Dart_Tbl[DART_NUM];//	�V���b�g�e�[�u��
	int Gr[1];	// �V���b�g�̃O���t�B�b�N �P��

public:
	Dart_Manager();		//	�R���X�g���N�^
	~Dart_Manager();	//	�f�X�g���N�^
	void Register(float,float,float,int);	// �藠�����e�[�u���ɓo�^����
	void Move();		//	���t���[���Ă�
	void Judge_Enemy();	//	�G�Ƃ̓����蔻��
};