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
		m_currentRoll(0.0f),
		m_prevRoll(0.0f),
		m_initialized(false),
		m_hasInput(false),
		m_returnToNeutral(false),
		m_fullEnergy(false)
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

		wstringstream wss(L"");
		wss << "X : " << nowPos.x << " " << "Y : " << nowPos.y << " " << "Z : " << nowPos.z << " " << endl;
		wss << m_speedCurrent << endl;
		wss << m_playerIndex << endl; 
		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());

		// プレイヤーの挙動
		PlayerMove();
		PlayerAngle();

		// プレイヤーの装備
		CreateBarrier();
		CreateBullet();
		
		// dpadでコントローラーを変える
		ChangController();
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
			m_speedCurrent += m_accleRation * deltaTime;

			if (m_speedCurrent > m_speedMax)
			{
				m_speedCurrent = m_speedMax;
			}
		}
		else if(m_speedCurrent < 0.0f)
		{
			// 押していないと減速
			m_speedCurrent -= m_deceleRation * deltaTime;

			if (m_speedCurrent <= 0.0f)
			{
				m_speedCurrent = 0.0f;
			}
		}

		// 移動処理（減衰付き）
		if (m_speedCurrent > 0.0f)
		{
			m_velocity = forward * m_speedCurrent;
			m_velocity *= damping;
			currentPos += m_velocity * deltaTime;
		}

		ptrTrans->SetPosition(currentPos);
	}

	void Player::PlayerAngle()
	{
		auto& game = GameManager::GetGameManager();
		auto& input = InputManager::GetInputManager();
		float deltaTime = game->GetDeltaTime();

		auto ptrTrans = GetComponent<Transform>();
		auto currentQuat = ptrTrans->GetQuaternion();

		Vec2 Lstick;

		// Playerのコントローラー番号で変わる
		if (m_playerIndex == 0)
		{
			Lstick = input->GetLStick();
		}
		else if (m_playerIndex == 1)
		{
			Lstick = input->GetLStick2();
		}

		// 最初の初期姿勢の保存
		if (!m_initialized)
		{
			m_initialQuat = currentQuat;
			m_initialized = true;
		}

		Quat deltaQuat;

		// 上下の角度調整
		if (fabs(Lstick.y) > DEAD_ZONE)
		{
			float pitch = Lstick.y * m_angleSpeed * deltaTime;
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
			m_currentRoll += -Lstick.x * m_angleSpeed * deltaTime;

			static float maxRoll = XMConvertToRadians(60.0f);
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
			m_startTime += deltaTime;

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

		// なぜSlerpはどのクォータニオンの補間用
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
			float t = deltaTime * 2.0f;
			Quat resultQuat = Slerp(currentQuat, m_initialQuat, t);

			m_returnToNeutral = false;

			ptrTrans->SetQuaternion(resultQuat);
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
	}}
//end basecross

