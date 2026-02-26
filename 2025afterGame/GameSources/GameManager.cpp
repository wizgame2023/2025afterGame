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

	unique_ptr<GameManager, GameManager::GameManagerDeleter> GameManager::m_gameManager;

	// シングルトンによる生成
	unique_ptr<GameManager, GameManager::GameManagerDeleter>& GameManager::CreateGameManager()
	{
		try
		{
			if (m_gameManager.get() == 0)
			{
				// 自分を作成
				m_gameManager.reset(new GameManager());

				// 初期化
				m_gameManager->OnCreate();
			}
			return m_gameManager;
		}
		catch(...)
		{
			throw;
		}

		return m_gameManager;
	}

	// 自分を渡す
	unique_ptr<GameManager, GameManager::GameManagerDeleter>& GameManager::GetGameManager()
	{
		return m_gameManager;
	}


	// 初期化処理
	void GameManager::OnCreate()
	{
		// 入力マネージャーの作成
		InputManager::CreateInputManager();
		ScoreManager::CreateScoreManager();
		//UIManager::CreateUIManager();

		m_gameStartFlag = false;
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
			if (!m_pauseActive)
			{
				m_timeGamePlaying += m_deltaTime;
				m_timeLimit -= m_deltaTime;
			}
		}

		if (m_timeLimit < 0.0f)
		{
			SetGameEnd(true);
			Pose(true);

			if (!m_endDraw)
			{
				m_endSprite = m_currentStage->AddGameObject<Sprite>(L"GameEnd_TX", Vec2(500.0f, 250.0f));
				m_endDraw = true;
			}

			if (m_endDraw)
			{
				m_endDrawTime -= m_deltaTime;
			}

			if (m_endDrawTime <= 0.0f)
			{
				m_currentStage->RemoveGameObject<Sprite>(m_endSprite);
				m_resultDrawActive = true;
			}
		}

		// ゲーム開始時のカウントダウン処理
		if (m_countDown && !m_gameStartFlag)
		{
			CountDown(true);
		}
  
		NowPhase();

		// 入力マネージャーの更新
		InputManager::GetInputManager()->Update();

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
				m_audioManager = App::GetApp()->GetXAudio2Manager();
				m_se = m_audioManager->Start(L"CountDownSE", 0, GetSEVolume());
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
				SetCountEnd(true);

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
				m_gameStartFlag = true;
				m_countTimeGameStart = 0.0f; // 終わったら時間計測リセット
				
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
			m_pauseActive = true;

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
			m_pauseActive = false;

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
		m_gameManager.reset();
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
			{
				L"配列外の物を指定しようとしています。\n",
				L"if(number > m_checkPoints.size() - 1 || number < 0)",
				L"GameManager::GetCheckPoint(int number)"
			};

			// ↓これにしたいのにエラーが出てしまうため英文にしている
				//L"配列外の物を指定しようとしています。",
				//L"if(number > m_checkPoints.size() - 1 || number < 0)",
				//L"GameManager::GetCheckPoint(int number)"
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

	void GameManager::SetTimeLimit(float limit)
	{
		m_timeLimit = limit;
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
  
	void GameManager::ChangePhase(GamePhase nowPhase)
	{
		m_phase = nowPhase;

		if (m_phase == GamePhase::Score)
		{
			m_createScoreObj = false;
			m_scoreObjecCout = 0;
		}
		else if (m_phase == GamePhase::Item)
		{
			m_itemPhaseLimit = 5.0f;
		}
	}

	void GameManager::NowPhase()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		auto& obj = StageCreateManager::GetStageCreateManager();

		if (dynamic_pointer_cast<TitleStage>(stage) != nullptr || dynamic_pointer_cast<SelectStage>(stage) != nullptr) return;

		if (m_phase == GamePhase::Score)
		{	
			if (!m_createScoreObj)
			{
				obj->CreateScoreObject();
				obj->CreateScoreObject();
				obj->CreateScoreObject();
				obj->CreateScoreObject();
				obj->CreateScoreObject();
				obj->CreateScoreObject();
				obj->CreateScoreObject();
				m_createScoreObj = true;
			}

			if (m_phase == GamePhase::Score && m_scoreObjecCout == 0)
			{
				ChangePhase(GamePhase::Item);
			}
		}

		if(m_phase == GamePhase::Item)
		{
			if (m_itemObj)
			{
				obj->CreateAmmoObject();
				obj->CreateItemObject();
				m_itemObj = false;
			}

			m_itemPhaseLimit -= 1.0f * m_deltaTime;

			if (m_itemPhaseLimit <= 0.0f)
			{
				m_itemObj = true;
				ChangePhase(GamePhase::Score);
			}
		}
	}

	GamePhase GameManager::GetCurrentPhase()
	{
		return m_phase;
	}

	void GameManager::AddscoreObjecCout()
	{
		m_scoreObjecCout++;
	}

	void GameManager::RemoveScoreObjectCout()
	{
		m_scoreObjecCout--;
	}

	void GameManager::SetCreateScoreObjFlag(bool createFlag)
	{
		m_createScoreObj = createFlag;
	}

	bool GameManager::GetCreateScoreObjFlag()
	{
		return m_createScoreObj;
	}
	 
	int  GameManager::GetGameStartCountDown()
	{
		return m_gameStartPhase;
	}

	void GameManager::SetCountEnd(bool flag)
	{
		m_countEnd = flag;
	}

	bool GameManager::GetCountEnd()
	{
		return m_countEnd;
	}
	
	void GameManager::SetGameEnd(bool flag)
	{
		m_gameEnd = flag;
	}

	bool GameManager::GetGameEnd()
	{
		return m_gameEnd;
	}

	// ゲーム終了時のゲームマネージャリセット処理
	void GameManager::ResetGameManager()
	{
		SetGameEnd(false);
		SetTimeLimit(180.0f);

		m_resultDrawActive = false;
		m_countDown = false;
		m_gameStartFlag = false;
		m_countDownSEFlag = true;
		m_gameStartPhase = GAMESTART_Start;
		m_countTimeGameStart = 0.0f;
		m_itemObj = false;
		m_createScoreObj = false;
		m_scoreObjecCout = 0;
		ChangePhase(GamePhase::Score);

		// スコアオブジェクトを管理するマネージャの初期化
		auto& scoreObjectManager = ScoreObjectManager::GetScoreObjectManager();
		scoreObjectManager->ResetObject();

		// アイテムオブジェクトを管理するマネージャの初期化
		auto& repairObjectManager = RepairObjectManager::GetRepairObjectManager();
		repairObjectManager->ResetObject();

		// 弾オブジェクトを管理するマネージャの初期化
		auto& ammoObjectManager = AmmoObjectManager::GetAmmoObjectManager();
		ammoObjectManager->ResetObject();

		auto& uiManager = UIManager::GetUIManager();
		uiManager->ForceRefreshOperationUI();
	}

	void GameManager::SetResultDrawActive(bool flag)
	{
		m_resultDrawActive = flag;
	}

	bool GameManager::GetResultDrawActive()
	{
		return m_resultDrawActive;
	}
}

//end basecross