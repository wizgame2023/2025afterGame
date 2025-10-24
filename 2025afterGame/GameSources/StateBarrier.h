/*!
@file StateBarrier.h
@brief バリアのステート
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class StateBarrier:public BaseState
	{
	public:
		StateBarrier();
		~StateBarrier();

		void OnEnter()override;
		void OnUpdate()override;
		void OnExit()override;

	};
}
//end basecross
