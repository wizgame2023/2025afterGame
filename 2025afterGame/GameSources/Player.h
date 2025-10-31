/*!
@file Player.h
@brief プレイヤーなど
担当：吉田 智貴
*/

#pragma once
#include "stdafx.h"
#include "Actor.h"
#include "Barrier.h"
#include "Bullet.h"

namespace basecross{

	// clampテンプレート関数
	template <typename T>
	T clamp(T value, T minValue, T maxValue)
	{
		if (value < minValue) return minValue;
		if (value > maxValue) return maxValue;
		return value;
	}

	enum class ActionMode
	{
		None,
		Barrier,
		Bullet,
		jama
	};

	class Player : public FighterAircraftBase
	{
	private:
		// 参照するもの
		shared_ptr<Barrier> m_barrier;
		shared_ptr<Bullet> m_bullet;

		Vec3 m_velocity;

		// 加速・減速
		float m_speedCurrent;
		float m_speedMax;
		float m_accleRation;
		float m_deceleRation;
		float m_angleSpeed;
		float m_rollSpeed;
		float m_prevRoll;
		bool m_acceleration; // 加速しているか

		// lerp 用変数
		float m_currentRoll;
		float m_startRoll;
		float m_startTime;
		float m_endTime;
		
		// Slerp 用変数
		Quat m_initialQuat;
		bool m_initialized;
		bool m_hasInput;
		bool m_returnToNeutral;
		bool m_isReturning;

		// その他の状態
		bool m_fullEnergy;
		int m_playerIndex;
		ActionMode m_actionMode;

		// 入力状態
		bool m_aButton;
		bool m_prevDDown;
		bool m_uprightTime;

		float m_yawAmount;
		float m_initialRoll;
		float m_yawSpeed;
		Vec3 m_rotation;
		float m_turnStrength;

		// クラス全体で共有される定数
		// 何度も関数内でローカル変数で読むのは悪いなのでここで初期化、コンパイル時に値を決定
		static constexpr float DEAD_ZONE = 0.1f;
		static constexpr float DEAD_ZONE_PITCH = 0.4f;
		static constexpr float MAX_SPEED = 7.0f;
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
		Quat PlayerPitch(const float stickY, float deltaTime);
		Quat PlayerRoll(const float stickY, float deltaTime);
		Quat PlayerYawWorld(const float stickY, float deltaTime);
		Quat PlayerYaw(const float stickY, float deltaTime);

		// PlayerBustの入力判定を条件式の関数化
		// ゲームパッドの入力状態
		bool GetIsBoostInputActive() const;

		// プレイヤーの装備
		// プレイヤーバリア
		void CreateBarrier();
		// プレイヤーの銃撃
		void CreateBullet();

		void ChangController();

		Quat Slerp(const Quat& q1, const Quat& q2, float t);

		Quat FromAxisAngle(const Vec3& axis, float angleRad);

		Vec3 QuaternionToEuler(const Quat& q)
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


		float AngleBetWeen(const Quat& a, const Quat& b);

		void AutoUpright(float deltaTime);

		// プレイヤーのコントローラ番号をセッタ
		void SetPlayerIndex(int index);

		// プレイヤーのコントローラ番号ゲッタ
		int GetPlayerIndex() const;

		// 加速しているかのゲッタ
		bool GetAcceleration();

	};
}
//end basecross

