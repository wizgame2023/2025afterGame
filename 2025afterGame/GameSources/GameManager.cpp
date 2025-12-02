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
	}

	// 更新
	void GameManager::OnUpdate()
	{
		// デルタタイム取得
		auto& app = App::GetApp();
		m_deltaTime = app->GetElapsedTime();

		// ゲーム経過時間を計測
		if (m_gameStartFlag)
		{
			m_timeGamePlaying += m_deltaTime;
			m_timeLimit -= m_deltaTime;
		}

		// 入力マネージャーの更新
		InputManager::GetInputManager()->Update();
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

}

//end basecross