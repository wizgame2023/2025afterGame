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
		float m_previewTime;
		int m_checkPointID;
		shared_ptr<Transform> m_trans;

	public :
		CheckPoint(const shared_ptr<Stage>& stagePtr);
		~CheckPoint();

		void OnCreate()override;
		void OnUpdate()override;

		float SetDifferenceTime();

		void SetCheckPointID(int id);

		Vec3 SetNextCheckPoint();

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
