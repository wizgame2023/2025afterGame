/*!
@file ProjectShader.cpp
@brief バリアを妨害する空間の実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	DisableShield::DisableShield(const shared_ptr<Stage>& ptrStage,const shared_ptr<Actor>& parent,float sizeMax) :
		Actor(ptrStage),
		m_parent(parent),
		m_sizeMax(sizeMax)
	{
		
	}

	DisableShield::~DisableShield()
	{

	}

	void DisableShield::OnCreate()
	{
		m_color = Col4(1.0f, 0.0f, 0.0f, 0.5f);

		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(Vec3(-3.0f, 0.0f, 1.0f));
		m_trans->SetQuaternion(Quat(0.0f, 0.0f, 0.0f, -1.0f));
		m_trans->SetScale(Vec3(1.5f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);
		ptrCol->SetAfterCollision(AfterCollision::None);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetDiffuse(m_color);
		SetAlphaActive(true);

		AddTag(L"DisableShield");

		m_use = true;//デバック用

	}

	void DisableShield::OnUpdate()
	{
		m_parentLock = m_parent.lock();

		// 親オブジェクトが消えたら自分も消える
		if (!m_parentLock)
		{
			GetStage()->RemoveGameObject<DisableShield>(GetThis<DisableShield>());
			return;
		}

		Actor::OnUpdate();

		auto delta = App::GetApp()->GetElapsedTime();

		// 親オブジェクトについていく処理
		FollowMove();
		// 使用時処理
		UseProcess();

	}

	// 親オブジェクトについていく処理
	void DisableShield::FollowMove()
	{
		Vec3 parentPos = m_parentLock->GetComponent<Transform>()->GetPosition();

		// 位置追従
		m_pos = parentPos;

		// 回転度追従
		Quat parentQt = m_parentLock->GetComponent<Transform>()->GetQuaternion();
		m_qt = parentQt;
	}

	// 使用時処理
	void DisableShield::UseProcess()
	{
		// 使用状態になったら効果範囲を広くなって最大になったら小さくなる
		if (m_use)
		{
			// 拡大処理
			if (!m_reduction)
			{
				if (m_scale.x < m_sizeMax)
				{
					m_scale += m_delta * m_sizeAddSpeed;
					m_trans->SetScale(m_scale);
				}
				else if (m_scale.x > m_sizeMax)
				{
					m_reduction = true;
					m_trans->SetScale(Vec3(m_sizeMax));
				}
			}

			// 縮小処理
			if (m_reduction)
			{
				m_scale -= (m_delta * m_sizeAddSpeed) / 2;
				m_trans->SetScale(m_scale);

				// 範囲が0以下になったら使用状態をやめる
				if (m_scale.x <= 0.0f)
				{
					// リセット
					m_scale = Vec3(0.0f);
					m_reduction = false;
					m_use = false;
				}
			}

		}

	}


	void DisableShield::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto player = dynamic_pointer_cast<Player>(obj);

		if (m_use)
		{
			// playerにバリア妨害が当たったらしばらくバリアを貼れない状態にする
			if (player)
			{
				// バリアを使用可能というフラグをPlayer側で作られたら作成する

			}
		}

	}

	// m_useのゲッタ
	bool DisableShield::GetUse()
	{
		return m_use;
	}

	// m_useのセッタ
	void DisableShield::SetUse(bool use)
	{
		m_use = use;
	}

}
//end basecross


