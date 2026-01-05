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
		shared_ptr<Actor> m_trakingObjLock;

		float m_speed = 1.5f; // スピード

		// X軸に向いている方向
		float m_pitchAngle;
		// ロール方向に向いている方向
		float m_rollAngle;
		// ヨー方向に向いている方向
		float m_yawAngle;

		// 回転の目的地
		Vec3 m_goalRotVec;

		// 無敵フラグ
		bool m_invincibleFlag;
		float m_timeOfInvincible = 5.0f; // 無敵になる時間
		float m_countTimeOfInvincible; // どのくらい無敵になっているか計測する変数
		float m_countTimeOfBlinking; // 点滅している時間計測変数

		// 進みたいベクトル
		Vec3 m_moveVec;

		unique_ptr<StateEnemyMachine> m_stateMachine; // ステートマシン

	public:
		Enemy(const shared_ptr<Stage>& obj,const Vec3& pos,const Quat& qt,const Vec3& scale, const shared_ptr<CheckPoint>& startCheckPoint,const shared_ptr<Actor>& trackingObj);
		~Enemy();

		void OnCreate()override;
		void OnUpdate()override;

		// 当たり判定
		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;

		// 角度の調整0~360度までしか出ないようにする
		float AdjustmentAngle(float angle);

		// 角度移動する際に0から350度に移動する際の差の大きさがでかくなりすぎないようにする
		float CorrectRotationDirection(float differenceAngle);

		// ステートの変更処理
		void ChangeState(wstring stateName);

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

		// 無敵時の処理
		void Invincible();

		// 無敵時の点滅処理
		void DrawBlinking();

		// 障害物を避けるルートを考える処理
		shared_ptr<ObstaclesDodge> DodgeRoute();

		// 追いかける対象ポインタのゲッタ
		shared_ptr<Actor> GetTrackingObj();

		// 無敵フラグのゲッタ
		bool GetInvincibleFlag();
		// 無敵フラグをオンにする処理
		void OnInvincibleFlag();
	};

}

//end basecross

