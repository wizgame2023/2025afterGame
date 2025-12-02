/*!
@file ScoreManager.cpp
@brief スコアマネージャー実体
担当:佐藤 海斗
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	ScoreManager::ScoreManager(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
	{
		ResetScore();
	}

	// ==============================================================================
	// 生成
	// ==============================================================================
	void ScoreManager::OnCreate()
	{
		m_stage = GetStage();

		//m_numberSprite.push_back()
	}

	// ==============================================================================
	// 更新
	// ==============================================================================
	void ScoreManager::OnUpdate()
	{

	}

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

}
//end basecross
