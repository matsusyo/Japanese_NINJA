#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"Player.h"
#include"QtrCalc.h"

//	�X�e�[�W��
const int STAGE_NUM = 6;
const int CHIP_SIZE_X = 64;
const int CHIP_SIZE_Y = 64;
//
//const char* STAGE_LIST[STAGE_NUM + 1]={
//	"",
//	"Data/map1_1.csv",
//	"Data/map1_2.csv",
//	"Data/map1_3.csv",
//	"Data/map1_4.csv",
//	"Data/map1_5.csv",
//	"Data/map1_6.csv"
//};

const enum STAGE_NAME {
	NO_STAGE, AAA, BBB, CCC, DDD, EEE, FFF
};


class Map {
		
	int Map_Data[MAP_Y][MAP_X];	//	�}�b�v�f�[�^
	int	Chip_X, Chip_Y;			//	�}�b�v�`�b�v�ԍ� X,Y
	int Chip[16];				//	�}�b�v�`�b�v�� �P�U��
	int	Chip_Num;				//	�}�b�v�`�b�v�̕ϐ�

	float Qtr_X, Qtr_Y;			//	�N�H�[�^���W
	float Size_X, Size_Y;		//	�}�b�v�T�C�Y
	float H_Size_X, H_Size_Y;	//	�}�b�v�T�C�Y�̔���
	float Camera_X, Camera_Y;	//	�J�����ʒu

public:
	
	Map();		//	�R���X�g���N�^
	Map(int stage);	//	�������̃R���X�g���N�^���g�p
	~Map();		//	�f�X�g���N�^

	//	�}�b�v�̏������`��
	void Tile_Draw();

	//	�}�b�v�̒ʂ�Ȃ�������`��N���X�ɓo�^
	void Map_Register();		

	//	�}�b�v�`�b�v�̒l���擾����֐�
	int Map_Get_Chip(const float &topx,const float &topy);
};