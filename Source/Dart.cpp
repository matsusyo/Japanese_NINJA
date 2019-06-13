//	�藠��1��1�̏���

#include"Dart.h"

extern Player *Ply;						//	�v���C���[
extern Enemy_Manager *Eny;				//	�G
extern Map *My_Map;						//	�}�b�v
extern Quarter_Calculation *Qtr_Calc;	//	���W�v�Z
extern Qtr_Draw_Manager *My_Draw;		//	�`��e�[�u��
extern Sound_Manager *My_Sound;			//	��

//	�R���X�g���N�^
Dart::Dart() {}
//	�f�X�g���N�^
Dart::~Dart(){}

//	�藠���̓���
void Dart::Move() {
	if (!Flg) return;	//	�t���O�̗����Ă��Ȃ���Ή������Ȃ�
	
	//	 ���ˌ�̓��� �ǔ�����
	Target = Eny->Get_Target(Top_X, Top_Y);	//	�^�[�Q�b�g�����炤
	
	//	�^�[�Q�b�g��������������Đ��񂷂�
	if (Target!=NULL) {
		float bx = cosf(Top_Angle);		//	�V���b�g�̐i��ł��� X ����
		float by = sinf(Top_Angle);		//	�V���b�g�̐i��ł��� Y ����
		float ax = Target->Get_Top_X() - Top_X;	//	�i�ނׂ� X ����
		float ay = Target->Get_Top_Y() - Top_Y;	//	�i�ނׂ� Y ����
		float Radius = DART_RADIUS;		//	���� (�t���[�����Ƃ̃��W�A��)

		//	�O�ς𗘗p���^�[�Q�b�g���ɐ��񂷂�
		Top_Angle += (ax * by - ay * bx < 0.0) ? +Radius : -Radius;
	}

	//	�藠���̌����Ɉړ�������
	Top_X += cosf(Top_Angle) * Speed;	//	x�ړ�
	Top_Y += sinf(Top_Angle) * Speed;	//	y�ړ�

	//	��ʊO�֏o���������
	if (Top_Y < 0 || Top_Y > MAP_Y*CHIP_SIZE_Y || Top_X < 0 || Top_X > MAP_X*CHIP_SIZE_X) {
		Flg = false;
	}

	//	�ǂɓ������Ă�����
	if (My_Map->Map_Get_Chip(Top_X, Top_Y) >= 7) {
		Flg = false;
	}

	Count--;	//	���ݎ��Ԃ����炷
	if (Count <= 0) Flg = false;	//	0�ɂȂ��������

	//	�N�H�[�^���W�v�Z
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	�N�H�[�^���W�v�Z
	float X = (float)(Qtr_X + QTR_OFFSET_X - Ply->Get_Camera_X());	//	�`��ʒu�v�Z
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Ply->Get_Camera_Y());
	My_Draw->Register(X, Y, H_Size_X, H_Size_Y, DART, Graph);		//	�`��e�[�u���ɓo�^
}


//	�G�Ƃ̓����蔻��
void Dart::Judge_Enemy()
{
	//	�G�ɓ������Ă��邩���肵����������藠���͏�����
	RECT rc = Get_Rect();
	if (Eny->Judge_Player_Attack(rc, DART_DAMAGE, DART)) {
		//	������
		Top_X = Top_Y = NULL;
		Flg = false;
	}
}