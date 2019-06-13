//	 敵のタイプごとの処理

#include"Enemy.h" 

extern Player *Ply;	//	プレイヤー
extern Map *My_Map;	//	マップ
extern Input_Controll *Inp_Ctrl;		//	キー入力　マウス入力
extern Quarter_Calculation *Qtr_Calc;	//	クォータ座標変換		
extern Clone *Ply_Clone;				//	分身
extern Qtr_Draw_Manager *My_Draw;		//	描画テーブル
extern Effect_Manager *My_Effect;		//	爆発
extern Sound_Manager *My_Sound;			//	音
extern Time_Manager *My_Time;			//	クリアタイム、経過時間などを管理

Enemy::Enemy(){}	//	コンストラクタ
Enemy::~Enemy() {}	//	デストラクタ

//	敵の挙動
void Enemy::Move(){}

bool Enemy::Judge_Move(RECT rc, float *topx, float *topy, float movex, float movey) {
	
	bool flg = false;	//	結果を受け取る
	//	壁にぶつかってなければ移動させる
	//	横方向の移動　6点で判定
	if ((My_Map->Map_Get_Chip((float)rc.left + movex, (float)rc.top) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.left + movex, (float)rc.bottom) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.left + movex, (float)((rc.top + rc.bottom) / 2)) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right + movex, (float)((rc.top + rc.bottom) / 2)) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right + movex, (float)rc.top) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right + movex, (float)rc.bottom) < 7)) {
		*topx += movex;
		flg = true;	//	移動したらtrue
	}

	//	縦方向の移動　6点で判定
	if ((My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)((rc.left + rc.right) / 2), (float)rc.bottom + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + movey) < 7) &&
		(My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + movey) < 7)) {
		*topy += movey;
		flg = true;	//	移動したらtrue
	}
	return flg;
}

//	ステータスの更新
void Enemy::Status_Update()
{
	//	ステータスがダメージ場合
	if (Status.Is(DAMAGE)) {

		//	カウントが０でなければデクリメント
		if (Damage_Cnt > 0) {
			Damage_Cnt--;
		}

		//	０以下ならステータスをノーマルにする
		else {
			Status.Reset();
			Status.On(NORMAL);
		}
	}

	//	死んでいる場合
	else if (Status.Is(DYING)) {
	
		//	カウントが０でなければデクリメント
		if (Dying_Cnt > 0) {
			Dying_Cnt--;
		}
		
		//	０以下ならステータスを死亡にする
		else {
			Status.Reset();
			Status.On(DIE);
		}
	}
}


//	HPと外枠を描画
void Enemy::Draw_Gauge()
{
	//	ウィンドウ外なら描画しない
	int X = (int)(Qtr_X - Camera_X);
	int Y = (int)(Qtr_Y - Camera_Y);
	if (X > WINDOW_X + CHIP_SIZE_X * 2 || X < -CHIP_SIZE_X * 2) { return; }
	if (Y > WINDOW_Y + CHIP_SIZE_Y * 2 || Y < -CHIP_SIZE_Y * 2) { return; }

	// HPの外枠を描画
	int FrameX1 = (int)(Qtr_X - Camera_X - H_Size_X);		//	始点X
	int FrameY1 = (int)(Qtr_Y - Camera_Y + H_Size_Y / 2);	//	始点Y
	int FrameX2 = (int)(Qtr_X - Camera_X + H_Size_X);		//	終点X
	int FrameY2 = (int)(Qtr_Y - Camera_Y + H_Size_Y);		//	終点Y
	DrawExtendGraph(FrameX1, FrameY1, FrameX2, FrameY2, Gauge_Gr[FRAME], true);	//	敵の頭上に描画

	//	Hpを描画
	int HpX1 = (int)(Qtr_X - Camera_X - H_Size_X + 2);			//	始点X　外枠に収まるようにする +2
	int HpY1 = (int)(Qtr_Y - Camera_Y + H_Size_Y / 2 + 2);		//	始点Y　外枠に収める +2
	int HpX2 = (int)(HpX1 + (Gauge_Bar[HP]->Get_Now() / 1.8));	//	終点X　現在のHp分の長さにする
	int HpY2 = (int)(Qtr_Y - Camera_Y + H_Size_Y - 2);			//	終点Y　外枠に収める -2
	DrawExtendGraph(HpX1, HpY1, (int)((float)HpX2), (int)HpY2, Gauge_Gr[HP], true);	//	外枠にHPが収まるように描画
}


