#pragma once
#include"DxLib.h"
#include"Literal.h"
#include"InputCtrl.h"
#include"SoundManager.h"

const int MANUAL_GR_NUM = 5;	//	説明画像のグラフィック数

class Manual {
private:
	int Page;		//	説明ページ番号
	int Page_tmp;	//	仮番号
	int Cnt = 0;	//	カウンタ　
	int Back_Gr;		//	背景画像
	int Manual_Gr[MANUAL_GR_NUM];	//	説明画像5個
	int Font_Handle[FONT_MAX];		// フォント作成

public:
	Manual();					//	コンストラクタ
	~Manual();					//	デストラクタ
	int Update();				//	マニュアル画面中に呼ぶ
	void Load_Font();			//	フォント読み込み
	void Delete_Font();			//	フォント削除
	void Load_Graph();			//	グラフィック読み込み
	void Delete_Graph();		//	グラフィック削除
	void Draw_BackGr();			//	背景描画
	void Draw_Title();			//	タイトル見出し描画
	void Draw_Menu_Back(float,float);	//	メニューに戻るアイコンを描画
	void Draw_TeachGr();		//	遊び方を説明する画像を描画
	void Draw_Contents_Select();//	現在の説明画像ページを表す□を赤色で描画
	void Draw_Contents_Out();	//	ページを表す□の外枠をページ数分描画
	void Draw_Contents_Target(float,float);		//	マウスの位置にある□を白色で描画
	void Set_Page(int arg) { Page = arg; }		//	ページ番号セット	
	int Get_Page() { return Page; }				//	ページ番号ゲット
	void Set_Cnt(int arg) { Cnt = arg; }		//	カウントセット
	int Get_Cnt() { return Cnt; }				//	カウントゲット
	int Get_Manual_Gr(int arg) { return Manual_Gr[arg]; }	//	指定したグラフィック番号の画像をゲット
};