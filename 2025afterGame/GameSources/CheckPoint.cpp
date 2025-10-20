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
		m_enemyCheckPointTime(0.0f),
		m_nextCheckPoint(0.0f, 0.0f, 0.0f)
	{

	}

	CheckPoint::~CheckPoint()
	{

	}

	void CheckPoint::OnCreate()
	{
		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void CheckPoint::OnUpdate()
	{
		
	}

	float GetRaceTime(float racetime)
	{
		return racetime;
	}

	float SetEnemyCheckPointTime(float enemycheckpointtime)
	{
		return enemycheckpointtime;
	}

	float GetEnemyCheckPointTime(float enemycheckpointtime,float racetime)
	{
		enemycheckpointtime -= racetime;
		return enemycheckpointtime;
	}

	Vec3 GetNextCheckPoint(Vec3 nextcheckpoint)
	{
		return nextcheckpoint;
	}

	void CheckPoint::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}
}
//end basecross
