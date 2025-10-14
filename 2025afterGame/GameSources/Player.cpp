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
		m_yawvelocity(Vec3(0.0f)),
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
		m_pitch(0.0f),
		m_roll(0.0f),
		m_yaw(0.0f)
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
		ptrDraw->SetMeshResource(L"Sentouki");
		ptrDraw->SetTextureResource(L"diffuse_TX");

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.2f),
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
		PlayerHealBust();
		//PlayerBust();
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
		float elapsed = app->GetElapsedTime();
		Vec3 currentPos = ptrTrans->GetPosition();
		Quat currentQuat = ptrTrans->GetQuaternion();

		Vec3 forward = ptrTrans->GetForward();
		float damping = 0.9f;
		auto Lstick = input->GetLStick();
		float m_yawSpeed = 1.0f;

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

		ptrTrans->SetPosition(currentPos);
	}

	void Player::PlayerAngle()
	{
		auto& app = App::GetApp();
		auto& input = InputManager::GetInputManager();

		auto ptrTrans = GetComponent<Transform>();
		auto elapsed = app->GetElapsedTime();
		auto Lstick = input->GetLStick();

		Quat currentQuat = ptrTrans->GetQuaternion();

		Quat qPitch, qYaw, qRoll;
		qPitch.rotationX(-Lstick.y * m_angleSpeed);
		qYaw.rotationY(sin(m_roll) * 0.5f);
		qRoll.rotationZ(-Lstick.x * m_angleSpeed);
		Quat targetQuat = qYaw * qPitch * qRoll;
		targetQuat.normalize();

		currentQuat = Slerp(currentQuat, targetQuat, elapsed * 5.0f);
		currentQuat.normalize();

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

		bool isBoosting = GetIsBoostInputActive() && (m_bustGauge > 0.0f);

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

		bool currentlyBoosting = GetIsBoostInputActive() && (m_bustGauge > 0.0f);

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

	bool Player::GetIsBoostInputActive() const
	{
		auto& inputMgr = InputManager::GetInputManager();

		bool stickActive = fabs(inputMgr->GetLStick().x) > DEAD_ZONE;
		bool shoulderActive = inputMgr->GetButton(L"L") || inputMgr->GetButton(L"R");
		bool triggerActive = inputMgr->GetLeftTrigger() > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

		return (stickActive || shoulderActive) && triggerActive;
	}
}
//end basecross

