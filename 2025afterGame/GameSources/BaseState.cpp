/*!
@file BaseState.cpp
@brief 基盤ステートのクラス
担当：三瓶裕太
*/

#include "stdafx.h"
#include "Project.h"
#include "BaseState.h"

namespace basecross {
	BaseState::BaseState(const shared_ptr<Stage>& ptrStage):
		MyGameObject(ptrStage)
	{

	}

	BaseState::~BaseState()
	{

	}


	void BaseState::OnCreate()
	{

	}

}
//end basecross


