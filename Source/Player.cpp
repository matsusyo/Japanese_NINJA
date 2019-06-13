#include"Player.h"

extern Input_Controll *Inp_Ctrl;		//	�L�[����
extern Quarter_Calculation *Qtr_Calc;	//	���W�v�Z
extern Map *My_Map;						//	�}�b�v
extern Qtr_Draw_Manager *My_Draw;		//	�`��e�[�u��
extern Sound_Manager *My_Sound;			//	��
extern Effect_Manager *My_Effect;		//	�G�t�F�N�g

//	�R���X�g���N�^
Player::Player()
{
	//	�����ʒu
	Top_X = 140.0f; Top_Y = 500.0f;

	//	�N�H�[�^�r���[��ł̍��W
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	
	//	�J�����ʒu
	Camera_X = Qtr_X - H_WINDOW_X;Camera_Y = Qtr_Y - H_WINDOW_Y;
	
	//	�p�x�@�X�s�[�h
	Top_Angle = 0;
	Speed = PLY_DEF_SPEED;
	
	//	�T�C�Y�v�Z
	Size_X = PLY_SIZE_X; Size_Y = PLY_SIZE_Y;
	H_Size_X = Size_X / 2;H_Size_Y = Size_Y / 2;

	//	�O���t�B�b�N�ǂݍ���
	LoadDivGraph("Image/Player/Ninja.png", 24, 6, 4, 32, 50, Ply_Gr);
	Ply_Gr_Num = 0;

	//	�Q�[�W�֘A
	//	HP,MP�ő�l�ƍŏ��l�ݒ�
	Gauge_Bar[HP] = new Gauge(HP_MAX, 0);
	Gauge_Bar[MP] = new Gauge(MP_MAX, 0);
	//	�O���t�B�b�N�ǂݍ���
	Gauge_Gr[HP] = LoadGraph("Image/Gauge/GaugeHp.png");
	Gauge_Gr[MP] = LoadGraph("Image/Gauge/GaugeMP.png");
	Gauge_Gr[FRAME] = LoadGraph("Image/Gauge/GaugeFrame.png");
	
	//	�t���O�ƃJ�E���g��������
	Mp_Flg = Clear_Flg = false;
	Damage_Cnt = Dying_Cnt = 0;
	
	//	�X�e�[�^�X���m�[�}���ɐݒ�
	Status.On(NORMAL);
}

//	�f�X�g���N�^
Player::~Player(){}

////////////	����
void Player::Move()
{
	//	�N�H�[�^�[�L�������W�v�Z
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	
	//	�J�����ʒu���v�Z
	Camera_X = Qtr_X - (H_WINDOW_X);
	Camera_Y = Qtr_Y - (H_WINDOW_Y);
	if (Camera_X < 0) Camera_X = 0;
	if (Camera_X >(MAP_X*CHIP_SIZE_X))Camera_X = MAP_X * CHIP_SIZE_X;
	if (Camera_Y >(MAP_Y*CHIP_SIZE_Y))Camera_Y = MAP_Y * CHIP_SIZE_Y;

	//	�N�H�[�^�}�E�X���W�l��
	Inp_Ctrl->Get_Mouse_Pointer(&Qtr_MX, &Qtr_MY);

	//	�N�H�[�^�}�E�X���W���g�b�v�r���[�}�E�X���W�ɕϊ�
	Qtr_Calc->Qtr_Top((Qtr_MX + Camera_X), (Qtr_MY + Camera_Y), &Top_MX, &Top_MY);

	//	�}�E�X�J�[�\���̕������v���C���[�̊p�x�ɂ���
	Top_Angle = atan2f(Top_MY - Top_Y, Top_MX - Top_X);

	Ply_Gr_Num = 0;

	//	���N���b�N���ړ�
	if (Inp_Ctrl->Is_Input(MOUSE_LEFT)) {

		if (rand() % 100 == 0)My_Sound->Play_Se(SE_DASH);

		//	�L�����̉摜��ς���
		Ply_Gr_Num = rand() % 3;

		//	�ړ��ʌv�Z
		float MoveX = cosf(Top_Angle)*Speed;
		float MoveY = sinf(Top_Angle)*Speed;

		float x = (float)(Qtr_X + QTR_OFFSET_X);	//	�o�^������W���v�Z
		float y = (float)(Qtr_Y + QTR_OFFSET_Y);

		//	�������̈ړ��@6�_�Ŕ���
		RECT rc = Get_Rect();
		if (((My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.top) < 7) && (My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.top) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.bottom) < 7) && (My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.bottom) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.top + H_Size_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.top + H_Size_Y) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)((rc.top + rc.bottom) / 2)) < 7) && (My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)((rc.top + rc.bottom) / 2)) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)rc.top) < 7) && (My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)rc.top) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)rc.bottom) < 7) && (My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)rc.bottom) >= 0)))
			Top_X += MoveX;
		else {
			My_Sound->Play_Se(SE_COLLISION_WALL);	//	�Փˉ�
			My_Effect->Register(x, y, EFFECT_WALL);			//	�Փ˃G�t�F�N�g
			
		}
		//	�c�����̈ړ� 6�_�Ŕ���
		if (((My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + MoveY) < 7) && (My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + MoveY) < 7) && (My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + MoveY) < 7) && (My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + MoveY) < 7) && (My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + MoveY) < 7) && (My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + MoveY) < 7) && (My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + MoveY) >= 0)))
			Top_Y += MoveY;
		else {
			My_Sound->Play_Se(SE_COLLISION_WALL);	//	�Փˉ�
			My_Effect->Register(x, y, EFFECT_WALL);			//	�Փ˃G�t�F�N�g
		}
	}

	//	�������łȂ����MP��������,�v���C���[�̉摜
	else {
		Ply_Gr_Num = 1;				//	�L�����̉摜���~�܂��Ă����Ԃɂ���
		if (Inp_Ctrl->Is_Input(KEY_LSHIFT)) {
			Gauge_Bar[MP]->Add_Now(+0.5f);	//	�����Ă��Ȃ��Ԃ�MP����
		}
	}

	if (!Gauge_Bar[MP]->Is_Full())Gauge_Bar[MP]->Add_Now(0.05f);

	//	�}�E�X�̕�������������
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	Qtr_Angle = atan2f((Qtr_MY + QTR_OFFSET_X + Camera_Y) - (Qtr_Y + QTR_OFFSET_Y), (Qtr_MX + Camera_X) - Qtr_X);
	Qtr_Angle = Qtr_Angle * 180.0f / DX_PI_F;

	//	�N�H�[�^�r���[��ł̊p�x8���������ꂩ������
	if (Qtr_Angle < 175 && Qtr_Angle>95)Ply_Gr_Num += 3;				//	���΂߉�
	else if (Qtr_Angle <= 95 && Qtr_Angle >= 85)Ply_Gr_Num += 0;		//	����
	else if (Qtr_Angle < 85 && Qtr_Angle>5)Ply_Gr_Num += 9;				//	�E�΂߉�
	else if (Qtr_Angle >= 175 || Qtr_Angle <= -175)Ply_Gr_Num += 6;		//	����
	else if (Qtr_Angle <= 5 && Qtr_Angle >= -5)Ply_Gr_Num += 12;		//	�E��
	else if (Qtr_Angle > -175 && Qtr_Angle < -95)Ply_Gr_Num += 15;		//	���΂ߏ�
	else if (Qtr_Angle >= -95 && Qtr_Angle <= -85)Ply_Gr_Num += 18;		//	������
	else if (Qtr_Angle > -85 && Qtr_Angle < -5)Ply_Gr_Num += 21;		//	�E�΂ߏ�

	//	�N�H�[�^�[�r���[��ł̒��S���W��`��e�[�u���ɓo�^
	float X = (float)(Qtr_X + QTR_OFFSET_X - Camera_X);
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);
	My_Draw->Register(X, Y, H_Size_X, H_Size_Y, PLAYER, Ply_Gr[Ply_Gr_Num]);	//	�o�^
}

