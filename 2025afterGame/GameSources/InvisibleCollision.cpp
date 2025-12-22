/*!
@file InvisibleCollsion.cpp
@brief Œ©‚¦‚È‚¢“–‚½‚è”»’è
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	InvisibleCollision::InvisibleCollision(const shared_ptr<Stage>& stagePtr,
		const Vec3& pos,
		const Vec3& rot,
		const Vec3& siz,
		const Col4& color
	) :
		Actor(stagePtr,pos,rot,siz,color)
	{
		try
		{

		}
		catch (...)
		{
			throw;
		}
	}

	InvisibleCollision::~InvisibleCollision(){

	}

	void InvisibleCollision::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_scale);
		PtrTrans->SetRotation(m_rot);
		PtrTrans->SetPosition(m_pos);

		//ƒRƒŠƒWƒ‡ƒ“
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetFixed(true);
		ptrCol->SetDrawActive(true);
		ptrCol->SetAfterCollision(AfterCollision::Auto);
	}
}
//end basecross
