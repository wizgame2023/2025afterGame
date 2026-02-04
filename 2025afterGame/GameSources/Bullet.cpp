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
		m_trans->SetPosition(parentPos + (m_parentForward * 1.6f));
		m_trans->SetQuaternion(Quat(0.0f,0.0f,0.0f,-1.0f));
		m_trans->SetScale(Vec3(0.1f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(false);
		ptrCol->SetAfterCollision(AfterCollision::None); // 物理判定無し

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"Bullet_Mesh");
		ptrDraw->SetTextureResource(L"BulletTex");

		// 進む方向に向く
		m_qt = SetRotate(m_parentForward);
		m_trans->SetQuaternion(m_qt);
	
		AddTag(L"Bullet");

		// 弾の所属を登録
		shared_ptr<Player> player = dynamic_pointer_cast<Player>(m_parent.lock());
		shared_ptr<Enemy> enemy = dynamic_pointer_cast<Enemy>(m_parent.lock());
		
		if (player) // 味方
		{
			m_affiliation = true;
		}
		else if (enemy) // 敵
		{
			m_affiliation = false;
		}

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



		//m_trans->SetQuaternion(qt);

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
		m_limitLenghtCount += abs(addPosVec.x) + abs(addPosVec.y) + abs(addPosVec.z);
	}

	// 当たり判定
	void Bullet::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}

	// 弾の所属のゲッタ
	bool Bullet::GetAffiliation()
	{
		return m_affiliation;
	}

	// ダメージのセッタ
	void Bullet::SetDamage(int damage)
	{
		m_damage = damage;
	}


	// ダメージのゲッタ
	int Bullet::GetDamage()
	{
		return m_damage;
	}

	// 親オブジェクトのゲッタ
	weak_ptr<Actor> Bullet::GetParentObj()
	{
		return m_parent;
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
		Actor::OnCreate();

		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
		trans->SetQuaternion(m_qt);
		trans->SetScale(m_scale);

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(true);
		ptrCol->SetFixed(true);

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