/*!
@file GameManaegr.h
@brief ゲーム内部を管理するマネージャー
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	enum class GamePhase
	{
		Itme,
		Score,
		Result,
	};

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class CheckPoint;
	class Sprite;
	class GameManager
	{
	private:
		// 削除処理
		struct GameManagerDeleter
		{
			void operator()(GameManager* p) { delete p; };
		};

		static unique_ptr<GameManager, GameManagerDeleter> m_GameManager;

		float m_deltaTime = 0.0f; // 次のフレームに移動するまでの時間
		int m_gameStageNow = 0; // 現在のゲームステージの変数

		bool m_gameStartFlag = false; // ゲームが開始しているかのフラグ
		float m_timeGamePlaying = 0.0f; // ゲームの経過時間
		float m_timeLimit = 180.0f;
		float m_countTimeGameStart = 0.0f; //カウントダウンの経過時間

		GamePhase m_phase = GamePhase::Itme; // 最初はItmeフェーズから 

		shared_ptr<Stage> m_currentStage; // 現在ステージ

		// BGM音量
		float m_BGMVolume = 1.0f;

		// 現在ポーズしているMyGameObjectを保存する配列
		vector<weak_ptr<MyGameObject>> m_myGameObjectVec;

		//GameStartの流れ
		enum GameStartCount
		{
			GAMESTART_Start,
			GAMESTART_CountDown_One,
			GAMESTART_CountDown_Two,
			GAMESTART_CountDown_Three,
			GAMESTART_CountDown_Start,
			GAMESTART_End
		};

		// カウントダウンの状態
		bool m_countDown = false;
		bool m_countDownSEFlag = true;
		int m_gameStartPhase = GAMESTART_Start;
		shared_ptr<Sprite> m_startSprite; // カウントダウン時のStartスプライト

		// BGM、SE用
		shared_ptr<XAudio2Manager> m_AudioManager;
		shared_ptr<SoundItem> m_se;
	public:
		float GetBGMVolume() const { return m_BGMVolume; };
		void SetBGMVolume(const float volume) { m_BGMVolume = volume; };

		//構築と破棄
		GameManager();
		virtual ~GameManager();

		// ゲームマネージャーを作成
		static unique_ptr<GameManager, GameManagerDeleter>& CreateGameManager();

		// ゲームマネージャーを取得
		static unique_ptr<GameManager, GameManagerDeleter>& GetGameManager();

		// チェックポイントを管理するメンバ配列
		vector<shared_ptr<CheckPoint>> m_checkPoints;

		// 破棄処理
		void DeleteGameManager();

		// 子マネージャー破棄処理
		void DeleteChildManager();


		//初期化
		virtual void OnCreate();
		virtual void OnUpdate();

		// ゲーム開始のカウントダウン
		void CountDown(bool StartEnd);

		// ポーズ処理
		void Pose(bool OnOff);
		
		// デルタタイムのゲッタ
		float GetDeltaTime();

		// gameStageNowのゲッタ
		int GetGameStageNow();
		// gameStageNowのセッタ
		void SetGameStageNow(int gameStageNow);

		// m_gameStartFlagのゲッタ
		bool GetGameStartFlag();
		// m_gameStartFlagのセッタ
		void SetGameStartFlag(bool gameStartFlag);

		// m_timeGamePlayingのゲッタ
		float GetTimeGamePlaying();

		// m_checkPointsのゲッタ
		shared_ptr<CheckPoint> GetCheckPoint(int number);
		// m_checkPointsの追加関数
		void AddCheckPoint();// 後々、チェックポイントの位置とかを指定する引数入れたい

		// m_chackPointsのサイズ取得
		int GetChackPointsSize();

		// チェックポイント配列の初期化
		void ResetCheckPoint();

		// 残り時間の取得
		float GetTimeLimit();

		// カウントダウンを開始するフラグのセッタ
		void SetCountDown(bool onOff);
	};


}
//end basecross

