#include"Player.h"

extern Input_Controll *Inp_Ctrl;		//	キー入力
extern Quarter_Calculation *Qtr_Calc;	//	座標計算
extern Map *My_Map;						//	マップ
extern Qtr_Draw_Manager *My_Draw;		//	描画テーブル
extern Sound_Manager *My_Sound;			//	音
extern Effect_Manager *My_Effect;		//	エフェクト

//	コンストラクタ
Player::Player()
{
	//	初期位置
	Top_X = 140.0f; Top_Y = 500.0f;

	//	クォータビュー上での座標
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	
	//	カメラ位置
	Camera_X = Qtr_X - H_WINDOW_X;Camera_Y = Qtr_Y - H_WINDOW_Y;
	
	//	角度　スピード
	Top_Angle = 0;
	Speed = PLY_DEF_SPEED;
	
	//	サイズ計算
	Size_X = PLY_SIZE_X; Size_Y = PLY_SIZE_Y;
	H_Size_X = Size_X / 2;H_Size_Y = Size_Y / 2;

	//	グラフィック読み込み
	LoadDivGraph("Image/Player/Ninja.png", 24, 6, 4, 32, 50, Ply_Gr);
	Ply_Gr_Num = 0;

	//	ゲージ関連
	//	HP,MP最大値と最小値設定
	Gauge_Bar[HP] = new Gauge(HP_MAX, 0);
	Gauge_Bar[MP] = new Gauge(MP_MAX, 0);
	//	グラフィック読み込み
	Gauge_Gr[HP] = LoadGraph("Image/Gauge/GaugeHp.png");
	Gauge_Gr[MP] = LoadGraph("Image/Gauge/GaugeMP.png");
	Gauge_Gr[FRAME] = LoadGraph("Image/Gauge/GaugeFrame.png");
	
	//	フラグとカウントを初期化
	Mp_Flg = Clear_Flg = false;
	Damage_Cnt = Dying_Cnt = 0;
	
	//	ステータスをノーマルに設定
	Status.On(NORMAL);
}

//	デストラクタ
Player::~Player(){}

////////////	動き
void Player::Move()
{
	//	クォーターキャラ座標計算
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	
	//	カメラ位置を計算
	Camera_X = Qtr_X - (H_WINDOW_X);
	Camera_Y = Qtr_Y - (H_WINDOW_Y);
	if (Camera_X < 0) Camera_X = 0;
	if (Camera_X >(MAP_X*CHIP_SIZE_X))Camera_X = MAP_X * CHIP_SIZE_X;
	if (Camera_Y >(MAP_Y*CHIP_SIZE_Y))Camera_Y = MAP_Y * CHIP_SIZE_Y;

	//	クォータマウス座標獲得
	Inp_Ctrl->Get_Mouse_Pointer(&Qtr_MX, &Qtr_MY);

	//	クォータマウス座標をトップビューマウス座標に変換
	Qtr_Calc->Qtr_Top((Qtr_MX + Camera_X), (Qtr_MY + Camera_Y), &Top_MX, &Top_MY);

	//	マウスカーソルの方向をプレイヤーの角度にする
	Top_Angle = atan2f(Top_MY - Top_Y, Top_MX - Top_X);

	Ply_Gr_Num = 0;

	//	左クリック中移動
	if (Inp_Ctrl->Is_Input(MOUSE_LEFT)) {

		if (rand() % 100 == 0)My_Sound->Play_Se(SE_DASH);

		//	キャラの画像を変える
		Ply_Gr_Num = rand() % 3;

		//	移動量計算
		float MoveX = cosf(Top_Angle)*Speed;
		float MoveY = sinf(Top_Angle)*Speed;

		float x = (float)(Qtr_X + QTR_OFFSET_X);	//	登録する座標を計算
		float y = (float)(Qtr_Y + QTR_OFFSET_Y);

		//	横方向の移動　6点で判定
		RECT rc = Get_Rect();
		if (((My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.top) < 7) && (My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.top) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.bottom) < 7) && (My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.bottom) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.top + H_Size_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.left + MoveX, (float)rc.top + H_Size_Y) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)((rc.top + rc.bottom) / 2)) < 7) && (My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)((rc.top + rc.bottom) / 2)) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)rc.top) < 7) && (My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)rc.top) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)rc.bottom) < 7) && (My_Map->Map_Get_Chip((float)rc.right + MoveX, (float)rc.bottom) >= 0)))
			Top_X += MoveX;
		else {
			My_Sound->Play_Se(SE_COLLISION_WALL);	//	衝突音
			My_Effect->Register(x, y, EFFECT_WALL);			//	衝突エフェクト
			
		}
		//	縦方向の移動 6点で判定
		if (((My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + MoveY) < 7) && (My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + MoveY) < 7) && (My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + MoveY) < 7) && (My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + MoveY) < 7) && (My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + MoveY) < 7) && (My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + MoveY) >= 0)) &&
			((My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + MoveY) < 7) && (My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + MoveY) >= 0)))
			Top_Y += MoveY;
		else {
			My_Sound->Play_Se(SE_COLLISION_WALL);	//	衝突音
			My_Effect->Register(x, y, EFFECT_WALL);			//	衝突エフェクト
		}
	}

	//	押下中でなければMPを自動回復,プレイヤーの画像
	else {
		Ply_Gr_Num = 1;				//	キャラの画像を止まっている状態にする
		if (Inp_Ctrl->Is_Input(KEY_LSHIFT)) {
			Gauge_Bar[MP]->Add_Now(+0.5f);	//	動いていない間はMPを回復
		}
	}

	if (!Gauge_Bar[MP]->Is_Full())Gauge_Bar[MP]->Add_Now(0.05f);

	//	マウスの方向を向かせる
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	Qtr_Angle = atan2f((Qtr_MY + QTR_OFFSET_X + Camera_Y) - (Qtr_Y + QTR_OFFSET_Y), (Qtr_MX + Camera_X) - Qtr_X);
	Qtr_Angle = Qtr_Angle * 180.0f / DX_PI_F;

	//	クォータビュー上での角度8方向いずれかを向く
	if (Qtr_Angle < 175 && Qtr_Angle>95)Ply_Gr_Num += 3;				//	左斜め下
	else if (Qtr_Angle <= 95 && Qtr_Angle >= 85)Ply_Gr_Num += 0;		//	正面
	else if (Qtr_Angle < 85 && Qtr_Angle>5)Ply_Gr_Num += 9;				//	右斜め下
	else if (Qtr_Angle >= 175 || Qtr_Angle <= -175)Ply_Gr_Num += 6;		//	左横
	else if (Qtr_Angle <= 5 && Qtr_Angle >= -5)Ply_Gr_Num += 12;		//	右横
	else if (Qtr_Angle > -175 && Qtr_Angle < -95)Ply_Gr_Num += 15;		//	左斜め上
	else if (Qtr_Angle >= -95 && Qtr_Angle <= -85)Ply_Gr_Num += 18;		//	後ろ向き
	else if (Qtr_Angle > -85 && Qtr_Angle < -5)Ply_Gr_Num += 21;		//	右斜め上

	//	クォータービュー上での中心座標を描画テーブルに登録
	float X = (float)(Qtr_X + QTR_OFFSET_X - Camera_X);
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);
	My_Draw->Register(X, Y, H_Size_X, H_Size_Y, PLAYER, Ply_Gr[Ply_Gr_Num]);	//	登録
}

