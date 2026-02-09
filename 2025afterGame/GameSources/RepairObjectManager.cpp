/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	RepairObjectManager::RepairObjectManager()
		:
		m_count(0)
	{
	}



	RepairObjectManager::~RepairObjectManager()
	{

	}

	unique_ptr<RepairObjectManager, RepairObjectManager::AmmoObjectManagerDeleter> RepairObjectManager::m_repairObjectManager;

	unique_ptr<RepairObjectManager, RepairObjectManager::AmmoObjectManagerDeleter>& RepairObjectManager::CreateRepairObjectManager()
	{
		try
		{
			if (m_repairObjectManager.get() == 0)
			{
				//自分を作成
				m_repairObjectManager.reset(new RepairObjectManager());

				//初期化
				m_repairObjectManager->OnCreate();
			}
			return m_repairObjectManager;
		}
		catch (...)
		{
			throw;
		}
		return m_repairObjectManager;
	}

	unique_ptr<RepairObjectManager, RepairObjectManager::AmmoObjectManagerDeleter>& RepairObjectManager::GetRepairObjectManager()
	{
		return m_repairObjectManager;
	}

	void RepairObjectManager::OnCreate()
	{
		
	}

	void RepairObjectManager::CreateRepairObject(Vec3 pos, Vec3 rot, Vec3 siz, int tag)
	{
		if (m_count < 3 && m_current[tag] != true)
		{
			m_count++;
			m_current[tag] = true;
			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			stage->AddGameObject<ItemObject>(pos, rot, siz, tag);
		}
	}

	void RepairObjectManager::RemoveObject(int id)
	{
		m_current[id] = false;
		m_count--;
	}

	// オブジェクトの配置処理を全て初期化する関数
	void RepairObjectManager::ResetObject()
	{
		for (int i = 0; i < m_current.size() - 1; i++)
		{
			m_current[i] = false;
		}
		m_count = 0;
	}

	void RepairObjectManager::SetVector()
	{
		auto& manager = StageCreateManager::GetStageCreateManager();
		int roop = manager->GetRepairAnchorCount();

		for (int a = 0; a < roop; a++)
		{
			m_current.push_back(false);
		}
	}
}
//end basecross
