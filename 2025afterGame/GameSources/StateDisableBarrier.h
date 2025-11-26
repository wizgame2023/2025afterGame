/*!
@file StateDisableBarrier.h
@brief バリア無効化処理
担当者：三瓶裕太
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class StateDisableBarrier :public StateBase
	{
	private:
		StateDisableBarrier(const shared_ptr<MyGameObject>& obj);
		~StateDisableBarrier();

		void OnEnter();
		void OnUpdate();
		void OnExit();
	};

	// ステートマシン
	class StateMachineDisableBarrier :StateMachineBase
	{
	public:
		StateMachineDisableBarrier(const shared_ptr<MyGameObject>& obj):
			StateMachineBase(obj)
		{
			//AddState(L"Start",shared_ptr<StateDisableBarrier>(new StateDisableBarrier(obj)))
		}
	};
}

//end basecross

