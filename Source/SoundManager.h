#pragma once

#include "DxLib.h"		// DXライブラリの本体

const int FADE_RATE = 5;	// フェードのスピード
const int FADE_MAX = 100;

const int BGM_NUM = 19;	// BGMの数

// BackGroundMusicのナンバー
const enum BGM {
	BGM_PLAY,			//	プレイ中通常時
	BGM_PLAY_FOUND,		//	プレイ中敵に発見時
	BGM_OVER,			//	ゲームオーバー
	BGM_CLEAR,			//	ゲームクリア
	BGM_RANKING_IN,		//	ランクイン
	BGM_RANKING_OUT,	//	ランクアウト
	BGM_TITLE,			//	タイトル
	BGM_MENU            //	メニュー ステージ選択や遊び方も含む
};

// BGMのファイル名リスト
static const char *BGM_FILE_NAME[BGM_NUM] = {
	"BGM_PLAY2.mp3",        // 通常
	"BGM_PLAY3.mp3",		// 発見中
	"BGM_OVER.mp3",         // ゲームオーバー 
	"BGM_CLEAR.mp3",        // ゲームクリア
	"BGM_RANKING_IN.mp3",   // ランクイン
	"BGM_RANKING_OUT.mp3",  // ランクアウト
	"BGM_TITLE.mp3",        // タイトル
	"BGM_MENU.mp3"          // メニュー
};

const int SE_NUM = 15;	// SEの数

// SoundEffectのナンバー
const enum SE {
	SE_SELECTION,		//	画面選択
	SE_DECISION,		//	画面決定
	SE_DART_REGISTER,	//	手裏剣登録
	SE_BOMB_REGISTER,	//	爆弾登録
	SE_CLONE_REGISTER,	//	分身登録
	SE_DART_SPARK,		//	手裏剣衝突
	SE_BOMB_SPARK,		//	爆弾衝突
	SE_COLLISION_WALL,	//	壁衝突
	SE_COLLISION,       //	敵とプレイヤー衝突
	SE_ERASE_PLAYER,	//	プレイヤー死亡
	SE_ERASE_ENEMY,		//	敵死亡
	SE_ERASE_BOSS,      //	ボス死亡
	SE_ALERT,			//	発見
	SE_DASH,			//	ダッシュ
	SE_MISS,			//	ゲージが空の時
};

// SEのファイル名リスト
static const char *SE_FILE_NAME[SE_NUM] = {
	"SE_SELECT.mp3",			//	画面選択
	"SE_DECISION.mp3",          //	画面決定
	"SE_DART_REGISTER.mp3",     //	手裏剣登録
	"SE_BOMB_REGISTER.mp3",     //	爆弾登録
	"SE_CLONE_REGISTER.mp3",    //	分身登録
	"SE_DART_SPARK.mp3",	    //	手裏剣衝突
	"SE_BOMB_SPARK.mp3",		//	爆弾衝突
	"SE_COLLISION_WALL.mp3",	//	壁衝突
	"SE_COLLISION.mp3",	        //	敵とプレイヤー衝突
	"SE_ERASE_PLAYER.mp3",      //	プレイヤー死亡
	"SE_ERASE_ENEMY.mp3",       //	敵死亡
	"SE_ERASE_BOSS.mp3",        //	ボス死亡
	"SE_ALERT.mp3",				//	発見
	"SE_DASH.mp3",				//	ダッシュ
	"SE_MISS.mp3",				//	ゲージ空
};

// サウンドマネージャークラス
class Sound_Manager {

	int Bgm[BGM_NUM];	// BGMのハンドラ
	int Se[SE_NUM];		// SEのハンドラ

	int Fade;	// フェード値
	int Bgm_Current;	// フェードインしていく
	int Bgm_Out;		// フェードアウトして停止

public:

	// コンストラクタ
	Sound_Manager() {

		char FullPath[256]; // Soundファイルのある場所へのフルパス

		// BGMテーブルのクリア
		for (int i = 0; i < BGM_NUM; i++) Bgm[i] = 0;	// 読み込んでいない状態にしておいて
		Bgm[BGM_PLAY] = LoadSoundMem("Sound/BGM_PLAY2.mp3");	// 容量の重い曲だけあらかじめ読んでおく
		Bgm[BGM_PLAY_FOUND] = LoadSoundMem("Sound/BGM_PLAY3.mp3");	// 容量の重い曲だけあらかじめ読んでおく

		// SE読み込み
		// SEはファイルも小さいし負担は少ないだろうから全部読み込んでおく
		for (int i = 0; i < SE_NUM; i++) {
			sprintf_s(FullPath, "%s%s", "Sound/", SE_FILE_NAME[i]);	// サウンドファイルへのパスをつくる
			Se[i] = LoadSoundMem(FullPath);	// メモリへ読み込み
			ChangeVolumeSoundMem(120, Se[i]);
		}

		Bgm_Current = BGM_TITLE;	// 起動時に鳴らすBGMのセット
		Fade = 0;	// フェーダーを下げておく
	}

	// デストラクタ
	~Sound_Manager() {
		InitSoundMem();	// 読み込んだ音を全消去
	}

	// 更新　毎フレーム呼ぶこと
	void Update() {

		// クロスフェード処理
		ChangeVolumeSoundMem(Fade, Bgm_Current);	// フェードイン
		ChangeVolumeSoundMem(FADE_MAX - Fade, Bgm_Out);	// フェードアウト
		Fade += FADE_RATE;	// フェード
		if (Fade > FADE_MAX) {	// 超えないように
			Fade = FADE_MAX;	// 最大
			if (CheckSoundMem(Bgm_Out) == 1) {
				StopSoundMem(Bgm_Out);	// フェードしきったらとめて
				//DeleteSoundMem(Bgm_Out);	// いらないので破棄する
			}
		}
	}

	// BGMの再生
	void Change_Bgm(int bgmnum, bool loop, bool fadesw = true) {	// BGM番号, ループするか, フェードインするか

		if (CheckSoundMem(Bgm_Out) == 1) StopSoundMem(Bgm_Out);	// フェード中に変えられたら後ろは止める

		// 曲のロード
		if (Bgm[bgmnum] == 0) {		// もし読み込まれていなかったら
			char FullPath[256];		// Soundファイルのある場所へのフルパス
			sprintf_s(FullPath, "%s%s", "Sound/", BGM_FILE_NAME[bgmnum]);	// サウンドファイルへのパスをつくる
			Bgm[bgmnum] = LoadSoundMem(FullPath);	// メモリへ読み込み
		}

		Bgm_Out = Bgm_Current;	// 再生中の音をフェードアウトへ
		Bgm_Current = Bgm[bgmnum];		// 指定された曲をカレントへ
		int Loop = DX_PLAYTYPE_BACK;		// ループ設定しない
		if (loop) Loop = DX_PLAYTYPE_LOOP;	// ループ設定する
		PlaySoundMem(Bgm_Current, Loop, true);	// カレント曲の再生
		if (fadesw) {	// クロスフェードするか
			// クロスフェードする場合
			ChangeVolumeSoundMem(1, Bgm_Current);	// フェードインする場合はボリュームは絞っておく
			Fade = 0;	// フェーダーの初期化
		}
		else {
			// フェードしない場合
			Fade = FADE_MAX;	// フェーダーはあげておく
		}
	}

	// SEの再生
	void Play_Se(int arg) {
		PlaySoundMem(Se[arg], DX_PLAYTYPE_BACK, true);
	}

};