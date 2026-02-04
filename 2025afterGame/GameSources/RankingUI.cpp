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
		const bool& isPlayerOnly,
		const int& layer,
		const Vec3& rot,
		const Col4& color
	) :
		MyGameObject(stage),
		m_pos(pos),
		m_rankingNumber(rankingNumber),
		m_isPlayerOnly(isPlayerOnly),
		m_layer(layer)
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

		Vec3 rankPos =  m_pos + Vec3(-80,  0, 0);
		Vec3 namePos =  m_pos + Vec3( 30,-10, 0);
		Vec3 scorePos = m_pos + Vec3(230,  0, 0);

		// 左：順位
		// 1:2
		if (!m_isPlayerOnly)
		{
			m_rankUI = stage->AddGameObject<NumberSprite>(Vec2(35, 70), rankPos);
			m_rankUI->SetRankingNumberCount(m_rankingNumber);
			m_rankUI->SetMyType(NumberType::RankingNumber);
		}

		// 中：名前
		// 3 : 1 
		m_nameSprite = stage->AddGameObject<Sprite>(
			L"ResultPlayer",
			Vec2(180,60),
			namePos,
			1
		);

		// 右：スコア
		m_scoreUI = stage->AddGameObject<NumberSprite>(Vec2(30,60), scorePos);
	}

	void RankingUI::OnUpdate()
	{
		auto& scoreManager = ScoreManager::GetScoreManager();
		// ソート化されたスコアの情報
		auto scores = scoreManager->GetSortedScores();
		// Playerのスコア
		auto plScore = scoreManager->GetPlScore();
		// ランキング順位
		int index = m_rankingNumber - 1;
		// 同じ要素から名前とスコアを取る
		const auto& info = scores[index];

		// 名前
		// 0番目は必ずPlayer
		if (info.id == 0)
		{
			m_nameSprite->SetTexture(L"ResultPlayer");
		}
		else
		{
			m_nameSprite->SetTexture(L"ResultEnemy");
		}

		// スコア
		m_scoreUI->SetNumber(info.crntScore);
		
		// リザルト生成
		if (m_isPlayerOnly)
		{
			m_scoreUI->SetDrawLayer(m_layer + 1);
			m_nameSprite->SetDrawLayer(m_layer);

			m_nameSprite->SetTexture(L"ResultPlayer");
			m_scoreUI->SetNumber(plScore);
			return;
		}
	}

	void RankingUI::SetLayer(int layer)
	{
		m_layer = layer;
	}
}
//end basecross
