#pragma once
#include<iostream>
#include"BitFlag.h"

//	�v���C���[��G�̏�Ԃ̎��
const int
NORMAL = 0x01,	//	00000001	�m�[�}��
DAMAGE = 0x02,	//	00000010	�_���[�W
DYING = 0x04,	//	00000100	����ł��� ���񂾎��̃��[�V������
DIE = 0x08,		//	00001000	���S
ALL = 0xFF;		//	11111111	�S��

//	�X�e�[�^�X�Ǘ����r�b�g�ōs��
class State
{
private:
	BitFlag Flg;	//	��Ԃ��Ǘ�����t���O(8bit�Ȃ̂�8��ނ܂ŊǗ��ł���)

public:
	// �R���X�g���N�^��Flag��0�ɏ�����(�s��l�΍�)
	State() :Flg() {}
	~State() {}


			// ���\�b�h
	void Reset() { Flg.Off(ALL); }						//	��Ԃ����Z�b�g
	void On(int flg) { Flg.On(flg); }					//	�w�肳�ꂽ��Ԃ�ON(�L��)�ɂ���
	void Off(int flg) { Flg.Off(flg); }					//	�w�肳�ꂽ��Ԃ�OFF(����)�ɂ���
	int Get() { return Flg.Get(); }						//	���݂̏�Ԃ�Ⴄ
	bool Is(int flg) { return Flg.Is(flg); }			//	�����ꂽ��Ԃƈ�v����H
	bool Is_Either(int flg) { return Flg.Is_Either(flg); }	//	�����ꂽ��Ԃ̂����ꂩ���Y������H
	bool Is_Healthy() { return Flg.Is(0); }				//	�m�[�}����Ԃ�

};