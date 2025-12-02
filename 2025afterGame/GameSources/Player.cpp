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
		m_angleSpeed(2.0f),
		m_playerIndex(0),
		m_respawnPos(Vec3(0.0f)),
		m_visualRoll(0.0f),
		m_bankRoll(0.0f),
		m_turnPower(2.0f)
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
		ptrTrans->SetPosition(Vec3(0.0f, 0.0f, -1.0f));

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

		Vec3 forward = ptrTrans->GetForward();
		forward.normalize();
		Vec3 moveDir = forward;

		Vec2 lstick = input->GetLStick();

		m_speedAdd = 3.0f;
		m_speedMax = 10.0f;

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
			// 現在のスピードに加速度を足して動かす
			m_speedCurrent += m_speedAdd * deltaTime;
			// 現在のスピードをclampで0.0f以下m_speedMax以上にならないよう
			m_speedCurrent = clamp(m_speedCurrent, 0.0f, m_speedMax);

			// --- 移動処理 ---
			m_velocity = moveDir * m_speedCurrent;
			currentPos += m_velocity * deltaTime;
		}

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
		bool hasInput = (fabs(lstick.x) > 0.01f || fabs(lstick.y) > 0.01f);

		ChangePlayer(lstick);

		//----------------------------------------
		// Pitch
		//----------------------------------------
		float pitchInput = 0.0f;

		if (fabs(lstick.y) > fabs(lstick.x) * 1.1f)
		{
			pitchInput = lstick.y * m_angleSpeed * deltaTime;
		}

		Quat pitchQuat;
		pitchQuat.rotationAxisAngle(Vec3(1, 0, 0), pitchInput);


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
		// BankTurnによるYaw
		//----------------------------------------
		float yawInput = lstick.x * m_turnPower * deltaTime;
		Quat yawQuat;
		yawQuat.rotationAxisAngle(Vec3(0, 1, 0), yawInput);


		//----------------------------------------
		// 姿勢として累積するのは Pitch + Yawのみ
		//----------------------------------------
		m_currentQuat = yawQuat * pitchQuat * m_currentQuat;
		m_currentQuat.normalize();

		if (!hasInput)
		{
			m_bankRoll = lerp(m_bankRoll, 0.0f, deltaTime * 4.0f);
		}

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

	// フラグのゲッタ、セッタ
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