//	ここから Enemy_Blue
Enemy_Blue::Enemy_Blue(float argx, float argy,int graph[],int gaugegr[])
{
	Top_X = argx;	//	座標登録 
	Top_Y = argy;	
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	クォータ座標変換
	Camera_X = Ply->Get_Camera_X();	//	カメラセット
	Camera_Y = Ply->Get_Camera_Y();
	Speed = ENEMY_SPEED;			//	スピードセット
	Size_X = ENY_SIZE_X; Size_Y = ENY_SIZE_Y;		//	画像サイズセット
	H_Size_X = Size_X / 2; H_Size_Y = Size_Y / 2;	//	半分のサイズ
	for (int i = 0; i < ENEMY_GR_NUM; i++) { Eny_Gr[i] = graph[i]; }	//	敵の画像を分割読み込み
	Eny_Gr_Num = 0;	//	画像ナンバー初期化

	//	適当な方向を向く
	Top_Angle = (float)(GetRand(180));
	if (GetRand(1) != 0)Top_Angle *= -1;
	Top_Radian = Top_Angle * (DX_PI_F / 180);	//	ラジアンに変換

	Change_Angle_Cnt = GetRand(CHANGE_ANGLE_TIMING);	//	向きを変えるタイミングをランダムに初期化
	Walk_Cnt = GetRand(MOVE_TIMING);	//	移動するタイミングをランダムに初期化
	 
	//	視認できる距離を決める
	Vision_Distance = VISION_BASE;	//	基準距離を代入
	if (GetRand(1) == 0)Vision_Distance += (float)GetRand(VISION_ADD);	//	視認距離を伸ばす
	else Vision_Distance -= (float)GetRand(VISION_ADD);	//	短くする

	//	視認できる角度の絶対値を決める
	Abs_Angle = ABS_ANGLE_BASE;	//	基準角を代入
	if (GetRand(1) == 0)Abs_Angle += (float)GetRand(ABS_ANGLE_ADD);	//	視界を広げる
	else Abs_Angle -= (float)GetRand(ABS_ANGLE_ADD);	//	狭くする

	//	ゲージ関連
	Gauge_Bar[HP] = new Gauge(ENY_BLUE_HP_MAX, 0);	//	HP初期化
	Gauge_Gr[HP] = gaugegr[HP];
	Gauge_Gr[FRAME] = gaugegr[FRAME];

	Hp_Flg = false;			//	HPフラグを落とす
	Find_Flg = false;
	Damage_Cnt = Dying_Cnt = 0;		//	被弾,死亡無敵時間を初期化
	Status.On(NORMAL);				//	ステータスを通常状態にする
}

//	デストラクタ
Enemy_Blue::~Enemy_Blue(){}

