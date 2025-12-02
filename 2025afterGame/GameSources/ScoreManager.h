/*!
@file ScoreManager.h
@brief スコアマネージャー
担当:佐藤 海斗
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	// ==============================================================================
	// ScoreManagerクラス
	// ==============================================================================

	class ScoreManager : public MyGameObject
	{
		// スコア
		int m_score;

		// スプライト
		vector<Sprite> m_numberSprite;

		// ステージ
		shared_ptr<Stage> m_stage;

	public:
		// コンストラクタ
		ScoreManager(const shared_ptr<Stage>& stagePtr);
		// デストラクタ
		~ScoreManager() {};

		// スコア更新
		// 引数 : 変動するスコア
		void UpdateScore(int score);

		// 現在のスコア取得
		int GetScore() const;

		// スコア初期化
		void ResetScore();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

	};

}
//end basecross

