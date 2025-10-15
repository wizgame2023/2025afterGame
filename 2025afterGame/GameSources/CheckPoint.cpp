/*!
@file CheckPoint.cpp
@brief �`�F�b�N�|�C���g�̎���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	CheckPoint::CheckPoint(const shared_ptr<Stage>& stageptr) :
		Actor(stageptr)
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

	void GetRaceTime(float racetime)
	{
		return racetime;
	}

	void GetEnemyCheckPointTime(float enemycheckpointtime)
	{
		return enemycheckpointtime;
	}

	void GetNextCheckPoint(Vec3 nextcheckpoint)
	{
		return nextcheckpoint;
	}

	// �����蔻��
	void CheckPoint::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}
}
//end basecross
