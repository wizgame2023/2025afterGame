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
}
//end basecross
