/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class RankingUI : public MyGameObject
	{
	private:
		weak_ptr<FighterAircraftBase> m_fightBase;
		shared_ptr<Transform> m_trans;
		Vec3 m_pos;
		int m_rankingNumber;   // 自分は何位か
		bool m_isPlayerOnly;
		wstring m_textureName;
		shared_ptr<NumberSprite> m_rankUI;
		shared_ptr<NumberSprite> m_scoreUI;
		shared_ptr<Sprite> m_nameSprite;
		int m_layer;

	public:
		RankingUI::RankingUI(
		const shared_ptr<Stage>& stage,	
		const Vec3& pos,
		const int& m_rankingNumber,
		const bool& isPlayerOnly,
		const int& layer = 1,
		const Vec3& rot = Vec3(0.0f,0.0f,0.0f),
		const Col4& color = Col4(1.0f, 1.0f, 1.0f, 1.0f)
		);

		virtual RankingUI::~RankingUI();

		// 初期化
		void OnCreate() override;

		// 更新
		void OnUpdate() override;

		void SetLayer(int layer);

		void SetFightBase(const shared_ptr<FighterAircraftBase>& fightBase)
		{
			m_fightBase = fightBase;
		}

		// 全体のランキング
		void AllRanking();
		// Playerだけのランキング
		void PlayerOnlyRanking();
	};

}
//end basecross
