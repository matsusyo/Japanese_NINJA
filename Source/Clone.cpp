//	���g�̏��� 1�̂����o��Ȃ�

#include"Clone.h"

extern Quarter_Calculation *Qtr_Calc;	//	�g�b�v,�N�H�[�^�[���W���ݕϊ�
extern Map *My_Map;						//	�}�b�v
extern Player *Ply;						//	�v���C���[
extern Qtr_Draw_Manager *My_Draw;		//	�`��e�[�u��

//	�R���X�g���N�^
Clone::Clone() {};	//	�������͎g��Ȃ�
Clone::Clone(float topX,float topY,float topAngle,float qtrAngle)
{
	//	�v���C���[�������Ă�������ɕ��g������悤�ɂ���
	Top_X = topX;	//	�v���C���[�̍��W��o�^
	Top_Y = topY;
	Top_Angle = topAngle;	//	�g�b�v�r���[��ł̊p�x
	Qtr_Angle = qtrAngle;	//	�N�H�[�^�r���[��ł̊p�x
	Speed = CLONE_SPEED;	//	���g�̃X�s�[�h

	//	�v���C���[�Ɠ����T�C�Y������
	Size_X = PLY_SIZE_X; Size_Y = PLY_SIZE_Y;
	H_Size_X = Size_X / 2; H_Size_Y = Size_Y / 2;

	LoadDivGraph("Image/Player/Ninja.png", 24, 6, 4, 32, 50, Clone_Gr);	//	�摜�ǂݍ���
	Clone_Gr_Num = 0;	//	������
	Clone_Flg = true;	//	���g���݃t���O�𗧂Ă�
}

//	�f�X�g���N�^
Clone::~Clone() {}

//	����
void Clone::Move()
{
	Clone_Gr_Num = rand() % 3;	//	�v���C���[�摜�𓮂���

	//	X,Y�̈ړ��ʌv�Z
	float MoveX = cosf(Top_Angle)*Speed;
	float MoveY = sinf(Top_Angle)*Speed;

	//	���W�ɉ����ĕ��g�𓮂���
	Top_X += MoveX;	
	Top_Y += MoveY;

	//	�ǂɂԂ���������ł�����
	if (My_Map->Map_Get_Chip(Top_X, Top_Y) >= 7) {
		//	������
		Top_X = Top_Y = NULL;
		Clone_Flg = false;	//	���g���݃t���O�𗎂Ƃ�
	}

	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	�N�H�[�^�[�L�������W�v�Z

	//	�J�����ʒu���v�Z
	Camera_X = Ply->Get_Camera_X();
	Camera_Y = Ply->Get_Camera_Y();

	//	���g�̊p�x�ɂ���ĕ��g�̉摜��ς���
	if (Qtr_Angle < 175 && Qtr_Angle>95)Clone_Gr_Num += 3;				//	���΂߉�
	else if (Qtr_Angle <= 95 && Qtr_Angle >= 85)Clone_Gr_Num += 0;		//	����
	else if (Qtr_Angle < 85 && Qtr_Angle>5)Clone_Gr_Num += 9;			//	�E�΂߉�
	else if (Qtr_Angle >= 175 || Qtr_Angle <= -175)Clone_Gr_Num += 6;	//	����
	else if (Qtr_Angle <= 5 && Qtr_Angle >= -5)Clone_Gr_Num += 12;		//	�E��
	else if (Qtr_Angle > -175 && Qtr_Angle < -95)Clone_Gr_Num += 15;	//	���΂ߏ�
	else if (Qtr_Angle >= -95 && Qtr_Angle <= -85)Clone_Gr_Num += 18;	//	������
	else if (Qtr_Angle > -85 && Qtr_Angle < -5)Clone_Gr_Num += 21;		//	�E�΂ߏ�

	//	�N�H�[�^�[�r���[��ł̒��S���W��`��e�[�u���ɓo�^
	float X = (float)(Qtr_X + QTR_OFFSET_X - Camera_X);	//	���S���W���v�Z
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);
	My_Draw->Register(X, Y, H_Size_X, H_Size_Y, CLONE, Clone_Gr[Clone_Gr_Num]);	//	�o�^
}

