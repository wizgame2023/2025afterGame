/*!
@file StateEnemy.h
@brief 敵のステート処理
担当者：三瓶裕太
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	// 敵ステートのベースになるクラス
	class StateEnemy :public StateBase
	{
	protected:
		weak_ptr<Enemy> m_enemy; // 親オブジェクトとなる敵
		shared_ptr<Enemy> m_enemyLock; // 親オブジェクトをロック(存在していることを確認できた変数)

	public:
		StateEnemy(const shared_ptr<MyGameObject>& parentObj);
		~StateEnemy();
		
		void OnEnter()override;
		void OnUpdate()override;
	};

	// オブジェクトに対して追いかけるステート
	class StateTrackingEnemy :public StateEnemy
	{
	private:
		weak_ptr<Actor> m_trackingObj; // 追いかける対象のポインタ
		shared_ptr<Actor> m_trackingObjLock; // ロック解除した追いかける対象のポインタ

	public:
		StateTrackingEnemy(const shared_ptr<MyGameObject>& parentObj);
		~StateTrackingEnemy();

		void OnEnter()override;
		void OnUpdate()override;

		// 自分と追尾対象の座標の差を計算する
		Vec3 CheckDifferencePos(Vec3 goalPos);
	};

	// 障害物を回避するステート
	class StateObstaclesDodgeEnemy :public StateTrackingEnemy
	{
	private:
		weak_ptr<Actor> m_trackingObj; // 追いかける対象のポインタ
		weak_ptr<ObstaclesDodge> m_obstaclesDodge; // 障害物を回避するためのルート
		shared_ptr<ObstaclesDodge> m_obstaclesDodgeLock; // 障害物を回避するためのルート

	public:
		StateObstaclesDodgeEnemy(const shared_ptr<MyGameObject>& parentObj);
		~StateObstaclesDodgeEnemy();

		void OnEnter()override;
		void OnUpdate()override;

		// 障害物を回避するためにここを通過点にしろと伝えるセッタ
		void SetObstaclesDodge(weak_ptr<ObstaclesDodge> obstaclesDodge);
	};


	// リスポーン待機ステート
	class StateRespawnEnemy :public StateEnemy
	{
	private:
		float m_timeOfReturn;	   // リスポーンまでの時間
		float m_countTimeOfReturn; // リスポーンの時間計測
	public:
		StateRespawnEnemy(const shared_ptr<MyGameObject>& parentObj);
		~StateRespawnEnemy();

		void OnEnter()override;
		void OnUpdate()override;
	};

	// 敵のステートマシンの作成
	class StateEnemyMachine :public StateMachineBase
	{
	public:
		StateEnemyMachine(const shared_ptr<MyGameObject>& parentObj):
			StateMachineBase(parentObj)
		{
			AddState(L"Base", shared_ptr<StateEnemy>(new StateEnemy(parentObj))); // 基盤部分
			AddState(L"Respawn", shared_ptr<StateRespawnEnemy>(new StateRespawnEnemy(parentObj))); // リスポーン
			AddState(L"Tracking", shared_ptr<StateTrackingEnemy>(new StateTrackingEnemy(parentObj))); // 追尾処理
			AddState(L"ObstaclesDodge", shared_ptr<StateObstaclesDodgeEnemy>(new StateObstaclesDodgeEnemy(parentObj))); // 障害物回避ステート
		}
	};


}

//end basecross

