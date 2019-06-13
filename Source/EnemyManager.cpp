//	�S�Ă̓G�̊Ǘ����s���֐�

#include"EnemyManager.h"
extern Map *My_Map;
extern Effect_Manager *My_Effect;
extern Sound_Manager *My_Sound;

//	�R���X�g���N�^
Enemy_Manager::Enemy_Manager()
{
	for (int i = 0; i < ENY_NUM; i++) { Eny_Tbl[i] = NULL;}
	LoadDivGraph("Image/Enemy/Heishi.png", ENEMY_BLUE_GR_NUM, 6, 4, 32, 48, Enemy_Blue_Graph);	//	�G���G�̉摜
	LoadDivGraph("Image/Enemy/king.png", ENEMY_BOSS_GR_NUM, 6, 4, 40, 60, Enemy_Boss_Graph);	//	�{�X�̉摜
	Blue_Gauge_Graph[HP] = LoadGraph("Image/Gauge/GaugeEny.png");		//	�G���G��Hp�摜�ǂݍ���
	Blue_Gauge_Graph[FRAME] = LoadGraph("Image/Gauge/GaugeFrame.png");	//	Hp�̊O�g�摜��ǂݍ���
	Boss_Gauge_Graph[HP]= LoadGraph("Image/Gauge/GaugeBoss.png");		//	�{�X��Hp�摜�ǂݍ���
	Boss_Gauge_Graph[FRAME] = Blue_Gauge_Graph[FRAME];					//	�O�g�摜���G���G�̂���ǂݍ���
	Register_Enemy();
}

//	�f�X�g���N�^
Enemy_Manager::~Enemy_Manager()
{
	//	��ʏ�Ɏc���Ă���G���폜����
	for (int i = 0; i < ENY_NUM; i++) {
		if (Eny_Tbl[i] != NULL) delete Eny_Tbl[i];
	}
}

//	�X�V
void Enemy_Manager::Move()
{
	for (int i = 0; i < ENY_NUM; i++) {

		if (Eny_Tbl[i] == NULL) continue;	// �󂢂Ă�e�[�u���̓X�L�b�v����

		Eny_Tbl[i]->Move();	//	����

		//	�G�����񂾏�ԂȂ�폜
		if (Eny_Tbl[i]->Status.Is(DIE)) {
			delete Eny_Tbl[i];
			Eny_Tbl[i] = NULL;
		}
	}
}

//	�v���C���[�̍U���ɑ΂���G�̓����蔻�� �藠���Ɣ��e
bool Enemy_Manager::Judge_Player_Attack(RECT rc, float Dmg,int Type)
{
	bool Hit = false;	//	������̓G�Ƃ��������Ă��Ȃ�

	//	�����ꂩ�̓G�Ɠ������Ă��邩���ׂ�
	for (int i = 0; i < ENY_NUM; i++) {

		if (Eny_Tbl[i] == NULL) continue;	//	�G���o�^����Ă��Ȃ��ꏊ�̓p�X
		if (!Eny_Tbl[i]->Status.Is(NORMAL)) continue;	//	��Ԃ�NORMA�łȂ��Ȃ�_���[�W�v�Z���Ȃ�
		if (!Eny_Tbl[i]->Collision(rc)) continue;	//	rc�ƏՓ˂��Ă��Ȃ���΃_���[�W�v�Z���Ȃ�
		
		Hit = true;	//	��x�ł������ɗ�����return��true��Ԃ�

		//	Type�ɂ���ďՓ� �G�t�F�N�g,����ς���
		float x = (float)(Eny_Tbl[i]->Get_Qtr_X() + QTR_OFFSET_X);	//	�o�^������W���v�Z
		float y = (float)(Eny_Tbl[i]->Get_Qtr_Y() + QTR_OFFSET_Y);
		switch (Type){
		case DART:	//	�藠���̎�
			My_Effect->Register(x, y, EFFECT_DART);	//	�G�t�F�N�g�o�^
			My_Sound->Play_Se(SE_DART_SPARK);	//	����炷
			//	�Ԃ����������Ɍ���
			Eny_Tbl[i]->Set_Top_Radian(atan2f((rc.top + rc.bottom) / 2 - Eny_Tbl[i]->Get_Top_Y(), (rc.left + rc.right) / 2 - Eny_Tbl[i]->Get_Top_X()));
			Eny_Tbl[i]->Set_Top_Angle(Eny_Tbl[i]->Get_Top_Radian() * 180.0f / DX_PI_F);
			break;

		case BOMB:	//	���e�̎�
			My_Effect->Register(x, y, EFFECT_BOMB);	//	�G�t�F�N�g�o�^
			My_Sound->Play_Se(SE_BOMB_SPARK);	//	����炷
			break;

		default:	//	�\������ Type �Ȃ牽�����Ȃ�
			break;
		}

		Eny_Tbl[i]->Gauge_Bar[HP]->Add_Now(Dmg);	//	���������G��HP�����炷
		Eny_Tbl[i]->Status.Reset();			//	�X�e�[�^�X�����Z�b�g����

		//	�G��HP��0�Ȃ玀��ł����Ԃɂ���
		if (Eny_Tbl[i]->Gauge_Bar[HP]->Is_Empty()) {
			Eny_Tbl[i]->Status.On(DYING);	//	����ł��Ԃɂ���
			
			//	BOSS�����񂾂�{�X���S��
			if (Eny_Tbl[i]->Get_Type() == ENEMY_BOSS) {
				Eny_Tbl[i]->Set_Dying_Count(BOSS_DYING_COUNT);	//	�G�����ʂ܂łɂ����鎞��
				My_Sound->Play_Se(SE_ERASE_BOSS);
			}
			
			//	�������G���G���S��
			else {
				Eny_Tbl[i]->Set_Dying_Count(E_DYING_COUNT);	//	�G�����ʂ܂łɂ����鎞��
				My_Sound->Play_Se(SE_ERASE_ENEMY);
			}
		}

		//	HP������΃_���[�W��Ԃɂ���
		else {
			Eny_Tbl[i]->Status.On(DAMAGE);
			Eny_Tbl[i]->Set_Damage_Count(E_DAMAGE_COUNT);
		}
	}

	//	�G�̂ǂꂩ�ɂ������Ă���
	if (Hit) return true;
	else return false;
}