//	挙動
void Enemy_Blue::Move()
{
	Eny_Gr_Num = 0;	//	画像ナンバーを初期化
	
	//	見える距離かつ視界内ならプレイヤーを追いかける
	if (Search_Player()) {

		//	敵を最初に見つけた時
		if (!Find_Flg) {
			Find_Flg = true;	//	フラグを立てる
			Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	クォーター座標に変換
			My_Effect->Register(Qtr_X + QTR_OFFSET_X, Qtr_Y + QTR_OFFSET_Y, EFFECT_ALERT);	//	！を描画
			My_Sound->Play_Se(SE_ALERT);	//	発見音を鳴らす
			if (Get_Target_Name() == PLAYER)My_Time->Add_Time_Limit(-5.0f);	//	プレイヤーなら制限時間を短縮
		}

		Top_Angle = Target_Angle;	//	ターゲットへの角度と自分の角度を同じにする

		//	角度のエラー対策
		if (Top_Angle > 180)Top_Angle -= 360;	//	角度が180を上回ったら-360度を加える
		if (Top_Angle < -180)Top_Angle += 360;	//	-180度を下回ったら+360度を加える
		Top_Radian = Top_Angle * (DX_PI_F / 180);	//	ラジアンに変換

		//	移動量計算
		float MoveX = (cosf(Top_Radian)*Speed);
		float MoveY = (sinf(Top_Radian)*Speed);
		
		//	壁にぶつかってなければ移動させる
		if (Judge_Move(Get_Rect(), &Top_X, &Top_Y, MoveX, MoveY)) 
			Eny_Gr_Num = rand() % 3;	//	歩いているようにみせるために画像を変更
	}

	//	プレイヤーを見つけていない時に向きを変更
	else {
		Change_Angle_Cnt++;	//	カウントを進める

		//	向きを変えるタイミングになったら変える
		if (Change_Angle_Cnt > CHANGE_ANGLE_TIMING) {
			Change_Angle_Cnt = 0;	//	初期化
			float PlusAngle = (float)(GetRand(45));	//	ランダムに向きを設定
			if (GetRand(1) != 0)PlusAngle *= -1;
			Top_Angle += PlusAngle;	//	ここで角度をずらす

			if (Top_Angle > 180)Top_Angle -= 360;
			if (Top_Angle < -180)Top_Angle += 360;

			Top_Radian = Top_Angle * (DX_PI_F / 180);	//	ラジアンに変換
		}

		Walk_Cnt++;	//	カウントを進める

		//	カウントが上回っている間は移動を続ける
		if (Walk_Cnt > MOVE_TIMING) {

			//	移動量計算
			float MoveX = (cosf(Top_Radian)*Speed);
			float MoveY = (sinf(Top_Radian)*Speed);

			//	壁にぶつかってなければ移動させる
			if (Judge_Move(Get_Rect(), &Top_X, &Top_Y, MoveX, MoveY)) 
				Eny_Gr_Num = rand() % 3;	//	歩いているようにみせるために画像を変更
		}
		if (Walk_Cnt > MOVE_TIMING + MOVE_TIME)Walk_Cnt = 0;	//	カウントが増えてきたらリセット
	}

	//	クォータビューでの角度を求める
	Top_End_X = Top_X + (cosf(Top_Radian) * Vision_Distance);	//	向いてる方向を求めるための座標
	Top_End_Y = Top_Y + (sinf(Top_Radian) * Vision_Distance);
	Qtr_Calc->Top_Qtr(Top_End_X, Top_End_Y, &Qtr_End_X, &Qtr_End_Y);	//	クォータ座標変換
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	
	//	atan2で向いてる角度を求める
	Qtr_Angle = atan2f((Qtr_Y + QTR_OFFSET_Y) - (Qtr_End_Y + QTR_OFFSET_Y), (Qtr_X + QTR_OFFSET_X) - (Qtr_End_X + QTR_OFFSET_X));
	Qtr_Angle = Qtr_Angle * 180.0f / DX_PI_F;

	Eny_Gr_Num += Change_Gr(Qtr_Angle);	//	クォータビュー上での角度によって画像を変える

	Camera_X = Ply->Get_Camera_X();	//	プレイヤ－のカメラセット
	Camera_Y = Ply->Get_Camera_Y();

	//	ウィンドウ内での座標を計算
	float X = (float)(Qtr_X + QTR_OFFSET_X - Camera_X);	//	座標計算
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);

	//	ウィンドウ外なら登録しない
	if (X > WINDOW_X + CHIP_SIZE_X * 2 || X < -CHIP_SIZE_X * 2) { return; }
	if (Y > WINDOW_Y + CHIP_SIZE_Y * 2 || Y < -CHIP_SIZE_Y * 2) { return; }

	//	ウィンドウ内なら登録
	My_Draw->Register(X, Y, H_Size_X, H_Size_Y, ENEMY, Eny_Gr[Eny_Gr_Num]);
}

