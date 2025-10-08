/*!
@file BaseState.h
@brief ステートの基盤クラス
担当：三瓶裕太
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross {
	class BaseState :public MyGameObject
	{
	private:

	public:
		BaseState(const shared_ptr<Stage>& ptrStage);
		~BaseState();

		void OnCreate()override;
	};

}

//end basecross

