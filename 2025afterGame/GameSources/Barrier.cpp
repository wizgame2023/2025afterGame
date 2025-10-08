/*!
@file Barrier.cpp
@brief 弾の実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Barrier.h"

namespace basecross {
	Barrier::Barrier(const shared_ptr<Stage>& stagePtr,const shared_ptr<Actor>& parent) :
		Actor(stagePtr),
		m_parent(parent)
	{

	}

	Barrier::~Barrier()
	{

	}

	void Barrier::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(0.5f, 0.0f, 0.0f));
		trans->SetQuaternion(Quat(0.0f, 0.0f, 0.0f, 1.0f));
		trans->SetScale(Vec3(1.5f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);

		AddTag(L"Barrier");
	}

	void Barrier::OnUpdate()
	{
		// 親オブジェクトに追従する
		auto parentlock = m_parent.lock();
		Vec3 parentPos = parentlock->GetComponent<Transform>()->GetPosition();
		m_pos = parentPos;

		// 位置更新
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
	}

	// 当たり判定
	void Barrier::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}

	// m_useのゲッタ
	bool Barrier::GetUse()
	{
		return m_use;
	}

	// m_useのセッタ
	// 第一引数 m_useに代入したい値
	void Barrier::SetUse(bool use)
	{
		m_use = use;
	}


}
//end basecross