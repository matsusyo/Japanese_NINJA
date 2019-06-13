#include"Map.h"

//	�I�u�W�F�N�g�̃|�C���^
extern Player *Ply;						//	�v���C���[
extern Quarter_Calculation *Qtr_Calc;	//	���W�v�Z
extern Qtr_Draw_Manager *My_Draw;		//	�`��e�[�u��


//	�R���X�g���N�^
Map::Map() {};
Map::Map(int stage)
{
	Size_X = (float)CHIP_SIZE_X;
	Size_Y = (float)CHIP_SIZE_Y;
	H_Size_X = Size_X / 2;
	H_Size_Y = Size_Y / 2;
	FILE* Fp;	//	�t�@�C���̃|�C���^��錾
	errno_t Err;

	//	�}�b�v��txt�t�@�C������ǂݍ���
	switch (stage) {
	default:
	case AAA:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_1.csv", "r");//Fp��ǂݎ��`���ŊJ��
		break;
	case BBB:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_2.csv", "r");//Fp��ǂݎ��`���ŊJ��
		break;
	case CCC:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_3.csv", "r");//Fp��ǂݎ��`���ŊJ��
		break;
	case DDD:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_4.csv", "r");//Fp��ǂݎ��`���ŊJ��
		break;
	case EEE:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_5.csv", "r");//Fp��ǂݎ��`���ŊJ��
		break;
	case FFF:
		LoadDivGraph("Image/Map/MainChip.png", 16, 4, 4, 64, 64, Chip);
		Err = fopen_s(&Fp, "Data/map1_6.csv", "r");//Fp��ǂݎ��`���ŊJ��
		break;
	}

	if (Fp == NULL)DebugBreak();
	int c;			//	�������i�[����ϐ�
	int y = 0;
	int x = 0;
	char buf[10];	//	��������i�[����
	memset(buf, 0, sizeof(buf));
	bool Flg = false;
	while (1) {
		while (1) {
			c = fgetc(Fp);	//	�����ǂ��c�Ɋi�[
			if (c == EOF) { Flg = true;	break; }	

			//	c���Z���̋�؂肩���s�łȂ���΁Abuf�ɘA������
			if (c != ',' && c != '\n') { strcat_s(buf, (const char*)&c); }
			
			//	��؂肩���s�Ȃ烋�[�v�𔲂���
			else {
				Chip_Num = atoi(buf);	//	buf��int�^�ɂ��āAChip_Num�ɑ��
				Map_Data[y][x] = Chip_Num;
				memset(buf, 0, sizeof(buf));	//	buf�����Z�b�g
				break;
			}
		}

		//	1�Z�����̃��[�v�𔲂�����
		if (Flg)break;					//	�t�@�C����S�ēǂݍ��ݏI������break
		if (c == ',')x++;				//	�ǂݍ��񂾕������J���}�Ȃ�1�s�Z�����ړ�
		if (c == '\n') { y++; x = 0; }	//	���s�Ȃ�Z����0�s�ڂɂ���1��Z�����ړ�
	}
	fclose(Fp);
}

//	�f�X�g���N�^
Map::~Map(){}

//	�}�b�v��`��e�[�u���ɓo�^
void Map::Map_Register()
{
	//	�J�����ʒu
	Camera_X = Ply->Get_Camera_X();
	Camera_Y = Ply->Get_Camera_Y();

	//	�}�b�v��`��e�[�u���ɓo�^ X=MAP_X
	for (int y = 0; y < MAP_Y; y++)
		for (int x = MAP_X - 1; x >= 0; x--) {
			Qtr_X = (float)((x + y) * (CHIP_SIZE_X / 2));
			Qtr_Y = (float)((-x + y) * (CHIP_SIZE_Y / 4));
			float X = (float)(Qtr_X + H_Size_X - Camera_X);
			float Y = (float)(Qtr_Y + H_Size_Y - Camera_Y);
			//	�E�B���h�E�Ƀ}�b�v�����肫�镪�����o�^
			if (X <= WINDOW_X + CHIP_SIZE_X * 2 && X >= -CHIP_SIZE_X * 2 && Y <= WINDOW_Y + CHIP_SIZE_Y * 2 && Y >= -CHIP_SIZE_Y * 2 && Map_Data[y][x] >= 7)
				My_Draw->Register(X, Y, H_Size_X, H_Size_Y, MAP, Chip[Map_Data[y][x]]);
		}
}

//	�ړ��ł���}�b�v�`�b�v�����`�悷��
void Map::Tile_Draw()
{
	//	�J�����ʒu
	Camera_X = Ply->Get_Camera_X();
	Camera_Y = Ply->Get_Camera_Y();
	//	�N�H�[�^�[�}�b�v�`��@������`�悷�� X=MAP_X
	for (int y = 0; y < MAP_Y; y++)
		for (int x = MAP_X - 1; x >= 0; x--)
			if (Map_Data[y][x] < 7) {
				Qtr_X = (float)((x + y) * (CHIP_SIZE_X / 2));
				Qtr_Y = (float)((-x + y) * (CHIP_SIZE_Y / 4));
				float X = (float)(Qtr_X - Camera_X);
				float Y = (float)(Qtr_Y - Camera_Y);
				if (X <= WINDOW_X + CHIP_SIZE_X * 2 && X >= -CHIP_SIZE_X * 2 && Y <= WINDOW_Y + CHIP_SIZE_Y * 2 && Y >= -CHIP_SIZE_Y * 2)
					DrawGraph((int)(Qtr_X - Camera_X), (int)(Qtr_Y - Camera_Y), Chip[Map_Data[y][x]], TRUE);
			}
}

//	�g�b�v�r���[���W����}�b�v�`�b�v�̒l���擾����
int Map::Map_Get_Chip(const float &topx, const float &topy)
{	
	//	�ϊ����ĕԂ�
	Qtr_Calc->Top_TopChip(topx, topy, &Chip_X, &Chip_Y);
	return Map_Data[Chip_Y][Chip_X];
}