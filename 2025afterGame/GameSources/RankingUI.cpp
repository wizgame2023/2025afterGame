/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	RankingUI::RankingUI(const shared_ptr<Stage>& stage,
		const Vec2& size, 
		const Vec3& pos, 
		const int& rankingNumber,
		const Vec3& rot,
		const Col4& color,
		const int& layer
	) :
		MyGameObject(stage),
		m_nowIndex(0),
		m_layer(layer),
		m_pos(pos),
		m_rankingNumber(rankingNumber)
	{

	}
	
	RankingUI::~RankingUI()
	{

	}

	void RankingUI::OnCreate()
	{
		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos.x, m_pos.y, m_pos.z);
		SetDrawActive(true);
	}

	void RankingUI::OnUpdate()
	{
		RankingNumber();
	}

	void RankingUI::RankingNumber()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		for (int i = 0; i < m_rankingNumber; i++)
		{
			auto number = stage->AddGameObject<NumberSprite>(Vec2(25.0f, 25.0f), Vec3(m_pos.x, m_pos.y, m_pos.z));
			number->SetMyType(NumberType::Ranking);
		}
	}

}
//end basecross
