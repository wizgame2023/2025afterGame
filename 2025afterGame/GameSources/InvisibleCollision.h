/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class InvisibleCollision : public Actor {
		//Vec3 m_pos;
		//Vec3 m_rot;
		//Vec3 m_siz;

	public:
		InvisibleCollision(const shared_ptr<Stage>& stagePtr,
			const Vec3& pos,
			const Vec3& rot,
			const Vec3& scale,
			const Col4& color = Col4(0.0f)
		);
		virtual ~InvisibleCollision();

		virtual void OnCreate() override;
	};
}
//end basecross
