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
}
//end basecross