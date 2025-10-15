/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	DisableShield::DisableShield(const shared_ptr<Stage>& ptrStage) :
		Actor(ptrStage)
	{

	}

	DisableShield::~DisableShield()
	{

	}

	void DisableShield::OnCreate()
	{
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(Vec3(-3.0f, 0.0f, 1.0f));
		m_trans->SetQuaternion(Quat(0.0f, 0.0f, 0.0f, -1.0f));
		m_trans->SetScale(Vec3(0.5f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.5f));
		SetAlphaActive(true);

		AddTag(L"DisableShield");

	}

	void DisableShield::OnUpdate()
	{
		Actor::OnUpdate();

		auto delta = App::GetApp()->GetElapsedTime();

		// 使用状態になったら効果範囲を広くなって最大になったら小さくなる
		if (m_use)
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

			// 縮小処理
			if (m_reduction)
			{
				m_scale += m_delta * m_sizeAddSpeed;
			
				// 範囲が0以下になったら使用状態をやめる
				if (m_scale.x <= 0.0f)
				{
					m_reduction = false;
					m_use = false;
				}
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


