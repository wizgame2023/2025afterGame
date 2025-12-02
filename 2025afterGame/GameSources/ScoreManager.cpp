/*!
@file ScoreManager.cpp
@brief スコアマネージャー実体
担当:佐藤 海斗
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	unique_ptr<ScoreManager, ScoreManager::ScoreManagerDeleter> ScoreManager::m_scoreManager;

	// コンストラクタ
	ScoreManager::ScoreManager() :
		m_score(0)
	{}

	// ==============================================================================
	// 関数
	// ==============================================================================

	void ScoreManager::UpdateScore(int score)
	{
		m_score += score;
		// スコアがマイナスにならないようにする
		max(m_score, 0);
	}

	void ScoreManager::ResetScore()
	{
		m_score = 0;
	}

	int ScoreManager::GetScore() const
	{
		return m_score;
	}

	// シングルトンによる生成
	unique_ptr<ScoreManager, ScoreManager::ScoreManagerDeleter>& ScoreManager::CreateScoreManager()
	{
		try
		{
			if (m_scoreManager.get() == 0)
			{
				// 自分を作成
				m_scoreManager.reset(new ScoreManager());
			}
			return m_scoreManager;
		}
		catch (...)
		{
			throw;
		}

		return m_scoreManager;
	}

	unique_ptr<ScoreManager, ScoreManager::ScoreManagerDeleter>& ScoreManager::GetScoreManager()
	{
		return m_scoreManager;
	}


	//void ScoreManager::UpdateNumSprite(const shared_ptr<Sprite>& numSp)
	//{
	//	int displayScore = m_score;
	//}

}
//end basecross
