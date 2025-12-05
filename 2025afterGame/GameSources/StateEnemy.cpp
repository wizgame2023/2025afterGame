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
		// 追いかける対象を取得
		m_trackingObj = m_enemyLock->GetTrackingObj();
	}

	void StateTrackingEnemy::OnUpdate()
	{
		m_trackingObj = m_enemyLock->GetTrackingObj();
		auto trackingObjLock = m_trackingObj.lock();

		if (trackingObjLock)
		{
			throw BaseException
			{
				L"追いかける対象が存在しません",
				L"if (trackingObjLock)",
				L"void StateTrackingEnemy::OnUpdate()"
			};
		}

		auto parentPos = m_enemyLock->GetComponent<Transform>()->GetPosition();

		// xz方面の距離の差を求める 次はzy方面の距離の差を求める
		//auto goalObj = GetStage()->GetSharedGameObject<DebagPlayer>(L"Player"); // いったんゴールの位置をプレイヤーにする
		auto goalPos = trackingObjLock->GetComponent<Transform>()->GetPosition();
		Vec3 posPlayerDifference = goalPos - parentPos; // ゴールと敵の位置の差を求める
		Vec2 differenceYZ = Vec2(posPlayerDifference.y, abs(posPlayerDifference.z));
		differenceYZ.normalize();
		posPlayerDifference.normalize(); // 正規化

		// 目的地の角度を取得
		float goalAngle = atan2f(posPlayerDifference.x, posPlayerDifference.z);
		float goalAngleZY = atan2f(posPlayerDifference.y, abs(posPlayerDifference.z));
		float goalAngleYX = atan2f(posPlayerDifference.y, -posPlayerDifference.x);


		// 角度がマイナスつかないように変更
		goalAngle = m_enemyLock->AdjustmentAngle(goalAngle);
		goalAngleZY = m_enemyLock->AdjustmentAngle(-goalAngleZY);
		goalAngleYX = m_enemyLock->AdjustmentAngle(goalAngleYX);


		// ピッチの向きたい方向を求める処理
		// これで、向いている方向のY座標を0にしたものを求める
		auto posPlayerDifferenceZY = posPlayerDifference;
		posPlayerDifferenceZY.y = 0.0f;


		// 内積
		float dotf = posPlayerDifference.dot(posPlayerDifferenceZY);
		// なす角を求める
		auto pitchAngle = acosf(dotf);

		// 敵から見てプレイヤーが下にいたら角度をマイナスにする
		if (posPlayerDifference.y > 0)
		{
			pitchAngle = -pitchAngle;
		}

		// 敵から見てプレイヤーのいるZX平面の角度
		auto playerAngle = atan2f(posPlayerDifference.z, posPlayerDifference.x);
		playerAngle = m_enemyLock->AdjustmentAngle(playerAngle);

		// 進むスピード(仮)
		float speed = 1.0f;

		// Pos移動
		parentPos.x += cos(playerAngle) * speed * m_deltaTime;
		parentPos.z += sin(playerAngle) * speed * m_deltaTime;

		// y方向の差が＋かーか確認する
		int ysign = 0;
		if (posPlayerDifference.y > 0.05f)
		{
			ysign = 1;
		}
		else if (posPlayerDifference.y < -0.05f)
		{
			ysign = -1;
		}
		else
		{
			ysign = 0;
		}
		parentPos.y += ysign * speed * m_deltaTime; // 向いている角度によってスピード変えないと違和感が出るかも


	}
	//


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
			m_enemyLock->ChangeState(L"Base");

		}
	}
	//

}
//end basecross