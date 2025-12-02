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

	class ScoreManager
	{
		// 削除処理
		struct ScoreManagerDeleter
		{
			void operator()(ScoreManager* p) { delete p; };
		};

		static unique_ptr<ScoreManager, ScoreManagerDeleter> m_scoreManager;

		// スコア
		int m_score = 0;

		// スプライト
		vector<shared_ptr<Sprite>> m_numberSprite;

		// ステージ
		shared_ptr<Stage> m_stage;

		// 数値のUV座標変更
		//void UpdateNumSprite(const shared_ptr<Sprite>& numSp);

	public:
		// コンストラクタ
		ScoreManager();
		// デストラクタ
		~ScoreManager() {};

		// スコアマネージャーを作成
		static unique_ptr<ScoreManager, ScoreManager::ScoreManagerDeleter>& CreateScoreManager();

		// 自分を渡す
		static unique_ptr<ScoreManager, ScoreManager::ScoreManagerDeleter>& GetScoreManager();

		// スコア更新
		// 引数 : 変動するスコア
		void UpdateScore(int score);

		// 現在のスコア取得
		int GetScore() const;

		// スコア初期化
		void ResetScore();

	};

}
//end basecross

