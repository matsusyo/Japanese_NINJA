//	全ての敵の管理を行う関数

#include"EnemyManager.h"
extern Map *My_Map;
extern Effect_Manager *My_Effect;
extern Sound_Manager *My_Sound;

//	コンストラクタ
Enemy_Manager::Enemy_Manager()
{
	for (int i = 0; i < ENY_NUM; i++) { Eny_Tbl[i] = NULL;}
	LoadDivGraph("Image/Enemy/Heishi.png", ENEMY_BLUE_GR_NUM, 6, 4, 32, 48, Enemy_Blue_Graph);	//	雑魚敵の画像
	LoadDivGraph("Image/Enemy/king.png", ENEMY_BOSS_GR_NUM, 6, 4, 40, 60, Enemy_Boss_Graph);	//	ボスの画像
	Blue_Gauge_Graph[HP] = LoadGraph("Image/Gauge/GaugeEny.png");		//	雑魚敵のHp画像読み込み
	Blue_Gauge_Graph[FRAME] = LoadGraph("Image/Gauge/GaugeFrame.png");	//	Hpの外枠画像を読み込み
	Boss_Gauge_Graph[HP]= LoadGraph("Image/Gauge/GaugeBoss.png");		//	ボスのHp画像読み込み
	Boss_Gauge_Graph[FRAME] = Blue_Gauge_Graph[FRAME];					//	外枠画像を雑魚敵のから読み込む
	Register_Enemy();
}

//	デストラクタ
Enemy_Manager::~Enemy_Manager()
{
	//	画面上に残っている敵を削除する
	for (int i = 0; i < ENY_NUM; i++) {
		if (Eny_Tbl[i] != NULL) delete Eny_Tbl[i];
	}
}

//	更新
void Enemy_Manager::Move()
{
	for (int i = 0; i < ENY_NUM; i++) {

		if (Eny_Tbl[i] == NULL) continue;	// 空いてるテーブルはスキップする

		Eny_Tbl[i]->Move();	//	動き

		//	敵が死んだ状態なら削除
		if (Eny_Tbl[i]->Status.Is(DIE)) {
			delete Eny_Tbl[i];
			Eny_Tbl[i] = NULL;
		}
	}
}

//	プレイヤーの攻撃に対する敵の当たり判定 手裏剣と爆弾
bool Enemy_Manager::Judge_Player_Attack(RECT rc, float Dmg,int Type)
{
	bool Hit = false;	//	いずれの敵とも当たっていない

	//	いずれかの敵と当たっているか調べる
	for (int i = 0; i < ENY_NUM; i++) {

		if (Eny_Tbl[i] == NULL) continue;	//	敵が登録されていない場所はパス
		if (!Eny_Tbl[i]->Status.Is(NORMAL)) continue;	//	状態がNORMAでないならダメージ計算しない
		if (!Eny_Tbl[i]->Collision(rc)) continue;	//	rcと衝突していなければダメージ計算しない
		
		Hit = true;	//	一度でもここに来たらreturnでtrueを返す

		//	Typeによって衝突 エフェクト,音を変える
		float x = (float)(Eny_Tbl[i]->Get_Qtr_X() + QTR_OFFSET_X);	//	登録する座標を計算
		float y = (float)(Eny_Tbl[i]->Get_Qtr_Y() + QTR_OFFSET_Y);
		switch (Type){
		case DART:	//	手裏剣の時
			My_Effect->Register(x, y, EFFECT_DART);	//	エフェクト登録
			My_Sound->Play_Se(SE_DART_SPARK);	//	音を鳴らす
			//	ぶつかった方向に向く
			Eny_Tbl[i]->Set_Top_Radian(atan2f((rc.top + rc.bottom) / 2 - Eny_Tbl[i]->Get_Top_Y(), (rc.left + rc.right) / 2 - Eny_Tbl[i]->Get_Top_X()));
			Eny_Tbl[i]->Set_Top_Angle(Eny_Tbl[i]->Get_Top_Radian() * 180.0f / DX_PI_F);
			break;

		case BOMB:	//	爆弾の時
			My_Effect->Register(x, y, EFFECT_BOMB);	//	エフェクト登録
			My_Sound->Play_Se(SE_BOMB_SPARK);	//	音を鳴らす
			break;

		default:	//	予期せぬ Type なら何もしない
			break;
		}

		Eny_Tbl[i]->Gauge_Bar[HP]->Add_Now(Dmg);	//	当たった敵のHPを減らす
		Eny_Tbl[i]->Status.Reset();			//	ステータスをリセットする

		//	敵のHPが0なら死んでいる状態にする
		if (Eny_Tbl[i]->Gauge_Bar[HP]->Is_Empty()) {
			Eny_Tbl[i]->Status.On(DYING);	//	死んでる状態にする
			
			//	BOSSが死んだらボス死亡音
			if (Eny_Tbl[i]->Get_Type() == ENEMY_BOSS) {
				Eny_Tbl[i]->Set_Dying_Count(BOSS_DYING_COUNT);	//	敵が死ぬまでにかかる時間
				My_Sound->Play_Se(SE_ERASE_BOSS);
			}
			
			//	違ったら雑魚敵死亡音
			else {
				Eny_Tbl[i]->Set_Dying_Count(E_DYING_COUNT);	//	敵が死ぬまでにかかる時間
				My_Sound->Play_Se(SE_ERASE_ENEMY);
			}
		}

		//	HPがあればダメージ状態にする
		else {
			Eny_Tbl[i]->Status.On(DAMAGE);
			Eny_Tbl[i]->Set_Damage_Count(E_DAMAGE_COUNT);
		}
	}

	//	敵のどれかにあたってたら
	if (Hit) return true;
	else return false;
}

