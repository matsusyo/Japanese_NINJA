//	���e1��1�̏���

#include"Bomb.h"

extern Player *Ply;						//	�v���C���[
extern Enemy_Manager *Eny;				//	�G
extern Map *My_Map;						//	�}�b�v
extern Quarter_Calculation *Qtr_Calc;	//	���W�v�Z
extern Qtr_Draw_Manager *My_Draw;		//	�`��e�[�u��

//	�R���X�g���N�^
Bomb::Bomb() {}
//	�f�X�g���N�^
Bomb::~Bomb() {}

//	���e�̋���
void Bomb::Move() {
	if (!Flg) return;	//	�t���O�̗����Ă��Ȃ���ΏI���

	Count--;	//	�������Ԃ�i�߂�

	//	�N�H�[�^�[���W�ɕϊ�
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);

	//	�J�����ʒu�𓾂�
	float CameraX = Ply->Get_Camera_X();	
	float CameraY = Ply->Get_Camera_Y();

	//	�`��e�[�u���ɓo�^������W���v�Z
	float X = (float)(Qtr_X + QTR_OFFSET_X - CameraX);	
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - CameraY);

	//	���e��傫����F���J�E���g�ɂ���ĕς���
	//	���e�����݂ł���܂�5�b�O
	if (4 * 60 < Count) {
		//	�`��e�[�u���ɓo�^
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[0]);	
		return;
	}

	//	4�b�O
	if (3 * 60 < Count) {
		//	�`��e�[�u���ɓo�^ �摜�������_���ɕς��Ĕ��e�炵�������� 
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[rand() % 5]);
		return;
	}

	//	3�b�O
	if (2 * 60 < Count) {
		//	�`��e�[�u���ɓo�^
		Spread_Size_X(EXPLOSION_SPREAD_SIZE_X);	//	���e�̃T�C�Y���L����
		Spread_Size_Y(EXPLOSION_SPREAD_SIZE_Y);
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[rand() % 5]);	//	�o�^
		return;
	}

	//	2�b�O
	if (1 * 60 < Count) {
		//	�`��e�[�u���ɓo�^
		Spread_Size_X(EXPLOSION_SPREAD_SIZE_X);	//	���e�̃T�C�Y���L����
		Spread_Size_Y(EXPLOSION_SPREAD_SIZE_Y);
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[rand() % 7]);	//	�o�^
		return;
	}

	//	1�b�O
	if (0.5 * 60 < Count) {
		//	�`��e�[�u���ɓo�^�@�������O�ȉ摜�Ƀ`�F���W
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[rand() % 3 + 5]);
		return;
	}

	//	0.5�b�O	��������
	if (0.5 * 60 >= Count) {
		
		//	�����̏��߂ɉ���炷
		if (!Explosion_Flg) {
			My_Sound->Play_Se(SE_BOMB_SPARK);	//	������
			Explosion_Flg = true;	//	�������ɂ���
		}
		//	�`��e�[�u���ɓo�^
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[9]);
		return;
	}
}

//	�G��}�b�v�Ƃ̓����蔻��
void Bomb::Judge_Enemy()
{
	//	�������͔���
	if (!Explosion_Flg) return;

	//	�G�ɓ������Ă��邩����
	RECT rc = Get_Rect();
	Eny->Judge_Player_Attack(rc, BOMB_DAMAGE, BOMB);

	//	�����I������甚�e������
	if (0 > Count) {
		Top_X = Top_Y = NULL;
		Flg = false;
	}
}