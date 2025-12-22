/*!
@file ObstaclesDodge.cpp
@brief 障害物を回避するための目印となるオブジェクト
担当：三瓶裕太
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	ObstaclesDodge::ObstaclesDodge(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color):
		Actor(stagePtr,pos,rot,scale,color)
	{

	}

	ObstaclesDodge::~ObstaclesDodge()
	{

	}

	void ObstaclesDodge::OnCreate()
	{
		Actor::OnCreate();

		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
	}
}
