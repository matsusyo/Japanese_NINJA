//	 �G�̃^�C�v���Ƃ̏���

#include"Enemy.h" 

extern Player *Ply;	//	�v���C���[
extern Map *My_Map;	//	�}�b�v
extern Input_Controll *Inp_Ctrl;		//	�L�[���́@�}�E�X����
extern Quarter_Calculation *Qtr_Calc;	//	�N�H�[�^���W�ϊ�		
extern Clone *Ply_Clone;				//	���g
extern Qtr_Draw_Manager *My_Draw;		//	�`��e�[�u��
extern Effect_Manager *My_Effect;		//	����
extern Sound_Manager *My_Sound;			//	��
extern Time_Manager *My_Time;			//	�N���A�^�C���A�o�ߎ��ԂȂǂ��Ǘ�

Enemy::Enemy(){}	//	�R���X�g���N�^
Enemy::~Enemy() {}	//	�f�X�g���N�^

//	�G�̋���
void Enemy::Move(){}

bool Enemy::Judge_Move(RECT rc, float *topx, float *topy, float movex, float movey) {
	
	bool flg = false;	//	���ʂ��󂯎��
	//	�ǂɂԂ����ĂȂ���Έړ�������
	//	�������̈ړ��@6�_�Ŕ���
	if ((My_Map->Map_Get_Chip((float)rc.left + movex, (float)rc.top) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.left + movex, (float)rc.bottom) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.left + movex, (float)((rc.top + rc.bottom) / 2)) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right + movex, (float)((rc.top + rc.bottom) / 2)) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right + movex, (float)rc.top) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right + movex, (float)rc.bottom) < 7)) {
		*topx += movex;
		flg = true;	//	�ړ�������true
	}

	//	�c�����̈ړ��@6�_�Ŕ���
	if ((My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + movey) < 7)) {
		*topy += movey;
		flg = true;	//	�ړ�������true
	}
	return flg;
}

//	�X�e�[�^�X�̍X�V
void Enemy::Status_Update()
{
	//	�X�e�[�^�X���_���[�W�ꍇ
	if (Status.Is(DAMAGE)) {

		//	�J�E���g���O�łȂ���΃f�N�������g
		if (Damage_Cnt > 0) {
			Damage_Cnt--;
		}

		//	�O�ȉ��Ȃ�X�e�[�^�X���m�[�}���ɂ���
		else {
			Status.Reset();
			Status.On(NORMAL);
		}
	}

	//	����ł���ꍇ
	else if (Status.Is(DYING)) {
	
		//	�J�E���g���O�łȂ���΃f�N�������g
		if (Dying_Cnt > 0) {
			Dying_Cnt--;
		}
		
		//	�O�ȉ��Ȃ�X�e�[�^�X�����S�ɂ���
		else {
			Status.Reset();
			Status.On(DIE);
		}
	}
}


//	HP�ƊO�g��`��
void Enemy::Draw_Gauge()
{
	//	�E�B���h�E�O�Ȃ�`�悵�Ȃ�
	int X = (int)(Qtr_X - Camera_X);
	int Y = (int)(Qtr_Y - Camera_Y);
	if (X > WINDOW_X + CHIP_SIZE_X * 2 || X < -CHIP_SIZE_X * 2) { return; }
	if (Y > WINDOW_Y + CHIP_SIZE_Y * 2 || Y < -CHIP_SIZE_Y * 2) { return; }

	// HP�̊O�g��`��
	int FrameX1 = (int)(Qtr_X - Camera_X - H_Size_X);		//	�n�_X
	int FrameY1 = (int)(Qtr_Y - Camera_Y + H_Size_Y / 2);	//	�n�_Y
	int FrameX2 = (int)(Qtr_X - Camera_X + H_Size_X);		//	�I�_X
	int FrameY2 = (int)(Qtr_Y - Camera_Y + H_Size_Y);		//	�I�_Y
	DrawExtendGraph(FrameX1, FrameY1, FrameX2, FrameY2, Gauge_Gr[FRAME], true);	//	�G�̓���ɕ`��

	//	Hp��`��
	int HpX1 = (int)(Qtr_X - Camera_X - H_Size_X + 2);			//	�n�_X�@�O�g�Ɏ��܂�悤�ɂ��� +2
	int HpY1 = (int)(Qtr_Y - Camera_Y + H_Size_Y / 2 + 2);		//	�n�_Y�@�O�g�Ɏ��߂� +2
	int HpX2 = (int)(HpX1 + (Gauge_Bar[HP]->Get_Now() / 1.8));	//	�I�_X�@���݂�Hp���̒����ɂ���
	int HpY2 = (int)(Qtr_Y - Camera_Y + H_Size_Y - 2);			//	�I�_Y�@�O�g�Ɏ��߂� -2
	DrawExtendGraph(HpX1, HpY1, (int)((float)HpX2), (int)HpY2, Gauge_Gr[HP], true);	//	�O�g��HP�����܂�悤�ɕ`��
}


