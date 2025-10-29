/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	StageWall::StageWall(const shared_ptr<Stage>& stagePtr) :
		Actor(stagePtr, Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(100.0f, 100.0f, 10.0f))
	{

	}

	StageWall::~StageWall()
	{

	}

	void StageWall::OnCreate()
	{
		// 親クラス処理
		Actor::OnCreate();

		// Trans処理追加
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetQuaternion(m_qt);
		m_trans->SetScale(m_scale);

		// コリジョン追加
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetFixed(true);
		ptrCol->SetDrawActive(true);
		ptrCol->SetAfterCollision(AfterCollision::Auto);

		// ドロー処理
		//auto ptrDraw = AddComponent<PNTStaticDraw>();
		//ptrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		//SetAlphaActive(false);
	}

	void StageWall::OnUpdate()
	{

	}
}
//end basecross
