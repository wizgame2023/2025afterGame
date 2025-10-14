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
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(Vec3(-3.0f,0.0f,1.0f));
		m_trans->SetQuaternion(Quat(0.0f,0.0f,0.0f,-1.0f));
		m_trans->SetScale(Vec3(0.5f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
	
		AddTag(L"Bullet");
	}

	void Bullet::OnUpdate()
	{
		auto delta = App::GetApp()->GetElapsedTime();

		m_pos = m_trans->GetPosition();
		m_pos.x += 1.0f * delta;

		m_trans->SetPosition(m_pos);
	}

	// “–‚½‚è”»’è
	void Bullet::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		
	}


	TestCube::TestCube(const shared_ptr<Stage>& stagePtr) :
		Actor(stagePtr)
	{

	}

	TestCube::~TestCube()
	{

	}

	void TestCube::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(0.5f, 0.0f, 0.0f));
		trans->SetQuaternion(Quat(0.0f, 0.0f, 0.0f, 1.0f));
		trans->SetScale(Vec3(0.5f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void TestCube::OnUpdate()
	{

	}

	// “–‚½‚è”»’è
	void TestCube::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}


}
//end basecross