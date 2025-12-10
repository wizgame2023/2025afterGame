/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class ScoreObject : public GameObject {
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_siz;
		int m_score;

	public:
		ScoreObject(const shared_ptr<Stage>& StagePtr,
			const Vec3& Pos,
			const Vec3& Rot,
			const Vec3& Siz
		);
		virtual ~ScoreObject();

		virtual void OnCreate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
