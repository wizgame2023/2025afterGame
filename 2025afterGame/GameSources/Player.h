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
		Vec3 m_velocity;
		Vec3 m_yawvelocity;

		// スピード関連
		float m_speed;
		float m_targetSpeed;
		float m_accleRation;
		float m_deceleRation;
		float m_maxSpeed;
		float m_angleSpeed;
		float m_prevRoll;
		float m_lastYaw;

		float m_bustGauge;

		// 攻撃発生時間
		float m_timeOfStartAttack;
		
		// 攻撃時間計測
		float m_timeOfAttack;

		// プラスする攻撃力
		int m_plusAttack;

		// 攻撃判定出現フラグ
		bool m_attackCollisionFlag;

		float m_pitch;
		float m_roll;
		float m_autoYawSpeed;
		float m_currentRollAngle;
		float m_currentRoll;

		float m_startRoll;
		float m_startTime;
		float m_endTime;
		
		Quat m_initialQuat;
		bool m_initialized;
		bool m_hasInput;
		bool m_returnToNeutral;
		bool m_isReturning;
		float m_returnSpeed;

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
		void OnCollisionEnter(const shared_ptr<GameObject>& Other);

		// プレイヤーの移動処理
		void PlayerMove();
		
		// プレイヤーの角度処理
		void PlayerAngle();

		// プレイヤーのブースト
		void PlayerBust();

		// プレイヤーのブースト回復
		void PlayerHealBust();

		// PlayerBustの入力判定を条件式の関数化
		// ゲームパッドの入力状態
		bool GetIsBoostInputActive() const;

		// ゲージのクランプ処理
		void ClampBustGauge();

		Quat Slerp(const Quat& q1, const Quat& q2, float t)
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
	};
}
//end basecross

