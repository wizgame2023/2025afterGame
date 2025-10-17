/*!
@file MyGameObject.cpp
@brief 全てのゲームのオブジェクトの元となるクラス
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	MyGameObject::MyGameObject(const shared_ptr<Stage>& stagePtr) :
		GameObject(stagePtr)
	{

	}

	MyGameObject::~MyGameObject()
	{

	}

	void MyGameObject::OnCreate()
	{

	}

	void MyGameObject::OnUpdate()
	{
		//デルタタイム取得
		m_delta = App::GetApp()->GetElapsedTime();
	}
}
//end basecross