//	�������� Enemy_Blue
Enemy_Blue::Enemy_Blue(float argx, float argy,int graph[],int gaugegr[])
{
	Top_X = argx;	//	���W�o�^ 
	Top_Y = argy;	
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	�N�H�[�^���W�ϊ�
	Camera_X = Ply->Get_Camera_X();	//	�J�����Z�b�g
	Camera_Y = Ply->Get_Camera_Y();
	Speed = ENEMY_SPEED;			//	�X�s�[�h�Z�b�g
	Size_X = ENY_SIZE_X; Size_Y = ENY_SIZE_Y;		//	�摜�T�C�Y�Z�b�g
	H_Size_X = Size_X / 2; H_Size_Y = Size_Y / 2;	//	�����̃T�C�Y
	for (int i = 0; i < ENEMY_GR_NUM; i++) { Eny_Gr[i] = graph[i]; }	//	�G�̉摜�𕪊��ǂݍ���
	Eny_Gr_Num = 0;	//	�摜�i���o�[������

	//	�K���ȕ���������
	Top_Angle = (float)(GetRand(180));
	if (GetRand(1) != 0)Top_Angle *= -1;
	Top_Radian = Top_Angle * (DX_PI_F / 180);	//	���W�A���ɕϊ�

	Change_Angle_Cnt = GetRand(CHANGE_ANGLE_TIMING);	//	������ς���^�C�~���O�������_���ɏ�����
	Walk_Cnt = GetRand(MOVE_TIMING);	//	�ړ�����^�C�~���O�������_���ɏ�����
	 
	//	���F�ł��鋗�������߂�
	Vision_Distance = VISION_BASE;	//	���������
	if (GetRand(1) == 0)Vision_Distance += (float)GetRand(VISION_ADD);	//	���F������L�΂�
	else Vision_Distance -= (float)GetRand(VISION_ADD);	//	�Z������

	//	���F�ł���p�x�̐�Βl�����߂�
	Abs_Angle = ABS_ANGLE_BASE;	//	��p����
	if (GetRand(1) == 0)Abs_Angle += (float)GetRand(ABS_ANGLE_ADD);	//	���E���L����
	else Abs_Angle -= (float)GetRand(ABS_ANGLE_ADD);	//	��������

	//	�Q�[�W�֘A
	Gauge_Bar[HP] = new Gauge(ENY_BLUE_HP_MAX, 0);	//	HP������
	Gauge_Gr[HP] = gaugegr[HP];
	Gauge_Gr[FRAME] = gaugegr[FRAME];

	Hp_Flg = false;			//	HP�t���O�𗎂Ƃ�
	Find_Flg = false;
	Damage_Cnt = Dying_Cnt = 0;		//	��e,���S���G���Ԃ�������
	Status.On(NORMAL);				//	�X�e�[�^�X��ʏ��Ԃɂ���
}

//	�f�X�g���N�^
Enemy_Blue::~Enemy_Blue(){}

//	����
void Enemy_Blue::Move()
{
	Eny_Gr_Num = 0;	//	�摜�i���o�[��������
	
	//	�����鋗�������E���Ȃ�v���C���[��ǂ�������
	if (Search_Player()) {

		//	�G���ŏ��Ɍ�������
		if (!Find_Flg) {
			Find_Flg = true;	//	�t���O�𗧂Ă�
			Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	�N�H�[�^�[���W�ɕϊ�
			My_Effect->Register(Qtr_X + QTR_OFFSET_X, Qtr_Y + QTR_OFFSET_Y, EFFECT_ALERT);	//	�I��`��
			My_Sound->Play_Se(SE_ALERT);	//	��������炷
			if (Get_Target_Name() == PLAYER)My_Time->Add_Time_Limit(-5.0f);	//	�v���C���[�Ȃ琧�����Ԃ�Z�k
		}

		Top_Angle = Target_Angle;	//	�^�[�Q�b�g�ւ̊p�x�Ǝ����̊p�x�𓯂��ɂ���

		//	�p�x�̃G���[�΍�
		if (Top_Angle > 180)Top_Angle -= 360;	//	�p�x��180����������-360�x��������
		if (Top_Angle < -180)Top_Angle += 360;	//	-180�x�����������+360�x��������
		Top_Radian = Top_Angle * (DX_PI_F / 180);	//	���W�A���ɕϊ�

		//	�ړ��ʌv�Z
		float MoveX = (cosf(Top_Radian)*Speed);
		float MoveY = (sinf(Top_Radian)*Speed);
		
		//	�ǂɂԂ����ĂȂ���Έړ�������
		if (Judge_Move(Get_Rect(), &Top_X, &Top_Y, MoveX, MoveY)) 
			Eny_Gr_Num = rand() % 3;	//	�����Ă���悤�ɂ݂��邽�߂ɉ摜��ύX
	}

	//	�v���C���[�������Ă��Ȃ����Ɍ�����ύX
	else {
		Change_Angle_Cnt++;	//	�J�E���g��i�߂�

		//	������ς���^�C�~���O�ɂȂ�����ς���
		if (Change_Angle_Cnt > CHANGE_ANGLE_TIMING) {
			Change_Angle_Cnt = 0;	//	������
			float PlusAngle = (float)(GetRand(45));	//	�����_���Ɍ�����ݒ�
			if (GetRand(1) != 0)PlusAngle *= -1;
			Top_Angle += PlusAngle;	//	�����Ŋp�x�����炷

			if (Top_Angle > 180)Top_Angle -= 360;
			if (Top_Angle < -180)Top_Angle += 360;

			Top_Radian = Top_Angle * (DX_PI_F / 180);	//	���W�A���ɕϊ�
		}

		Walk_Cnt++;	//	�J�E���g��i�߂�

		//	�J�E���g�������Ă���Ԃ͈ړ��𑱂���
		if (Walk_Cnt > MOVE_TIMING) {

			//	�ړ��ʌv�Z
			float MoveX = (cosf(Top_Radian)*Speed);
			float MoveY = (sinf(Top_Radian)*Speed);

			//	�ǂɂԂ����ĂȂ���Έړ�������
			if (Judge_Move(Get_Rect(), &Top_X, &Top_Y, MoveX, MoveY)) 
				Eny_Gr_Num = rand() % 3;	//	�����Ă���悤�ɂ݂��邽�߂ɉ摜��ύX
		}
		if (Walk_Cnt > MOVE_TIMING + MOVE_TIME)Walk_Cnt = 0;	//	�J�E���g�������Ă����烊�Z�b�g
	}

	//	�N�H�[�^�r���[�ł̊p�x�����߂�
	Top_End_X = Top_X + (cosf(Top_Radian) * Vision_Distance);	//	�����Ă���������߂邽�߂̍��W
	Top_End_Y = Top_Y + (sinf(Top_Radian) * Vision_Distance);
	Qtr_Calc->Top_Qtr(Top_End_X, Top_End_Y, &Qtr_End_X, &Qtr_End_Y);	//	�N�H�[�^���W�ϊ�
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	
	//	atan2�Ō����Ă�p�x�����߂�
	Qtr_Angle = atan2f((Qtr_Y + QTR_OFFSET_Y) - (Qtr_End_Y + QTR_OFFSET_Y), (Qtr_X + QTR_OFFSET_X) - (Qtr_End_X + QTR_OFFSET_X));
	Qtr_Angle = Qtr_Angle * 180.0f / DX_PI_F;

	Eny_Gr_Num += Change_Gr(Qtr_Angle);	//	�N�H�[�^�r���[��ł̊p�x�ɂ���ĉ摜��ς���

	Camera_X = Ply->Get_Camera_X();	//	�v���C���|�̃J�����Z�b�g
	Camera_Y = Ply->Get_Camera_Y();

	//	�E�B���h�E���ł̍��W���v�Z
	float X = (float)(Qtr_X + QTR_OFFSET_X - Camera_X);	//	���W�v�Z
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);

	//	�E�B���h�E�O�Ȃ�o�^���Ȃ�
	if (X > WINDOW_X + CHIP_SIZE_X * 2 || X < -CHIP_SIZE_X * 2) { return; }
	if (Y > WINDOW_Y + CHIP_SIZE_Y * 2 || Y < -CHIP_SIZE_Y * 2) { return; }

	//	�E�B���h�E���Ȃ�o�^
	My_Draw->Register(X, Y, H_Size_X, H_Size_Y, ENEMY, Eny_Gr[Eny_Gr_Num]);
}

//	���G
bool Enemy_Blue::Search_Player() {

	float Ply_Distance = Calc_Distance(Ply->Get_Rect());	//	�G�ƃv���C���[�̋��������߂�

	float Ply_Radian = atan2f(Ply->Get_Top_Y() - Top_Y, Ply->Get_Top_X() - Top_X);	//	�v���C���[�ւ̃��W�A�������߂�
	float Ply_Angle = Ply_Radian * 180.0f / DX_PI_F;		//	�p�x�ϊ�
	float Ply_Abs_Angle = Calc_Abs_Angle(Top_Angle, Ply_Angle);	//	�G�̌����ƃv���C���[�ւ̊p�x�̐�Βl���v�Z
	
	//	���g�����Ȃ��ꍇ�����Ō��ʂ�Ԃ�
	if (Ply_Clone == NULL) {
		//	�v���C���[�����E���Ȃ�ǉz���ɂ݂��Ă��邩����
		if (Ply_Distance <= Vision_Distance && Ply_Abs_Angle <= Abs_Angle) {
			Set_Target_Name(PLAYER);
			return Judge_Wall(Ply_Angle, Ply_Abs_Angle, Ply_Distance, &Find_Flg);	//	�ǉz���Ȃ�false���Ԃ��Ă���
		}
		//	���E�O�Ȃ� false ��Ԃ�
		else return Find_Flg = false;
	}

	//	���g���łĂ���ꍇ
	float Clone_Distance = Calc_Distance(Ply_Clone->Get_Rect());	//	���g���łĂ��狗��������

	float Clone_Radian = atan2f(Ply_Clone->Get_Top_Y() - Top_Y, Ply_Clone->Get_Top_X() - Top_X);	//	���W�A���v�Z
	float Clone_Angle = Clone_Radian * 180.0f / DX_PI_F;	//	�p�x�ϊ�
	float Clone_Abs_Angle = Calc_Abs_Angle(Top_Angle, Clone_Angle);	//	�G�̌����ƃv���C���[�ւ̊p�x�̐�Βl���v�Z

	//	�v���C���[�ƕ��g�Ƃ��Ɏ��E�O�Ȃ� false ��Ԃ�
	if (Ply_Abs_Angle > Abs_Angle && Clone_Abs_Angle > Abs_Angle) return Find_Flg = false;

	//	�v���C���[�ƕ��g�Ƃ��Ɏ��E���̎�
	if (Ply_Abs_Angle <= Abs_Angle && Clone_Abs_Angle <= Abs_Angle && Ply_Distance <= Vision_Distance && Clone_Distance <= Vision_Distance) {

		//	�߂������^�[�Q�b�g�ɂ��� �v���C���[
		if (Ply_Distance <= Clone_Distance) {
			Set_Target_Name(PLAYER);
			return Judge_Wall(Ply_Angle, Ply_Abs_Angle, Ply_Distance, &Find_Flg);	//	�ǉz���Ȃ�false���Ԃ��Ă���
		}

		else //	���g
			Set_Target_Name(CLONE);
			return Judge_Wall(Clone_Angle, Clone_Abs_Angle, Clone_Distance,&Find_Flg);	//	�ǉz���Ȃ�false���Ԃ��Ă���
	}

	//	���g�݂̂����E���̎�
	if (Ply_Abs_Angle > Abs_Angle && Clone_Abs_Angle <= Abs_Angle && Clone_Distance <= Vision_Distance) {
		Set_Target_Name(CLONE);
		return Judge_Wall(Clone_Angle, Clone_Abs_Angle, Clone_Distance, &Find_Flg);	//	�ǉz���Ȃ�false���Ԃ��Ă���
	}
	return Find_Flg = false;	//	�G���݂���Ȃ����Find_Flg�𗎂Ƃ��ĕԂ�
}

//	�v���C���[�܂��͕��g�ƓG�̋���
float Enemy_Blue::Calc_Distance(RECT rc) {
	float CenterPosX = (float)((rc.left + rc.right) / 2);	//	���S���W�����߂�
	float CenterPosY = (float)((rc.top + rc.bottom) / 2);
	float a = pow(CenterPosX - Top_X, 2);	//	�v���C���[�܂��͕��g�ƓG�̈ʒu�֌W
	float b = pow(CenterPosY - Top_Y, 2);
	return sqrt(a + b);// �v���C���[�܂ł̋���
}

//	��p�x����v���C���[�p�x�ւ̐�Βl�����߂�
float Enemy_Blue::Calc_Abs_Angle(float topangle, float targetangle) {

	//	�ǂ��炩�̊p�x���}�C�i�X�ɂȂ����ꍇ�Ɍv�Z���@��ς���


	if (topangle <= -180 + Abs_Angle && targetangle >= 180 - Abs_Angle)
		return abs(topangle + targetangle);
	
	else if (topangle >= 180 - Abs_Angle && targetangle <= -180 + Abs_Angle)
		return abs(targetangle + topangle);
	
	//	�ǂ��炩���}�C�i�X�łȂ��ꍇ
	else return abs(topangle - targetangle);
}

//	���E�p�x�ɓ������v���C���[���ǉz���ɂ��邩�ǂ���
bool Enemy_Blue::Judge_Wall(float angle,float absangle,float distance,bool *findflg) {
	Target_Angle = angle;			//	���g�ւ̊p�x
	Target_Radian = Target_Angle * (DX_PI_F / 180);	//	���W�A���ϊ�
	Target_Abs_Angle = absangle;	//	��p�x����͈̔�

	//	1�`�b�v���̈ړ���
	float MoveX = (cosf(Target_Radian)*CHIP_SIZE_X);
	float MoveY = (sinf(Target_Radian)*CHIP_SIZE_Y);
	//	�ǂ����Ȃ�ǔ����ė��Ȃ��悤�ɂ���
	for (int i = 1; i <= (int)(Vision_Distance / 40); i++) {
		float a = pow((float)(i*MoveX), 2);
		float b = pow((float)(i*MoveY), 2);
		//	�v���C���[����O�ɕǂ���������ǔ������Ȃ�
		if (distance >= sqrt(a + b) && My_Map->Map_Get_Chip(Top_X + i * MoveX, Top_Y + i * MoveY) >= 7)
			return *findflg = false;	//	�G���݂���Ȃ����Find_Flg�𗎂Ƃ�
	}
	return true;	//	�ǂ��Ȃ����true
}

//	�G�̎��E��`��
void Enemy_Blue::Draw_Ray()
{
	if (Qtr_X - Camera_X <= WINDOW_X + CHIP_SIZE_X * 2 && Qtr_X - Camera_X >= -CHIP_SIZE_X * 2 && Qtr_Y - Camera_Y <= WINDOW_Y + CHIP_SIZE_Y * 2 && Qtr_Y - Camera_Y >= -CHIP_SIZE_Y * 2) {

		//	���E�̍�����Ray����`��
		float Ray_Angle = Top_Angle - Abs_Angle;		//	�p�x�����߂�
		float Ray_Radian = Ray_Angle * (DX_PI_F / 180);	//	���W�A�������߂�
		float MoveRayX = cosf(Ray_Radian) * (Vision_Distance / 10);	//	Ray�̈ړ��ʂ����߂�
		float MoveRayY = sinf(Ray_Radian) * (Vision_Distance / 10);

		//	���E�̎n�_�����߂Ă���
		int X1 = (int)(Qtr_X + QTR_OFFSET_X - Camera_X);	//	�n�_X
		int Y1 = (int)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);	//	�n�_Y
		float QtrRayX, QtrRayY;	//	Ray�̃N�H�[�^�r���[���W���v�Z

		//	�ǂɂԂ���܂�Ray��L�΂�
		for (int i = 0; i < Vision_Distance / 10; i++) {
			float RayX = Top_X + (MoveRayX * i);
			float RayY = Top_Y + (MoveRayY * i);
			//	�n�_����I�_�̈ʒu�֌W
			float a = pow(RayX - Top_X, 2);
			float b = pow(RayY - Top_Y, 2);
			float Distance = sqrt(a + b);	// �n�_����I�_�܂ł̋���
			//	Ray���ǂɂԂ��邩��������������Ray��`�悷��
			if (My_Map->Map_Get_Chip(RayX, RayY) != 4 || Distance >= Vision_Distance) {
				Qtr_Calc->Top_Qtr(RayX, RayY, &QtrRayX, &QtrRayY);
				int X2 = (int)(QtrRayX + QTR_OFFSET_X - Camera_X);
				int Y2 = (int)(QtrRayY + QTR_OFFSET_Y - Camera_Y);
				DrawLine(X1, Y1, X2, Y2, COLOR[WHITE]);	//	�n�_����I�_�����Ԑ���`��
				break;
			}
		}

		//	�E��
		Ray_Angle = Top_Angle + Abs_Angle;
		Ray_Radian = Ray_Angle * (DX_PI_F / 180);
		MoveRayX = cosf(Ray_Radian) * (Vision_Distance / 10);
		MoveRayY = sinf(Ray_Radian) * (Vision_Distance / 10);

		for (int i = 0; i < Vision_Distance / 10; i++) {
			float RayX = Top_X + (MoveRayX * i);
			float RayY = Top_Y + (MoveRayY * i);
			float a = pow(RayX - Top_X, 2);
			float b = pow(RayY - Top_Y, 2);
			float Distance = sqrt(a + b);

			if (My_Map->Map_Get_Chip(RayX, RayY) != 4 || Distance >= Vision_Distance) {
				Qtr_Calc->Top_Qtr(RayX, RayY, &QtrRayX, &QtrRayY);
				int X2 = (int)(QtrRayX + QTR_OFFSET_X - Camera_X);
				int Y2 = (int)(QtrRayY + QTR_OFFSET_Y - Camera_Y);
				DrawLine(X1, Y1, X2, Y2, COLOR[WHITE]);
				break;
			}
		}
	}
}


//	Enemy_Boss
Enemy_Boss::Enemy_Boss(float argx, float argy,int graph[],int gaugegr[])
{
	Top_X = argx;	//	���W�Z�b�g
	Top_Y = argy;
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	�N�H�[�^���W�ϊ�
	Camera_X = Ply->Get_Camera_X();	//	�J�����Z�b�g
	Camera_Y = Ply->Get_Camera_Y();
	Top_Angle = 0;					//	�p�x������
	Speed = ENEMY_SPEED;			//	�X�s�[�h�Z�b�g
	Size_X = ENY_SIZE_X; Size_Y = ENY_SIZE_Y;		//	�T�C�Y�Z�b�g
	H_Size_X = Size_X / 2; H_Size_Y = Size_Y / 2;	//	�����̃T�C�Y�Z�b�g
	for (int i = 0; i < ENEMY_GR_NUM; i++) { Eny_Gr[i] = graph[i]; }	//	�摜�����ǂݍ���
	Eny_Gr_Num = 0;	//	�摜�i���o�[������
	Cnt_Angle = 0;	//	�p�x��ς���J�E���g������

	//	�K���ȕ���������
	Top_Angle = (float)(GetRand(180));
	if (GetRand(1) != 0)Top_Angle *= -1;		//	�p�x��ݒ肵��
	Top_Radian = Top_Angle * (DX_PI_F / 180);	//	���W�A���ɕϊ�

	Hp_Flg = Move_Flg_X = Move_Flg_Y = false;	//	�t���O�𗎂Ƃ�
	Damage_Cnt = Dying_Cnt = 0;	//	��e�A���S���ԏ�����
	Status.On(NORMAL);			//	�X�e�[�^�X���m�[�}����

	//	Hp��������Hp�摜�ǂݍ���
	Gauge_Bar[HP] = new Gauge(ENY_BOSS_HP_MAX, 0);	//	HP������
	Gauge_Gr[HP] = gaugegr[HP];
	Gauge_Gr[FRAME] = gaugegr[FRAME];
}

//	�f�X�g���N�^
Enemy_Boss::~Enemy_Boss() {}

//	����
void Enemy_Boss::Move()
{
	//	�v���C���[�̍��W�𓾂�
	float Ply_X = Ply->Get_Top_X();
	float Ply_Y = Ply->Get_Top_Y();

	//	�v���C���[�����F���Ă�Βǔ�����
	if (Move_Flg_X&&Move_Flg_Y) {

		//	�v���C���[�̔��΂����������
		Top_Angle = atan2f(Ply->Get_Top_Y() - Top_Y, Ply->Get_Top_X() - Top_X);
		Top_Angle *= -1;

		//	���E����`��摜��ς���̂ŕK������
		Vision_X = cosf(Top_Angle)*((CHIP_SIZE_X) * 5);
		Vision_Y = sinf(Top_Angle)*((CHIP_SIZE_Y) * 5);

		//	�ړ��ʌv�Z
		float Move_X = cosf(Top_Angle)*Speed;
		float Move_Y = sinf(Top_Angle)*Speed;

		//	�ړ���̃}�b�v�`�b�v�ԍ��ňړ������邩���߂� X��Y�ł킯��
		if (My_Map->Map_Get_Chip(Top_X + Move_X, Top_Y) < 7) {
			if (My_Map->Map_Get_Chip(Top_X + Move_X, Top_Y) >= 0) {
				Top_X += Move_X;
			}
		}
		if (My_Map->Map_Get_Chip(Top_X, Top_Y + Move_Y) < 7) {
			if (My_Map->Map_Get_Chip(Top_X, Top_Y + Move_Y) >= 0) {
				Top_Y += Move_Y;
			}
		}

		//	�v���C���[���ǐՏo���鋗���ɑ��݂��邩
		//	X�������Ƀv���C���[�����邩�ǂ���
		if (Top_X + Vision_X > Top_X) {
			if (Top_X + Vision_X >= Ply_X && Ply_X >= Top_X) {}
			else Move_Flg_X = false;
		}
		//	��ɓ���
		else if (Top_X + Vision_X <= Top_X) {
			if (Top_X + Vision_X <= Ply_X && Ply_X <= Top_X) {}
			else Move_Flg_X = false;
		}
		//	Y�������Ƀv���C���[�����邩�ǂ���
		if (Top_Y + Vision_Y > Top_Y) {
			if (Top_Y + Vision_Y >= Ply_Y && Ply_Y >= Top_Y) {}
			else Move_Flg_Y = false;
		}
		else if (Top_Y + Vision_Y <= Top_Y) {
			if (Top_Y + Vision_Y <= Ply_Y && Ply_Y <= Top_Y) {}
			else Move_Flg_Y = false;
		}

		//	�t���O������false�łȂ��ƒǐՂ𑱂���
		if (!Move_Flg_X && !Move_Flg_Y) {}
		else Move_Flg_X = Move_Flg_Y = true;
	}

	//	���G �����Ă��������3�`�b�v����G�̎��E�Ƃ��� 
	else {
		Move_Flg_X = Move_Flg_Y = false;

		Vision_X = cosf(Top_Angle)*((CHIP_SIZE_X) * 3);
		Vision_Y = sinf(Top_Angle)*((CHIP_SIZE_Y) * 3);

		//	X�������Ƀv���C���[�����邩�ǂ���
		if (Top_X + Vision_X > Top_X) {
			if (Top_X + Vision_X >= Ply_X && Ply_X >= Top_X) {
				Move_Flg_X = true;
			}
		}
		//	��ɓ���
		else if (Top_X + Vision_X <= Top_X) {
			if (Top_X + Vision_X <= Ply_X && Ply_X <= Top_X) {
				Move_Flg_X = true;
			}
		}
		//	Y�������Ƀv���C���[�����邩�ǂ���
		if (Top_Y + Vision_Y > Top_Y) {
			if (Top_Y + Vision_Y >= Ply_Y && Ply_Y >= Top_Y) {
				Move_Flg_Y = true;
			}
		}
		else if (Top_Y + Vision_Y <= Top_Y) {
			if (Top_Y + Vision_Y <= Ply_Y && Ply_Y <= Top_Y) {
				Move_Flg_Y = true;
			}
		}
		Cnt_Angle++;
		//	10�t���[����������K���ȕ���������
		if (Cnt_Angle > 20* 60) {
			Top_Angle = (float)(rand() % 180);
			Cnt_Angle = 0;
			if (rand() % 2 != 0)
				Top_Angle *= -1;
		}

		//	�v���C���[�������瓮��
		if (!Move_Flg_X && !Move_Flg_Y&&Cnt_Angle > 25 * 60) {

			float Move_X = cosf(Top_Angle)*Speed;
			float Move_Y = sinf(Top_Angle)*Speed;
			RECT rc = Get_Rect();
			if (((My_Map->Map_Get_Chip((float)rc.left + Move_X, (float)rc.top) < 7) && (My_Map->Map_Get_Chip((float)rc.left + Move_X, (float)rc.top) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.left + Move_X, (float)rc.bottom) < 7) && (My_Map->Map_Get_Chip((float)rc.left + Move_X, (float)rc.bottom) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.right + Move_X, (float)rc.top) < 7) && (My_Map->Map_Get_Chip((float)rc.right + Move_X, (float)rc.top) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.right + Move_X, (float)rc.bottom) < 7) && (My_Map->Map_Get_Chip((float)rc.right + Move_X, (float)rc.bottom) >= 0)))
				Top_X += Move_X;

			if (((My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + Move_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + Move_Y) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + Move_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + Move_Y) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + Move_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + Move_Y) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + Move_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + Move_Y) >= 0)))
				Top_Y += Move_Y;
		}

		//	���W�̌��E�𒴂��Ă��Ȃ���
		if (Top_X < 0)Top_X = 0;
		if (Top_Y < 0)Top_Y = 0;
		if (Top_X > MAP_X*CHIP_SIZE_X)Top_X = (float)MAP_X * CHIP_SIZE_X;
		if (Top_Y > MAP_Y*CHIP_SIZE_Y)Top_Y = (float)MAP_Y * CHIP_SIZE_Y;
	}

	//	�g�b�v���W����N�H�[�^���W��
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	Qtr_Calc->Top_Qtr(Top_X + Vision_X, Top_Y + Vision_Y, &Qtr_Vision_X, &Qtr_Vision_Y);

	Qtr_Angle = atan2f((Qtr_Y + QTR_OFFSET_Y) - (Qtr_Vision_Y + QTR_OFFSET_Y), (Qtr_X + QTR_OFFSET_X) - (Qtr_Vision_X + QTR_OFFSET_X));
	Qtr_Angle = Qtr_Angle * 180.0f / DX_PI_F;

	Eny_Gr_Num = 0;
	//	�N�H�[�^�r���[��ł̊p�x�ɂ���ĉ摜��ς���
	if (Qtr_Angle < 175 && Qtr_Angle>95)Eny_Gr_Num += 21;				//	���΂߉�
	else if (Qtr_Angle <= 95 && Qtr_Angle >= 85)Eny_Gr_Num += 18;		//	����
	else if (Qtr_Angle < 85 && Qtr_Angle>5)Eny_Gr_Num += 15;			//	�E�΂߉�
	else if (Qtr_Angle >= 175 || Qtr_Angle <= -175)Eny_Gr_Num += 12;	//	����
	else if (Qtr_Angle <= 5 && Qtr_Angle >= -5)Eny_Gr_Num += 6;			//	�E��
	else if (Qtr_Angle > -175 && Qtr_Angle < -95)Eny_Gr_Num += 9;		//	���΂ߏ�
	else if (Qtr_Angle >= -95 && Qtr_Angle <= -85)Eny_Gr_Num += 0;		//	������
	else if (Qtr_Angle > -85 && Qtr_Angle < -5)Eny_Gr_Num += 3;			//	�E�΂ߏ�

	Camera_X = Ply->Get_Camera_X();
	Camera_Y = Ply->Get_Camera_Y();
	
	//	�`��e�[�u���ɓo�^
	float X = (float)(Qtr_X + QTR_OFFSET_X - Camera_X);
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);
	//	�E�B���h�E���Ȃ�o�^
	if (X <= WINDOW_X && X >= 0 && Y <= WINDOW_Y && Y >= 0) {
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, ENEMY, Eny_Gr[Eny_Gr_Num]);
	}
}

//	�X�e�[�^�X�̍X�V
void Enemy_Boss::Status_Update()
{
	//	�X�e�[�^�X���_���[�W�ꍇ
	if (Status.Is(DAMAGE)) {
		//	�J�E���g���O�łȂ���΃f�N�������g
		if (Damage_Cnt > 0) {
			Damage_Cnt--;
		}
		//	�O�ȉ��Ȃ�X�e�[�^�X���m�[�}���ɂ���
		else {
			Status.Reset();
			Status.On(NORMAL);
		}
	}
	//	����ł���ꍇ
	else if (Status.Is(DYING)) {
		//	�J�E���g���O�łȂ���΃f�N�������g
		if (Dying_Cnt > 0) {
			Dying_Cnt--;
		}
		//	�O�ȉ��Ȃ�X�e�[�^�X�����S�ɂ���
		else {
			Status.Reset();
			Status.On(DIE);
			Ply->Set_Clear_Flg(true);
		}

		//	�������Ԃ𒴂��Ă�����N���A�ɂ���
		if (My_Time->Get_Time_Limit() < 0) {
			Ply->Set_Clear_Flg(true);
		}
	}
}