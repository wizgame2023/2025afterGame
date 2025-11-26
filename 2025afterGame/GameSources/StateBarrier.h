/*!
@file StateBarrier.h
@brief バリアのステート
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	// バリアステートのベースとなるクラス兼デフォルトステート
	class StateBarrierBase :public StateBase
	{
	protected:
		float m_sizePercent; // サイズの倍率
		weak_ptr<Barrier> m_barrier; // 親オブジェクトとなるバリア
		shared_ptr<Barrier> m_barrierLock; // 親オブジェクトをロック(存在していることを確認できた変数)

	public:
		StateBarrierBase(const shared_ptr<MyGameObject>& parentObj);
		~StateBarrierBase();

		void OnEnter() override;
		void OnUpdate() override;
		//void OnExit() override;
	};



	// バリア展開開始時の拡大ステート
	class StateBarrierStart:public StateBarrierBase
	{
	private:
		float m_sizePercent; // サイズの倍率
		weak_ptr<Barrier> m_barrier; // 親オブジェクトとなるバリア

	public:
		StateBarrierStart(const shared_ptr<MyGameObject>& parentObj);
		~StateBarrierStart();

		void OnEnter() override;
		void OnUpdate() override;
		void OnExit() override;

	};


	// バリア終了時の縮小処理ステート
	class StateBarrierEnd :public StateBarrierBase
	{
	public:
		StateBarrierEnd(const shared_ptr<MyGameObject>& parentObj);
		~StateBarrierEnd();

		void OnEnter() override;
		void OnUpdate() override;
		void OnExit() override;
	};


	// バリアのステートマシンの作成
	class StateBarrierMachine :public StateMachineBase
	{
	public:
		StateBarrierMachine(const shared_ptr<MyGameObject>& parentObj):
			StateMachineBase(parentObj)
		{
			AddState(L"Start", shared_ptr<StateBarrierStart>(new StateBarrierStart(parentObj))); // 開始時の拡大処理
			AddState(L"End", shared_ptr<StateBarrierEnd>(new StateBarrierEnd(parentObj))); // 終了時の縮小処理
			AddState(L"Default", shared_ptr<StateBarrierBase>(new StateBarrierBase(parentObj))); // デフォルトステート
		}

	};
}
//end basecross
