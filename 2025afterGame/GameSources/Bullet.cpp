/*!
@file Bullet.cpp
@brief 弾の実体
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
		Actor::OnCreate();

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
		// 継承元ののUpdate更新
		Actor::OnUpdate();

		// ここで親オブジェクトの向いている方向を取得しその方向に向かう

		auto delta = App::GetApp()->GetElapsedTime();

		m_pos = m_trans->GetPosition();
		m_pos.x += 1.0f * delta;

		m_trans->SetPosition(m_pos);
	}

	// 当たり判定
	void Bullet::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}


	TestCube::TestCube(const shared_ptr<Stage>& stagePtr,Vec3 pos,Quat qt,Vec3 scale) :
		Actor(stagePtr,pos,qt,scale)
	{

	}

	TestCube::~TestCube()
	{

	}

	void TestCube::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
		trans->SetQuaternion(m_qt);
		trans->SetScale(m_scale);

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void TestCube::OnUpdate()
	{

	}

	// 当たり判定
	void TestCube::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}


}
//end basecross