//	索敵
bool Enemy_Blue::Search_Player() {

	float Ply_Distance = Calc_Distance(Ply->Get_Rect());	//	敵とプレイヤーの距離を求める

	float Ply_Radian = atan2f(Ply->Get_Top_Y() - Top_Y, Ply->Get_Top_X() - Top_X);	//	プレイヤーへのラジアンを求める
	float Ply_Angle = Ply_Radian * 180.0f / DX_PI_F;		//	角度変換
	float Ply_Abs_Angle = Calc_Abs_Angle(Top_Angle, Ply_Angle);	//	敵の向きとプレイヤーへの角度の絶対値を計算
	
	//	分身がいない場合ここで結果を返す
	if (Ply_Clone == NULL) {
		//	プレイヤーが視界内なら壁越しにみつけているか判定
		if (Ply_Distance <= Vision_Distance && Ply_Abs_Angle <= Abs_Angle) {
			Set_Target_Name(PLAYER);
			return Judge_Wall(Ply_Angle, Ply_Abs_Angle, Ply_Distance, &Find_Flg);	//	壁越しならfalseが返ってくる
		}
		//	視界外なら false を返す
		else return Find_Flg = false;
	}

	//	分身がでている場合
	float Clone_Distance = Calc_Distance(Ply_Clone->Get_Rect());	//	分身がでてたら距離をだす

	float Clone_Radian = atan2f(Ply_Clone->Get_Top_Y() - Top_Y, Ply_Clone->Get_Top_X() - Top_X);	//	ラジアン計算
	float Clone_Angle = Clone_Radian * 180.0f / DX_PI_F;	//	角度変換
	float Clone_Abs_Angle = Calc_Abs_Angle(Top_Angle, Clone_Angle);	//	敵の向きとプレイヤーへの角度の絶対値を計算

	//	プレイヤーと分身ともに視界外なら false を返す
	if (Ply_Abs_Angle > Abs_Angle && Clone_Abs_Angle > Abs_Angle) return Find_Flg = false;

	//	プレイヤーと分身ともに視界内の時
	if (Ply_Abs_Angle <= Abs_Angle && Clone_Abs_Angle <= Abs_Angle && Ply_Distance <= Vision_Distance && Clone_Distance <= Vision_Distance) {

		//	近い方をターゲットにする プレイヤー
		if (Ply_Distance <= Clone_Distance) {
			Set_Target_Name(PLAYER);
			return Judge_Wall(Ply_Angle, Ply_Abs_Angle, Ply_Distance, &Find_Flg);	//	壁越しならfalseが返ってくる
		}

		else //	分身
			Set_Target_Name(CLONE);
			return Judge_Wall(Clone_Angle, Clone_Abs_Angle, Clone_Distance,&Find_Flg);	//	壁越しならfalseが返ってくる
	}

	//	分身のみが視界内の時
	if (Ply_Abs_Angle > Abs_Angle && Clone_Abs_Angle <= Abs_Angle && Clone_Distance <= Vision_Distance) {
		Set_Target_Name(CLONE);
		return Judge_Wall(Clone_Angle, Clone_Abs_Angle, Clone_Distance, &Find_Flg);	//	壁越しならfalseが返ってくる
	}
	return Find_Flg = false;	//	敵がみつからなければFind_Flgを落として返す
}

//	プレイヤーまたは分身と敵の距離
float Enemy_Blue::Calc_Distance(RECT rc) {
	float CenterPosX = (float)((rc.left + rc.right) / 2);	//	中心座標を求める
	float CenterPosY = (float)((rc.top + rc.bottom) / 2);
	float a = pow(CenterPosX - Top_X, 2);	//	プレイヤーまたは分身と敵の位置関係
	float b = pow(CenterPosY - Top_Y, 2);
	return sqrt(a + b);// プレイヤーまでの距離
}

//	基準角度からプレイヤー角度への絶対値を求める
float Enemy_Blue::Calc_Abs_Angle(float topangle, float targetangle) {

	//	どちらかの角度がマイナスになった場合に計算方法を変える


	if (topangle <= -180 + Abs_Angle && targetangle >= 180 - Abs_Angle)
		return abs(topangle + targetangle);
	
	else if (topangle >= 180 - Abs_Angle && targetangle <= -180 + Abs_Angle)
		return abs(targetangle + topangle);
	
	//	どちらかがマイナスでない場合
	else return abs(topangle - targetangle);
}

