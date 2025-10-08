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

		// スピード関連
		float m_speed;
		float m_targetSpeed;
		float m_accleRation;
		float m_deceleRation;
		float m_maxSpeed;
		float m_angleSpeed;

		float m_bustGauge;

		// 攻撃発生時間
		float m_timeOfStartAttack;
		
		// 攻撃時間計測
		float m_timeOfAttack;

		// プラスする攻撃力
		int m_plusAttack;

		// 攻撃判定出現フラグ
		bool m_attackCollisionFlag;

		float m_pitchAngle; // 現在のピッチ角 (ラジアン)
		float m_rollAngle;  // 現在のロール角 (ラジアン)

		// ピッチとロールの最大制限角度 (ラジアン)
		static constexpr float MAX_PITCH_LIMIT = XMConvertToRadians(90.0f);
		static constexpr float MAX_ROLL_LIMIT = XMConvertToRadians(90.0f);

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

		// コントローラを持ってくるとき
		CONTROLER_STATE GetFirstPad()
		{
			auto& app = App::GetApp();
			auto& input = app->GetInputDevice();
			auto& controllers = input.GetControlerVec();

			if (controllers.empty())
			{
				return CONTROLER_STATE{};
			}

			return controllers[0];
		}

		// プレイヤーの移動処理
		void PlayerMove();
		
		// プレイヤーの角度処理
		void PlayerAngle();

		// プレイヤーのブースト
		void PlayerBust();

		// プレイヤーのブースト回復
		void PlayerHealBust();

		// プレイヤーの攻撃
		void PlayerAttack();

	private:
		// PlayerBustの入力判定を条件式の関数化
		// ゲームパッドの入力状態
		bool Player::IsBoostInputActive() const
		{
			auto& inputMgr = InputManager::GetInputManager();

			bool stickActive = fabs(inputMgr->GetLStick().x) > DEAD_ZONE;
			bool shoulderActive = inputMgr->GetButton(L"L") || inputMgr->GetButton(L"R");
			bool triggerActive = inputMgr->GetLeftTrigger() > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

			return (stickActive || shoulderActive) && triggerActive;
		}

		// ゲージのクランプ処理
		void ClampBustGauge();

		Vec3 QuatToEuler(const Quat& q)
		{
			Vec3 euler;

			float sinr_cosp = 2.0f * (q.w * q.z + q.x * q.y);
			float cosr_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
			euler.z = atan2(sinr_cosp, cosr_cosp);

			float sinp = 2.0f * (q.w * q.y - q.z * q.x);

			if (abs(sinp) >= 1)
			{
				euler.y = copysign(XM_PI / 2, sinp);
			}
			else
			{
				euler.y = asin(sinp);
			}

			float siny_cosp = 2.0f * (q.w * q.x + q.y * q.z);
			float cosy_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
			euler.x = atan2(siny_cosp, cosy_cosp);

			return euler;
		}
	};

}
//end basecross

