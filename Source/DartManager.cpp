//	�藠����S�ĊǗ�����

#include"DartManager.h"	//	���ׂĂ̎藠���Ǘ�

extern Input_Controll *Inp_Ctrl;		//	�L�[����
extern Quarter_Calculation *Qtr_Calc;	//	���W�v�Z

//	�R���X�g���N�^
Dart_Manager::Dart_Manager() 
{
	//	�藠���̃O���t�B�b�N�̓ǂݍ���
	Gr[0] = LoadGraph("image/Dart/Knife1.png");
	//	�e�[�u�����̃t���O�𗎂Ƃ��Ă���
	for (int i = 0; i < DART_NUM; i++) Dart_Tbl[i].Set_Flg(false);	
}

//	�f�X�g���N�^
Dart_Manager::~Dart_Manager(){}

//	�藠�����e�[�u���ɓo�^����
void Dart_Manager::Register(float Top_X, float Top_Y, float Angle, int Type)
{
	//	�g�p����Ă���Ƃ���̓X�L�b�v
	for (int i = 0; i < DART_NUM; i++) {

		if (Dart_Tbl[i].Is_Flg()) continue;

		//	�藠���o�^
		Dart_Tbl[i].Set_Flg(true);	//	���݃t���O�𗧂Ă�
		Dart_Tbl[i].Set_Type(Type);	//	�藠���̃^�C�v���Z�b�g
		Dart_Tbl[i].Set_Top_XY(Top_X, Top_Y);	//	���W���Z�b�g
		Dart_Tbl[i].Set_Speed(DART_SPEED);		//	�X�s�[�h�Z�b�g
		Dart_Tbl[i].Set_Top_Angle(Angle);		//	�g�b�v�r���[�ł̊p�x���Z�b�g
		Dart_Tbl[i].Set_Size((float)DART_SIZE_X, (float)DART_SIZE_Y);	//	�T�C�Y���Z�b�g
		Dart_Tbl[i].Set_Graph_Handle(Gr[Type]);	//	�^�C�v�ɂ���ăO���t�B�b�N�����߂�
		Dart_Tbl[i].Set_Count(DART_COUNT);		//	���݂ł��鎞�Ԃ��Z�b�g	

		break;	// �ЂƂo�^�����甲����
	}
}

//	�t���[�����Ƃ̋���
void Dart_Manager::Move() 
{
	//	�藠������Ƃ̋���
	for (int i = 0; i<DART_NUM; i++) {
		Dart_Tbl[i].Move();	
	}
}

//	�����蔻��
void Dart_Manager::Judge_Enemy()
{
	//	�藠������Ƃɔ���
	for (int i = 0; i < DART_NUM; i++) {
		Dart_Tbl[i].Judge_Enemy();
	}
}