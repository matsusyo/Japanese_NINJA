//	 �S�Ă̔��e���Ǘ�����

#include"BombManager.h"

extern Input_Controll *Inp_Ctrl;		//	�L�[����
extern Quarter_Calculation *Qtr_Calc;	//	���W�v�Z

//	�R���X�g���N�^
Bomb_Manager::Bomb_Manager() 
{
	//	�O���t�B�b�N�̓ǂݍ���
	LoadDivGraph("image/Bomb/Bombs.png", 11, 6, 2, 32, 32, Gr);

	//	�e�[�u�����̑S�Ẵt���O�𗎂Ƃ�
	for (int i = 0; i < BOMB_NUM; i++)Bomb_Tbl[i].Set_Flg(false);	
}

//	�f�X�g���N�^
Bomb_Manager::~Bomb_Manager() {}

//	���e���e�[�u���ɓo�^����
void Bomb_Manager::Register(float Top_X, float Top_Y)
{
	//	�g�p����Ă���Ƃ���̓X�L�b�v
	for (int i = 0; i < BOMB_NUM; i++) {

		if (Bomb_Tbl[i].Is_Flg()) continue;

		//	�o�^
		Bomb_Tbl[i].Set_Flg(true);	//	�t���O�𗧂Ă�
		Bomb_Tbl[i].Set_Explosion_Flg(false);		//	�����t���O�𗎂Ƃ�
		Bomb_Tbl[i].Set_Top_XY(Top_X, Top_Y);		//	���W��ݒ�
		Bomb_Tbl[i].Set_Count(BOMB_EXPLOSION_TIME);	//	�^�C�����~�b�g��ݒ�
		Bomb_Tbl[i].Set_Size((float)BOMB_SIZE_X, (float)BOMB_SIZE_Y);	//	�T�C�Y��ݒ�
	
		//	11�̉摜�����ׂēǂݍ��܂���
		for (int n = 0; n < BOMB_GRAPH; n++) { Bomb_Tbl[i].Set_Graph_Handle(n, Gr[n]); }

		break;	// �ЂƂo�^�����甲����
	}
}

//	�t���[�����Ƃ̋���
void Bomb_Manager::Move() 
{
	//	�e�[�u���ɓo�^���ꂽ���e�̋���
	for (int i = 0; i < BOMB_NUM; i++) { Bomb_Tbl[i].Move(); }
}

//	�G�Ƃ̓����蔻��
void Bomb_Manager::Judge_Enemy()
{
	//	���e���Ƃɔ���
	for (int i = 0; i < BOMB_NUM; i++) { Bomb_Tbl[i].Judge_Enemy(); }
}

//	�v���C���[�Ƃ̓����蔻�� 
bool Bomb_Manager::Judge_Player(RECT rc)
{
	bool Hit = false;
	for (int i = 0; i < BOMB_NUM; i++) {

		//	�o�^����Ă��Ȃ��ꏊ�̓X�L�b�v
		if (!Bomb_Tbl[i].Is_Flg()) continue;

		//	������������rc���ڐG���Ă���true��Ԃ�
		if (Bomb_Tbl[i].Is_Explosion_Flg() && Bomb_Tbl[i].Collision(rc)) {
			return true;
		}
	}
	return false;
}