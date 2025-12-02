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

		// 自分が見えない状態に変更する
	}

	void StateRespawnEnemy::OnUpdate()
	{
		// 親クラスUpdate処理
		StateEnemy::OnUpdate();

		// リスポーン可能時間まで待機する
	}
	//

}
//end basecross