//	HP,MP�Q�[�W�`��
void Player::Draw_Gauge()
{
	// Hp �O�g �{��
	DrawExtendGraph((int)5, (int)5, (int)(115 + 11), (int)55, Gauge_Gr[FRAME], TRUE);

	float rate = Gauge_Bar[HP]->Rate();
	DrawExtendGraph((int)11, (int)5, (int)(111 * rate + 11), (int)30, Gauge_Gr[HP], TRUE);

	rate = Gauge_Bar[MP]->Rate();
	DrawExtendGraph((int)11, (int)30, (int)(111 * rate + 11), (int)55, Gauge_Gr[MP], TRUE);
}

//	MP�v�Z
void Player::Mp_Proc(float value)
{

	//	Mp���O�ɂȂ����Ƃ��t���O�𗧂Ă�
	if (!Gauge_Bar[MP]->Is_Empty()) {
		Gauge_Bar[MP]->Add_Now(value);
	}

	//	�t���O�������Ă���Mp��Max�ɂȂ�����t���O��������
	if (Mp_Flg && Gauge_Bar[MP]->Is_Full()) {
		Mp_Flg = false;
	}
}

//	�X�e�[�^�X�v�Z
void Player::Status_Update()
{
	//	�_���[�W��Ԃ̏ꍇ
	if (Status.Is(DAMAGE)){
		
		//	�J�E���g���O���Ⴍ�Ȃ���΃f�N�������g
		if (Damage_Cnt > 0)
			Damage_Cnt--;
		
		//	�J�E���g���O�Ȃ�m�[�}���ɂ���
		else {
			Status.Reset();
			Status.On(NORMAL);
		}
	}

	//	����ł����Ԃ̏ꍇ
	else if (Status.Is(DYING)) {
		
		//	�O�łȂ��Ȃ�f�N�������g
		if (Dying_Cnt > 0)
			Dying_Cnt--;
		
		//	�J�E���g���O�Ȃ玀�S�ɂ���
		else{
			Status.Reset();
			Status.On(DIE);
		}
	}
}

//	HP�v�Z
void Player::Hp_Proc(float Dmg)
{
	//	HP���O�Ȃ�X�e�[�^�X������ł����Ԃɂ���
	if (Gauge_Bar[HP]->Is_Empty()) {
		Status.Reset();
		Status.On(DYING);
		return;
	}

	//	�_���[�W��������
	Gauge_Bar[HP]->Add_Now(Dmg);
	Status.Reset();	//	�X�e�[�^�X���Z�b�g

	//	Hp���O�łȂ���΃X�e�[�^�X���_���[�W��Ԃɂ���
	if (!Gauge_Bar[HP]->Is_Empty()) {
		Status.On(DAMAGE);
		Set_Damage_Cnt(DAMAGE_COUNT);
	}

	//	�O�Ȃ玀��ł��Ԃɂ���
	else {
		Status.On(DYING);
		Set_Dying_Cnt(DYING_COUNT);
		My_Sound->Play_Se(SE_ERASE_PLAYER);
	}
}