//	�S�ēG���Ǘ�	

#pragma once
#include"Enemy.h"

const int ENY_NUM = 100;	//	�G�̍ő吔
const int ENEMY_BLUE_GR_NUM = 24;	//	�G���G�̃O���t�B�b�N��
const int ENEMY_BOSS_GR_NUM = 24;	//	�{�X�̃O���t�B�b�N

const float DART_TARGET_DISTANCE = 50;

class Enemy_Manager
{
private:
	Enemy* Eny_Tbl[ENY_NUM];	//	�ő吔���̓G�e�[�u��
	int Enemy_Blue_Graph[ENEMY_BLUE_GR_NUM];	//	�G���G�̉摜
	int Enemy_Boss_Graph[ENEMY_BOSS_GR_NUM];	//	�{�X�̉摜
	int Blue_Gauge_Graph[2];	//	�G����Hp�摜
	int Boss_Gauge_Graph[2];	//	�{�X��Hp�摜

public:
	Enemy_Manager();
	~Enemy_Manager();

	bool Judge_Player_Attack(RECT rc, float dmg, int type);	//	�G�S�Ẵ_���[�W�v�Z
	bool Judge_Player(RECT rc);	//	�G�Ɖ������ڐG���Ă��邩�Ԃ�
	
	//	���t���[���Ă�
	void Move();			//	����
	void Status_Update();	//	�X�e�[�^�X�̍X�V �����Ă邩����ł邩�Ƃ�
	void Draw_Ray();		//	���E��`��
	void Draw_Gauge();		//	HP��`��

	//	�G�𐶐�����@�G���A���Ƃɕ����ČĂ�
	void Register_Enemy();	
	//	�G���z�[�~���O���鎞�Ɏg�p
	Enemy* Get_Target(float, float);	//	�藠���̃^�[�Q�b�g�����炤
};