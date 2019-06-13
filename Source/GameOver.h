#pragma once
#include<math.h>
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"SoundManager.h"


class Game_Over
{
private:
	int BackGr;				//	�w�i�摜
	int Cnt;					//	�J�E���^�[
	int Font_Handle[FONT_MAX];	//	�t�H���g��

public:
	Game_Over();		//	�R���X�g���N�^
	~Game_Over();		//	�f�X�g���N�^
	int Update();		//	�Q�[���I�[�o�[��ʒ��Ă�
	void Load_Font();	//	�t�H���g�ǂݍ���
	void Delete_Font();	//	�t�H���g�폜
	void Load_Graph();	//	�摜�ǂݍ���
	void Delete_Graph();//	�摜�폜
	void Draw_BackGr();//	�w�i�`��
	void Draw_Title();	//	���o���`��
	void Draw_Text();	//	�e�L�X�g�`��
};