//	HP,MPゲージ描画
void Player::Draw_Gauge()
{
	// Hp 外枠 本体
	DrawExtendGraph((int)5, (int)5, (int)(115 + 11), (int)55, Gauge_Gr[FRAME], TRUE);

	float rate = Gauge_Bar[HP]->Rate();
	DrawExtendGraph((int)11, (int)5, (int)(111 * rate + 11), (int)30, Gauge_Gr[HP], TRUE);

	rate = Gauge_Bar[MP]->Rate();
	DrawExtendGraph((int)11, (int)30, (int)(111 * rate + 11), (int)55, Gauge_Gr[MP], TRUE);
}

//	MP計算
void Player::Mp_Proc(float value)
{

	//	Mpが０になったときフラグを立てる
	if (!Gauge_Bar[MP]->Is_Empty()) {
		Gauge_Bar[MP]->Add_Now(value);
	}

	//	フラグが立っていてMpがMaxになったらフラグをさげる
	if (Mp_Flg && Gauge_Bar[MP]->Is_Full()) {
		Mp_Flg = false;
	}
}

//	ステータス計算
void Player::Status_Update()
{
	//	ダメージ状態の場合
	if (Status.Is(DAMAGE)){
		
		//	カウントが０より低くなければデクリメント
		if (Damage_Cnt > 0)
			Damage_Cnt--;
		
		//	カウントが０ならノーマルにする
		else {
			Status.Reset();
			Status.On(NORMAL);
		}
	}

	//	死んでいる状態の場合
	else if (Status.Is(DYING)) {
		
		//	０でないならデクリメント
		if (Dying_Cnt > 0)
			Dying_Cnt--;
		
		//	カウントが０なら死亡にする
		else{
			Status.Reset();
			Status.On(DIE);
		}
	}
}

//	HP計算
void Player::Hp_Proc(float Dmg)
{
	//	HPが０ならステータスを死んでいる状態にする
	if (Gauge_Bar[HP]->Is_Empty()) {
		Status.Reset();
		Status.On(DYING);
		return;
	}

	//	ダメージをうける
	Gauge_Bar[HP]->Add_Now(Dmg);
	Status.Reset();	//	ステータスリセット

	//	Hpが０でなければステータスをダメージ状態にする
	if (!Gauge_Bar[HP]->Is_Empty()) {
		Status.On(DAMAGE);
		Set_Damage_Cnt(DAMAGE_COUNT);
	}

	//	０なら死んでる状態にする
	else {
		Status.On(DYING);
		Set_Dying_Cnt(DYING_COUNT);
		My_Sound->Play_Se(SE_ERASE_PLAYER);
	}
}