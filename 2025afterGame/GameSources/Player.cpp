/*!
@file Player.cpp
@brief プレイヤーなど実体
担当：吉田 智貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Player::Player(const shared_ptr<Stage>& ptrStage) :
		Actor(ptrStage),
		m_stickL(Vec3(0.0f)),
		m_velocity(Vec3(0.0f)),
		m_speed(NORMAL_SPEED),
		m_targetSpeed(0.0f),
		m_accleRation(3.0f),
		m_deceleRation(2.0f),
		m_maxSpeed(2.5f),
		m_angleSpeed(1.0f),
		m_bustGauge(MAX_GAUGE),
		m_timeOfStartAttack(1.0f),
		m_timeOfAttack(0.0f),
		m_plusAttack(0),
		m_attackCollisionFlag(false),
		m_pitchAngle(0.0f),
		m_rollAngle(0.0f)
	{
	}

	Player::~Player()
	{

	}

	void Player::OnCreate()
	{
		Actor::OnCreate();
		auto& inputMgr = InputManager::CreateInputManager();

		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(Vec3(0.0f, 0.0f, -1.0f));

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"Player");
		ptrDraw->SetTextureResource(L"P_TX");

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.59f, 0.0f)
		);
		ptrDraw->SetMeshToTransformMatrix(spanMat);
	}

	void Player::OnUpdate()
	{
		auto& app = App::GetApp();
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		auto elapsed = app->GetElapsedTime();
		auto nowPos = GetComponent<Transform>()->GetPosition();
		auto nowRot = GetComponent<Transform>()->GetRotation();
		InputManager::CreateInputManager()->Update();

		PlayerMove();
		PlayerAngle();
		//PlayerBust();
		PlayerHealBust();
		// PlayerAttack();

		wstringstream wss(L"");
		wss << "X : " << nowPos.x << " " << "Y : " << nowPos.y << " " << "Z : " << nowPos.z << " " << endl;
		wss << "X : " << nowRot.x << " " << "Y : " << nowRot.y << " " << "Z : " << nowRot.z << " " << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());
	}

	void Player::OnCollisionEnter(const shared_ptr<GameObject>& Other)
	{

	}

	void Player::PlayerMove()
	{
		auto& app = App::GetApp();
		auto& input = InputManager::GetInputManager();

		auto ptrTrans = GetComponent<Transform>();
		auto elapsed = app->GetElapsedTime();
		auto currentPos = ptrTrans->GetPosition();
		auto forward = ptrTrans->GetForward();
		float damping = 0.9f;
		auto Lstick = input->GetLStick();

		if (input->GetButton(L"A"))
		{
			m_speed += m_accleRation * elapsed;
			if (m_speed > m_maxSpeed)
			{
				m_speed = m_maxSpeed;
			}
		}
		else
		{
			m_speed -= m_deceleRation * elapsed;
			
			if (m_speed < 0.0f)
			{
				m_speed = 0.0f;
			}
		}

		if (m_speed > 1.0f)
		{
			m_velocity = forward * m_speed;
			m_velocity *= damping; 

			currentPos += m_velocity * elapsed;
		}

		// 左右の移動
		if (abs(Lstick.x) > DEAD_ZONE)
		{
			currentPos.x += Lstick.x * m_speed * elapsed;
		}

		ptrTrans->SetPosition(currentPos);
	}

	void Player::PlayerAngle()
	{
		auto& app = App::GetApp();
		auto& input = InputManager::GetInputManager();

		auto ptrTrans = GetComponent<Transform>();
		auto elapsed = app->GetElapsedTime();
		auto currentQuat = ptrTrans->GetQuaternion();
		auto Lstick = input->GetLStick();

		// 上昇下降
		if (fabs(Lstick.y) > DEAD_ZONE)
		{
			// ピッチ変化量を計算
			float pitch = -Lstick.y * m_angleSpeed * elapsed;

			// ピッチ回転用のクォータニオンを作成
			Quat pitchQuat;
			pitchQuat.rotationX(pitch); // X軸の回転

			// 現在の回転に合成
			currentQuat = currentQuat * pitchQuat;
			currentQuat.normalize();
		}

		// 左右の傾け
		if (fabs(Lstick.x) > DEAD_ZONE)
		{
			float roll = -Lstick.x * m_angleSpeed * elapsed;

			// m_rollAngle += roll;

			Quat rollQuat;
			rollQuat.rotationZ(roll);
			currentQuat = currentQuat * rollQuat;
			currentQuat.normalize();
		}

		ptrTrans->SetQuaternion(currentQuat);
	}

	void Player::ClampBustGauge()
	{
		// bustGaugeの上限をMaxGaugeにすることが出来る
		m_bustGauge = clamp(m_bustGauge,0.0f, MAX_GAUGE);
	}

	void Player::PlayerBust()
	{
		auto& app = App::GetApp();
		auto pad = GetFirstPad();
		auto elapsed = app->GetElapsedTime();

		bool isBoosting = IsBoostInputActive() && (m_bustGauge > 0.0f);

		// 加速処理
		if (isBoosting)
		{
			m_speed = MAX_SPEED;
			m_bustGauge -= GAUGE_CONSUMPTION_RATE * elapsed;
		}
		else
		{
			m_speed = NORMAL_SPEED;
		}
	}

	void Player::PlayerHealBust()
	{
		auto& app = App::GetApp();
		auto pad = GetFirstPad();
		auto elapsed = app->GetElapsedTime();

		bool currentlyBoosting = IsBoostInputActive() && (m_bustGauge > 0.0f);

		// ゲージ回復
		if (currentlyBoosting)
		{
			m_bustGauge += GAUGE_RECOVERY_RATE * elapsed;
		}

		ClampBustGauge();
	}

	void Player::PlayerAttack()
	{
		auto& app = App::GetApp();
		auto& input = InputManager::GetInputManager();

		if (input->GetDownButton(L"A"))
		{
			m_attackCollisionFlag = true;
		}

		// 攻撃が有効になるタイミングに達したら、攻撃判定情報をセットする
		if (m_attackCollisionFlag)
		{
			auto attack = GetAttackPtr();
			auto& info = attack->GetHitInfo();
			info.Damage = 10 + m_plusAttack;
			info.HitOnce = true;

			attack->ActivateCollision(0.2f);

			m_attackCollisionFlag = false;
		}
	}
}
//end basecross

