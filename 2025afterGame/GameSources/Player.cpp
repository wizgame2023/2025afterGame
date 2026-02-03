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
		m_recoveryTime(0.0f),
		m_yawMoveFlag(false),
		m_pitchMoveFlag(false),
		m_pitchAngle(0.0f),
		m_yawAngle(0.0f),
		m_rollAngle(0.0f),
		m_baseMeshMat(),
		m_moveAnimationFlag(false),
		m_stopAnimationFlag(false),
		m_aButton(false),
		m_isGrounded(false)
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
		auto ptrShadow = AddComponent<Shadowmap>();

		m_draw = AddComponent<PNTBoneModelDraw>();
		m_draw->SetMeshResource(L"Sentouki");
		m_draw->SetTextureResource(L"diffuse_TX");

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

		m_baseMeshMat = spanMat;
		m_draw->SetMeshToTransformMatrix(spanMat);
		ptrShadow->SetMeshResource(L"Sentouki");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		m_draw->AddAnimation(L"PropellerMove",  0, 50, 60.0f);
		m_draw->AddAnimation(L"PropellerDown", 40, 20, 30.0f);
		m_draw->AddAnimation(L"PropellerStop",  0,  1,  0.0f);
		
		m_draw->ChangeCurrentAnimation(L"PropellerStop");

		// m_gravity = AddComponent<Gravity>();

	}

	void Player::OnUpdate()
	{
		if (GetPauseFlag()) return;

		FighterAircraftBase::OnUpdate();
		auto& app = App::GetApp();
		auto deltaTime = app->GetElapsedTime();
		auto pos = GetComponent<Transform>()->GetPosition();
		auto& gameManager = GameManager::GetGameManager();
		auto start = gameManager->GetGameStartCountDown();

		if (start == 0) return;
		
		// プレイヤーの挙動
		PlayerMove();
		TurnUpdate(deltaTime);
		PlayerGravity(deltaTime);

		// プレイヤーの装備
		// CreateBarrier();
		CreateBullet();
		
		// dpadでコントローラーを変える
		ChangController();

		PlayerRespon();
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		FighterAircraftBase::OnCollisionEnter(obj);
		
		auto bullet = dynamic_pointer_cast<Bullet>(obj);

		if (bullet)
		{
			bool bulletAffiliation = bullet->GetAffiliation();
			GetStage()->RemoveGameObject<Bullet>(bullet);

			// 弾の所属がエネミーならダメージを受ける
			if (bulletAffiliation == false)
			{
				m_hpCurrent -= bullet->GetDamage();
			}

			// HPが０になったらリスポーンする
			if (m_hpCurrent <= 0)
			{
				// スコアを10%倒した敵に譲渡する
				DownTransferScore(bullet, 0.1f);
			}
		}

		if (obj->FindTag(L"CameraObsDiffuse"))
		{
			m_isGrounded = true;
		}

	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& obj)
	{
		if (obj->FindTag(L"CameraObsDiffuse"))
		{
			m_isGrounded = false;
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

		m_aButton = input->GetButton(L"A");

		// Aボタンを押して加速移動
		if (m_aButton)
		{
			if (!m_moveAnimationFlag)
			{
				m_draw->ChangeCurrentAnimation(L"PropellerMove");
				m_moveAnimationFlag = true;
			}

			m_recoveryTime = 0.0f;
			// 現在のスピードに加速度を足して動かす
			m_speedCurrent += m_speedAdd * deltaTime;

		}
		else
		{
			if (m_moveAnimationFlag)
			{
				m_draw->ChangeCurrentAnimation(L"PropellerDown");
				m_moveAnimationFlag = false;
			}

			auto animTime = m_draw->GetCurrentAnimationTime();
			if (animTime >= 0.5f)
			{
				m_draw->ChangeCurrentAnimation(L"PropellerStop");
			}

			m_recoveryTime += deltaTime;
			m_speedCurrent -= speedBrake * deltaTime;
		}

		m_draw->UpdateAnimation(deltaTime);

		// Flight(deltaTime);

		// 現在のスピードをclampで0.0f以下m_speedMax以上にならないよう
		m_speedCurrent = clamp(m_speedCurrent, 0.0f, m_speedMax);

		// --- 移動処理 ---
		m_velocity = moveDir * m_speedCurrent;
		currentPos += m_velocity * deltaTime;

		// 移動反映
		ptrTrans->SetPosition(currentPos);
	}

	void Player::TurnUpdate(float deltaTime)
	{
		auto& input = InputManager::GetInputManager();
		Vec2 lstick = input->GetLStick();
		auto ptrTrans = GetComponent<Transform>();

		const float deadZone = 0.1f;
		const float resetFactor = 0.5f;

		float stickMagnitude = lstick.length();

		m_pitchMoveFlag = false;
		m_yawMoveFlag = false;

		// -----------------------------
		// 入力方向判定
		// -----------------------------
		if (stickMagnitude > deadZone)
		{
			float stickAngle = atan2(lstick.y, lstick.x);
			float angleDeg = fabs(XMConvertToDegrees(stickAngle));

			// Pitch優先
			if (angleDeg > 60.0f && angleDeg < 120.0f)
			{
				m_pitchMoveFlag = true;
				m_yawSpeed *= resetFactor;
			}
			// Yaw優先
			else if (angleDeg < 30.0f || angleDeg > 150.0f)
			{
				m_yawMoveFlag = true;
				m_pitchSpeed *= resetFactor;
			}
			else
			{
				m_yawMoveFlag = true;
				m_pitchMoveFlag = true;
			}
		}

		// -----------------------------
		// Yaw 更新
		// -----------------------------
		if (m_yawMoveFlag)
		{
			if (fabs(lstick.x) > deadZone)
			{
				m_yawSpeed += lstick.x * m_turnPower * deltaTime;
			}
			else
			{
				m_yawSpeed = lerp(m_yawSpeed, 0.0f, deltaTime * 2.5f);
			}

			float maxYawSpeed = XMConvertToRadians(90.0f);
			m_yawSpeed = clamp(m_yawSpeed, -maxYawSpeed, maxYawSpeed);

			Quat yawDelta;
			yawDelta.rotationAxisAngle(Vec3(0, 1, 0), m_yawSpeed * deltaTime);

			m_currentQuat = yawDelta * m_currentQuat;
			m_currentQuat.normalize();
		}

		// -----------------------------
		// Pitch 更新
		// -----------------------------
		if (m_pitchMoveFlag && !m_yawMoveFlag)
		{
			m_yawSpeed = 0.0f;

			float accel = 0.7f;

			if (fabs(lstick.y) > deadZone)
			{
				m_pitchSpeed += lstick.y * accel * deltaTime;
			}

			float maxPitchSpeed = XMConvertToRadians(60.0f);
			m_pitchSpeed = clamp(m_pitchSpeed, -maxPitchSpeed, maxPitchSpeed);

			Quat pitchDelta;
			pitchDelta.rotationAxisAngle(Vec3(1, 0, 0), m_pitchSpeed * deltaTime);

			m_currentQuat = pitchDelta * m_currentQuat;
			m_currentQuat.normalize();
		}

		// -----------------------------
		// Yaw中は Pitch を消す（Yaw保持水平化）
		// -----------------------------
		else if (m_yawMoveFlag)
		{
			m_pitchSpeed = 0.0f;

			Vec3 forward = ptrTrans->GetForward();
			Vec3 forwardXZ(forward.x, 0.0f, forward.z);

			if (forwardXZ.lengthSqr() > 0.0001f)
			{
				forwardXZ.normalize();
			}

			float yaw = atan2f(forwardXZ.x, forwardXZ.z);

			Quat yawOnly;
			yawOnly.rotationAxisAngle(Vec3(0, 1, 0), yaw);

			float returnSpeed = 1.0f;
			float t = clamp(deltaTime * returnSpeed, 0.0f, 1.0f);

			m_currentQuat = m_currentQuat.Slerp(m_currentQuat, yawOnly, t);
			m_currentQuat.normalize();
		}
		else
		{
			m_yawSpeed = 0.0f;
			m_pitchSpeed = 0.0f;
		}

		ptrTrans->SetQuaternion(m_currentQuat);

		float leanScale = 0.4f;
		float maxRoll = XMConvertToRadians(30.0f);

		float targetRollAngle = -m_yawSpeed * leanScale;
		targetRollAngle = clamp(targetRollAngle, -maxRoll, maxRoll);

		float rollReturnSpeed = 5.0f;
		
		m_rollAngle = lerp(
			m_rollAngle,        // 現在の傾き
			targetRollAngle,    // 目標の傾き
			deltaTime * rollReturnSpeed
		);

		// Roll行列
		Quat rollQuat;
		rollQuat.rotationAxisAngle(Vec3(0, 0, 1), m_rollAngle);

		Mat4x4 rollMat;
		rollMat.identity();
		rollMat.rotation(rollQuat);

		// 初期行列 × Roll
		Mat4x4 finalMeshMat = m_baseMeshMat * rollMat;

		// 描画用に反映
		m_draw->SetMeshToTransformMatrix(finalMeshMat);
	}

	void Player::Flight(float deltaTime)
	{
		if (!m_gravity) return;

		// ワールド座標
		Vec3 transUp = Vec3(0.0f,1.0f,0.0f);
		// 空気密度
		float rho = 1.2f;
		// 前進速度
		float v = m_speedCurrent;
		// 面積パラメーター
		float s = 5.0f;

		// --- 追加：迎角（AoA）の計算 ---
		Vec3 forward = GetComponent<Transform>()->GetForward();
		// 重力コンポーネント等から現在の速度ベクトルを取得（仮にvelocityとする）
		Vec3 velocity = m_gravity->GetGravityVelocity();
		float aoa = 0.0f;

		if (velocity.lengthSqr() > 0.001f)
		{
			Vec3 vNorm = velocity; vNorm.normalize();
			// 進行方向と機首方向の角度差を求める
			float dot = forward.dot(vNorm);
			aoa = acos(clamp(dot, -1.0f, 1.0f));

			// 進行方向より機首が上を向いていれば正（プラス）の角度にする
			if (forward.y < vNorm.y)
			{
				aoa *= -1.0f;
			}
		}

		// --- 背面判定 ---
		// 機体のローカル上ベクトルが地面（マイナス方向）を向いているか
		bool isInverted = (GetComponent<Transform>()->GetUp().y < 0);

		// --- 揚力係数 cl の動的決定 ---
		float liftSlope = 1.5f; // どのくらい機敏に揚力が変わるかの係数
		float cl = aoa * liftSlope;

		// --- 背面飛行時：揚力を弱める ---
		if (isInverted)
		{
			cl *= 0.5f;
		}

		// 失速：角度が大きすぎたら揚力をゼロにする
		if (abs(aoa) > XMConvertToRadians(20.0f))
		{
			cl = 0.0f;
		}

		Vec3 liftDir = GetComponent<Transform>()->GetUp();

		// ----------------------------
		// Yaw中の高度維持補正（重要）
		// ----------------------------
		if (m_yawMoveFlag)
		{
			// 0.0 = 完全リアル（落ちる）
			// 1.0 = 完全ゲーム（絶対落ちない）
			float yawLiftAssist = 0.7f;

			Vec3 worldUp(0.0f, 1.0f, 0.0f);
			liftDir = liftDir * (1.0f - yawLiftAssist) + worldUp * yawLiftAssist;

			liftDir.normalize();
		}

		// --- 揚力の大きさを計算 ---
		// ここが9.8より小さいとーが蓄積されて最終的に落ちてしまう
		float liftMag = 0.5f * rho * v * v * s * cl;

		// --- 加速度 --- // 9.8に近い数字になればいい
		Vec3 liftAcc = liftDir * liftMag;

		//  ---重力加速度を取得 ---
		auto vel = m_gravity->GetGravityVelocity();

		// --- 重力に勝つためvelを足す、重力はーでliftAccは+で量で勝ったら浮く ---
		vel += liftAcc * deltaTime;

		float maxFallSpeed = -3.0f;

		//  --- Aボタンを離しても直ぐには落ちないように制限を付けている ---
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

	void Player::PlayerGravity(float deltaTime)
	{
		auto& input = InputManager::GetInputManager();
		auto transform = GetComponent<Transform>();

		Vec3 pos = transform->GetPosition();

		// ----------------------------
		// Y方向速度
		// ----------------------------
		static float verticalVelocity = 0.0f;
		static float fallTimer = 0.0f;

		// ----------------------------
		// 入力判定（Aボタン）
		// ----------------------------
		bool isFlyInput = input->GetButton(L"A");

		// ----------------------------
		// 落下制御
		// ----------------------------
		const float slowFallSpeed = -3.0f;   // 最初の落下速度
		const float fastFallSpeed = -9.0f;   // 後半の落下速度
		const float slowFallTime = 4.0f;     // ゆっくり落ちる時間


		if (isFlyInput)
		{
			m_isGrounded = false;
			// A押下中は完全停止
			verticalVelocity = 0.0f;
			fallTimer = 0.0f;
		}
		else if (pos.y <= -14.0f)
		{
			verticalVelocity = 0.0f;
			fallTimer = 0.0f;
		}
		else if (m_isGrounded)
		{
			verticalVelocity = 0.0f;
			fallTimer = 0.0f;
		}
		else
		{
			// タイマー進行
			fallTimer += deltaTime;

			// 0〜1 に正規化
			float t = fallTimer / slowFallTime;
			t = clamp(t, 0.0f, 1.0f);

			// 重力を徐々に強くする
			float gravity = lerp(slowFallSpeed, fastFallSpeed, t);

			// 重力加速
			verticalVelocity += gravity * deltaTime;
		}

		// ----------------------------
		// 位置反映
		// ----------------------------
		pos.y += verticalVelocity * deltaTime;
		transform->SetPosition(pos);
	}

	void Player::PlayerRespon()
	{
		if (m_hpCurrent == 0)
		{
			auto trans = GetComponent<Transform>();
			trans->SetPosition(Vec3(0.0f,-14.0f, -1.0f));

			m_hpCurrent = m_hpMax;
		}
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
		auto ptrMana = App::GetApp()->GetXAudio2Manager();

		auto& game = GameManager::GetGameManager();
		wstring& bulletKey = game->GetBulletKey();


		// 右か左トリガーに設定されていれば
		if (bulletKey == L"LTrigger" || bulletKey == L"RTrigger")
		{
			function<BYTE(wstring)> getTrigger = nullptr;
			if (bulletKey == L"LTrigger")
			{
				getTrigger = [&](wstring key)
					{
						return input->GetLeftTrigger();
					};
			}
			else if (bulletKey == L"RTrigger")
			{
				getTrigger = [&](wstring key)
					{
						return input->GetRightTrigger();
					};
			}

			//BYTE nowTrigger = input->GetRightTrigger();
			BYTE threshold = 30;


			//「押した瞬間」だけ発射する
			if ((prevTrigger <= threshold && getTrigger(bulletKey) > threshold))
			{
				if (m_bulletNumCurrentNow > 0)
				{
					ptrMana->Start(L"ShotSE", 0, 0.1f);

					m_bullet = stage->AddGameObject<Bullet>(GetThis<Player>());
					m_bulletNumCurrentNow -= 1;
				}
			}

			// 前フレーム値の更新を忘れない
			prevTrigger = getTrigger(bulletKey);

		}
		else
		{
			if (input->GetDownButton(bulletKey))
			{
				if (m_bulletNumCurrentNow > 0)
				{
					ptrMana->Start(L"ShotSE", 0, 0.1f);

					m_bullet = stage->AddGameObject<Bullet>(GetThis<Player>());
					m_bulletNumCurrentNow -= 1;
				}
			}
		}

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

