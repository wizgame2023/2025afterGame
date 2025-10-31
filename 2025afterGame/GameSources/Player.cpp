/*!
@file Player.cpp
@brief プレイヤーなど実体
担当：吉田 智貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Player::Player(const shared_ptr<Stage>& ptrStage) :
		FighterAircraftBase(ptrStage),
		m_speedCurrent(1.0f),
		m_speedMax(2.5f),
		m_velocity(Vec3(0.0f)),
		m_accleRation(3.0f),
		m_deceleRation(2.0f),
		m_angleSpeed(1.0f),
		m_rollSpeed(1.0f),
		m_currentRoll(0.0f),
		m_prevRoll(0.0f),
		m_initialized(false),
		m_hasInput(false),
		m_returnToNeutral(false),
		m_fullEnergy(false),
		m_actionMode(ActionMode::None),
		m_turnStrength(1.0f)
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
		m_initialQuat = ptrTrans->GetQuaternion();

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"Sentouki");
		ptrDraw->SetTextureResource(L"diffuse_TX");

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(true);

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
		auto elapsed = app->GetElapsedTime();
		auto nowPos = GetComponent<Transform>()->GetPosition();
		auto& input = InputManager::GetInputManager();

		// プレイヤーの挙動
		PlayerMove();
		PlayerAngle();

		// プレイヤーの装備
		CreateBarrier();
		CreateBullet();
		
		// dpadでコントローラーを変える
		ChangController();

		wstringstream wss;

		wss << "NowPos X : " << nowPos.x
			<< "\nNowPos Y : " << nowPos.y
			<< "\nNowPos Z : " << nowPos.z;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());
	}

	void Player::OnCollisionEnter(const shared_ptr<GameObject>& Other)
	{

	}

	void Player::PlayerMove()
	{
		auto& app = App::GetApp();
		auto& game = GameManager::GetGameManager();
		auto pads = app->GetInputDevice();
		auto& input = InputManager::GetInputManager();

		auto ptrTrans = GetComponent<Transform>();
		float deltaTime = game->GetDeltaTime();
		Vec3 currentPos = ptrTrans->GetPosition();

		Vec3 forward = ptrTrans->GetForward();
		float damping = 0.9f;

		auto Lstick = input->GetLStick();
		
		if (m_playerIndex == 0)
		{
			Lstick = input->GetLStick();
		}
		else if (m_playerIndex == 1)
		{
			Lstick = input->GetLStick2();
		}
			
		if (m_playerIndex == 0)
		{
			m_aButton = input->GetButton(L"A");
		}
		else if (m_playerIndex == 1)
		{
			m_aButton = input->GetButton2(L"A");
		}

		// Aボタンを押して加速
		if (m_aButton)
		{
			m_velocity = forward * m_speedCurrent;
			currentPos += m_velocity * deltaTime;
		}

		//	m_speedCurrent += m_accleRation * deltaTime;

		//	if (m_speedCurrent > m_speedMax)
		//	{
		//		m_speedCurrent = m_speedMax;
		//	}
		//}
		//else if(m_speedCurrent < 0.0f)
		//{
		//	// 押していないと減速
		//	m_speedCurrent -= m_deceleRation * deltaTime;

		//	if (m_speedCurrent <= 0.0f)
		//	{
		//		m_speedCurrent = 0.0f;
		//	}
		//}

		//// 移動処理（減衰付き）
		//if (m_speedCurrent > 0.0f)
		//{
		//	m_velocity = forward * m_speedCurrent;
		//	m_velocity *= damping;
		//	currentPos += m_velocity * deltaTime;
		//}

		ptrTrans->SetPosition(currentPos);
	}

	void Player::PlayerAngle()
	{
		auto& game = GameManager::GetGameManager();
		auto& input = InputManager::GetInputManager();
		float deltaTime = game->GetDeltaTime();

		auto ptrTrans = GetComponent<Transform>();
		auto currentQuat = ptrTrans->GetQuaternion();
		Vec3 forward = ptrTrans->GetForward();

		Vec2 lstick;

		// Playerのコントローラー番号で変わる
		if (m_playerIndex == 0)
		{
			lstick = input->GetLStick();
		}
		else if (m_playerIndex == 1)
		{
			lstick = input->GetLStick2();
		}

		// 入力がほとんど無い場合は自動的に水平復帰
		if (fabs(lstick.x) < DEAD_ZONE && fabs(lstick.y) < DEAD_ZONE)
		{
			//AutoUpright(deltaTime);
			return;
		}

		// 姿勢の更新
		Quat pitchQuat = PlayerPitch(lstick.y, deltaTime);
		Quat rollQuat = PlayerRoll(lstick.x, deltaTime);
		Quat deltaQuat = pitchQuat * rollQuat;
		Quat resultQuat = deltaQuat * currentQuat;
		resultQuat.normalize();
		ptrTrans->SetQuaternion(resultQuat);

		// バンク旋回
		Vec3 euler = QuaternionToEuler(resultQuat);
		float rollAngle = euler.z;
		Vec3 right = ptrTrans->GetRight();
		Vec3 turnForce = right * sin(rollAngle) * m_turnStrength * deltaTime;

		//// ========= デバッグ出力 =========
		//auto& app = App::GetApp();

		//// クォータニオン → オイラー角（ラジアン → 度）
		//Vec3 euler;
		//{
		//	float ysqr = resultQuat.y * resultQuat.y;

		//	// pitch (X)
		//	float t0 = +2.0f * (resultQuat.w * resultQuat.x + resultQuat.y * resultQuat.z);
		//	float t1 = +1.0f - 2.0f * (resultQuat.x * resultQuat.x + ysqr);
		//	euler.x = atan2f(t0, t1);

		//	// yaw (Y)
		//	float t2 = +2.0f * (resultQuat.w * resultQuat.y - resultQuat.z * resultQuat.x);
		//	t2 = t2 > 1.0f ? 1.0f : (t2 < -1.0f ? -1.0f : t2);
		//	euler.y = asinf(t2);

		//	// roll (Z)
		//	float t3 = +2.0f * (resultQuat.w * resultQuat.z + resultQuat.x * resultQuat.y);
		//	float t4 = +1.0f - 2.0f * (ysqr + resultQuat.z * resultQuat.z);
		//	euler.z = atan2f(t3, t4);

		//	// ラジアン → 度
		//	euler.x = XMConvertToDegrees(euler.x);
		//	euler.y = XMConvertToDegrees(euler.y);
		//	euler.z = XMConvertToDegrees(euler.z);
		//}

		//wstringstream wss(L"");
		//wss << L"Euler角（度）:"
		//	<< L"\n Pitch(X): " << euler.x
		//	<< L"\n Yaw(Y):   " << euler.y
		//	<< L"\n Roll(Z):  " << euler.z
		//	<< L"\n\nQuat:"
		//	<< L"\n X: " << resultQuat.x
		//	<< L"\n Y: " << resultQuat.y
		//	<< L"\n Z: " << resultQuat.z
		//	<< L"\n W: " << resultQuat.w
		//	<< endl;

		//auto scene = app->GetScene<Scene>();
		//scene->SetDebugString(wss.str());
	}
	

	Quat Player::PlayerPitch(const float stickY, float deltaTime)
	{
		float pitch = stickY * m_angleSpeed * deltaTime;
		Quat pitchQuat;
		pitchQuat.rotationX(pitch);
		return pitchQuat;
	}

	Quat Player::PlayerYawWorld(const float stickX, float deltaTime)
	{
		float yawAngle = stickX * m_yawSpeed * deltaTime;
		return FromAxisAngle(Vec3(0, 1, 0), yawAngle);
	}

	Quat Player::PlayerRoll(float stickX, float deltaTime)
	{
		float roll = -stickX * m_rollSpeed * deltaTime;
		Quat rollQuat;
		rollQuat.rotationZ(roll); // ローカルZ軸
		return rollQuat;
	}

	Quat Player::PlayerYaw(const float stickX, float deltaTime)
	{
		float yawAmout = stickX * m_angleSpeed * deltaTime;
		Quat yawQuat;
		yawQuat.rotationY(yawAmout);
		return yawQuat;
	}

	void Player::AutoUpright(float deltaTime)
	{
		auto ptrTrans = GetComponent<Transform>();
		Quat currentQuat = ptrTrans->GetQuaternion();
		Quat target = m_initialQuat;

		// 時間を累積
		m_uprightTime += deltaTime;
		float duration = 1.0f; // 1秒で水平に戻す
		float t = clamp(m_uprightTime / duration, 0.0f, 1.0f);

		Quat result = Slerp(currentQuat, target, t);
		result.normalize();
		ptrTrans->SetQuaternion(result);

		// 終了したらタイマーをリセット
		if (t >= 1.0f)
		{
			m_uprightTime = 0.0f;
		}
	}

	void Player::CreateBarrier()
	{
		auto stage = GetStage();
		auto& input = InputManager::GetInputManager();

		Vec3 pos = GetComponent<Transform>()->GetPosition();

		if (!m_barrier)
		{
			m_barrier = stage->AddGameObject<Barrier>(GetThis<Player>());
		}

		auto useflag = m_barrier->GetUse();

		if (input->GetButton(L"X"))
		{
			if (!useflag)
			{
				m_barrier->SetUse(true);
			}
		}
	}

	void Player::CreateBullet()
	{
		auto stage = GetStage();
		auto& input = InputManager::GetInputManager();

		if (input->GetDownButton(L"B"))
		{
			m_bullet = stage->AddGameObject<Bullet>(GetThis<Player>());
		}
	}

	// フラグのゲッタ、セッタ
	// プレイヤーのコントローラ番号をセッタ
	void Player::SetPlayerIndex(int index)
	{
		m_playerIndex = index;
	}

	// プレイヤーのコントローラ番号ゲッタ
	int Player::GetPlayerIndex() const
	{
		return m_playerIndex;
	}

	// 加速しているかのゲッタ
	bool Player::GetAcceleration()
	{
		return m_acceleration;
	}

	bool Player::GetIsBoostInputActive() const
	{
		auto& game = GameManager::GetGameManager();
		auto& inputMgr = InputManager::GetInputManager();

		bool stickActive = fabs(inputMgr->GetLStick().x) > DEAD_ZONE;
		bool shoulderActive = inputMgr->GetButton(L"L") || inputMgr->GetButton(L"R");
		bool triggerActive = inputMgr->GetLeftTrigger() > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

		return (stickActive || shoulderActive) && triggerActive;
	}

	void Player::ChangController()
	{
		auto& input = InputManager::GetInputManager();

		bool dDown = input->GetButton(L"DDown");

		// 「押した瞬間」を検出
		if (dDown && !m_prevDDown)
		{
			if (m_playerIndex == 0)
			{
				m_playerIndex = 1;
			}				
			else
			{
				m_playerIndex = 0;
			}
		}

		// 次フレーム用に状態を保存
		m_prevDDown = dDown;
	}

	float Player::AngleBetWeen(const Quat& a, const Quat& b)
	{
		float dot = fabs(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);

		dot = clamp(dot, -1.0f, 1.0f);

		return acosf(dot) * 2.0f;
	}

	Quat Player::Slerp(const Quat& q1, const Quat& q2, float t)
	{
		// ドット積（角度のcosθを求める）
		float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

		Quat q2b = q2;
		if (dot < 0.0f)
		{
			dot = -dot;
			q2b = Quat(-q2.x, -q2.y, -q2.z, -q2.w);
		}

		const float THRESHOLD = 0.9995f;
		if (dot > THRESHOLD)
		{
			// 線形補間でOK
			Quat result(
				q1.x + t * (q2b.x - q1.x),
				q1.y + t * (q2b.y - q1.y),
				q1.z + t * (q2b.z - q1.z),
				q1.w + t * (q2b.w - q1.w)
			);
			result.normalize();
			return result;
		}

		// 角度計算
		float theta_0 = acosf(dot);
		float theta = theta_0 * t;
		float sin_theta = sinf(theta);
		float sin_theta_0 = sinf(theta_0);

		float s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
		float s1 = sin_theta / sin_theta_0;

		// 補間結果
		Quat result(
			(s0* q1.x) + (s1 * q2b.x),
			(s0* q1.y) + (s1 * q2b.y),
			(s0* q1.z) + (s1 * q2b.z),
			(s0* q1.w) + (s1 * q2b.w)
		);

		result.normalize();
		return result;
	}

	Quat Player::FromAxisAngle(const Vec3& axis, float angleRad)
	{
		Vec3 normAxis = axis;
		normAxis.normalize();
		float halfAngle = angleRad * 0.5f;
		float s = sinf(halfAngle);

		return Quat(cosf(halfAngle),
			normAxis.x * s,
			normAxis.y * s,
			normAxis.z * s);
	}
	
}
//end basecross

