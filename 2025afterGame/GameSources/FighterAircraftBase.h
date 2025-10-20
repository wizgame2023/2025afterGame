/*!
@file FighterAircraftBaseBase.h
@brief プレイヤーなど
担当：吉田 智貴
*/

#pragma once
#include "stdafx.h"
#include "Actor.h"

namespace basecross{
	class FighterAircraftBase : public Actor
	{
		// パラメーター
		float m_hp;
		float m_maxHP;

		//エネルギー
		bool m_availableWeapons;
		float m_coolTime;

		// スリップストリーム

		//DRS

		// スピード関連
		float m_speed;

	
	public:
		FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& ptrStage);
		FighterAircraftBase::~FighterAircraftBase();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}
//end basecross

