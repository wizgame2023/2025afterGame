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
		m_velocity(Vec3(0.0f)),
		m_angleSpeed(1.5f),
		m_playerIndex(0),
		m_respawnPos(Vec3(0.0f)),
		m_visualRoll(0.0f),
		m_bankRoll(0.0f),
		m_turnPower(1.0f),
		m_pitchSpeed(0.0f),
		m_yawSpeed(0.0f),
		m_recoveryTime(0.0f)
	{
	}

	Player::~Player()
	{

	}

	void Player::OnCreate()
	{
		FighterAircraftBase::OnCreate();

		m_hpMax = 100;
		m_hpCurrent = m_hpMax;

		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(Vec3(0.0f, -14.0f, -1.0f));

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"Sentouki");
		ptrDraw->SetTextureResource(L"diffuse_TX");

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(false);

		// モデルとトランスフォーム間の差分行列
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.2f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, -0.59f, 0.0f)
		);

		ptrDraw->SetMeshToTransformMatrix(spanMat);	

		m_gravity = AddComponent<Gravity>();
	}

	void Player::OnUpdate()
	{
		FighterAircraftBase::OnUpdate();

		auto& app = App::GetApp();
		auto deltaTime = app->GetElapsedTime();
		auto nowPos = GetComponent<Transform>()->GetPosition();
		auto nowRot = GetComponent<Transform>()->GetRotation();
		auto& input = InputManager::GetInputManager();
		
		// プレイヤーの挙動
		PlayerMove();
		TurnUpdate(deltaTime);

		// プレイヤーの装備
		//CreateBarrier();
		CreateBullet();
		
		// dpadでコントローラーを変える
		ChangController();
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		FighterAircraftBase::OnCollisionEnter(obj);
		
		auto bullet = dynamic_pointer_cast<Bullet>(obj);

		if (bullet)
		{
			auto trans = GetComponent<Transform>();
			trans->SetPosition(m_respawnPos);
		}
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

		Vec3 worldup = Vec3(0,1,0);
		Vec3 forward = ptrTrans->GetForward();
		forward.normalize();
		Vec3 moveDir = forward;

		Vec2 lstick = input->GetLStick();
		float pitchInput = 0.0f;

		// 入力の確保
		if (fabs(lstick.y) > fabs(lstick.x) * 1.1f)
		{
			pitchInput = lstick.y;
		}

		auto speedBrake = 1.0f;

		m_speedAdd = 3.0f;
		m_speedMax = 7.5f;

		ChangePlayer(lstick);

		if (m_playerIndex == 0)
		{
			m_aButton = input->GetButton(L"A");
		}
		else if (m_playerIndex == 1)
		{
			m_aButton = input->GetButton2(L"A");
		}

		// Aボタンを押して加速移動
		if (m_aButton)
		{
			m_recoveryTime = 0.0f;
			// 現在のスピードに加速度を足して動かす
			m_speedCurrent += m_speedAdd * deltaTime;
		}
		else
		{
			m_recoveryTime += deltaTime;
			m_speedCurrent -= speedBrake * deltaTime;
		}

		// 現在のスピードをclampで0.0f以下m_speedMax以上にならないよう
		m_speedCurrent = clamp(m_speedCurrent, 1.0f, m_speedMax);

		if (m_gravity)
		{
			// 空気密度
			float rho = 1.2f;
			// 前進速度
			float v = m_speedCurrent;
			// 面積パラメーター
			float s = 5.0f;
			// 揚力係数
			float cl = 0.0581;

			// 揚力の大きさを計算
			// ここが9.8より小さいとーが蓄積されて最終的に落ちてしまう
			float liftMag = 0.5f * rho * v * v * s * cl;

			// 加速度 // 9.8に近い数字になればいい
			Vec3 liftAcc = worldup * liftMag;

			// 重力の大きさ
			auto vel = m_gravity->GetGravityVelocity();
			
			// 重力に勝つためvelを足す重力はーでliftAccは＋でACC量で勝ったら受ける
			vel += liftAcc * deltaTime;

			float maxFallSpeed = -3.0f;

			if (m_recoveryTime < 5.0f)
			{
				maxFallSpeed = -3.0;
			}
			else
			{
				maxFallSpeed = -20.0f;
			}

			vel.y = max(vel.y, maxFallSpeed);

			m_gravity->SetGravityVerocity(vel);
		}

		// --- 移動処理 ---
		m_velocity = moveDir * m_speedCurrent;
		currentPos += m_velocity * deltaTime;

		// 移動反映
		ptrTrans->SetPosition(currentPos);
	}

	void Player::TurnUpdate(float deltaTime)
	{
		auto& app = App::GetApp();
		auto& game = GameManager::GetGameManager();
		auto pads = app->GetInputDevice();
		auto& input = InputManager::GetInputManager();
		Vec2 lstick = input->GetLStick();
		auto ptrTrans = GetComponent<Transform>();
		float deadZone = 0.1f;

		ChangePlayer(lstick);

		//----------------------------------------
		// Pitch
		//----------------------------------------
		float maxPitchSpeed = XMConvertToRadians(60.0f);
		
		float accel = 0.7f;
		float damping = 0.98; // 減少

		// Pitchのステックの判定の拡大
		if (fabs(lstick.y) > fabs(lstick.x) * 1.1f)
		{	
			if (lstick.y > deadZone)
			{
				accel = 0.9;
			} 
			else if(lstick.y < -deadZone)
			{
				accel = 0.4;
			}

			m_pitchSpeed += lstick.y * accel * deltaTime;
		}
		else
		{
			m_pitchSpeed *= damping;
		}

		m_pitchSpeed = clamp(m_pitchSpeed, -maxPitchSpeed, maxPitchSpeed);

		Quat pitchQuat;
		pitchQuat.rotationAxisAngle(Vec3(1, 0, 0), m_pitchSpeed * deltaTime);

		//----------------------------------------
		// Roll
		//----------------------------------------
		float maxRoll = XMConvertToRadians(45.0f);
		float rollInput = 0.0f;

		if (fabs(lstick.x) > fabs(lstick.y) * 1.1f)
		{
			rollInput = lstick.x * m_angleSpeed * deltaTime;
		}

		m_bankRoll = clamp(m_bankRoll + rollInput, -maxRoll, maxRoll);
		
		Quat rollQuat;
		rollQuat.rotationAxisAngle(Vec3(0, 0, 1), -m_bankRoll);


		//----------------------------------------
		// Yaw
		//----------------------------------------
		if (fabs(lstick.x) > 0.01f)
		{
			m_yawSpeed += lstick.x * m_turnPower * deltaTime;
		}
		else
		{
			m_yawSpeed = lerp(m_yawSpeed, 0.0f, deltaTime * 2.5f);
		}

		float maxYawSpeed = XMConvertToRadians(90.0f);
		m_yawSpeed = clamp(m_yawSpeed, -maxYawSpeed, maxYawSpeed);

		Quat yawQuat;
		yawQuat.rotationAxisAngle(Vec3(0, 1, 0), m_yawSpeed * deltaTime);

		//----------------------------------------
		// 姿勢として累積するのは Pitch + Yawのみ
		//----------------------------------------
		m_currentQuat = yawQuat * pitchQuat * m_currentQuat;
		m_currentQuat.normalize();

		//----------------------------------------
		// Rollはルック（見た目）だけ後から合成
		//----------------------------------------
		Quat finalQuat = rollQuat * m_currentQuat;
		finalQuat.normalize();

		ptrTrans->SetQuaternion(finalQuat);
	}


	//void Player::CreateBarrier()
	//{
	//	auto stage = GetStage();
	//	auto& input = InputManager::GetInputManager();

	//	Vec3 pos = GetComponent<Transform>()->GetPosition();

	//	if (!m_barrier)
	//	{
	//		m_barrier = stage->AddGameObject<Barrier>(GetThis<Player>());
	//	}

	//	auto useflag = m_barrier->GetUse();

	//	if (input->GetDownButton(L"X", m_playerIndex))
	//	{
	//		if (!useflag)
	//		{
	//			m_barrier->SetUse(true);
	//		}
	//		else
	//		{
	//			m_barrier->SetUse(false);
	//		}
	//	}
	//}

	void Player::CreateBullet()
	{
		auto stage = GetStage();
		auto& input = InputManager::GetInputManager();
		// 前フレームのTrigger値
		static BYTE prevTrigger = 0;

		BYTE nowTrigger = input->GetRightTrigger();
		BYTE threshold = 30;

		//「押した瞬間」だけ発射する
		if (prevTrigger <= threshold && nowTrigger > threshold)
		{
			if (m_bulletNumCurrentNow > 0)
			{
				m_bullet = stage->AddGameObject<Bullet>(GetThis<Player>());
				m_bulletNumCurrentNow -= 1;
			}
		}

		// 前フレーム値の更新を忘れない
		prevTrigger = nowTrigger;
	}

	// プレイヤーのコントローラ番号をセッタ
	void Player::SetPlayerIndex(int index)
	{
		m_playerIndex = index;
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

	void Player::ChangePlayer(Vec2 lstick)
	{
		auto& input = InputManager::GetInputManager();

		if (m_playerIndex == 0)
		{
			lstick = input->GetLStick();
		}
		else if (m_playerIndex == 1)
		{
			lstick = input->GetLStick2();
		}
	}
}
//end basecross

