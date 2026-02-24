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

	void ScoreManager::SetID(const int id)
	{
		if (FindScoreInfo(id) == nullptr)
		{
			ScoreInfo newScore;
			newScore.id = id;
			newScore.crntScore = 0;

			m_scores.push_back(newScore);
		}
	}

	ScoreManager::ScoreInfo* ScoreManager::FindScoreInfo(const int id)
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

	const ScoreManager::ScoreInfo* ScoreManager::FindScoreInfo(const int id) const
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

	void ScoreManager::ScoreManagerReset()
	{
		m_scores.clear();
		m_fighterBases.clear();
	}

	vector<ScoreManager::ScoreInfo> ScoreManager::GetSortedScores() const
	{
		// スコア情報のコピーを作成
		vector<ScoreInfo> sortedScores = m_scores;

		// スコアの高い順にソート
		// スコアの最初から最後まで見ていって大きい順に並び替え
		sort(sortedScores.begin(), sortedScores.end(),
			// ラムダ式(aとbを比較して並び替え)
			[](const ScoreInfo& a, const ScoreInfo& b)
			{ return a.crntScore > b.crntScore; }
		);

		return sortedScores;
	}

	void ScoreManager::SetScore(const int id, int score)
	{
		// IDからスコア情報を探す
		if (auto* entity = FindScoreInfo(id))
		{
			entity->crntScore = score;
		}
	}

	void ScoreManager::AddScore(const int id, int addScore)
	{
		// IDからスコア情報を探す
		if (auto* entity = FindScoreInfo(id))
		{
			entity->crntScore += addScore;
		}
	}

	void ScoreManager::SubScore(const int id, int subScore)
	{
		// IDからスコア情報を探す
		if (auto* entity = FindScoreInfo(id))
		{
			min(0, entity->crntScore - subScore);
			entity->crntScore -= subScore;
		}
	}

	int ScoreManager::GetScore(const int id) const
	{
		// IDからスコア情報を探す
		if (auto* entity = FindScoreInfo(id))
		{
			return entity->crntScore;
		}
		return 0;
	}

	void ScoreManager::ResetScore(const int id)
	{
		SetScore(id, 0);
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

		// バイナリがあるかを確認
		ifstream ifs(scorePath, ios::binary);

		// ないなら生成
		if (!ifs)
		{
			ResetHighScoreBinary();
			return;
		}

		// ファイルがあるならファイル内のデータをm_highScoresに入れる
		LoadHighScoreBinary();

	}

	void ScoreManager::SaveHighScoreBinary()
	{
		int plScore = GetPlScore();

		// 5番目より高ければ
		if (plScore > m_highScores[4])
		{
			// 仮置き
			m_highScores[4] = plScore;

			// 降順ソート
			sort(m_highScores.begin(), m_highScores.end(),
				[](const int a, const int b){ return a > b;	}
			);

			wstring scorePath = GetBinaryPath() + L"HighScore.bin";
			ofstream ofs(scorePath, ios::binary);
			
			// スコアを書き込み
			ofs.write(reinterpret_cast<const char*>(m_highScores.data()), sizeof(m_highScores));
		}
	}

	//int ScoreManager::LoadHighScoreBinary()
	//{
	//	// スコアデータのパスを取得
	//	wstring scorePath = GetBinaryPath() + L"HighScore.bin";

	//	// [注意]ofstreamではなくifstream
	//	// ifstreamでファイルを開く
	//	ifstream ifs(scorePath, ios::binary);

	//	if (!ifs)
	//	{
	//		// ファイルが存在しない場合は0を返す
	//		return 0;
	//	}

	//	int loadedScore = 0;

	//	// スコアを読み込み
	//	ifs.read(reinterpret_cast<char*>(&loadedScore), sizeof(loadedScore));

	//	return loadedScore;
	//}

	void ScoreManager::LoadHighScoreBinary()
	{
		// スコアデータのパスを取得
		wstring scorePath = GetBinaryPath() + L"HighScore.bin";

		// [注意]ofstreamではなくifstream
		// ifstreamでファイルを開く
		ifstream ifs(scorePath, ios::binary);

		if (!ifs)
		{
			return;
		}

		ifs.read(reinterpret_cast<char*>(m_highScores.data()), sizeof(m_highScores));
	}

	void ScoreManager::ResetHighScoreBinary()
	{
		// スコアデータのパスを取得
		wstring scorePath = GetBinaryPath() + L"HighScore.bin";
		
		// 初期ハイスコア
		int initFirstHighScore = 10000;

		// ofstreamでファイルを生成
		ofstream ofs(scorePath, ios::binary);
		m_highScores.fill(0);
		m_highScores[0] = initFirstHighScore;
		ofs.write(reinterpret_cast<const char*>(m_highScores.data()), sizeof(m_highScores));
		return;

	}

	void ScoreManager::WriteHighScoreBinary()
	{
		const wstring& scorePath = GetBinaryPath() + L"HighScore.bin";
		ofstream ofs(scorePath, ios::binary);
		if (ofs)
		{
			ofs.write(reinterpret_cast<const char*>(m_highScores.data()), sizeof(m_highScores));
		}
	}

	// 戦闘機配列に対し戦闘機の中身を追加する
	void ScoreManager::PushBackFighterBase(const shared_ptr<FighterAircraftBase>& fighterAircraftBase)
	{
		m_fighterBases.push_back(fighterAircraftBase);
		int countId = m_fighterBases.size() - 1;
		fighterAircraftBase->SetId(countId);
		SetID(countId);
	}
}
//end basecross
