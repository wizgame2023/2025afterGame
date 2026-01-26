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
			game->SetCountDown(true);
			game->NowPhase();
			//game->SetGameStartFlag(true);
			game->ResetCheckPoint();
			auto& obj = StageCreateManager::GetStageCreateManager();
			m_count = 0;

			auto backgroundPath = path + L"Backgrounds/";
			for (const auto& keyName : Background::pairs) {
				app->RegisterTexture(keyName.first, backgroundPath + keyName.first + L".bmp");
			}

			//ビューとライトの作成
			CreateViewLight();

			//背景
			AddGameObject<Background>();

			int max = 0;


			//for (int i = 0; i < 3; i++)
			//{
			//	game->AddCheckPoint();

			//	max++;
			//}

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

			obj->CreateStageObject();

			obj->CreateRingObject();

			obj->CreateWallObject();

			obj->CreateInvisibleCollision();
			//
			//obj->CreateScoreObject();

			// BGM、SE用のマネージャー作成
			m_audioManager = App::GetApp()->GetXAudio2Manager();
			m_bgm = m_audioManager->Start(L"StageBGM", XAUDIO2_LOOP_INFINITE, 0.6f);

			auto& gameManager = GameManager::GetGameManager();
			gameManager->AddCheckPoint();
			gameManager->SetCountDown(true);
			gameManager->SetGameStageNow(1);
			auto startCheckPoint = gameManager->GetCheckPoint(0);

			auto enemy = AddGameObject<Enemy>(Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			auto testenemy = AddGameObject<Enemy>(Vec3(50.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			AddGameObject<Enemy>(Vec3(-50.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			AddGameObject<Enemy>(Vec3(-50.0f, 0.0f, 25.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			AddGameObject<Enemy>(Vec3(0.0f, 0.0f, -50.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			AddGameObject<Enemy>(Vec3(50.0f, 0.0f, -50.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			AddGameObject<Enemy>(Vec3(25.0f, 0.0f, -20.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			AddGameObject<Enemy>(Vec3(-50.0f, 0.0f, -50.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			//AddGameObject<Enemy>(Vec3(-50.0f, 0.0f, -50.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);
			//auto enemy = AddGameObject<Enemy>(Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);


		}
		catch (...) {
			throw;
		}

		UIManager::CreateUIManager();
		auto mainCamMana = AddGameObject<MainCameraManager>();
		SetSharedGameObject(L"MainCameraManager", mainCamMana);
	}

	void GameStage::OnUpdate()
	{
		UIManager::GetUIManager()->OnUpdate();
		auto& obj = StageCreateManager::GetStageCreateManager();
	}

	// 消去される際の処理
	void GameStage::OnDestroy()
	{
		m_audioManager->Stop(m_bgm);
	}

}
//end basecross
