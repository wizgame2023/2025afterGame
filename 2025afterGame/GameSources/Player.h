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

	template <typename T>
	constexpr T lerp(T a, T b, T t) noexcept
	{
		return a + (b - a) * t;
	}

	class Player : public FighterAircraftBase
	{
	private:
		shared_ptr<Barrier> m_barrier;
		shared_ptr<Bullet> m_bullet;

		// 速度ベクトル
		Vec3 m_velocity;
		// 弾が当たった時の位置
		Vec3 m_respawnPos;
		// 傾きのスピード
		float m_angleSpeed;
		// 十字キーの下が押されたら
		bool m_prevDDown;
		// Aボタンが押されたら
		bool m_aButton;
		// プレイヤーを識別するID
		int  m_playerIndex;
		// 現在の回転
		Quat m_currentQuat; 
		// 見た目の傾き
		float m_visualRoll;
		// 旋回の計算で使う
		float m_bankRoll;
		// 傾きから生じる旋回パワー
		float m_turnPower;

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
		@details Pitch（上下）・Roll（傾き）を入力に応じて制御し、 スティックを離した際には自動的に傾きを水平に戻す。
		@return なし
		*/
		void TurnUpdate(float deltaTime);

		/*
		@brief プレイヤーのバリア生成処理
		@details Xボタン入力時にバリアオブジェクトを生成・有効化する。
		@return なし
		*/
		//void CreateBarrier();

		/*
		@brief プレイヤーの弾丸生成処理
		@details Bボタン入力時に弾丸（Bullet）を生成する。
		@return なし
		*/
		//void CreateBullet();

		/*
		@brief プレイヤーのコントローラ番号を設定する
		@param index コントローラ番号（0または1）
		@return なし
		*/
		void SetPlayerIndex(int index);

		/*
		@brief コントローラ切り替え処理
		@details DPadの下入力により、プレイヤーインデックスを切り替える。
		@return なし
		*/
		void ChangController();

	};
}
//end basecross

