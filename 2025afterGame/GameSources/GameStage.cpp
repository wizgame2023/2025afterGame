/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Bullet.h"
#include "Barrier.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		const Vec3 eye(0.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}



	void GameStage::OnCreate() {
		try {
			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();
			auto& game = GameManager::GetGameManager();
			game->SetGameStartFlag(true);
			game->ResetCheckPoint();

			auto backgroundPath = path + L"Backgrounds/";
			for (const auto& keyName : Background::pairs) {
				app->RegisterTexture(keyName.first, backgroundPath + keyName.first + L".bmp");
			}

			//ビューとライトの作成
			CreateViewLight();

			//背景
			AddGameObject<Background>();

			int max = 0;


			for (int i = 0; i < 3; i++)
			{
				game->AddCheckPoint();

				max++;
			}

			auto player = AddGameObject<Player>();
			player->SetPlayerIndex(0);
			SetSharedGameObject(L"Player", player);


			//auto player2 = AddGameObject<Player>();
			//player2->SetPlayerIndex(1);
			//SetSharedGameObject(L"Player2", player2);
			//player2->GetComponent<Transform>()->SetPosition(Vec3(10.0f, 0.0f, 0.0f));

			// ステージオブジ
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			DataDir += L"Stage/";
			//CSVファイルの読み込み
			m_objectFile.SetFileName(DataDir + L"positions.csv");
			m_objectFile.ReadCsv();

			CreateTestObject();

			CreateRingObject();

			CreateWallObject();


			auto& gameManager = GameManager::GetGameManager();
			gameManager->AddCheckPoint();
			auto startCheckPoint = gameManager->GetCheckPoint(0);

			auto enemy = AddGameObject<Enemy>(Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);


		}
		catch (...) {
			throw;
		}

		auto mainCamMana = AddGameObject<MainCameraManager>();
		SetSharedGameObject(L"MainCameraManager", mainCamMana);
	}


	void GameStage::CreateTestObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;
		//抜き出し
		m_objectFile.GetSelect(ObjectLine, 0, L"Object");
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

			AddGameObject<TestCsv>(Pos, Rot, Siz);
		}
	}

	void GameStage::CreateRingObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;
		//抜き出し
		m_objectFile.GetSelect(ObjectLine, 0, L"DashRing");
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

			AddGameObject<DashRing>(Pos, Rot, Siz);
		}
	}

	void GameStage::CreateWallObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;
		//抜き出し
		m_objectFile.GetSelect(ObjectLine, 0, L"StageWall");
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

			AddGameObject<StageWall>(Pos, Rot, Siz);
		}
	}


}
//end basecross