//	視界角度に入ったプレイヤーが壁越しにいるかどうか
bool Enemy_Blue::Judge_Wall(float angle,float absangle,float distance,bool *findflg) {
	Target_Angle = angle;			//	分身への角度
	Target_Radian = Target_Angle * (DX_PI_F / 180);	//	ラジアン変換
	Target_Abs_Angle = absangle;	//	基準角度からの範囲

	//	1チップ分の移動量
	float MoveX = (cosf(Target_Radian)*CHIP_SIZE_X);
	float MoveY = (sinf(Target_Radian)*CHIP_SIZE_Y);
	//	壁ごしなら追尾して来ないようにする
	for (int i = 1; i <= (int)(Vision_Distance / 40); i++) {
		float a = pow((float)(i*MoveX), 2);
		float b = pow((float)(i*MoveY), 2);
		//	プレイヤーより手前に壁があったら追尾させない
		if (distance >= sqrt(a + b) && My_Map->Map_Get_Chip(Top_X + i * MoveX, Top_Y + i * MoveY) >= 7)
			return *findflg = false;	//	敵がみつからなければFind_Flgを落とす
	}
	return true;	//	壁がなければtrue
}

//	敵の視界を描画
void Enemy_Blue::Draw_Ray()
{
	if (Qtr_X - Camera_X <= WINDOW_X + CHIP_SIZE_X * 2 && Qtr_X - Camera_X >= -CHIP_SIZE_X * 2 && Qtr_Y - Camera_Y <= WINDOW_Y + CHIP_SIZE_Y * 2 && Qtr_Y - Camera_Y >= -CHIP_SIZE_Y * 2) {

		//	視界の左側のRayから描画
		float Ray_Angle = Top_Angle - Abs_Angle;		//	角度を求める
		float Ray_Radian = Ray_Angle * (DX_PI_F / 180);	//	ラジアンを求める
		float MoveRayX = cosf(Ray_Radian) * (Vision_Distance / 10);	//	Rayの移動量を求める
		float MoveRayY = sinf(Ray_Radian) * (Vision_Distance / 10);

		//	視界の始点を求めておく
		int X1 = (int)(Qtr_X + QTR_OFFSET_X - Camera_X);	//	始点X
		int Y1 = (int)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);	//	始点Y
		float QtrRayX, QtrRayY;	//	Rayのクォータビュー座標を計算

		//	壁にぶつかるまでRayを伸ばす
		for (int i = 0; i < Vision_Distance / 10; i++) {
			float RayX = Top_X + (MoveRayX * i);
			float RayY = Top_Y + (MoveRayY * i);
			//	始点から終点の位置関係
			float a = pow(RayX - Top_X, 2);
			float b = pow(RayY - Top_Y, 2);
			float Distance = sqrt(a + b);	// 始点から終点までの距離
			//	Rayが壁にぶつかるか距離を上回ったらRayを描画する
			if (My_Map->Map_Get_Chip(RayX, RayY) != 4 || Distance >= Vision_Distance) {
				Qtr_Calc->Top_Qtr(RayX, RayY, &QtrRayX, &QtrRayY);
				int X2 = (int)(QtrRayX + QTR_OFFSET_X - Camera_X);
				int Y2 = (int)(QtrRayY + QTR_OFFSET_Y - Camera_Y);
				DrawLine(X1, Y1, X2, Y2, COLOR[WHITE]);	//	始点から終点を結ぶ線を描画
				break;
			}
		}

		//	右側
		Ray_Angle = Top_Angle + Abs_Angle;
		Ray_Radian = Ray_Angle * (DX_PI_F / 180);
		MoveRayX = cosf(Ray_Radian) * (Vision_Distance / 10);
		MoveRayY = sinf(Ray_Radian) * (Vision_Distance / 10);

		for (int i = 0; i < Vision_Distance / 10; i++) {
			float RayX = Top_X + (MoveRayX * i);
			float RayY = Top_Y + (MoveRayY * i);
			float a = pow(RayX - Top_X, 2);
			float b = pow(RayY - Top_Y, 2);
			float Distance = sqrt(a + b);

			if (My_Map->Map_Get_Chip(RayX, RayY) != 4 || Distance >= Vision_Distance) {
				Qtr_Calc->Top_Qtr(RayX, RayY, &QtrRayX, &QtrRayY);
				int X2 = (int)(QtrRayX + QTR_OFFSET_X - Camera_X);
				int Y2 = (int)(QtrRayY + QTR_OFFSET_Y - Camera_Y);
				DrawLine(X1, Y1, X2, Y2, COLOR[WHITE]);
				break;
			}
		}
	}
}


