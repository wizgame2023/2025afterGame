/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	StageCreateManager::StageCreateManager()
	{

	}

	StageCreateManager::~StageCreateManager()
	{

	}

	unique_ptr<StageCreateManager, StageCreateManager::StageCreateManagerDeleter> StageCreateManager::m_stageCreateManager;

	unique_ptr<StageCreateManager, StageCreateManager::StageCreateManagerDeleter>& StageCreateManager::CreateStageCreateManager()
	{
		try
		{
			if (m_stageCreateManager.get() == 0)
			{
				// 自分を作成
				m_stageCreateManager.reset(new StageCreateManager());

				// 初期化
				m_stageCreateManager->OnCreate();
			}
			return m_stageCreateManager;
		}
		catch (...)
		{
			throw;
		}

		return m_stageCreateManager;
	}

	unique_ptr<StageCreateManager, StageCreateManager::StageCreateManagerDeleter>& StageCreateManager::GetStageCreateManager()
	{
		return m_stageCreateManager;
	}

	void StageCreateManager::OnCreate()
	{

	}

	void StageCreateManager::CreateStageObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;

		//CSVファイルの宣言
		CsvFile objectFile;

		//CSVファイルの読み込み
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		DataDir += L"Stage/";
		objectFile.SetFileName(DataDir + L"positions.csv");
		objectFile.ReadCsv();

		//抜き出し
		objectFile.GetSelect(ObjectLine, 0, L"Object");
		for (auto& v : ObjectLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			wstring Tag = Tokens[10];

			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			stage->AddGameObject<TestCsv>(Pos, Rot, Siz, Tag);
		}
	}

	void StageCreateManager::CreateWallObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;

		//CSVファイルの宣言
		CsvFile objectFile;

		//CSVファイルの読み込み
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		DataDir += L"Stage/";
		objectFile.SetFileName(DataDir + L"positions.csv");
		objectFile.ReadCsv();

		//抜き出し
		objectFile.GetSelect(ObjectLine, 0, L"StageWall");
		for (auto& v : ObjectLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			wstring Tag = Tokens[10];

			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			stage->AddGameObject<StageWall>(Pos, Rot, Siz, Tag);
		}
	}

	void StageCreateManager::CreateInvisibleCollision()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;

		//CSVファイルの宣言
		CsvFile objectFile;

		//CSVファイルの読み込み
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		DataDir += L"Stage/";
		objectFile.SetFileName(DataDir + L"positions.csv");
		objectFile.ReadCsv();

		//抜き出し
		objectFile.GetSelect(ObjectLine, 0, L"InvisibleCollision");
		for (auto& v : ObjectLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			//wstring Tag = Tokens[10];

			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			stage->AddGameObject<InvisibleCollision>(Pos, Rot, Siz);
		}
	}

	void StageCreateManager::CreateRingObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;

		//CSVファイルの宣言
		CsvFile objectFile;

		//CSVファイルの読み込み
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		DataDir += L"Stage/";
		objectFile.SetFileName(DataDir + L"positions.csv");
		objectFile.ReadCsv();

		//抜き出し
		objectFile.GetSelect(ObjectLine, 0, L"DashRing");
		for (auto& v : ObjectLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			//wstring tag = Tokens[10];

			auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
			stage->AddGameObject<DashRing>(Pos, Rot, Siz);
		}
	}

	void StageCreateManager::CreateScoreObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;

		auto& score = ScoreObjectManager::GetScoreObjectManager();
		int select = rand() % m_scoreAnchorCount;

		//CSVファイルの宣言
		CsvFile objectFile;

		//CSVファイルの読み込み
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		DataDir += L"Stage/";
		objectFile.SetFileName(DataDir + L"positions.csv");
		objectFile.ReadCsv();

		//抜き出し
		objectFile.GetSelect(ObjectLine, 0, L"ScoreObjectAnchor");
		for (auto& v : ObjectLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			//wstring Tag = Tokens[10];
			int Tag = (int)_wtof(Tokens[10].c_str());

			if (Tag == select)
			{
				score->CreateScoreObject(Pos, Rot, Siz, Tag);
			}
		}
	}

	void StageCreateManager::CreateItemObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;

		auto& score = ScoreObjectManager::GetScoreObjectManager();
		int select = rand() % m_repairAnchorCount;

		//CSVファイルの宣言
		CsvFile objectFile;

		//CSVファイルの読み込み
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		DataDir += L"Stage/";
		objectFile.SetFileName(DataDir + L"positions.csv");
		objectFile.ReadCsv();

		//抜き出し
		objectFile.GetSelect(ObjectLine, 0, L"ItemObjectAnchor");
		for (auto& v : ObjectLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			//wstring Tag = Tokens[10];
			int Tag = (int)_wtof(Tokens[10].c_str());

			if (Tag == select)
			{
				auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
				stage->AddGameObject<ItemObject>(Pos, Rot, Siz, Tag);
			}
		}
	}

	void StageCreateManager::CreateAmmoObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;

		auto& score = ScoreObjectManager::GetScoreObjectManager();
		int select = rand() % m_ammoAnchorCount;

		//CSVファイルの宣言
		CsvFile objectFile;

		//CSVファイルの読み込み
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		DataDir += L"Stage/";
		objectFile.SetFileName(DataDir + L"positions.csv");
		objectFile.ReadCsv();

		//抜き出し
		objectFile.GetSelect(ObjectLine, 0, L"AmmoObjectAnchor");
		for (auto& v : ObjectLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			//wstring Tag = Tokens[10];
			int Tag = (int)_wtof(Tokens[10].c_str());

			if (Tag == select)
			{
				auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
				stage->AddGameObject<AmmoObject>(Pos, Rot, Siz, Tag);
			}
		}
	}

	void StageCreateManager::SetScoreAnchorCount(int scoreanchorcount)
	{
		m_scoreAnchorCount = scoreanchorcount;
	}

	void StageCreateManager::SetAmmoAnchorCount(int ammoanchorcount)
	{
		m_ammoAnchorCount = ammoanchorcount;
	}

	void StageCreateManager::SetRepairAnchorCount(int repairanchorcount)
	{
		m_repairAnchorCount = repairanchorcount;
	}
}
//end basecross
