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

	}

	void StateTrackingEnemy::OnUpdate()
	{

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
		if (m_timeOfReturn >= m_countTimeOfReturn)
		{
			// 復活処理
			m_enemyLock->GetComponent<Transform>()->SetPosition(Vec3(0.0f, -10.0f, 0.0f));

			// 自分が見える状態に変更する
			m_enemyLock->GetComponent<PNTStaticDraw>()->SetDrawActive(true);

			// 通常ステートに戻る
			m_enemyLock->ChangeState(L"Base");

		}
	}
	//

}
//end basecross