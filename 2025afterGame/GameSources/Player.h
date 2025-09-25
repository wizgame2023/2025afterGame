/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Actor.h"

namespace basecross{

	// clampテンプレート関数
	template <typename T>
	T clamp(T value, T minValue, T maxValue)
	{
		if (value < minValue) return minValue;
		if (value > maxValue) return maxValue;
		return value;
	}

	class Player : public Actor
	{
		Vec3 m_stickL;
		float m_speed;
		float m_bustGauge;

		// クラス全体で共有される定数
		static constexpr float DEAD_ZONE = 0.1f;
		static constexpr float MAX_SPEED = 7.0f;
		static constexpr float NORMAL_SPEED = 1.0f;
		static constexpr float MAX_GAUGE = 100.0f;
		static constexpr float GAUGE_CONSUMPTION_RATE = 1.0f;
		static constexpr float GAUGE_RECOVERY_RATE = 3.0f;
	
	public:
		Player::Player(const shared_ptr<Stage>& ptrStage);
		Player::~Player();

		void OnCreate() override;
		void OnUpdate() override;
		
		// プレイヤーの移動処理
		void PlayerMove();
		
		// プレイヤーのブースト
		void PlayerBust();

		// プレイヤーのブースト回復
		void PlayerHealBust();

		// プレイヤーの攻撃
		void PlayerAttack();

	private:
		// PlayerBustの入力判定を条件式の関数化
		bool IsBoostInputActive(const CONTROLER_STATE& pad) const;

		// ゲージのクランプ処理
		void ClampBustGauge();
	};

}
//end basecross

