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
#include "CheckPoint.h"

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
		shared_ptr<Barrier> m_barrier;
		shared_ptr<Bullet> m_bullet;
		shared_ptr<CheckPoint> m_checkPoint;

		Vec3 m_velocity;
		Vec3 m_checkPointPos;

		float m_angleSpeed;
		float m_rollSpeed;
		bool m_acceleration;

		Quat m_initialQuat;

		bool m_prevDDown;
		int  m_playerIndex;
		bool m_aButton;

		float m_yawSpeed;
		Quat  m_targetQuat;

		Vec3 m_respawnPos;

		// クラス全体
		// で共有される定数
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
		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;

		//----------------------------------------
		// プレイヤーの移動・回転・入力関連
		//----------------------------------------

		/*
		@brief プレイヤーの移動処理
		@details 左スティック入力や加速ボタンに応じて、機体を前進させる。
		@return なし
		*/
		void PlayerMove();

		/*
		@brief プレイヤーの角度処理
		@details Pitch（上下）・Roll（傾き）を入力に応じて制御し、
		          スティックを離した際には自動的に傾きを水平に戻す。
		@return なし
		*/
		void PlayerAngle();

		Quat PlayerPitch(const float stickY, float deltaTime);

		Quat PlayerRoll(const float stickX, float deltaTime);

		/*
		@brief ブースト入力の状態取得
		@details スティック、ショルダー、トリガー入力のいずれかが有効な場合に true を返す。
		@return ブースト入力がアクティブなら true
		*/
		bool GetIsBoostInputActive() const;

		/*
		@brief プレイヤーのバリア生成処理
		@details Xボタン入力時にバリアオブジェクトを生成・有効化する。
		@return なし
		*/
		void CreateBarrier();

		/*
		@brief プレイヤーの弾丸生成処理
		@details Bボタン入力時に弾丸（Bullet）を生成する。
		@return なし
		*/
		void CreateBullet();

		/*
		@brief コントローラ切り替え処理
		@details DPadの下入力により、プレイヤーインデックスを切り替える。
		@return なし
		*/
		void ChangController();

		/*
		@brief プレイヤーのコントローラ番号を設定する
		@param index コントローラ番号（0または1）
		@return なし
		*/
		void SetPlayerIndex(int index);

		/*
		@brief プレイヤーのコントローラ番号を取得する
		@return 現在のコントローラ番号
		*/
		int GetPlayerIndex() const;

		/*
		@brief プレイヤーが加速しているかを取得する
		@return 加速中なら true、そうでなければ false
		*/
		bool GetAcceleration();

		/*
		@brief クォータニオン補間（Slerp）
		@param q1 開始クォータニオン
		@param q2 終了クォータニオン
		@param t 補間係数（0〜1）
		@return 補間後のクォータニオン
		*/
		Quat Slerp(const Quat& q1, const Quat& q2, float t);

		/*
		@brief 軸と角度からクォータニオンを生成
		@param axis 回転軸ベクトル
		@param angleRad 回転角（ラジアン）
		@return 生成されたクォータニオン
		*/
		Quat FromAxisAngle(const Vec3& axis, float angleRad);

		/*
		@brief クォータニオンをオイラー角（Pitch, Yaw, Roll）に変換
		@param q クォータニオン
		@return オイラー角（ラジアン単位）
		*/
		Vec3 QuaternionToEuler(const Quat& q);

		/*
		@brief クォータニオンでベクトルを回転
		@param v 回転させたいベクトル
		@param q 回転に使用するクォータニオン
		@return 回転後のベクトル
		*/
		Vec3 RotateVectorByQuat(const Vec3& v, const Quat& q);

		/*
		@brief 2つのクォータニオン間の角度差を取得
		@param a クォータニオンA
		@param b クォータニオンB
		@return 2つのクォータニオン間の角度（ラジアン）
		*/
		float AngleBetWeen(const Quat& a, const Quat& b);

		/*
		@brief 指定軸まわりに回転するクォータニオンを生成
		@param axis 回転軸ベクトル
		@param angle 回転角（ラジアン）
		@return 回転クォータニオン
		*/
		inline Quat rotationAxis(const Vec3& axis, float angle);


		Vec3 GetCheckPointPos() const 
		{
			return m_checkPointPos;
		}
	};
}
//end basecross

