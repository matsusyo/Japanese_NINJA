#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"SoundManager.h"
#include"Map.h"
#include"QtrDraw.h"

class Clear
{
private:
	int Cnt;					//	�J�E���^�[		
	int Font_Handle[FONT_MAX];	//	�t�H���g�n���h��
	double ClearAlpha;  // CLEAR�摜�����x
	bool ClearAlphaType;  // �����x�グ�邩�����邩

public:
	Clear();				//	�R���X�g���N�^
	~Clear();				//	�f�X�g���N�^
	int Update();			//	�V�ѕ�������ʒ��ɌĂ΂��
	void Load_Font();		//	�t�H���g�ǂݍ���
	void Delete_Font();		//	�t�H���g�폜
	void Draw_BackGr();		//	�w�i�`��
	void Draw_Title();		//	���o���`��
	void Draw_Text();		//	�A�C�R�����̃e�L�X�g�`��
};