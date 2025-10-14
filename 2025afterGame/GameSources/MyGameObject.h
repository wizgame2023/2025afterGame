/*!
@file MyGameObject.h
@brief 全てのゲームのオブジェクトの元となるクラス
*/


#pragma once
#include "stdafx.h"

namespace basecross {
	class MyGameObject :public GameObject
	{	
	private:

	protected:
		float m_delta = 0.0f;

	public:
		MyGameObject(const shared_ptr<Stage>& stagePtr);
		~MyGameObject();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}
//end basecross
