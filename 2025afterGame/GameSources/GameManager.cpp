/*!
@file GameManager.cpp
@brief ゲーム内部を管理するマネージャー
*/

#include "stdafx.h"
#include "Project.h"


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

	void GameManager::OnUpdate()
	{
		// デルタタイム取得
		auto& app = App::GetApp();
		m_deltaTime = app->GetElapsedTime();

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

}

//end basecross