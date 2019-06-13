#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"Map.h"
#include"InputCtrl.h"
#include"SoundManager.h"

const int STAGE_GR_NUM = 6;	//	�X�e�[�W�摜�̃O���t�B�b�N��

class Stage
{
private:
	int Cnt;					//	�J�E���^�[
	int Select_Stage;				//	�X�e�[�W��
	int StageGr[STAGE_GR_NUM];	//	�e�X�e�[�W�̃O���t�B�b�N��
	int FontHandle[FONT_MAX];	//	�t�H���g

public:
	Stage();			//	�R���X�g���N�^
	~Stage();			//	�f�X�g���N�^
	int Update();		//	�X�e�[�W��ʒ��Ă�
	void Load_Font();	//	�t�H���g�ǂݍ���
	void Load_Graph();	//	�摜�ǂݍ���
	void Delete_Font();	//	�t�H���g�폜
	void Delete_Graph();//	�摜�폜
	void Draw_Title();	//	���o���`��
	void Draw_Stage();	//	�e�X�e�[�W�摜�`��
	int Draw_Target_Stage(float, float);	//	�}�E�X�ʒu�̃X�e�[�W�摜�`��
	int Get_Select_Stage() { return Select_Stage; }	//	���݂̃X�e�[�W���Q�b�g
	void Set_Select_Stage(int arg) { Select_Stage = arg; }	//	�X�e�[�W���Z�b�g
};