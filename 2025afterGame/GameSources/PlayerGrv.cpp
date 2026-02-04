/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	PlayerGrv::PlayerGrv(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr),
		m_flyCount(0),
		m_bindPos(Vec3(0, -.5, 0)),
		m_defaultRadius(.1f),
		m_isGrounded(true)
	{

	}

	PlayerGrv::PlayerGrv(const shared_ptr<Stage>& stagePtr, float collidecnt) :
		MyGameObject(stagePtr)
	{

	}

	PlayerGrv::~PlayerGrv()
	{

	}

	void PlayerGrv::OnCreate()
	{
		m_collision = AddComponent<CollisionSphere>();

		m_collision->SetAfterCollision(AfterCollision::None);
		m_collision->SetMakedRadius(m_defaultRadius);
		GetComponent<Transform>()->SetPosition(m_bindPos);

		m_collision->SetDrawActive(false);
	}

	void PlayerGrv::OnUpdate()
	{
		if (m_isGrounded)
		{
			m_flyCount--;
			if (m_flyCount <= 0)
			{
				m_isGrounded = false;
			}
		}
	}

	void PlayerGrv::OnCollisionExcute(shared_ptr<GameObject>& other)
	{
		if (other->FindTag(L"CameraObsDiffuse"))
		{
			m_isGrounded = true;
			m_flyCount = 5;
		}
	}

	bool PlayerGrv::GetLand()
	{
		return m_isGrounded;
	}

	void PlayerGrv::SetBindPos(Vec3 pos)
	{
		GetComponent<Transform>()->SetPosition(pos);
	}

	void PlayerGrv::SetCollScale(float scale)
	{
		GetComponent<CollisionSphere>()->SetMakedRadius(scale);
	}
}
//end basecross
