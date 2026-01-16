/*!
@file GameManager.cpp
@brief ゲーム内部を管理するマネージャー
*/

#include "stdafx.h"
#include "Project.h"
#include "GameManager.h"


namespace basecross {

	GameManager::GameManager()
	{
		
	}

	GameManager::~GameManager()
	{

	}

	unique_ptr<GameManager, GameManager::GameManagerDeleter> GameManager::m_GameManager;

	// シングルトンによる生成
	unique_ptr<GameManager, GameManager::GameManagerDeleter>& GameManager::CreateGameManager()
	{
		try
		{
			if (m_GameManager.get() == 0)
			{
				// 自分を作成
				m_GameManager.reset(new GameManager());

				// 初期化
				m_GameManager->OnCreate();
			}
			return m_GameManager;
		}
		catch(...)
		{
			throw;
		}

		return m_GameManager;
	}

	// 自分を渡す
	unique_ptr<GameManager, GameManager::GameManagerDeleter>& GameManager::GetGameManager()
	{
		return m_GameManager;
	}


	// 初期化処理
	void GameManager::OnCreate()
	{
		// 入力マネージャーの作成
		InputManager::CreateInputManager();
		ScoreManager::CreateScoreManager();
		UIManager::CreateUIManager();
	}

	// 更新
	void GameManager::OnUpdate()
	{
		// デルタタイム取得
		auto& app = App::GetApp();
		m_deltaTime = app->GetElapsedTime();

		// 現在使用しているステージを受け取る
		m_currentStage = app->GetScene<Scene>()->GetActiveStage();

		// ゲーム経過時間を計測
		if (m_gameStartFlag)
		{
			m_timeGamePlaying += m_deltaTime;
			m_timeLimit -= m_deltaTime;
		}

		// カウントダウン処理
		if (m_countDown && !m_gameStartFlag)
		{
			CountDown(true);
		}

		// 入力マネージャーの更新
		InputManager::GetInputManager()->Update();

		UIManager::GetUIManager()->OnUpdate();
	}

	// ゲーム開始のカウントダウン
	// 引数１がどのときにカウントダウンをするかを伝える処理　tureがStart falseがEnd
	void GameManager::CountDown(bool StartEnd)
	{
		m_countTimeGameStart += m_deltaTime;

		// カウントダウンが終わったらゲームを開始する
		if (m_gameStartPhase == GAMESTART_Start)
		{
			// ポーズ開始
			Pose(true);
			// 1を表示させるフェーズに移動
			m_gameStartPhase = GAMESTART_CountDown_One;
		}

		if (m_gameStartPhase == GAMESTART_CountDown_One)
		{
			// 一秒置いた後にSEを鳴らす
			if (m_countTimeGameStart >= 1.0f && m_countDownSEFlag)
			{
				// BGM、SE用のマネージャー作成
				m_AudioManager = App::GetApp()->GetXAudio2Manager();
				m_se = m_AudioManager->Start(L"CountDownSE", 0, 0.6f);
				m_countDownSEFlag = false;// なんどもSEを鳴らさない

			}

			if (m_countTimeGameStart >= 2.0f)
			{
				// 2を表示させるフェーズに移動
				m_gameStartPhase = GAMESTART_CountDown_Two;
			}
		}


		if (m_gameStartPhase == GAMESTART_CountDown_Two)
		{
			if (m_countTimeGameStart >= 3.0f)
			{
				// 3を表示させるフェーズに移動
				m_gameStartPhase = GAMESTART_CountDown_Three;
			}
		}

		if (m_gameStartPhase == GAMESTART_CountDown_Three)
		{
			if (m_countTimeGameStart > 4.0f)
			{
				//Startを出すフェーズに移動
				if (StartEnd)
				{
					m_startSprite = m_currentStage->AddGameObject<Sprite>(L"GameStart_TX", Vec2(500.0f, 250.0f));
				}
				else if(!StartEnd)
				{
					m_startSprite = m_currentStage->AddGameObject<Sprite>(L"GameEnd_TX", Vec2(500.0f, 250.0f));
				}

				m_gameStartPhase = GAMESTART_End;
			}
		}


		if (m_gameStartPhase == GAMESTART_End)
		{
			//ある程度Startのテクスチャを見せたら
			if (m_countTimeGameStart > 5.0f)
			{
				// ポーズ状態を解除する
				auto test = 0;
				m_countDown = false; // カウントダウンの使用状態を解除
				m_countDownSEFlag = true; // SEも使用可能に
				
				// ポーズ解除
				Pose(false);

				// Startスプライトの削除
				m_currentStage->RemoveGameObject<Sprite>(m_startSprite);
			}
		}

	}

