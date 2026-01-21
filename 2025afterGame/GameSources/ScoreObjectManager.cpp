/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	ScoreObjectManager::ScoreObjectManager()
		:
		m_count(0)
	{
	}



	ScoreObjectManager::~ScoreObjectManager()
	{

	}

	unique_ptr<ScoreObjectManager, ScoreObjectManager::ScoreObjectManagerDeleter> ScoreObjectManager::m_scoreObjectManager;

	unique_ptr<ScoreObjectManager, ScoreObjectManager::ScoreObjectManagerDeleter>& ScoreObjectManager::CreateScoreObjectManager()
	{
		try
		{
			if (m_scoreObjectManager.get() == 0)
			{
				//自分を作成
				m_scoreObjectManager.reset(new ScoreObjectManager());

				//初期化
				m_scoreObjectManager->OnCreate();
			}
			return m_scoreObjectManager;
		}
		catch (...)
		{
			throw;
		}
		return m_scoreObjectManager;
	}

	unique_ptr<ScoreObjectManager, ScoreObjectManager::ScoreObjectManagerDeleter>& ScoreObjectManager::GetScoreObjectManager()
	{
		return m_scoreObjectManager;
	}

	void ScoreObjectManager::OnCreate()
	{
		auto& manager = StageCreateManager::GetStageCreateManager();
		int roop = manager->GetScoreAnchorCount();

		for (int a = 0; a < roop; a++)
		{
			m_current.push_back(false);
		}
	}

	void ScoreObjectManager::CreateScoreObject(Vec3 pos, Vec3 rot, Vec3 siz, int tag)
	{
		if (m_count < 3 && m_current[tag] != true)
		{
			m_count++;
			m_current[tag] = true;
			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			stage->AddGameObject<ScoreObject>(pos, rot, siz, tag);
		}
	}

	void ScoreObjectManager::RemoveObject(int id)
	{
		m_current[id] = false;
		m_count--;
	}
}
//end basecross
