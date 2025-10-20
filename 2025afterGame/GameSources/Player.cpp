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
		m_autoYawSpeed(0.0f),
		m_currentRollAngle(0.0f),
		m_currentRoll(0.0f),
		m_prevRoll(0.0f),
		m_lastYaw(0.0f),
		m_initialized(false),
		m_hasInput(false),
		m_returnToNeutral(false),
		m_returnSpeed(0.0f)
	{
	}

	Player::~Player()
	{

	}

	void Player::OnCreate()
	{
		Actor::OnCreate();

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

		//Quat ptrQuat = Quat(1.0f, 1.0f, 1.0f, 1.0f);
		//ptrQuat.normalize();

		//Quat qPitch, qYaw, qRoll;
		//qPitch.rotationX(XMConvertToRadians(10.0f));
		//qYaw.rotationY(XMConvertToRadians(180.0f));
		//qRoll.rotationZ(XMConvertToRadians(0.0f));

		//ptrQuat = qPitch * qYaw * qRoll;  // 回転順に注意
		//ptrTrans->SetQuaternion(ptrQuat);
		
	}

	void Player::OnUpdate()
	{
		auto& app = App::GetApp();
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		auto elapsed = app->GetElapsedTime();
		auto nowPos = GetComponent<Transform>()->GetPosition();
		auto nowRot = GetComponent<Transform>()->GetRotation();
		auto nowQuaternion = GetComponent<Transform>()->GetQuaternion();
		InputManager::CreateInputManager()->Update();

		PlayerMove();
		PlayerAngle();
		PlayerHealBust();

		//wstringstream wss(L"");
		//wss << "X : " << nowPos.x << " " << "Y : " << nowPos.y << " " << "Z : " << nowPos.z << " " << endl;
		//Vec3 euler = nowQuaternion.toRotVec(); // ← BaseCrossならこの関数があるか確認

		//wss << "Yaw:" << XMConvertToDegrees(euler.y)
		//	<< " Pitch:" << XMConvertToDegrees(euler.x)
		//	<< " Roll:" << XMConvertToDegrees(euler.z) << endl;
		//auto scene = app->GetScene<Scene>();
		//scene->SetDebugString(wss.str());
	}

	void Player::OnCollisionEnter(const shared_ptr<GameObject>& Other)
	{

	}

	void Player::PlayerMove()
	{
		auto& app = App::GetApp();
		auto& game = GameManager::GetGameManager();
		auto& input = InputManager::GetInputManager();

		auto ptrTrans = GetComponent<Transform>();
		float elapsed = app->GetElapsedTime();
		Vec3 currentPos = ptrTrans->GetPosition();

		Vec3 forward = ptrTrans->GetForward();
		float damping = 0.9f;
		auto Lstick = input->GetLStick();

		// Aボタンを押して加速
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
			// 押していないと減速
			m_speed -= m_deceleRation * elapsed;
			if (m_speed < 0.0f)
			{
				m_speed = 0.0f;
			}
		}

		// 移動処理（減衰付き）
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
		auto& game = GameManager::GetGameManager();
		auto& input = InputManager::GetInputManager();

		auto ptrTrans = GetComponent<Transform>();
		auto elapsed = app->GetElapsedTime();
		auto Lstick = input->GetLStick();

		auto currentQuat = ptrTrans->GetQuaternion();  // 現在の回転

		if (!m_initialized)
		{
			m_initialQuat = currentQuat;
			m_initialized = true;
		}

		Quat deltaQuat;

		// 上下の角度調整
		if (fabs(Lstick.y) > DEAD_ZONE)
		{
			float pitch = Lstick.y * m_angleSpeed * elapsed;
			Quat pitchQuat;
			pitchQuat.rotationX(pitch);
			deltaQuat = deltaQuat * pitchQuat;
			m_hasInput = true;
		}

		// 横角度の制限60度以上に行かないようにしている
		if (fabs(Lstick.x) > DEAD_ZONE)
		{
			m_isReturning = false;

			// ロール角を更新
			m_currentRoll += -Lstick.x * m_angleSpeed * elapsed;

			static constexpr float maxRoll = XMConvertToRadians(60.0f);
			m_currentRoll = clamp(m_currentRoll, -maxRoll, maxRoll);
		}
		else
		{
			// 入力がない時は、時間経過で水平（0°）へ戻す
			if (!m_isReturning)
			{
				m_startRoll = m_currentRoll;
				m_startTime = 0.0f;
				m_endTime = 0.5f;
				m_isReturning = true;
			}

			// 経過時間更新
			m_startTime += elapsed;

			// Lerpで自然に戻す
			m_currentRoll = Lerp::CalculateLerp(
				m_startRoll,
				0.0f,
				0.0f,
				m_endTime,
				m_startTime,
				Lerp::Cos
			);

			// 終了判定
			if (fabs(m_currentRoll) < 0.001f)
			{
				m_currentRoll = 0.0f;
				m_isReturning = false;
			}
		}

		// 差分回転を適用
		float deltaRoll = m_currentRoll - m_prevRoll;

		Quat rollQuat;
		rollQuat.rotationZ(deltaRoll);
		deltaQuat = deltaQuat * rollQuat;

		// 次回のために保存
		m_prevRoll = m_currentRoll;

		// 入力がなくなったら
		if (m_hasInput && fabs(Lstick.x) < DEAD_ZONE && fabs(Lstick.y) < DEAD_ZONE)
		{
			m_hasInput = false;
		}

		// 入力がなくなったら「戻すモード」に
		if (!m_hasInput && !m_returnToNeutral)
		{
			m_returnToNeutral = true;
		}

		// 入力がされ続けていたら
		if (m_hasInput)
		{
			Quat targetQuat = deltaQuat * currentQuat;
			Quat resultQuat = Slerp(currentQuat, targetQuat, 1.0f);
			ptrTrans->SetQuaternion(resultQuat);
		}
		else if (m_returnToNeutral)
		{
			// 少しずつ初期回転に戻す
			float t = elapsed * 2.0f;
			Quat resultQuat = Slerp(currentQuat, m_initialQuat, t);

			m_returnToNeutral = false;

			ptrTrans->SetQuaternion(resultQuat);
		}
	}

	void Player::ClampBustGauge()
	{
		// bustGaugeの上限をMaxGaugeにすることが出来る
		m_bustGauge = clamp(m_bustGauge, 0.0f, MAX_GAUGE);
	}

	void Player::PlayerBust()
	{
		auto& app = App::GetApp();
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
		auto elapsed = app->GetElapsedTime();

		bool currentlyBoosting = GetIsBoostInputActive() && (m_bustGauge > 0.0f);

		// ゲージ回復
		if (currentlyBoosting)
		{
			m_bustGauge += GAUGE_RECOVERY_RATE * elapsed;
		}

		ClampBustGauge();
	}

	//void Player::PlayerAttack()
	//{
	//	auto& app = App::GetApp();
	//	auto& game = GameManager::GetGameManager();
	//	auto& input = InputManager::GetInputManager();

	//	if (input->GetDownButton(L"A"))
	//	{
	//		m_attackCollisionFlag = true;
	//	}

	//	// 攻撃が有効になるタイミングに達したら、攻撃判定情報をセットする
	//	if (m_attackCollisionFlag)
	//	{
	//		auto attack = GetAttackPtr();
	//		auto& info = attack->GetHitInfo();
	//		info.Damage = 10 + m_plusAttack;
	//		info.HitOnce = true;

	//		attack->ActivateCollision(0.2f);

	//		m_attackCollisionFlag = false;
	//	}
	//}

	bool Player::GetIsBoostInputActive() const
	{
		auto& game = GameManager::GetGameManager();
		auto& inputMgr = InputManager::GetInputManager();

		bool stickActive = fabs(inputMgr->GetLStick().x) > DEAD_ZONE;
		bool shoulderActive = inputMgr->GetButton(L"L") || inputMgr->GetButton(L"R");
		bool triggerActive = inputMgr->GetLeftTrigger() > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

		return (stickActive || shoulderActive) && triggerActive;
	}
}
//end basecross

