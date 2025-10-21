/*!
@file CheckPoint.cpp
@brief �`�F�b�N�|�C���g�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	CheckPoint::CheckPoint(const shared_ptr<Stage>& stageptr) :
		Actor(stageptr),
		m_raceTime(0.0f),
		m_previewTime(0.0f),
		m_checkPointID(0)
	{

	}

	CheckPoint::~CheckPoint()
	{

	}

	void CheckPoint::OnCreate()
	{
		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(false);
		ptrCol->SetAfterCollision(AfterCollision::None);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void CheckPoint::OnUpdate()
	{
		auto& game = GameManager::GetGameManager();
		m_raceTime = game->GetTimeGamePlaying();
	}

	void CheckPoint::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto playermachine = dynamic_pointer_cast<Player>(obj);

		if (playermachine)
		{
			if (m_previewTime > 0.0f)
			{
				SetDifferenceTime();
			}
			else
			{
				m_previewTime = m_raceTime;
			}
		}

		SetNextCheckPoint();
	}

	float CheckPoint::SetDifferenceTime()
	{
		auto& game = GameManager::GetGameManager();
		float differencetime = m_previewTime - game->GetTimeGamePlaying();
		m_previewTime = game->GetTimeGamePlaying();
		return differencetime;
	}

	void CheckPoint::SetCheckPointID(int id)
	{
		m_checkPointID = id;
	}

	Vec3 CheckPoint::SetNextCheckPoint() 
	{
		auto& game = GameManager::GetGameManager();
		auto& checkpoint = game->GetCheckPoint(m_checkPointID + 1);
		Vec3 newpoint = checkpoint->GetComponent<Transform>()->GetPosition();
		return newpoint;
	}
}
//end basecross
