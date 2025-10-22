/*!
@file CheckPoint.cpp
@brief �`�F�b�N�|�C���g�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	CheckPoint::CheckPoint(const shared_ptr<Stage>& stageptr) :
		Actor(stageptr,Vec3(0.0f, 0.0f, 10.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f)),
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
		//初期位置設定
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(Vec3(0.0f, 0.0f, 10.0f));
		m_trans->SetQuaternion(Quat(0.0f, 0.0f, 0.0f, 1.0f));
		m_trans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		/*m_trans->SetPosition(m_pos);
		m_trans->SetQuaternion(m_qt);
		m_trans->SetScale(m_scale);*/

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);
		ptrCol->SetAfterCollision(AfterCollision::None);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void CheckPoint::OnUpdate()
	{
		auto& game = GameManager::GetGameManager();
		m_raceTime = game->GetTimeGamePlaying();

		m_pos;
		m_qt;
		m_scale;
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
		auto& checkPoint = game->GetCheckPoint(m_checkPointID + 1);
		Vec3 newPoint = checkPoint->GetComponent<Transform>()->GetPosition();
		return newPoint;
	}
}
//end basecross
