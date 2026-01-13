/*!
@file StateEnemy.cpp
@brief 敵ステート
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	StateEnemy::StateEnemy(const shared_ptr<MyGameObject>& parentObj) :
		StateBase(parentObj),
		m_enemy(dynamic_pointer_cast<Enemy>(parentObj))
	{

	}

	StateEnemy::~StateEnemy()
	{

	}

	void StateEnemy::OnEnter()
	{
		// 親クラスEnter処理
		StateBase::OnEnter();

		// 親オブジェクトが消えていないか確認する
		m_enemyLock = m_enemy.lock();
	}

	void StateEnemy::OnUpdate()
	{
		// 親クラスUpdate処理
		StateBase::OnUpdate();

		// 親オブジェクトが消えていないか確認する
		m_enemyLock = m_enemy.lock();

		// 親オブジェクトが消えていたらエラーメッセージ
		if (!m_enemyLock)
		{
			throw BaseException
			{
				L"親オブジェクトが存在しません",
				L"if (!m_enemyLock)",
				L"void StateEnemy::OnUpdate()"
			};
		}
	}


	// 追いかけるステート
	StateTrackingEnemy::StateTrackingEnemy(const shared_ptr<MyGameObject>& parentObj):
		StateEnemy(parentObj)
	{

	}

	StateTrackingEnemy::~StateTrackingEnemy()
	{

	}

	void StateTrackingEnemy::OnEnter()
	{
		// 親クラスEnter呼び出し
		StateEnemy::OnEnter();

		// 追いかける対象を取得
		m_trackingObj = m_enemyLock->GetTrackingObj();
	}

	void StateTrackingEnemy::OnUpdate()
	{
		// 親クラスUpdate呼び出し
		StateEnemy::OnUpdate();

		// 追いかける対象の更新
		m_trackingObj = m_enemyLock->GetTrackingObj();
		m_trackingObjLock = m_trackingObj.lock();

		if (!m_trackingObjLock)
		{
			throw BaseException
			{
				L"追いかける対象が存在しません",
				L"if (m_trackingObjLock)",
				L"void StateTrackingEnemy::OnUpdate()"
			};
		}

		// 親オブジェクトの位置
		auto parentPos = m_enemyLock->GetComponent<Transform>()->GetPosition();

		// 自分と追尾対象の座標の差を計算する
		auto goalPos = m_trackingObjLock->GetComponent<Transform>()->GetPosition();
		Vec3 posPlayerDifference = CheckDifferencePos(goalPos);
		posPlayerDifference.normalize(); // 正規化


		// 追いかけるときのロール回転処理(デバック用処理しか書いていない)
		m_enemyLock->TrackingRollQt();
		// 追いかける対象にX軸に向く処理
		m_enemyLock->TrackingPitchQt(posPlayerDifference);
		// 追いかける対象に向かってヨーを回転させる処理
		m_enemyLock->TrackingYawQt(posPlayerDifference);

		// 対象に向かって追いかける処理
		m_enemyLock->TrackingMove(posPlayerDifference);

		// そのまま進むと障害物にぶつかるかレイを使用して判断する
		m_enemyLock->DodgeObstacles(posPlayerDifference);

		// 追いかけようとしているオブジェクトよりもスコアが高いものが近くにあるならそっちを追いかける
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto gameObjVec = stage->GetGameObjectVec();

		for (auto obj : gameObjVec)
		{
			// スコアと飛行機の距離を確認して今追いかけている物よりもスコアが高い又は近ければ追いかける
			auto fighter = dynamic_pointer_cast<FighterAircraftBase>(obj);
			auto scoreObj = dynamic_pointer_cast<ScoreObject>(obj);

			if (scoreObj)
			{
				
			}
		}

	}

	// 自分と追尾対象の座標の差を計算する
	Vec3 StateTrackingEnemy::CheckDifferencePos(Vec3 goalPos)
	{
		// 親オブジェクトの位置
		auto parentPos = m_enemyLock->GetComponent<Transform>()->GetPosition();
		Vec3 posPlayerDifference = goalPos - parentPos; // ゴールと敵の位置の差を求める

		return posPlayerDifference;
	}
	//


	// 障害物を回避するためのステート
	StateObstaclesDodgeEnemy::StateObstaclesDodgeEnemy(const shared_ptr<MyGameObject>& parentObj):
		StateTrackingEnemy(parentObj)
	{

	}

	StateObstaclesDodgeEnemy::~StateObstaclesDodgeEnemy()
	{
	}

	void StateObstaclesDodgeEnemy::OnEnter()
	{
		// 親クラスEnter処理
		StateEnemy::OnEnter();

		// (元々)追いかける対象を取得
		m_trackingObj = m_enemyLock->GetTrackingObj();
	}

	void StateObstaclesDodgeEnemy::OnUpdate()
	{
		// 親クラスUpdate処理
		StateEnemy::OnUpdate();

		// 追いかける対象の更新
		//m_trackingObj = m_enemyLock->GetTrackingObj();
		m_obstaclesDodgeLock = m_obstaclesDodge.lock();


		if (!m_obstaclesDodgeLock)
		{
			throw BaseException
			{
				L"追いかける対象が存在しません",
				L"if (m_trackingObjLock)",
				L"void StateTrackingEnemy::OnUpdate()"
			};
		}

		// 目的地と現在地点の方向ベクトルを求める
		auto goalPos = m_obstaclesDodgeLock->GetComponent<Transform>()->GetPosition();
		Vec3 posPlayerDifference = CheckDifferencePos(goalPos);
		float distanceVec = posPlayerDifference.length();
		Vec3 playerDirectionVec = posPlayerDifference;
		playerDirectionVec.normalize(); // 正規化

		// 追いかけるときのロール回転処理(デバック用処理しか書いていない)
		m_enemyLock->TrackingRollQt();
		// 追いかける対象にX軸に向く処理
		m_enemyLock->TrackingPitchQt(playerDirectionVec);
		// 追いかける対象に向かってヨーを回転させる処理
		m_enemyLock->TrackingYawQt(playerDirectionVec);

		// 対象に向かって追いかける処理
		m_enemyLock->TrackingMove(playerDirectionVec);

		// ここを作業する
		// ある程度目的地に着いたとみなせたら元の追跡対象を追いかける
		if (distanceVec < 1.5f)
		{
			m_enemyLock->ChangeState(L"Tracking");
		}
	}

	// 障害物を回避するためにここを通過点にしろと伝えるセッタ
	void StateObstaclesDodgeEnemy::SetObstaclesDodge(weak_ptr<ObstaclesDodge> obstaclesDodge)
	{
		m_obstaclesDodge = obstaclesDodge;
	}
	///////////////////////////////////////////////////////////



	// リスポーン待機ステート
	StateRespawnEnemy::StateRespawnEnemy(const shared_ptr<MyGameObject>& parentObj):
		StateEnemy(parentObj)
	{

	}

	StateRespawnEnemy::~StateRespawnEnemy()
	{

	}

	void StateRespawnEnemy::OnEnter()
	{
		// 親クラスEnter処理
		StateEnemy::OnEnter();

		// 初期化
		m_countTimeOfReturn = 0.0f;
		// 復活までの時間取得
		m_timeOfReturn = m_enemyLock->GetTimeOfReturn();


		// 自分が見えない状態に変更する
		m_enemyLock->GetComponent<PNTStaticDraw>()->SetDrawActive(false);
		// 無敵状態をオンにする
		m_enemyLock->OnInvincibleFlag();
	}

	void StateRespawnEnemy::OnUpdate()
	{
		// 親クラスUpdate処理
		StateEnemy::OnUpdate();

		m_countTimeOfReturn += m_deltaTime;

		// リスポーン可能時間まで待機する
		if (m_timeOfReturn <= m_countTimeOfReturn)
		{
			// 復活処理
			m_enemyLock->GetComponent<Transform>()->SetPosition(Vec3(0.0f, -10.0f, 0.0f));

			// 自分が見える状態に変更する
			m_enemyLock->GetComponent<PNTStaticDraw>()->SetDrawActive(true);

			m_enemyLock->GetHpMax();

			int maxHP = m_enemyLock->GetHpMax();
			m_enemyLock->SetHPCurrent(maxHP);

			// 通常ステートに戻る
			m_enemyLock->ChangeState(L"Tracking");
		}
	}
	//


}
//end basecross