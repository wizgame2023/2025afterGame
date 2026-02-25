/*!
@file GameManaegr.h
@brief ゲーム内部を管理するマネージャー
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	enum class GamePhase
	{
		Item,
		Score,
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

		static unique_ptr<GameManager, GameManagerDeleter> m_gameManager;

		float m_deltaTime = 0.0f; // 次のフレームに移動するまでの時間
		int m_gameStageNow = 0; // 現在のゲームステージの変数

		bool m_gameStartFlag; // ゲームが開始しているかのフラグ
		float m_timeGamePlaying = 0.0f; // ゲームの経過時間
		float m_timeLimit = 180.0f;
		float m_itemPhaseLimit = 5.0f;
		float m_endDrawTime = 3.0f;
		bool m_endDraw = false;
		bool m_resultDrawActive = false;
		int m_scoreObjecCout = 0;
		bool m_createScoreObj = false;
		bool m_itemObj = true;
		bool m_countEnd = false;
		bool m_gameEnd = false;
		float m_countTimeGameStart = 0.0f; //カウントダウンの経過時間
		GamePhase m_phase = GamePhase::Score; // 最初はItmeフェーズから

		shared_ptr<Stage> m_currentStage; // 現在ステージ


		// 現在ポーズしているMyGameObjectを保存する配列
		vector<weak_ptr<MyGameObject>> m_myGameObjectVec;
		bool m_pauseActive = false;

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

		enum class KeyType
		{
			Accel,
			Bullet,
			ViewBehind,
			Max
		};

		// カウントダウンの状態
		bool m_countDown;
		bool m_countDownSEFlag = true;
		int m_gameStartPhase = GAMESTART_Start;
		shared_ptr<Sprite> m_startSprite; // カウントダウン時のStartスプライト
		shared_ptr<Sprite> m_endSprite; // 時間制限が0になったら

		// BGM,SE音量
		float m_BGMVolume = 1.0f;
		float m_SEVolume = 1.0f;

		// BGM、SE用
		shared_ptr<XAudio2Manager> m_audioManager;
		shared_ptr<SoundItem> m_se;

		bool m_upDownSwap = false;
		wstring m_bulletKey = L"RTrigger";
		wstring m_viewBehindKey = L"Y";
		wstring m_accelKey = L"A";
 
	public:
		float GetBGMVolume() const { return m_BGMVolume; };
		void SetBGMVolume(const float volume) { m_BGMVolume = volume; };
		float GetSEVolume() const { return m_SEVolume; };
		void SetSEVolume(const float volume) { m_SEVolume = volume; };
		bool GetUpDownSwapFlag() const { return m_upDownSwap; };
		void SetUpDownSwapFlag(const bool flag) { m_upDownSwap = flag; };
		wstring GetBulletKey() const { return m_bulletKey; };
		wstring GetViewBehindKey() const { return m_viewBehindKey; };
		wstring GetAccelKey() const { return m_accelKey; };

		void SetKey(unsigned int keyType, const wstring& key)
		{
			if (keyType >= static_cast<int>(KeyType::Max)) return;

			KeyType localKeyType = static_cast<KeyType>(keyType);

			switch (localKeyType)
			{
				case KeyType::Accel:		m_accelKey = key; break;
				case KeyType::Bullet:		m_bulletKey = key; break;
				case KeyType::ViewBehind:	m_viewBehindKey = key; break;
				default:
					break;
			};
		}

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

		// 時間の設定
		void SetTimeLimit(float limit);

		// 残り時間の取得
		float GetTimeLimit();
		
		// カウントダウンを開始するフラグのセッタ
		void SetCountDown(bool onOff);

		// フェーズの切り替えに使う関数
		void ChangePhase(GamePhase nowPhase);

		// フェーズを切り替える条件
		void NowPhase();

		// 現在フェーズの取得
		GamePhase GetCurrentPhase();

		// ScoreObjectの生成した数を数える関数
		void AddscoreObjecCout();

		// ScoreObjectを取得した時の関数
		void RemoveScoreObjectCout();

		// m_createScoreObjのセッタ
		void SetCreateScoreObjFlag(bool createFlag);

		// m_createScoreObjのゲッタ
		bool GetCreateScoreObjFlag();

		// カウントダウンの取得
		int GetGameStartCountDown();

		// Countこれが終わったらSet
		void SetCountEnd(bool flag);
		
		// Countが終わったかの取得
		bool GetCountEnd();

		// Gameが終わったらSet
		void SetGameEnd(bool flag);

		// Gameが終わっているかの取得
		bool  GetGameEnd();
		
		// 情報をリセット
		void ResetGameManager();

		// リザルトを出すかをSet
		void SetResultDrawActive(bool flag);

		// リザルトを出すかを取得
		bool GetResultDrawActive();
	};
}
//end basecross

