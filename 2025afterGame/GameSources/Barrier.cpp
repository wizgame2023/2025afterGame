/*!
@file Barrier.cpp
@brief 弾の実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Barrier.h"
#include "Bullet.h"

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
		// Trans処理追加
		auto trans = GetComponent<Transform>();
		trans->SetPosition(Vec3(0.5f, 0.0f, 0.0f));
		trans->SetQuaternion(Quat(0.0f, 0.0f, 0.0f, 1.0f));
		trans->SetScale(Vec3(1.5f));

		// コリジョン追加
		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(false);

		// ドロー処理
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.5f, 0.5f));
		SetAlphaActive(true);

		// バリアタグ追加
		AddTag(L"Barrier");
	}

	void Barrier::OnUpdate()
	{
		// 親オブジェクトに追従する
		auto parentlock = m_parent.lock();
		Vec3 parentPos = parentlock->GetComponent<Transform>()->GetPosition();
		m_pos = parentPos;

		// バリアを使用している時はエネルギ-を消費する
		if (m_use)
		{
			m_energyDebag -= m_energyEfficiency;	
			
			// エネルギーが0以下なら使用できない
			if (m_energyDebag < 0)
			{
				m_use = false;
			}
		}

		// 位置更新
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
	}

	// 当たり判定
	void Barrier::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{		
		auto bullet = dynamic_pointer_cast<Bullet>(obj);

		if (m_use)
		{
			// 弾からプレイヤーを守ったらエネルギーを消費する
			if (bullet)
			{
				m_energyDebag -= m_energyLost;
				//GetStage()->RemoveGameObject<Bullet>(bullet);
			}
		}
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