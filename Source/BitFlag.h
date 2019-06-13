#pragma once

//	BitFlag�N���X �r�b�g�Ǘ��̖{��
class BitFlag
{
private:
	int Flg;

public:
	void Reset() { Flg = 0; }			//	�t���O���Z�b�g 0�ɂ��邾��
	void On(int flg) { Flg |= flg; }	//	BIT�t���OON	 �_���a���Ƃ�
	void Off(int flg) { Flg &= ~flg; }	//	BIT�t���OOFF ���]������BIT�Ƙ_���ςƂ�
	int Get() { return Flg; }			//	���݂̃t���O��Ԃ�
	bool Is(int flg) { return (Flg == flg); }			//	�w�肳�ꂽ�t���O�ƈ�v����H ���S��v�Ȃ��r����
	bool Is_Either(int flg) { return (Flg & flg) != 0; }//	�w�肳�ꂽ�t���O�̂����ꂩ����v����H �_���ς̌��ʂ�0�łȂ���Έ�v
};

