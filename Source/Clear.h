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
	int Cnt;					//	カウンター		
	int Font_Handle[FONT_MAX];	//	フォントハンドル
	double ClearAlpha;  // CLEAR画像透明度
	bool ClearAlphaType;  // 透明度上げるか下げるか

public:
	Clear();				//	コンストラクタ
	~Clear();				//	デストラクタ
	int Update();			//	遊び方説明画面中に呼ばれる
	void Load_Font();		//	フォント読み込み
	void Delete_Font();		//	フォント削除
	void Draw_BackGr();		//	背景描画
	void Draw_Title();		//	見出し描画
	void Draw_Text();		//	アイコン下のテキスト描画
};