/*!
@file Actor.cpp
@brief ゲーム中の動く3Dオブジェクトの親クラス
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Actor::Actor(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
	{

	}

	Actor::~Actor()
	{
	}

	void Actor::OnCreate()
	{
		CreateChildObjects();
	}
}
//end basecross