//	プレイヤーとの当たり判定
bool Enemy_Manager::Judge_Player(RECT rc)
{
	bool Hit = false;
	for (int i = 0; i < ENY_NUM; i++) {

		//	登録されていない場所はパス
		if (Eny_Tbl[i] == NULL) continue;

		//	NORMALかつrcと接触してたらHPを減らす
		if (Eny_Tbl[i]->Status.Is(NORMAL)){
			if (Eny_Tbl[i]->Collision(rc)){
				Hit = true;
			}
		}
		//	接触したら抜ける
		if (Hit)break;
	}
	return Hit;
}

//	ステータスの更新　毎フレーム呼ぶ
void Enemy_Manager::Status_Update()
{
	for (int i = 0; i < ENY_NUM; i++) {
		if (Eny_Tbl[i] == NULL) continue;
		//	敵の数だけ回す
		Eny_Tbl[i]->Status_Update();
	}
}


//	視界を描画
void Enemy_Manager::Draw_Ray()
{
	for (int i = 0; i < ENY_NUM; i++) {

		if (Eny_Tbl[i] == NULL) continue;	// 空いてるテーブルはスキップする

		Eny_Tbl[i]->Draw_Ray();	//	動き
	}
}


//	HPを描画
void Enemy_Manager::Draw_Gauge()
{
	for (int i = 0; i < ENY_NUM; i++) {

		if (Eny_Tbl[i] == NULL) continue;	// 空いてるテーブルはスキップする

		Eny_Tbl[i]->Draw_Gauge();	//	動き
	}
}


//	手裏剣の標的をもらう
Enemy* Enemy_Manager::Get_Target(float topX,float topY)
{
	int Cnt = 0;	//	有効な敵数を数える
	Enemy *TargetList[ENY_NUM];	//	有効な敵を 記録 するテーブル
	
	//	有効な敵と番号を記録する
	for (int i = 0; i < ENY_NUM; i++) {
		if (Eny_Tbl[i] == NULL) continue;	//	テーブルに敵がいなければスキップ
		TargetList[Cnt] = Eny_Tbl[i];
		Cnt++;
	}
	
	for (int i = 0; i < Cnt; i++) {
		RECT rc = TargetList[i]->Get_Rect();
		//	索敵判定 (三平方の定理で距離を判定) c2 = a2 + b2; c = √(a2 + b2)
		float Eny_Center_PosX = (float)(rc.left + (rc.right - rc.left));// 中心座標を出しておく
		float Eny_Center_PosY = (float)(rc.top + (rc.bottom - rc.top));
		float a = pow(Eny_Center_PosX - topX, 2);//	プレイヤーと自分の位置関係
		float b = pow(Eny_Center_PosY - topY, 2);
		double c = sqrt(a + b);// cからの距離
		if (c <= 130) { return TargetList[i]; }
	}
	
	//	敵が存在しない場合 NULLを返す
	return NULL;
}

