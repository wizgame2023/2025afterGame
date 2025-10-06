/*!
@file Player.cpp
@brief プレイヤーなど実体
担当：吉田 智貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& ptrStage) :
		Actor(ptrStage),
		m_stickL(Vec3()),
		m_speed(NORMAL_SPEED),
		m_bustGauge(MAX_GAUGE),
		m_comboCount(0),
		m_comboTime(0.0f),
		m_timeOfStartAttack(1.0f),
		m_timeOfAttack(0.0f),
		m_plusAttack(0),
		m_attackCollisionFlag(false),
		m_comboActive(false)
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

		PlayerMove();
		PlayerBust();
		PlayerHealBust();
		PlayerAttack();
		PlayerComboReset();

		wstringstream wss(L"");
		wss << "X : " << nowPos.x  << " " << "Y : " << nowPos.y << " " << "Z : " << nowPos.z << " " << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());
	}

	void Player::OnCollisionEnter(const shared_ptr<GameObject>& Other)
	{

	}

	void Player::PlayerMove()
	{
		auto& app = App::GetApp();
		auto pad = GetFirstPad();

		auto ptrTrans = GetComponent<Transform>();
		auto elapsed = app->GetElapsedTime();
		auto currentPos = ptrTrans->GetPosition();
		auto deadZone = 0.1f;

		// 左右の移動
		if (abs(pad.fThumbLX) > deadZone)
		{
			currentPos.x += pad.fThumbLX * m_speed * elapsed;
		}
		if (abs(pad.fThumbLY) > deadZone)
		{
			currentPos.z += pad.fThumbLY * m_speed * elapsed;
		}

		// 上下の移動
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			currentPos.y += m_speed * elapsed;
		}
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			currentPos.y -= m_speed * elapsed;
		}

		ptrTrans->SetPosition(currentPos);
	}

	bool Player::IsBoostInputActive(const CONTROLER_STATE& pad) const
	{
		// 入力とデッドゾーンのみで、ブーストの入力が有効かどうかを判定
		return ((abs(pad.fThumbLX) > DEAD_ZONE)
			|| (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			|| (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
			&& (pad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
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

		bool isBoosting = IsBoostInputActive(pad) && (m_bustGauge > 0.0f);

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

		bool currentlyBoosting = IsBoostInputActive(pad) && (m_bustGauge > 0.0f);

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
		auto pad = GetFirstPad();

		if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
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

