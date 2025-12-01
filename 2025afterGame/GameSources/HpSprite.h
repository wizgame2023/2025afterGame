/*!
@file HpSprite.h
@brief HP‚ÌéŒ¾
’S“–F‹g“c ’q‹M
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class HpSprite : public Sprite
	{
	private:
		float m_Rate;
		Vec3 m_size;
		Vec3 m_pos;

	public:
		HpSprite(const shared_ptr<Stage>& stagePtr,
			const wstring& textureName,
			const Vec2& size,
			const Vec3& pos,
			const Vec3& rot = Vec3(0.0f, 0.0f, 0.0f),
			const Col4& color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
			int layer = 1);
		~HpSprite();

		void OnCreate()override;
		void OnUpdate()override;
	};

}
//end basecross
