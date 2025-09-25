/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& ptrStage) :
		Actor(ptrStage),
		m_stickL(Vec3()),
		m_speed(NORMAL_SPEED),
		m_bustGauge(MAX_GAUGE)
	{
	}

	Player::~Player()
	{

	}

	void Player::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(Vec3(0.0f));

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void Player::OnUpdate()
	{
		auto& app = App::GetApp();
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];

		PlayerMove();
		PlayerBust();
		PlayerHealBust();
	}

	void Player::PlayerMove()
	{
		auto& app = App::GetApp();
		auto& input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];

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
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			currentPos.y += m_speed * elapsed;
		}
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			currentPos.y -= m_speed * elapsed;
		}

		ptrTrans->SetPosition(currentPos);
	}

	bool Player::IsBoostInputActive(const CONTROLER_STATE& pad) const
	{
		// 入力とデッドゾーンのみで、ブーストの入力が有効かどうかを判定
		return (abs(pad.fThumbLX) > DEAD_ZONE) &&
			(pad.wButtons & XINPUT_GAMEPAD_B);
	}

	void Player::ClampBustGauge()
	{
		// bustGaugeの上限をMaxGaugeにすることが出来る
		m_bustGauge = clamp(m_bustGauge,0.0f, MAX_GAUGE);
	}

	void Player::PlayerBust()
	{
		auto& app = App::GetApp();
		auto& input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
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
		auto elapsed = app->GetElapsedTime();

		auto& input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
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
		
	}

}
//end basecross

