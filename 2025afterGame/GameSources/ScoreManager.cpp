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

	void ScoreManager::SetScore(int score)
	{
		m_score = score;
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

	wstring ScoreManager::GetBinaryPath() const
	{
		auto& app = App::GetApp();
		wstring path = app->GetDataDirWString();
		return path + L"Bin/";
	}

	void ScoreManager::CreateHighScoreBinary()
	{
		// スコアデータのパスを取得
		wstring scorePath = GetBinaryPath() + L"HighScore.bin";

		// 初期ハイスコア
		int initFirstHighScore = 10000;

		// バイナリがあるかを確認
		ifstream ifs(scorePath, ios::binary);

		// ないなら生成
		if (!ifs)
		{
			// ofstreamでファイルを生成
			ofstream ofs(scorePath, ios::binary);
			ofs.write(reinterpret_cast<const char*>(&initFirstHighScore), sizeof(initFirstHighScore));
		}
		// あるならスコアを確認して初期スコアより低ければ上書き
		else if (LoadHighScoreBinary() < initFirstHighScore)
		{
			// ofstreamでファイルを開く
			ofstream ofs(scorePath, ios::binary);
			ofs.write(reinterpret_cast<const char*>(&initFirstHighScore), sizeof(initFirstHighScore));
		}
	}

	void ScoreManager::SaveHighScoreBinary()
	{
		// スコアデータのパスを取得
		wstring scorePath = GetBinaryPath() + L"HighScore.bin";

		ifstream ifs(scorePath, ios::binary);

		// ファイルが存在し、スコアがハイスコアより高ければ上書き
		if (ifs && LoadHighScoreBinary() < m_score)
		{
			// ofstreamでファイルを開く or 生成
			ofstream ofs(scorePath, ios::binary);
			assert(ofs);

			// スコアを書き込み
			ofs.write(reinterpret_cast<const char*>(&m_score), sizeof(m_score));
		}
	}

	int ScoreManager::LoadHighScoreBinary()
	{
		// スコアデータのパスを取得
		wstring scorePath = GetBinaryPath() + L"HighScore.bin";

		// [注意]ofstreamではなくifstream
		// ifstreamでファイルを開く
		ifstream ifs(scorePath, ios::binary);

		if (!ifs)
		{
			// ファイルが存在しない場合は0を返す
			return 0;
		}

		int loadedScore = 0;

		// スコアを読み込み
		ifs.read(reinterpret_cast<char*>(&loadedScore), sizeof(loadedScore));

		return loadedScore; 
	}
}
//end basecross
