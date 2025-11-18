/*!
@file DebagPlayer.cpp
@brief デバック用のプレイヤー
担当：三瓶 裕太
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	DebagPlayer::DebagPlayer(const shared_ptr<Stage>& ptrStage) :
		FighterAircraftBase(ptrStage)
	{
	}

	DebagPlayer::~DebagPlayer()
	{

	}

	void DebagPlayer::OnCreate()
	{
		FighterAircraftBase::OnCreate();

		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(Vec3(0.0f, 0.0f, -1.0f));
		m_initialQuat = m_trans->GetQuaternion();

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//ptrDraw->SetTextureResource(L"diffuse_TX");

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(false);

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.0f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);
	}

	void DebagPlayer::OnUpdate()
	{
		FighterAircraftBase::OnUpdate();

		auto& app = App::GetApp();
		auto elapsed = app->GetElapsedTime();
		auto nowPos = GetComponent<Transform>()->GetPosition();
		auto nowRot = GetComponent<Transform>()->GetRotation();
		auto& input = InputManager::GetInputManager();


		// コントローラーXZ移動
		auto lStick = input->GetLStick();
		float stickangle = atan2f(lStick.y, lStick.x);

		Vec3 moveVec;
		if (stickangle != 0.0f)
		{	
			float totalAngle = stickangle - m_rot.y;
			//auto forward = m_trans->GetForward();

			moveVec.x += (cos(totalAngle)) * 5.0f;
			moveVec.z += (sin(totalAngle)) * 5.0f;
		}

		//Vec3 moveVec = (Vec3(lStick.x, 0.0f, lStick.y) * 3.0f);

		m_pos += moveVec * m_delta;

		// コントローラーY移動
		if (input->GetButton(L"DUp"))
		{
			m_pos.y += m_delta*3.0f;
		}
		if (input->GetButton(L"DDown"))
		{
			m_pos.y += -m_delta*3.0f;
		}


		// 回転
		auto rStick = input->GetRStick();

		m_rot += Vec3(0.0f, XMConvertToRadians(rStick.x), 0.0f);


		m_trans->SetRotation(m_rot);
		m_trans->SetPosition(m_pos);
	}

	void DebagPlayer::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
	}


}
//end basecross

