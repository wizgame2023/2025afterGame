/*!
@file CheckPoint.h
@brief �`�F�b�N�|�C���g
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

		void GetRaceTime(float racetime);
		void GetEnemyCheckPointTime(float enemycheckpointtime);
		void GetNextCheckPoint(Vec3 nextcheckpoint);

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
