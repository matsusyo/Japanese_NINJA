//	爆弾1つ1つの処理

#include"Bomb.h"

extern Player *Ply;						//	プレイヤー
extern Enemy_Manager *Eny;				//	敵
extern Map *My_Map;						//	マップ
extern Quarter_Calculation *Qtr_Calc;	//	座標計算
extern Qtr_Draw_Manager *My_Draw;		//	描画テーブル

//	コンストラクタ
Bomb::Bomb() {}
//	デストラクタ
Bomb::~Bomb() {}

//	爆弾の挙動
void Bomb::Move() {
	if (!Flg) return;	//	フラグの立っていなければ終わり

	Count--;	//	制限時間を進める

	//	クォーター座標に変換
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);

	//	カメラ位置を得る
	float CameraX = Ply->Get_Camera_X();	
	float CameraY = Ply->Get_Camera_Y();

	//	描画テーブルに登録する座標を計算
	float X = (float)(Qtr_X + QTR_OFFSET_X - CameraX);	
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - CameraY);

	//	爆弾を大きさや色をカウントによって変える
	//	爆弾が存在できるまで5秒前
	if (4 * 60 < Count) {
		//	描画テーブルに登録
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[0]);	
		return;
	}

	//	4秒前
	if (3 * 60 < Count) {
		//	描画テーブルに登録 画像をランダムに変えて爆弾らしさをだす 
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[rand() % 5]);
		return;
	}

	//	3秒前
	if (2 * 60 < Count) {
		//	描画テーブルに登録
		Spread_Size_X(EXPLOSION_SPREAD_SIZE_X);	//	爆弾のサイズを広げる
		Spread_Size_Y(EXPLOSION_SPREAD_SIZE_Y);
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[rand() % 5]);	//	登録
		return;
	}

	//	2秒前
	if (1 * 60 < Count) {
		//	描画テーブルに登録
		Spread_Size_X(EXPLOSION_SPREAD_SIZE_X);	//	爆弾のサイズを広げる
		Spread_Size_Y(EXPLOSION_SPREAD_SIZE_Y);
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[rand() % 7]);	//	登録
		return;
	}

	//	1秒前
	if (0.5 * 60 < Count) {
		//	描画テーブルに登録　爆発寸前な画像にチェンジ
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[rand() % 3 + 5]);
		return;
	}

	//	0.5秒前	爆発する
	if (0.5 * 60 >= Count) {
		
		//	爆発の初めに音を鳴らす
		if (!Explosion_Flg) {
			My_Sound->Play_Se(SE_BOMB_SPARK);	//	爆発音
			Explosion_Flg = true;	//	爆発中にする
		}
		//	描画テーブルに登録
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, BOMB, Graph[9]);
		return;
	}
}

//	敵やマップとの当たり判定
void Bomb::Judge_Enemy()
{
	//	爆発中は判定
	if (!Explosion_Flg) return;

	//	敵に当たっているか判定
	RECT rc = Get_Rect();
	Eny->Judge_Player_Attack(rc, BOMB_DAMAGE, BOMB);

	//	爆発終わったら爆弾を消す
	if (0 > Count) {
		Top_X = Top_Y = NULL;
		Flg = false;
	}
}