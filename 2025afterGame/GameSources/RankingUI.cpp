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
		Vec3 namePos =  m_pos + Vec3( 30,  0, 0);
		Vec3 scorePos = m_pos + Vec3(180,  0, 0);

		// 左：順位
		// 1:2
		m_rankUI = stage->AddGameObject<NumberSprite>(Vec2(35, 70), rankPos);
		m_rankUI->SetNumber(m_rankingNumber);
		m_rankUI->SetNumberLayer(m_layer);

		// 中：名前
		// 3 : 1 
		m_nameSprite = stage->AddGameObject<Sprite>(
			L"ResultPlayer",
			Vec2(180,60),
			namePos,
			1
		);
		m_nameSprite->SetDrawLayer(m_layer);


		// 右：スコア
		m_scoreUI = stage->AddGameObject<NumberSprite>(Vec2(30,60), scorePos);
		m_scoreUI->SetNumberLayer(m_layer);
	}

	void RankingUI::OnUpdate()
	{
		// リザルト生成
		if (m_isPlayerOnly)
		{
			PlayerOnlyRanking();
		}
		else
		{
			AllRanking();
		}
	}

	void RankingUI::SetLayer(int layer)
	{
		m_layer = layer;
	}

	void RankingUI::AllRanking()
	{	
		auto& scoreManager = ScoreManager::GetScoreManager();
		// ソート化されたスコアの情報
		auto scores = scoreManager->GetSortedScores();
		// Playerのスコア
		auto plScore = scoreManager->GetPlScore();
		// ランキング順位
		int index = m_rankingNumber - 1;

		// Player順位を計算
		int playerRank = 1;
		for (const auto& s : scores)
		{
			if (s.crntScore > plScore)
			{
				playerRank++;
			}
			else
			{
				break;
			}
		}

		// Player
		if (m_rankingNumber == playerRank)
		{
			m_nameSprite->SetTexture(L"ResultPlayer");
			m_scoreUI->SetNumber(plScore);
			return;
		}

		// Player分のズレを補正
		int enemyIndex = index;
		if (m_rankingNumber > playerRank)
		{
			enemyIndex--;
		}

		if (enemyIndex < 0 || enemyIndex >= (int)scores.size())
		{
			return;
		}

		const auto& info = scores[enemyIndex];
		m_nameSprite->SetTexture(L"ResultEnemy");
		m_scoreUI->SetNumber(info.crntScore);
	}

	void RankingUI::PlayerOnlyRanking()
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


		// Player順位を計算
		int playerRank = 1;
		for (const auto& s : scores)
		{
			if (s.crntScore > plScore)
			{
				playerRank++;
			}
			else
			{
				break;
			}
		}

		// ランキングを動かす
		if (m_rankUI)
		{
			m_rankUI->SetNumber(playerRank);
		}


		// 名前
		// 0番目は必ずPlayer
		if (info.id == 0)
		{
			m_rankUI->SetNumber(m_rankingNumber);
		}

		if (index == 0)
		{
			m_nameSprite->SetTexture(L"ResultPlayer");
		}
		else
		{
			m_nameSprite->SetTexture(L"ResultEnemy");
		}

		// スコアの表示
		m_scoreUI->SetNumber(plScore);
	}

}
//end basecross
