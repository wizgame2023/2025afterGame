/*!
@file HpSprite.h
@brief HPÇÃêÈåæ
íSìñÅFãgìc íqãM
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class HpSprite : public Sprite
	{
	private:
		weak_ptr<FighterAircraftBase> m_fightBase;
		float m_Rate;
		Vec3 m_size;
		Vec3 m_pos;
		float m_hpWidth;
		bool m_spriteMoveFlag;

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

		void UpdateHpSprite();

		void SetSpriteMove(bool flag);
	};

}
//end basecross
