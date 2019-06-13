#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"SoundManager.h"

const int MANUAL_GR_NUM = 5;	//	�����摜�̃O���t�B�b�N��

class Manual {
private:
	int Page;		//	�����y�[�W�ԍ�
	int Page_tmp;	//	���ԍ�
	int Cnt = 0;	//	�J�E���^�@
	int Back_Gr;		//	�w�i�摜
	int Manual_Gr[MANUAL_GR_NUM];	//	�����摜5��
	int Font_Handle[FONT_MAX];		// �t�H���g�쐬

public:
	Manual();					//	�R���X�g���N�^
	~Manual();					//	�f�X�g���N�^
	int Update();				//	�}�j���A����ʒ��ɌĂ�
	void Load_Font();			//	�t�H���g�ǂݍ���
	void Delete_Font();			//	�t�H���g�폜
	void Load_Graph();			//	�O���t�B�b�N�ǂݍ���
	void Delete_Graph();		//	�O���t�B�b�N�폜
	void Draw_BackGr();			//	�w�i�`��
	void Draw_Title();			//	�^�C�g�����o���`��
	void Draw_Menu_Back(float,float);	//	���j���[�ɖ߂�A�C�R����`��
	void Draw_TeachGr();		//	�V�ѕ����������摜��`��
	void Draw_Contents_Select();//	���݂̐����摜�y�[�W��\������ԐF�ŕ`��
	void Draw_Contents_Out();	//	�y�[�W��\�����̊O�g���y�[�W�����`��
	void Draw_Contents_Target(float,float);		//	�}�E�X�̈ʒu�ɂ��遠�𔒐F�ŕ`��
	void Set_Page(int arg) { Page = arg; }		//	�y�[�W�ԍ��Z�b�g	
	int Get_Page() { return Page; }				//	�y�[�W�ԍ��Q�b�g
	void Set_Cnt(int arg) { Cnt = arg; }		//	�J�E���g�Z�b�g
	int Get_Cnt() { return Cnt; }				//	�J�E���g�Q�b�g
	int Get_Manual_Gr(int arg) { return Manual_Gr[arg]; }	//	�w�肵���O���t�B�b�N�ԍ��̉摜���Q�b�g
};