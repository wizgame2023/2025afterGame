/*!
@file Player.h
@brief プレイヤーなど
担当：吉田 智貴
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
		float m_comboCount;
		float m_comboTime;

		// 攻撃発生時間
		float m_timeOfStartAttack;
		
		// 攻撃時間計測
		float m_timeOfAttack;

		// プラスする攻撃力
		int m_plusAttack;

		// 攻撃判定出現フラグ
		bool m_attackCollisionFlag;

		bool m_comboActive;

		// クラス全体で共有される定数
		// 何度も関数内でローカル変数で読むのは悪いなのでここで初期化、コンパイル時に値を決定
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

		// プレイヤーのコンボ関数
		void PlayerCombo()
		{
			m_comboCount++;
			m_comboTime = 0.0f;
			m_comboActive = true;
		}

		// プレイヤーのコンボをリセット
		void PlayerComboReset()
		{
			auto& app = App::GetApp();
			auto elapsed = app->GetElapsedTime();
			const float resetTime = 10.0f;
			
			if (m_comboActive)
			{
				m_comboTime += elapsed;
			}
				
			if (m_comboTime >= resetTime)
			{
				m_comboCount = 0;
				m_comboTime = 0.0f;
				m_comboActive = false;
			}
		}

	private:
		// PlayerBustの入力判定を条件式の関数化
		// ゲームパッドの入力状態
		bool IsBoostInputActive(const CONTROLER_STATE& pad) const;

		// ゲージのクランプ処理
		void ClampBustGauge();
	};

}
//end basecross