//	�v���C���[�Ƃ̓����蔻��
bool Enemy_Manager::Judge_Player(RECT rc)
{
	bool Hit = false;
	for (int i = 0; i < ENY_NUM; i++) {

		//	�o�^����Ă��Ȃ��ꏊ�̓p�X
		if (Eny_Tbl[i] == NULL) continue;

		//	NORMAL����rc�ƐڐG���Ă���HP�����炷
		if (Eny_Tbl[i]->Status.Is(NORMAL)){
			if (Eny_Tbl[i]->Collision(rc)){
				Hit = true;
			}
		}
		//	�ڐG�����甲����
		if (Hit)break;
	}
	return Hit;
}

//	�X�e�[�^�X�̍X�V�@���t���[���Ă�
void Enemy_Manager::Status_Update()
{
	for (int i = 0; i < ENY_NUM; i++) {
		if (Eny_Tbl[i] == NULL) continue;
		//	�G�̐�������
		Eny_Tbl[i]->Status_Update();
	}
}


//	���E��`��
void Enemy_Manager::Draw_Ray()
{
	for (int i = 0; i < ENY_NUM; i++) {

		if (Eny_Tbl[i] == NULL) continue;	// �󂢂Ă�e�[�u���̓X�L�b�v����

		Eny_Tbl[i]->Draw_Ray();	//	����
	}
}


//	HP��`��
void Enemy_Manager::Draw_Gauge()
{
	for (int i = 0; i < ENY_NUM; i++) {

		if (Eny_Tbl[i] == NULL) continue;	// �󂢂Ă�e�[�u���̓X�L�b�v����

		Eny_Tbl[i]->Draw_Gauge();	//	����
	}
}


//	�藠���̕W�I�����炤
Enemy* Enemy_Manager::Get_Target(float topX,float topY)
{
	int Cnt = 0;	//	�L���ȓG���𐔂���
	Enemy *TargetList[ENY_NUM];	//	�L���ȓG�� �L�^ ����e�[�u��
	
	//	�L���ȓG�Ɣԍ����L�^����
	for (int i = 0; i < ENY_NUM; i++) {
		if (Eny_Tbl[i] == NULL) continue;	//	�e�[�u���ɓG�����Ȃ���΃X�L�b�v
		TargetList[Cnt] = Eny_Tbl[i];
		Cnt++;
	}
	
	for (int i = 0; i < Cnt; i++) {
		RECT rc = TargetList[i]->Get_Rect();
		//	���G���� (�O�����̒藝�ŋ����𔻒�) c2 = a2 + b2; c = ��(a2 + b2)
		float Eny_Center_PosX = (float)(rc.left + (rc.right - rc.left));// ���S���W���o���Ă���
		float Eny_Center_PosY = (float)(rc.top + (rc.bottom - rc.top));
		float a = pow(Eny_Center_PosX - topX, 2);//	�v���C���[�Ǝ����̈ʒu�֌W
		float b = pow(Eny_Center_PosY - topY, 2);
		double c = sqrt(a + b);// c����̋���
		if (c <= 130) { return TargetList[i]; }
	}
	
	//	�G�����݂��Ȃ��ꍇ NULL��Ԃ�
	return NULL;
}

