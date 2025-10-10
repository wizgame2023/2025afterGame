/*!
@file Bullet.cpp
@brief ’e‚ÌŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"
#include "Bullet.h"

namespace basecross {
	Bullet::Bullet(const shared_ptr<Stage>& stagePtr) :
		Actor(stagePtr)
	{

	}

	Bullet::~Bullet()
	{

	}

	void Bullet::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(0.5f,0.0f,0.0f));
		trans->SetQuaternion(Quat(0.0f,0.0f,0.0f,1.0f));
		trans->SetScale(Vec3(0.5f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
	
		AddTag(L"Bullet");
	}

	void Bullet::OnUpdate()
	{

	}

	// “–‚½‚è”»’è
	void Bullet::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		
	}

}
//end basecross