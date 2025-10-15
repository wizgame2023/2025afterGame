/*!
@file CheckPoint.h
@brief チェックポイント
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross{
	class CheckPoint: public Actor
	{
	private :

	public :
		float m_raceTime;
		float m_enemyCheckPointTime;
		Vec3 m_nextCheckPoint;

	public :
		CheckPoint(const shared_ptr<Stage>& stagePtr);
		~CheckPoint();

		void OnCreate()override;
		void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
