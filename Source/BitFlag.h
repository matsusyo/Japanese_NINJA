#pragma once

//	BitFlagクラス ビット管理の本体
class BitFlag
{
private:
	int Flg;

public:
	void Reset() { Flg = 0; }			//	フラグリセット 0にするだけ
	void On(int flg) { Flg |= flg; }	//	BITフラグON	 論理和をとる
	void Off(int flg) { Flg &= ~flg; }	//	BITフラグOFF 反転させたBITと論理積とる
	int Get() { return Flg; }			//	現在のフラグを返す
	bool Is(int flg) { return (Flg == flg); }			//	指定されたフラグと一致する？ 完全一致なら比較する
	bool Is_Either(int flg) { return (Flg & flg) != 0; }//	指定されたフラグのいずれかが一致する？ 論理積の結果が0でなければ一致
};

