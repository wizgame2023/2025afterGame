/*!
@file Bullet.cpp
@brief 弾の実体
担当：三瓶裕太
*/

#include "stdafx.h"
#include "Project.h"
#include "Bullet.h"

namespace basecross {
	Bullet::Bullet(const shared_ptr<Stage>& stagePtr,const shared_ptr<Actor>& parent) :
		Actor(stagePtr),
		m_parent(parent)
	{

	}

	Bullet::~Bullet()
	{

	}

	void Bullet::OnCreate()
	{
		Actor::OnCreate();

		// 親クラスの向いている方向を取得
		auto parentLock = m_parent.lock();
		m_parentForward = parentLock->GetComponent<Transform>()->GetForward();
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();


		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(parentPos);
		m_trans->SetQuaternion(Quat(0.0f,0.0f,0.0f,-1.0f));
		m_trans->SetScale(Vec3(0.2f));

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

		// 移動処理
		Move();

		// 射程範囲外にいるならこのオブジェクトは削除される
		if (m_limitLenght < m_limitLenghtCount)
		{
			GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
			return;
		}

		m_trans->SetPosition(m_pos);
	}

	// 移動用の関数
	void Bullet::Move()
	{
		// 親オブジェクトの向いていた方向(OnCreate時)に向かって移動する
		m_pos = m_trans->GetPosition();
		auto addPosVec = (m_speed * m_parentForward) * m_delta;
		m_pos += addPosVec;

		// どのくらい移動したのかを記録する
		m_limitLenghtCount += addPosVec.x + addPosVec.y + addPosVec.z;
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