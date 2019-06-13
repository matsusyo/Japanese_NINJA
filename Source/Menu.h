#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"SoundManager.h"

const int ICON_NUM = 2;		//	�e�A�C�R����

class Menu
{
private:
	int Cnt;					//	�J�E���^�[
	int Font_Handle[FONT_MAX];	//	�t�H���g�n���h��
	int Mission_Gr[ICON_NUM];	//	�X�e�[�W�I����ʃA�C�R��
	int Manual_Gr[ICON_NUM];	//	�V�ѕ�������ʃA�C�R��
	int Title_Gr[ICON_NUM];		//	�^�C�g����ʃA�C�R��
	int Exit_Gr[ICON_NUM];		//	�Q�[���I���A�C�R��

public:

	Menu();					//	�R���X�g���N�^
	~Menu();				//	�f�X�g���N�^
	int Update();			//	�V�ѕ�������ʒ��ɌĂ΂��
	void Load_Font();		//	�t�H���g�ǂݍ���
	void Load_Graph();		//	�摜�ǂݍ���
	void Delete_Font();		//	�t�H���g�폜
	void Delete_Graph();	//	�摜�폜	
	void Draw_Title();		//	���o���`��
	void Draw_Icon();		//	�A�C�R���`��
	void Draw_Icon_Text();	//	�A�C�R�����̃e�L�X�g�`��
	int Draw_Target_Icon(float, float);	//	�}�E�X�ʒu�̃A�C�R����`��
};