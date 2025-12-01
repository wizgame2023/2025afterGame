/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class StageWall : public GameObject {
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_siz;

	public:
		StageWall(const shared_ptr<Stage>& StagePtr,
			const Vec3& Pos,
			const Vec3& Rot,
			const Vec3& Siz
		);
		virtual ~StageWall();

		virtual void OnCreate() override;
	};
}
//end basecross
