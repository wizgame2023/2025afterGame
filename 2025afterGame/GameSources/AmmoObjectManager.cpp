/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	AmmoObjectManager::AmmoObjectManager()
		:
		m_count(0)
	{
	}



	AmmoObjectManager::~AmmoObjectManager()
	{

	}

	unique_ptr<AmmoObjectManager, AmmoObjectManager::AmmoObjectManagerDeleter> AmmoObjectManager::m_ammoObjectManager;

	unique_ptr<AmmoObjectManager, AmmoObjectManager::AmmoObjectManagerDeleter>& AmmoObjectManager::CreateAmmoObjectManager()
	{
		try
		{
			if (m_ammoObjectManager.get() == 0)
			{
				//自分を作成
				m_ammoObjectManager.reset(new AmmoObjectManager());

				//初期化
				m_ammoObjectManager->OnCreate();
			}
			return m_ammoObjectManager;
		}
		catch (...)
		{
			throw;
		}
		return m_ammoObjectManager;
	}

	unique_ptr<AmmoObjectManager, AmmoObjectManager::AmmoObjectManagerDeleter>& AmmoObjectManager::GetAmmoObjectManager()
	{
		return m_ammoObjectManager;
	}

	void AmmoObjectManager::OnCreate()
	{
		
	}

	void AmmoObjectManager::CreateAmmoObject(Vec3 pos, Vec3 rot, Vec3 siz, int tag)
	{
		if (m_count < 3 && m_current[tag] != true)
		{
			m_count++;
			m_current[tag] = true;
			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			stage->AddGameObject<AmmoObject>(pos, rot, siz, tag);
		}
	}

	void AmmoObjectManager::RemoveObject(int id)
	{
		m_current[id] = false;
		m_count--;
	}

	void AmmoObjectManager::SetVector()
	{
		auto& manager = StageCreateManager::GetStageCreateManager();
		int roop = manager->GetAmmoAnchorCount();

		for (int a = 0; a < roop; a++)
		{
			m_current.push_back(false);
		}
	}
}
//end basecross
