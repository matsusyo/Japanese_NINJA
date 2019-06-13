//	 全ての爆弾を管理する

#include"BombManager.h"

extern Input_Controll *Inp_Ctrl;		//	キー入力
extern Quarter_Calculation *Qtr_Calc;	//	座標計算

//	コンストラクタ
Bomb_Manager::Bomb_Manager() 
{
	//	グラフィックの読み込み
	LoadDivGraph("image/Bomb/Bombs.png", 11, 6, 2, 32, 32, Gr);

	//	テーブル内の全てのフラグを落とす
	for (int i = 0; i < BOMB_NUM; i++)Bomb_Tbl[i].Set_Flg(false);	
}

//	デストラクタ
Bomb_Manager::~Bomb_Manager() {}

//	爆弾をテーブルに登録する
void Bomb_Manager::Register(float Top_X, float Top_Y)
{
	//	使用されているところはスキップ
	for (int i = 0; i < BOMB_NUM; i++) {

		if (Bomb_Tbl[i].Is_Flg()) continue;

		//	登録
		Bomb_Tbl[i].Set_Flg(true);	//	フラグを立てる
		Bomb_Tbl[i].Set_Explosion_Flg(false);		//	爆発フラグを落とす
		Bomb_Tbl[i].Set_Top_XY(Top_X, Top_Y);		//	座標を設定
		Bomb_Tbl[i].Set_Count(BOMB_EXPLOSION_TIME);	//	タイムリミットを設定
		Bomb_Tbl[i].Set_Size((float)BOMB_SIZE_X, (float)BOMB_SIZE_Y);	//	サイズを設定
	
		//	11個の画像をすべて読み込ませる
		for (int n = 0; n < BOMB_GRAPH; n++) { Bomb_Tbl[i].Set_Graph_Handle(n, Gr[n]); }

		break;	// ひとつ登録したら抜ける
	}
}

//	フレームごとの挙動
void Bomb_Manager::Move() 
{
	//	テーブルに登録された爆弾の挙動
	for (int i = 0; i < BOMB_NUM; i++) { Bomb_Tbl[i].Move(); }
}

//	敵との当たり判定
void Bomb_Manager::Judge_Enemy()
{
	//	爆弾ごとに判定
	for (int i = 0; i < BOMB_NUM; i++) { Bomb_Tbl[i].Judge_Enemy(); }
}

//	プレイヤーとの当たり判定 
bool Bomb_Manager::Judge_Player(RECT rc)
{
	bool Hit = false;
	for (int i = 0; i < BOMB_NUM; i++) {

		//	登録されていない場所はスキップ
		if (!Bomb_Tbl[i].Is_Flg()) continue;

		//	爆発した時にrcが接触してたらtrueを返す
		if (Bomb_Tbl[i].Is_Explosion_Flg() && Bomb_Tbl[i].Collision(rc)) {
			return true;
		}
	}
	return false;
}