//	�G���G���A���Ƃɐ����@
void Enemy_Manager::Register_Enemy()
{
	float TopX, TopY;	//	����������W
	
	//	�G���ɓ͂��܂œG��o�^
	for (int i = 0; i < ENY_NUM-1; i++) 
		while (Eny_Tbl[i] == NULL) {
			TopX = (float)((CHIP_SIZE_X - 1)* GetRand(MAP_X));	//	�����_���ɍ��W������
			TopY = (float)((CHIP_SIZE_Y - 1)* GetRand(MAP_Y));

			//	�v���C���[�̋߂��ɂ͓o�^���Ȃ�
			if (TopX <= CHIP_SIZE_X * MAP_X / 6 && TopY <= CHIP_SIZE_Y * MAP_Y / 6) {
				continue;
			}

			//	�摜�̊p4�_���ǂ̒�����Ȃ�������o�^
			float Left = TopX - (ENY_SIZE_X / 2);
			float Top = TopY - (ENY_SIZE_Y / 2);
			float Right = TopX + (ENY_SIZE_X / 2);
			float Bottom = TopY + (ENY_SIZE_Y / 2);

			if ((My_Map->Map_Get_Chip(Left, Top) < 7 && My_Map->Map_Get_Chip(Left, Top) >= 0) &&		//	����
				(My_Map->Map_Get_Chip(Left, Bottom) < 7 && My_Map->Map_Get_Chip(Left, Bottom) >= 0) &&	//	����
				(My_Map->Map_Get_Chip(Right, Top) < 7 && My_Map->Map_Get_Chip(Right, Top) >= 0) &&		//	�E��
				(My_Map->Map_Get_Chip(Right, Bottom) < 7 && My_Map->Map_Get_Chip(Right, Bottom) >= 0))	//	�E��
				Eny_Tbl[i] = new Enemy_Blue(TopX, TopY, Enemy_Blue_Graph, Blue_Gauge_Graph);
		}
	
	
	//	�{�X��o�^
	while (Eny_Tbl[ENY_NUM - 1] == NULL) {
		TopX = (float)((CHIP_SIZE_X - 1)* GetRand(MAP_X));
		TopY = (float)((CHIP_SIZE_Y - 1)* GetRand(MAP_Y));
		
		//	�v���C���[�̋߂��ɂ͓o�^���Ȃ�
		if (TopX <= CHIP_SIZE_X * MAP_X / 2 && TopY <= CHIP_SIZE_Y * MAP_Y / 2) {
			continue;
		}

		//	�摜�̊p4�_���ǂ̒�����Ȃ�������o�^
		float Left = TopX - (ENY_SIZE_X / 2);
		float Top = TopY - (ENY_SIZE_Y / 2);
		float Right = TopX + (ENY_SIZE_X / 2);
		float Bottom = TopY + (ENY_SIZE_Y / 2);

		if ((My_Map->Map_Get_Chip(Left, Top) < 7 && My_Map->Map_Get_Chip(Left, Top) >= 0) &&		//	����
			(My_Map->Map_Get_Chip(Left, Bottom) < 7 && My_Map->Map_Get_Chip(Left, Bottom) >= 0) &&	//	����
			(My_Map->Map_Get_Chip(Right, Top) < 7 && My_Map->Map_Get_Chip(Right, Top) >= 0) &&		//	�E��
			(My_Map->Map_Get_Chip(Right, Bottom) < 7 && My_Map->Map_Get_Chip(Right, Bottom) >= 0))	//	�E��
			Eny_Tbl[ENY_NUM - 1] = new Enemy_Boss(TopX, TopY, Enemy_Boss_Graph, Boss_Gauge_Graph);
	}
}