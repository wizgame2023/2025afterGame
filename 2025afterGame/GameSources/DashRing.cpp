/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	DashRing::DashRing(const shared_ptr<Stage>& StagePtr,
		const Vec3& Pos,
		const Vec3& Rot,
		const Vec3& Siz
	) :
		GameObject(StagePtr),
		m_pos(Pos),
		m_rot(Rot),
		m_siz(Siz),
		m_speed(3.0f)
	{
		try
		{

		}
		catch (...)
		{
			throw;
		}
	}

	DashRing::~DashRing(){

	}

	void DashRing::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_siz);
		PtrTrans->SetRotation(m_rot);
		PtrTrans->SetPosition(m_pos);

		//メッシュの描画
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void DashRing::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}

	float DashRing::GetSpeedUp()
	{
		return m_speed;
	}
}
//end basecross
