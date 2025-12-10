/*!
@file ScoreManager.h
@brief スコアマネージャー
担当:佐藤 海斗
*/

#pragma once
#include "stdafx.h"
#include "fstream"

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

		// スコアバイナリファイルパス取得
		wstring GetBinaryPath() const;

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
		void SetScore(int score);

		// 現在のスコア取得
		int GetScore() const;

		// スコア初期化
		void ResetScore();

		// スコアバイナリ生成
		void CreateHighScoreBinary();

		// スコアバイナリ書き込み
		void SaveHighScoreBinary();

		// スコアバイナリ読み込み
		int LoadHighScoreBinary();
	};

}
//end basecross

