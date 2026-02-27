/*!
@file Enemy.h
@brief 敵
担当者：三瓶裕太
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class StateEnemyMachine;
	class Enemy:public FighterAircraftBase
	{
	private:
		float m_countDebagBulletTime; // デバック用の弾を出す時間計測変数

		// 追いかける目標のオブジェクト
		weak_ptr<Actor> m_trackingObj;
		//shared_ptr<Actor> m_trakingObjLock;

		float m_speed = 5.0f; // スピード

		// X軸に向いている方向
		float m_pitchAngle;
		// ロール方向に向いている方向
		float m_rollAngle;
		// ヨー方向に向いている方向
		float m_yawAngle;

		// 回転の目的地
		Vec3 m_goalRotVec;

		// 進みたいベクトル
		Vec3 m_moveVec;

		unique_ptr<StateEnemyMachine> m_stateMachine; // ステートマシン


		// 緊急処置
		float m_timeOfPlayerLock;
		bool m_playerLock;

		// HPゲージ
		shared_ptr<BillBoardGauge> m_Gauge;
		// ランキング
		shared_ptr<BillBoardNumber> m_rankNumber;

	public:
		// 今後は使わない
		Enemy(const shared_ptr<Stage>& obj, const Vec3& pos, const Quat& qt, const Vec3& scale, const shared_ptr<CheckPoint>& startCheckPoint, const shared_ptr<Actor>& trackingObj);
		// 今後はこちらを使う
		Enemy(const shared_ptr<Stage>& obj, const Vec3& pos, const Vec3& rot, const Vec3& scale, const shared_ptr<CheckPoint>& startCheckPoint, const shared_ptr<Actor>& trackingObj);
		~Enemy();

		void OnCreate()override;
		void OnUpdate()override;

		// 当たり判定
		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;

		// 角度の調整0~360度までしか出ないようにする
		float AdjustmentAngle(float angle);

		// 角度の差が大きいときに別方向に進んだ角度の差を求める処理
		float CorrectRotationDirection(float differenceAngle);

		// ステートの変更処理
		void ChangeState(wstring stateName);

		// 目標の変更処理
		void ChangeTarget(GamePhase currentPhase, const vector<shared_ptr<GameObject>>& objVec);

		// 対象に向かって追いかける処理
		void TrackingMove(const Vec3& posPlayerDifference);

		// 追いかける対象に向かってX軸回転方向で向く処理
		void TrackingPitchQt(const Vec3& posPlayerDifference);

		// 追いかける対象に向かってZ軸回転方向で向く処理
		void TrackingRollQt();

		// 追いかける対象に向かってY軸回転方向で向く処理
		void TrackingYawQt(const Vec3& posPlayerDifference);

		// ヨーピッチロールに沿って回転する処理
		void MoveRotate();

		// 障害物を避ける処理
		void DodgeObstacles(const Vec3& posPlayerDifference);

		// 障害物を避けるルートを考える処理
		shared_ptr<ObstaclesDodge> DodgeRoute();

		// 追いかける対象ポインタのゲッタ
		shared_ptr<Actor> GetTrackingObj();

		// ターゲットのセッタ
		void SetTracking(const shared_ptr<Actor>& target);
		// ターゲットのゲッタ
		const weak_ptr<Actor>& GetTracking();

		// 線形補間関数(Vec3用)
		// 参考 : https://taketakeshi.hatenablog.jp/entry/2025/05/19/205447
		// start : 開始地 end : 終了値 time : 補間係数(0.0f～1.0f)
		// 戻り値 : 補間後の値
		static Vec3 LerpV3(const Vec3& start, const Vec3& end, float time) {
			return start + (end - start) * time;
		}

	};

}

//end basecross

