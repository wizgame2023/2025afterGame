/*!
@file Actor.h
@brief ゲーム中の動く3Dオブジェクトの親クラス
*/


#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross {
	class Actor :public MyGameObject
	{
	public:
		Actor(const shared_ptr<Stage>& stagePtr);
		~Actor();

	private:
		virtual void OnCreate()override;

	};
}
//end basecross