	// ポーズ処理
    // 引数１　ポーズ状態にするかの確認trueがポーズにするfalseで解除
	void GameManager::Pose(bool OnOff)
	{
		// ポーズ開始
		if (OnOff)
		{
			// MyGameObjectの物を全て停止する
			auto objVec = m_currentStage->GetGameObjectVec();
			//アクターを継承しているものだけ取得
			for (auto obj : objVec)
			{
				auto myGameObjectCast = dynamic_pointer_cast<MyGameObject>(obj);

				//アクターを継承しているオブジェクト停止
				if (myGameObjectCast)
				{
					myGameObjectCast->SetPauseFlag(true);// ポーズ状態にする
					m_myGameObjectVec.push_back(myGameObjectCast);
				}
			}
		}

		// ポーズ終了
		if (!OnOff)
		{
			for (auto obj : m_myGameObjectVec)
			{
				auto gameObjectCheck = obj.lock();
				// ポーズ状態終了によって動けるようになる
				if (gameObjectCheck)
				{
					gameObjectCheck->SetPauseFlag(false);
				}
			}
		}

	}

	// 自分自身の破棄処理
	void GameManager::DeleteGameManager()
	{	
		// 子クラスマネージャーの破棄
		DeleteChildManager();

		// 自分自身の破棄
		m_GameManager.reset();
	}

	// 子マネージャーの破棄処理
	void GameManager::DeleteChildManager()
	{
		// 入力マネージャーの破棄
		InputManager::DeleteInputManager();
	}

	// DeltaTimeのゲッタ
	float GameManager::GetDeltaTime()
	{
		return m_deltaTime;
	}

	int GameManager::GetGameStageNow()
	{
		return m_gameStageNow;
	}

	void GameManager::SetGameStageNow(int gameStageNow)
	{
		m_gameStageNow = gameStageNow;
	}

	// m_gameStartFlagのゲッタ
	bool GameManager::GetGameStartFlag()
	{
		return m_gameStartFlag;
	}

	// m_gameStartFlagのセッタ
	// 第一引数 ゲームが開始しているかのフラグ
	void GameManager::SetGameStartFlag(bool gameStartFlag)
	{
		m_gameStartFlag = gameStartFlag;
	}

	// timeGamePlayingのゲッタ
	float GameManager::GetTimeGamePlaying()
	{
		return m_timeGamePlaying;
	}

	// m_checkPointsのゲッタ
	// 第一引数 何番目のチェックポイントを受け取るかの番号
	shared_ptr<CheckPoint> GameManager::GetCheckPoint(int number)
	{
		// もし、配列外を取得しようとしていたら間違っているとエラーを出す
		if (number > m_checkPoints.size() - 1 || number < 0)
		{
			throw BaseException
			(
				L"配列外の物を指定しようとしています。",
				L"if(number > m_checkPoints.size() - 1 || number < 0)",
				L"GameManager::GetCheckPoint(int number)"
			);
		}

		return m_checkPoints[number];
	}

	// m_checkPointsの追加関数
	void GameManager::AddCheckPoint()
	{
		// ステージを取得してから生成する
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		if (!dynamic_pointer_cast<GameStage>(stage))
		{
			auto test = 0;
		}
		auto addCheckPointObj = stage->AddGameObject<CheckPoint>();

		// チェックポイント管理配列に追加
		m_checkPoints.push_back(addCheckPointObj);

		// 生成したオブジェクトに自分の配列番号を渡す
		addCheckPointObj->SetCheckPointID(m_checkPoints.size());
	}

	// m_chackPointsのサイズ取得
	int GameManager::GetChackPointsSize()
	{
		return m_checkPoints.size();
	}

	// チェックポイント配列の初期化
	void GameManager::ResetCheckPoint()
	{
		m_checkPoints.clear();
	}

	float GameManager::GetTimeLimit()
	{
		return m_timeLimit;
	}

	// カウントダウンを開始するフラグのセッタ
	void GameManager::SetCountDown(bool onOff)
	{
		m_countDown = onOff;
	}

}

//end basecross