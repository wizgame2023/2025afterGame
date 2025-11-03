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
		m_velocity(Vec3(0.0f)),
		m_angleSpeed(1.0f),
		m_rollSpeed(1.0f)
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
		auto nowRot = GetComponent<Transform>()->GetRotation();
		auto& input = InputManager::GetInputManager();

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

		Vec2 lstick = input->GetLStick();

		if (m_playerIndex == 0)
		{
			lstick = input->GetLStick();
		}
		else if (m_playerIndex == 1)
		{
			lstick = input->GetLStick2();
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

		Vec2 lstick;

		if (m_playerIndex == 0)
		{
			lstick = input->GetLStick();
		}
		else
		{
			lstick = input->GetLStick2();
		}

		auto currentQuat = GetComponent<Transform>()->GetQuaternion();

		//----------------------------------------
		// Pitch（上下方向）
		//----------------------------------------
		float pitchInput = lstick.y;
		float pitchDelta = pitchInput * m_angleSpeed * deltaTime;

		// ピッチ回転をX軸に適用
		Quat pitchQuat;
		pitchQuat.rotationX(pitchDelta);

		//----------------------------------------
		// Roll（左右傾き）
		//----------------------------------------
		float rollInput = lstick.x;
		float rollDelta = -rollInput * m_rollSpeed * deltaTime;

		Vec3 up = ptrTrans->GetUp();
		Vec3 forward = ptrTrans->GetForward();
		Vec3 worldUp = Vec3(0, 1, 0);

		// 現在の傾き角（上方向と世界上方向のなす角）
		float dotUp = dot(up, worldUp);
		dotUp = clamp(dotUp, -1.0f, 1.0f);
		float tiltAngle = acosf(dotUp);

		// 傾き方向（右 or 左）を判定
		float rollDir = dot(cross(forward, up), worldUp);

		// 最大傾き制限
		constexpr float maxRoll = XMConvertToRadians(60.0f);
		if ((tiltAngle > maxRoll) &&
			((rollDir > 0.0f && rollDelta < 0.0f) || (rollDir < 0.0f && rollDelta > 0.0f)))
		{
			rollDelta = 0.0f;
		}

		// Roll回転をローカル前方軸で適用
		Vec3 rollAxis = ptrTrans->GetForward();
		rollAxis.normalize();
		Quat rollQuat = rotationAxis(rollAxis, rollDelta);
		rollQuat.normalize();

		//----------------------------------------
		// ★ 入力がないときは水平復帰（AutoUpright）
		//----------------------------------------
		if (fabs(lstick.x) < 0.05f) // スティックを離したら
		{
			auto ptrTrans = GetComponent<Transform>();
			Quat currentQuat = ptrTrans->GetQuaternion();

			// 現在の軸ベクトルを取得
			Vec3 up = ptrTrans->GetUp();
			Vec3 right = ptrTrans->GetRight();
			Vec3 forward = ptrTrans->GetForward();

			// 世界の上方向
			Vec3 worldUp = Vec3(0, 1, 0);

			// 現在の上ベクトルを世界上方向へ近づけるための回転を求める
			Vec3 correctionAxis = cross(up, worldUp);
			float correctionAngle = acosf(clamp(dot(up, worldUp), -1.0f, 1.0f));

			// 傾きがほぼ水平ならスキップ
			if (correctionAngle > XMConvertToRadians(0.5f))
			{
				// Rollだけ戻す（前方向は維持）
				Quat correctionQuat = rotationAxis(forward, correctionAngle * 0.1f); // 戻るスピード調整
				Quat resultQuat = Slerp(currentQuat, correctionQuat * currentQuat, deltaTime * 4.0f);
				resultQuat.normalize();
				ptrTrans->SetQuaternion(resultQuat);
			}
		}

		//----------------------------------------
		// 合成：Roll → Pitch（順序重要）
		//----------------------------------------
		Quat deltaQuat = rollQuat * pitchQuat;

		//----------------------------------------
		// 結果反映
		//----------------------------------------
		Quat resultQuat = deltaQuat * currentQuat;
		resultQuat.normalize();

		ptrTrans->SetQuaternion(resultQuat);

		auto scene = App::GetApp()->GetScene<Scene>();
		std::wstringstream wss;

		//// Pitch / Roll 入力値
		//wss << L"PitchInput: " << pitchInput
		//	<< L"\nRollInput : " << rollInput
		//	<< L"\nPitchDelta: " << pitchDelta
		//	<< L"\nRollDelta : " << rollDelta;

		//// クォータニオン要素
		//wss << L"\nPitchQuat: (" << pitchQuat.x << L"," << pitchQuat.y << L"," << pitchQuat.z << L"," << pitchQuat.w << L")";
		//wss << L"\nRollQuat : (" << rollQuat.x << L"," << rollQuat.y << L"," << rollQuat.z << L"," << rollQuat.w << L")";

		//// 傾き角度と方向
		//wss << L"\nTiltAngle(deg): " << XMConvertToDegrees(tiltAngle)
		//	<< L"\nRollDir: " << rollDir;

		//wss << L"\nResultQuat: (" << resultQuat.x << L"," << resultQuat.y << L"," << resultQuat.z << L"," << resultQuat.w << L")";

		//// シーンに表示
		//scene->SetDebugString(wss.str());
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
			(s0 * q1.x) + (s1 * q2b.x),
			(s0 * q1.y) + (s1 * q2b.y),
			(s0 * q1.z) + (s1 * q2b.z),
			(s0 * q1.w) + (s1 * q2b.w)
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
	
	Vec3 Player::QuaternionToEuler(const Quat& q)
	{
		Vec3 euler;

		// Pitch（X軸回転）
		float sinp = 2.0f * (q.w * q.x + q.y * q.z);
		float cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
		euler.x = atan2(sinp, cosp);

		// Yaw（Y軸回転）
		float siny = 2.0f * (q.w * q.y - q.z * q.x);
		siny = clamp(siny, -1.0f, 1.0f); // 安定化
		euler.y = asin(siny);

		// Roll（Z軸回転）
		float sinr = 2.0f * (q.w * q.z + q.x * q.y);
		float cosr = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
		euler.z = atan2(sinr, cosr);

		return euler; // ラジアン単位
	}

	Vec3 Player::RotateVectorByQuat(const Vec3& v, const Quat& q)
	{
		// クォータニオンをベクトル部分とスカラー部分に分解
		Vec3 qv(q.x, q.y, q.z);
		float qw = q.w;

		// まず、q と v（をクォータニオンとして扱ったもの）を掛ける
		Vec3 t = 2.0f * cross(qv, v);
		Vec3 rotated = v + qw * t + cross(qv, t);

		return rotated;
	}

	float Player::AngleBetWeen(const Quat& a, const Quat& b)
	{
		float dot = fabs(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);

		dot = clamp(dot, -1.0f, 1.0f);

		return acosf(dot) * 2.0f;
	}

	Quat Player::rotationAxis(const Vec3& axis, float angle)
	{
		Vec3 n = axis;
		n.normalize();
		float s = sinf(angle * 0.5f);
		float c = cosf(angle * 0.5f);
		return Quat(n.x * s, n.y * s, n.z * s, c);
	}
}
//end basecross

