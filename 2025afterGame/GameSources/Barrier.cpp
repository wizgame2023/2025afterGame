/*!
@file Barrier.cpp
@brief バリアの実体
担当：三瓶裕太
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
		// 親クラス処理
		Actor::OnCreate();

		// Trans処理追加
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(Vec3(0.5f, 0.0f, 1.0f));
		m_trans->SetQuaternion(Quat(0.0f, 0.0f, 0.0f, 1.0f));
		m_trans->SetScale(Vec3(1.5f,1.0f,0.1f));

		// コリジョン追加
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(false);
		ptrCol->SetAfterCollision(AfterCollision::None);

		// ドロー処理
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SQUARE");
		ptrDraw->SetDiffuse(Col4(0.0f, 1.0f, 0.5f, 0.2f));
		ptrDraw->SetEmissive(Col4(0.0f, 1.0f, 0.5f, 0.2f));
		ptrDraw->SetTextureResource(L"TestTex");
		SetAlphaActive(true);

		// 所属を決める (味方 = true,敵 = false)
		DecideAffiliation();

		// バリアタグ追加
		AddTag(L"Barrier");
	}

	void Barrier::OnUpdate()
	{		
		m_parentLock = m_parent.lock();

		// もし、親オブジェクトが消去されたら自分も消える
		if (!m_parentLock)
		{
			GetStage()->RemoveGameObject<Barrier>(GetThis<Barrier>());
			return;
		}

		// 使用状態でないなら見えないようにする
		if (!m_use)
		{
			m_trans->SetScale(Vec3(0.0f));
			SetDrawActive(false);
		}
		else if (m_use)
		{
			m_trans->SetScale(Vec3(Vec3(2.5f, 2.5f, 0.1f)));
			SetDrawActive(true);
		}

		// 親クラス処理
		Actor::OnUpdate();

		// 親オブジェクトについていく処理
		FollowMove();
		// バリア使用時のエネルギ-を消費処理
		EnergyConsumption();

		// 位置更新
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetQuaternion(m_qt);

		m_use = true;
	}

	//親クラスによって所属を決める処理
	void Barrier::DecideAffiliation()
	{
		// 親クラスによって所属を変える(味方 = true,敵 = false)
		auto parentLock = m_parent.lock();
		auto player = dynamic_pointer_cast<Player>(parentLock);
		if (player)
		{
			m_affiliation = true;
		}
		else if (!player)
		{
			m_affiliation = false;
		}
	}

	// 親オブジェクトに追従する処理
	void Barrier::FollowMove()
	{
		Vec3 parentPos = m_parentLock->GetComponent<Transform>()->GetPosition();
		Vec3 parentUp = m_parentLock->GetComponent<Transform>()->GetUp();
		Vec3 parentForward = m_parentLock->GetComponent<Transform>()->GetForward();

		m_pos = parentPos - (m_barrierLenght * parentForward) + (parentUp * -0.2f);

		Quat parentQt = m_parentLock->GetComponent<Transform>()->GetQuaternion();
		m_qt = parentQt;
	}

	// バリアを使うことによって起きるエネルギーを消費する処理
	void Barrier::EnergyConsumption()
	{
		// 使用している時はエネルギ-を消費
		if (m_use)
		{
			m_energyDebag -= m_energyEfficiency * m_delta;

			// エネルギーが0以下なら使用できない
			if (m_energyDebag < 0)
			{
				m_use = false;
			}
		}
	}

	// 当たり判定
	void Barrier::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto bullet = dynamic_pointer_cast<Bullet>(obj);

		if (m_use)
		{
			if (bullet)
			{
				// 弾の所属が同じならバリアの判定はしない
				//if()

				// 弾からプレイヤーを守ってエネルギーを消費する
				m_energyDebag -= m_energyLost;
				GetStage()->RemoveGameObject<Bullet>(bullet);
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