//	Enemy_Boss
Enemy_Boss::Enemy_Boss(float argx, float argy,int graph[],int gaugegr[])
{
	Top_X = argx;	//	座標セット
	Top_Y = argy;
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);	//	クォータ座標変換
	Camera_X = Ply->Get_Camera_X();	//	カメラセット
	Camera_Y = Ply->Get_Camera_Y();
	Top_Angle = 0;					//	角度初期化
	Speed = ENEMY_SPEED;			//	スピードセット
	Size_X = ENY_SIZE_X; Size_Y = ENY_SIZE_Y;		//	サイズセット
	H_Size_X = Size_X / 2; H_Size_Y = Size_Y / 2;	//	半分のサイズセット
	for (int i = 0; i < ENEMY_GR_NUM; i++) { Eny_Gr[i] = graph[i]; }	//	画像分割読み込み
	Eny_Gr_Num = 0;	//	画像ナンバー初期化
	Cnt_Angle = 0;	//	角度を変えるカウント初期化

	//	適当な方向を向く
	Top_Angle = (float)(GetRand(180));
	if (GetRand(1) != 0)Top_Angle *= -1;		//	角度を設定して
	Top_Radian = Top_Angle * (DX_PI_F / 180);	//	ラジアンに変換

	Hp_Flg = Move_Flg_X = Move_Flg_Y = false;	//	フラグを落とす
	Damage_Cnt = Dying_Cnt = 0;	//	被弾、死亡時間初期化
	Status.On(NORMAL);			//	ステータスをノーマルに

	//	Hp初期化とHp画像読み込み
	Gauge_Bar[HP] = new Gauge(ENY_BOSS_HP_MAX, 0);	//	HP初期化
	Gauge_Gr[HP] = gaugegr[HP];
	Gauge_Gr[FRAME] = gaugegr[FRAME];
}

//	デストラクタ
Enemy_Boss::~Enemy_Boss() {}

