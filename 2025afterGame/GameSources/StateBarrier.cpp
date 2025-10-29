/*!
@file StateBarrier.cpp
@brief バリアの実体ステート
*/

#include "stdafx.h"
#include "Project.h"
#include "StateBarrier.h"

namespace basecross {
	// バリアステートの元となるクラス----------------------------------------------------------------------------->
	StateBarrierBase::StateBarrierBase(const shared_ptr<MyGameObject>& parentObj) :
		StateBase(parentObj),
		m_barrier(dynamic_pointer_cast<Barrier>(parentObj))
	{

	}
	StateBarrierBase::~StateBarrierBase()
	{

	}
	// 作成時処理
	void StateBarrierBase::OnEnter()
	{
		StateBase::OnEnter();
		m_barrierLock = m_barrier.lock();
	}
	// 更新処理
	void StateBarrierBase::OnUpdate()
	{
		m_barrierLock = m_barrier.lock();
	}
	//----------------------------------------------------------------------------------------------------------->

	// バリア展開時の拡大処理--------------------------------------------------------------------------------------->
	StateBarrierStart::StateBarrierStart(const shared_ptr<MyGameObject>& parentObj):
		StateBarrierBase(parentObj),
		m_barrier(dynamic_pointer_cast<Barrier>(parentObj))
	{

	}
	StateBarrierStart::~StateBarrierStart()
	{

	}
	// 作成時処理
	void StateBarrierStart::OnEnter()
	{
		// 親クラスEnter処理
		StateBarrierBase::OnEnter();

		// この時点での親オブジェクトのサイズ倍率を取得する
		m_sizePercent = m_barrierLock->GetSizePercent();

		// 親オブジェクトを見えるようにする
		m_barrierLock->SetDrawActive(true);
	}
	// 更新
	void StateBarrierStart::OnUpdate()
	{// ここのm_sizePercentの数値が大きくなるタイミングがおかしいので注視すべし！
		// 親クラスUpdate処理
		StateBarrierBase::OnUpdate();

		float delta = GameManager::GetGameManager()->GetDeltaTime();
		m_sizePercent += delta;

		// サイズが規定以上になったら拡大処理をやめる
		if (m_sizePercent >= 1.0f)
		{
			m_sizePercent = 1.0f;

			// デフォルトステートへ変更する
			m_barrierLock->ChangeState(L"Default");
		}

		m_barrierLock->SetSizePercent(m_sizePercent);
	}
	// 終了時処理
	void StateBarrierStart::OnExit()
	{
		auto test = 0;
	}
	//------------------------------------------------------------------------------------------------------------------------->

	// バリア終了時の縮小処理ステート------------------------------------------------------------------------------------------>
	StateBarrierEnd::StateBarrierEnd(const shared_ptr<MyGameObject>& parentObj):
		StateBarrierBase(parentObj)
	{

	}

	StateBarrierEnd::~StateBarrierEnd()
	{

	}

	// 開始時処理
	void StateBarrierEnd::OnEnter()
	{
		// 親クラスEnter処理
		StateBarrierBase::OnEnter();

		// この時点での親オブジェクトのサイズ倍率を取得する
		m_sizePercent = m_barrierLock->GetSizePercent();
	}

	// 更新処理
	void StateBarrierEnd::OnUpdate()
	{
		// 親クラスUpdate処理
		StateBarrierBase::OnUpdate();

		m_sizePercent -= m_deltaTime;

		// サイズが規定以下になったら縮小処理をやめる
		if (m_sizePercent <= 0.0f)
		{
			m_sizePercent = 0.0f;

			// 親オブジェクトを見えないようにする
			m_barrierLock->SetDrawActive(false);

			// デフォルトステートへ変更する
			m_barrierLock->ChangeState(L"Default");
		}

		m_barrierLock->SetSizePercent(m_sizePercent);
	}

	// 終了時処理
	void StateBarrierEnd::OnExit()
	{
		auto test = 0;
	}
	//------------------------------------------------------------------------------------------------------------------------>
}
//end basecross
