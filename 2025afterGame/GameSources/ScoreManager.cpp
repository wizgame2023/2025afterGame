/*!
@file ScoreManager.cpp
@brief スコアマネージャー実体
担当:佐藤 海斗
*/

#include "stdafx.h"
#include "Project.h"
#include<algorithm>

namespace basecross {
	unique_ptr<ScoreManager, ScoreManager::ScoreManagerDeleter> ScoreManager::m_scoreManager;

	// コンストラクタ
	ScoreManager::ScoreManager() :
		m_scores(0)
	{}

	// ==============================================================================
	// 関数
	// ==============================================================================

	void ScoreManager::SetID(const wstring& id)
	{
		if (FindScoreInfo(id) == nullptr)
		{
			ScoreInfo newScore;
			newScore.id = id;
			newScore.crntScore = 0;

			m_scores.push_back(newScore);
		}
	}

	ScoreManager::ScoreInfo* ScoreManager::FindScoreInfo(const wstring& id)
	{
		// IDからスコア情報を探す
		for (auto& entity : m_scores)
		{
			// IDが一致したらそのポインタを返す
			if (entity.id == id)
			{
				return &entity;
			}
		}
		// なかったらnullptrを返す
		return nullptr;
	}

	const ScoreManager::ScoreInfo* ScoreManager::FindScoreInfo(const wstring& id) const
	{
		// IDからスコア情報を探す
		for (const auto& entity : m_scores)
		{
			// IDが一致したらそのポインタを返す
			if (entity.id == id)
			{
				return &entity;
			}
		}
		// なかったらnullptrを返す
		return nullptr;
	}

	vector<ScoreManager::ScoreInfo> ScoreManager::GetSortedScores() const
	{
		// スコア情報のコピーを作成
		vector<ScoreInfo> sortedScores = m_scores;

		// スコアの高い順にソート
		sort(sortedScores.begin(), sortedScores.end(),
			// ラムダ式(aとbを比較して並び替え)
			[](const ScoreInfo& a, const ScoreInfo& b)
			{ return a.crntScore > b.crntScore; }
		);

		return sortedScores;
	}

	void ScoreManager::SetScore(const wstring& id, int score)
	{
		// IDからスコア情報を探す
		if (auto* entity = FindScoreInfo(id))
		{
			entity->crntScore = score;
		}
	}

	int ScoreManager::GetScore(const wstring& id) const
	{
		// IDからスコア情報を探す
		if (auto* entity = FindScoreInfo(id))
		{
			return entity->crntScore;
		}
		return 0;
	}

	void ScoreManager::ResetScore(const wstring& id)
	{
		// IDからスコア情報を探す
		if (auto* entity = FindScoreInfo(id))
		{
			entity->crntScore = 0;
		}
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

		int plScore = GetPlScore();

		// ファイルが存在し、スコアがハイスコアより高ければ上書き
		if (ifs && LoadHighScoreBinary() < plScore)
		{
			// ofstreamでファイルを開く or 生成
			ofstream ofs(scorePath, ios::binary);
			assert(ofs);

			// スコアを書き込み
			ofs.write(reinterpret_cast<const char*>(&plScore), sizeof(plScore));
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

	// 戦闘機配列に対し戦闘機の中身を追加する
	void ScoreManager::PushBackFighterBase(const shared_ptr<FighterAircraftBase>& fighterAircraftBase)
	{
		m_fighterBases.push_back(fighterAircraftBase);
		int countId = m_fighterBases.size() - 1;
		fighterAircraftBase->SetId(countId);
	}
}
//end basecross
