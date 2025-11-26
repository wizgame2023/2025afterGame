/*!
@file HpSprite.h
@brief HP�̐錾
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"

namespace basecross{
	class HpSprite : public Sprite
	{
	private:
		float m_nowHP;
		float m_maxHP;

	public:
		HpSprite(const shared_ptr<Stage>& stagePtr,
			const wstring& textureName,
			const Vec2& size,
			const Vec3& pos = Vec3(0.0f, 0.0f, 0.0f),
			const Vec3& rot = Vec3(0.0f, 0.0f, 0.0f),
			const Col4& color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
			int layer = 1);
		~HpSprite();

		void OnCreate()override;
		//void OnUpdate()override;
	};

}
//end basecross