//	敵をエリアごとに生成　
void Enemy_Manager::Register_Enemy()
{
	float TopX, TopY;	//	生成する座標
	
	//	敵数に届くまで敵を登録
	for (int i = 0; i < ENY_NUM-1; i++) 
		while (Eny_Tbl[i] == NULL) {
			TopX = (float)((CHIP_SIZE_X - 1)* GetRand(MAP_X));	//	ランダムに座標を決定
			TopY = (float)((CHIP_SIZE_Y - 1)* GetRand(MAP_Y));

			//	プレイヤーの近くには登録しない
			if (TopX <= CHIP_SIZE_X * MAP_X / 6 && TopY <= CHIP_SIZE_Y * MAP_Y / 6) {
				continue;
			}

			//	画像の角4点が壁の中じゃなかったら登録
			float Left = TopX - (ENY_SIZE_X / 2);
			float Top = TopY - (ENY_SIZE_Y / 2);
			float Right = TopX + (ENY_SIZE_X / 2);
			float Bottom = TopY + (ENY_SIZE_Y / 2);

			if ((My_Map->Map_Get_Chip(Left, Top) < 7 && My_Map->Map_Get_Chip(Left, Top) >= 0) &&		//	左上
				(My_Map->Map_Get_Chip(Left, Bottom) < 7 && My_Map->Map_Get_Chip(Left, Bottom) >= 0) &&	//	左下
				(My_Map->Map_Get_Chip(Right, Top) < 7 && My_Map->Map_Get_Chip(Right, Top) >= 0) &&		//	右上
				(My_Map->Map_Get_Chip(Right, Bottom) < 7 && My_Map->Map_Get_Chip(Right, Bottom) >= 0))	//	右下
				Eny_Tbl[i] = new Enemy_Blue(TopX, TopY, Enemy_Blue_Graph, Blue_Gauge_Graph);
		}
	
	
	//	ボスを登録
	while (Eny_Tbl[ENY_NUM - 1] == NULL) {
		TopX = (float)((CHIP_SIZE_X - 1)* GetRand(MAP_X));
		TopY = (float)((CHIP_SIZE_Y - 1)* GetRand(MAP_Y));
		
		//	プレイヤーの近くには登録しない
		if (TopX <= CHIP_SIZE_X * MAP_X / 2 && TopY <= CHIP_SIZE_Y * MAP_Y / 2) {
			continue;
		}

		//	画像の角4点が壁の中じゃなかったら登録
		float Left = TopX - (ENY_SIZE_X / 2);
		float Top = TopY - (ENY_SIZE_Y / 2);
		float Right = TopX + (ENY_SIZE_X / 2);
		float Bottom = TopY + (ENY_SIZE_Y / 2);

		if ((My_Map->Map_Get_Chip(Left, Top) < 7 && My_Map->Map_Get_Chip(Left, Top) >= 0) &&		//	左上
			(My_Map->Map_Get_Chip(Left, Bottom) < 7 && My_Map->Map_Get_Chip(Left, Bottom) >= 0) &&	//	左下
			(My_Map->Map_Get_Chip(Right, Top) < 7 && My_Map->Map_Get_Chip(Right, Top) >= 0) &&		//	右上
			(My_Map->Map_Get_Chip(Right, Bottom) < 7 && My_Map->Map_Get_Chip(Right, Bottom) >= 0))	//	右下
			Eny_Tbl[ENY_NUM - 1] = new Enemy_Boss(TopX, TopY, Enemy_Boss_Graph, Boss_Gauge_Graph);
	}
}