//	挙動
void Enemy_Boss::Move()
{
	//	プレイヤーの座標を得る
	float Ply_X = Ply->Get_Top_X();
	float Ply_Y = Ply->Get_Top_Y();

	//	プレイヤーを視認してれば追尾する
	if (Move_Flg_X&&Move_Flg_Y) {

		//	プレイヤーの反対を方向を向く
		Top_Angle = atan2f(Ply->Get_Top_Y() - Top_Y, Ply->Get_Top_X() - Top_X);
		Top_Angle *= -1;

		//	視界から描画画像を変えるので必ず書く
		Vision_X = cosf(Top_Angle)*((CHIP_SIZE_X) * 5);
		Vision_Y = sinf(Top_Angle)*((CHIP_SIZE_Y) * 5);

		//	移動量計算
		float Move_X = cosf(Top_Angle)*Speed;
		float Move_Y = sinf(Top_Angle)*Speed;

		//	移動先のマップチップ番号で移動させるか決める XとYでわける
		if (My_Map->Map_Get_Chip(Top_X + Move_X, Top_Y) < 7) {
			if (My_Map->Map_Get_Chip(Top_X + Move_X, Top_Y) >= 0) {
				Top_X += Move_X;
			}
		}
		if (My_Map->Map_Get_Chip(Top_X, Top_Y + Move_Y) < 7) {
			if (My_Map->Map_Get_Chip(Top_X, Top_Y + Move_Y) >= 0) {
				Top_Y += Move_Y;
			}
		}

		//	プレイヤーが追跡出来る距離に存在するか
		//	X軸方向にプレイヤーがいるかどうか
		if (Top_X + Vision_X > Top_X) {
			if (Top_X + Vision_X >= Ply_X && Ply_X >= Top_X) {}
			else Move_Flg_X = false;
		}
		//	上に同じ
		else if (Top_X + Vision_X <= Top_X) {
			if (Top_X + Vision_X <= Ply_X && Ply_X <= Top_X) {}
			else Move_Flg_X = false;
		}
		//	Y軸方向にプレイヤーがいるかどうか
		if (Top_Y + Vision_Y > Top_Y) {
			if (Top_Y + Vision_Y >= Ply_Y && Ply_Y >= Top_Y) {}
			else Move_Flg_Y = false;
		}
		else if (Top_Y + Vision_Y <= Top_Y) {
			if (Top_Y + Vision_Y <= Ply_Y && Ply_Y <= Top_Y) {}
			else Move_Flg_Y = false;
		}

		//	フラグが共にfalseでないと追跡を続ける
		if (!Move_Flg_X && !Move_Flg_Y) {}
		else Move_Flg_X = Move_Flg_Y = true;
	}

	//	索敵 向いてる方向から3チップ分を敵の視界とする 
	else {
		Move_Flg_X = Move_Flg_Y = false;

		Vision_X = cosf(Top_Angle)*((CHIP_SIZE_X) * 3);
		Vision_Y = sinf(Top_Angle)*((CHIP_SIZE_Y) * 3);

		//	X軸方向にプレイヤーがいるかどうか
		if (Top_X + Vision_X > Top_X) {
			if (Top_X + Vision_X >= Ply_X && Ply_X >= Top_X) {
				Move_Flg_X = true;
			}
		}
		//	上に同じ
		else if (Top_X + Vision_X <= Top_X) {
			if (Top_X + Vision_X <= Ply_X && Ply_X <= Top_X) {
				Move_Flg_X = true;
			}
		}
		//	Y軸方向にプレイヤーがいるかどうか
		if (Top_Y + Vision_Y > Top_Y) {
			if (Top_Y + Vision_Y >= Ply_Y && Ply_Y >= Top_Y) {
				Move_Flg_Y = true;
			}
		}
		else if (Top_Y + Vision_Y <= Top_Y) {
			if (Top_Y + Vision_Y <= Ply_Y && Ply_Y <= Top_Y) {
				Move_Flg_Y = true;
			}
		}
		Cnt_Angle++;
		//	10フレームたったら適当な方向を向く
		if (Cnt_Angle > 20* 60) {
			Top_Angle = (float)(rand() % 180);
			Cnt_Angle = 0;
			if (rand() % 2 != 0)
				Top_Angle *= -1;
		}

		//	プレイヤーを見たら動く
		if (!Move_Flg_X && !Move_Flg_Y&&Cnt_Angle > 25 * 60) {

			float Move_X = cosf(Top_Angle)*Speed;
			float Move_Y = sinf(Top_Angle)*Speed;
			RECT rc = Get_Rect();
			if (((My_Map->Map_Get_Chip((float)rc.left + Move_X, (float)rc.top) < 7) && (My_Map->Map_Get_Chip((float)rc.left + Move_X, (float)rc.top) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.left + Move_X, (float)rc.bottom) < 7) && (My_Map->Map_Get_Chip((float)rc.left + Move_X, (float)rc.bottom) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.right + Move_X, (float)rc.top) < 7) && (My_Map->Map_Get_Chip((float)rc.right + Move_X, (float)rc.top) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.right + Move_X, (float)rc.bottom) < 7) && (My_Map->Map_Get_Chip((float)rc.right + Move_X, (float)rc.bottom) >= 0)))
				Top_X += Move_X;

			if (((My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + Move_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.left, (float)rc.top + Move_Y) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + Move_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.left, (float)rc.bottom + Move_Y) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + Move_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.right, (float)rc.top + Move_Y) >= 0)) &&
				((My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + Move_Y) < 7) && (My_Map->Map_Get_Chip((float)rc.right, (float)rc.bottom + Move_Y) >= 0)))
				Top_Y += Move_Y;
		}

		//	座標の限界を超えていないか
		if (Top_X < 0)Top_X = 0;
		if (Top_Y < 0)Top_Y = 0;
		if (Top_X > MAP_X*CHIP_SIZE_X)Top_X = (float)MAP_X * CHIP_SIZE_X;
		if (Top_Y > MAP_Y*CHIP_SIZE_Y)Top_Y = (float)MAP_Y * CHIP_SIZE_Y;
	}

	//	トップ座標からクォータ座標へ
	Qtr_Calc->Top_Qtr(Top_X, Top_Y, &Qtr_X, &Qtr_Y);
	Qtr_Calc->Top_Qtr(Top_X + Vision_X, Top_Y + Vision_Y, &Qtr_Vision_X, &Qtr_Vision_Y);

	Qtr_Angle = atan2f((Qtr_Y + QTR_OFFSET_Y) - (Qtr_Vision_Y + QTR_OFFSET_Y), (Qtr_X + QTR_OFFSET_X) - (Qtr_Vision_X + QTR_OFFSET_X));
	Qtr_Angle = Qtr_Angle * 180.0f / DX_PI_F;

	Eny_Gr_Num = 0;
	//	クォータビュー上での角度によって画像を変える
	if (Qtr_Angle < 175 && Qtr_Angle>95)Eny_Gr_Num += 21;				//	左斜め下
	else if (Qtr_Angle <= 95 && Qtr_Angle >= 85)Eny_Gr_Num += 18;		//	正面
	else if (Qtr_Angle < 85 && Qtr_Angle>5)Eny_Gr_Num += 15;			//	右斜め下
	else if (Qtr_Angle >= 175 || Qtr_Angle <= -175)Eny_Gr_Num += 12;	//	左横
	else if (Qtr_Angle <= 5 && Qtr_Angle >= -5)Eny_Gr_Num += 6;			//	右横
	else if (Qtr_Angle > -175 && Qtr_Angle < -95)Eny_Gr_Num += 9;		//	左斜め上
	else if (Qtr_Angle >= -95 && Qtr_Angle <= -85)Eny_Gr_Num += 0;		//	後ろ向き
	else if (Qtr_Angle > -85 && Qtr_Angle < -5)Eny_Gr_Num += 3;			//	右斜め上

	Camera_X = Ply->Get_Camera_X();
	Camera_Y = Ply->Get_Camera_Y();
	
	//	描画テーブルに登録
	float X = (float)(Qtr_X + QTR_OFFSET_X - Camera_X);
	float Y = (float)(Qtr_Y + QTR_OFFSET_Y - Camera_Y);
	//	ウィンドウ内なら登録
	if (X <= WINDOW_X && X >= 0 && Y <= WINDOW_Y && Y >= 0) {
		My_Draw->Register(X, Y, H_Size_X, H_Size_Y, ENEMY, Eny_Gr[Eny_Gr_Num]);
	}
}

//	ステータスの更新
void Enemy_Boss::Status_Update()
{
	//	ステータスがダメージ場合
	if (Status.Is(DAMAGE)) {
		//	カウントが０でなければデクリメント
		if (Damage_Cnt > 0) {
			Damage_Cnt--;
		}
		//	０以下ならステータスをノーマルにする
		else {
			Status.Reset();
			Status.On(NORMAL);
		}
	}
	//	死んでいる場合
	else if (Status.Is(DYING)) {
		//	カウントが０でなければデクリメント
		if (Dying_Cnt > 0) {
			Dying_Cnt--;
		}
		//	０以下ならステータスを死亡にする
		else {
			Status.Reset();
			Status.On(DIE);
			Ply->Set_Clear_Flg(true);
		}

		//	制限時間を超えていたらクリアにする
		if (My_Time->Get_Time_Limit() < 0) {
			Ply->Set_Clear_Flg(true);
		}
	}
}