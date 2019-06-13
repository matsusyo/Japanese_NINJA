#pragma once
#include<math.h>
#include"DxLib.h"
#include"Literal.h"
#include "InputCtrl.h"
#include"SoundManager.h"

const int TITLE_X = 60;	//	���o���̍��W
const int TITLE_Y = 110;
const int TEXT_X = 180;	//	�e�L�X�g�̍��W
const int TEXT_Y = 380;

class Title
{
private:
	int Back_Gr;				//	�w�i�摜
	int Cnt;					//	�J�E���^�[
	int Font_Handle[FONT_MAX];	//	�t�H���g��

public:
	Title();			//	�R���X�g���N�^
	~Title();			//	�f�X�g���N�^
	int Update();		//	�^�C�g����ʒ��Ă�
	void Load_Font();	//	�t�H���g�ǂݍ���
	void Delete_Font();	//	�t�H���g�폜
	void Load_Graph();	//	�摜�ǂݍ���
	void Delete_Graph();//	�摜�폜
	void Draw_Back_Gr();//	�w�i�`��
	void Draw_Title();	//	���o���`��
};
