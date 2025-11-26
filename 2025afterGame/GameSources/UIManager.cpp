/*!
@file UIManager.cpp
@brief UI管理の実体
*/

#include "stdafx.h"
#include "Project.h"
#include "UIManager.h"

namespace basecross
{
	UIManager::UIManager()
	{

	}

	UIManager::~UIManager()
	{

	}

	unique_ptr<UIManager, UIManager::UIManagerDeleter> UIManager::m_UIManager;

	// シングルトンによる生成
	unique_ptr<UIManager, UIManager::UIManagerDeleter>& UIManager::CreateUIManager()
	{
		try
		{
			if (m_UIManager.get() == 0)
			{
				// 自分を作成
				m_UIManager.reset(new UIManager());

				// 初期化
				m_UIManager->OnCreate();
			}
			return m_UIManager;
		}
		catch (...)
		{
			throw;
		}

		return m_UIManager;
	}

	// 自分を渡す
	unique_ptr<UIManager, UIManager::UIManagerDeleter>& UIManager::GetUIManager()
	{
		return m_UIManager;
	}


	// 初期化処理
	void UIManager::OnCreate()
	{
		// 入力マネージャーの作成
		InputManager::CreateInputManager();
	}

	// 更新
	void UIManager::OnUpdate()
	{

	}

	// 自分自身の破棄処理
	void UIManager::DeleteUIManager()
	{
		// 子クラスマネージャーの破棄
		DeleteChildManager();

		// 自分自身の破棄
		m_UIManager.reset();
	}

	void UIManager::GetPlayerHP()
	{
		auto& app = App::GetApp();
		auto objets = app->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec();

		for (auto obj : objets)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
		
			if (player)
			{
				player->GetHpCurrent();
			}
		}
	}


}
//end basecross
