/*!
@file AttackCollision.cpp
@brief PlayerçUåÇÇÃé¿ëÃ
íSìñÅFãgìc íqãM
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	AttackCollision::AttackCollision(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr),
		m_moveContact(false),
		m_ActiveTime(0.0f),
		m_info()
	{

	}
	AttackCollision::~AttackCollision()
	{

	}


	void AttackCollision::OnCreate()
	{
		m_collision = AddComponent<CollisionCapsule>();	
		m_collision->SetAfterCollision(AfterCollision::None);
	}

	void AttackCollision::OnUpdate()
	{
		auto elapsed = App::GetApp()->GetElapsedTime();

		if (m_ActiveTime > 0)
		{
			m_collision->SetUpdateActive(true);
			m_ActiveTime -= elapsed;
		}
		else
		{
			m_collision->SetUpdateActive(false);
		}
	}

	bool AttackCollision::GetMoveContact()
	{
		return m_moveContact;
	}

	void AttackCollision::SetMoveContact(bool mc)
	{
		m_moveContact = mc;
	}
}
//end basecross
