/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class TestCsv : public GameObject {
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_siz;

	public:
		TestCsv(const shared_ptr<Stage>& StagePtr,
			const Vec3& Pos,
			const Vec3& Rot,
			const Vec3& Siz
		);
		virtual ~TestCsv();

		virtual void OnCreate() override;
	};
}
//end basecross
