/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	RankingUI::RankingUI(const shared_ptr<Stage>& stage,
		const Vec3& pos, 
		const int& rankingNumber,
		const Vec3& rot,
		const Col4& color,
		const int& layer
	) :
		MyGameObject(stage),
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
		m_trans->SetPosition(m_pos);

		auto& app = App::GetApp();
		auto scene = App::GetApp()->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		Vec3 rankPos =  m_pos + Vec3(-80, 0, 0);
		Vec3 namePos =  m_pos + Vec3( 30, -10, 0);
		Vec3 scorePos = m_pos + Vec3(230, 0, 0);
		
		if (m_rankingNumber == 1)
		{
			namePos.y += 10;
		}

		// 左：順位
		// 5:6
		m_rankUI = stage->AddGameObject<NumberSprite>(Vec2(35 ,42), rankPos);
		m_rankUI->SetRankingNumberCount(m_rankingNumber);
		m_rankUI->SetMyType(NumberType::RankingNumber);

		// 中：名前
		// 3 : 1 
		// 103 : 32
		m_nameSprite = stage->AddGameObject<Sprite>(
			L"ResultPlayer",
			Vec2(180,60),
			namePos,
			1
		);

		// 右：スコア
		m_scoreUI = stage->AddGameObject<NumberSprite>(Vec2(30, 30), scorePos);
	}

	void RankingUI::OnUpdate()
	{
		auto& scoreManager = ScoreManager::GetScoreManager();
		auto scores = scoreManager->GetSortedScores();
		auto plScore = scoreManager->GetPlScore();

		int index = m_rankingNumber - 1;

		if (index < scores.size())
		{
			const auto& id = scores[index].id;

			if (id == 0)
			{
				m_nameSprite->SetTexture(L"ResultPlayer");
			}
			else if (id == 1)
			{
				m_nameSprite->SetTexture(L"ResultEnemy");
			}
			//else if (id == L"Enemy2")
			//{
			//	m_nameSprite->SetTexture(L"ResultEnemy");
			//}
			//else if (id == L"Enemy3")
			//{
			//	m_nameSprite->SetTexture(L"ResultEnemy");
			//}

			m_scoreUI->SetNumber(scores[index].crntScore);
		}
	/*	else
		{
			m_nameSprite->SetTexture(L"Enemy");
			m_scoreUI->SetNumber(0);
		}*/
	}
}
//end basecross
