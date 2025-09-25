/*!
@file MyGameObject.h
@brief 全てのゲームのオブジェクトの元となるクラス
*/


#pragma once
#include "stdafx.h"

namespace basecross {
	class MyGameObject :public GameObject
	{
	public:
		MyGameObject(const shared_ptr<Stage>& stagePtr);
		~MyGameObject();

	private:

	};
}
//end basecross
