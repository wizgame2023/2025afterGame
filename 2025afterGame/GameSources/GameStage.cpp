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


			auto player2 = AddGameObject<Player>();
			player2->SetPlayerIndex(1);
			SetSharedGameObject(L"Player2", player2);
			player2->GetComponent<Transform>()->SetPosition(Vec3(10.0f, 0.0f, 0.0f));

			//壁の仮生成
			auto wall1 = AddGameObject<StageWall>();
			wall1->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 55.0f);

			auto wall2 = AddGameObject<StageWall>();
			wall2->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -55.0f);

			auto wall3 = AddGameObject<StageWall>();
			wall3->GetComponent<Transform>()->SetPosition(0.0f, 55.0f, 0.0f);
			wall3->GetComponent<Transform>()->SetRotation(XMConvertToRadians(90.0f), 0.0f, 0.0f);

			auto wall4 = AddGameObject<StageWall>();
			wall4->GetComponent<Transform>()->SetPosition(0.0f, -55.0f, 0.0f);
			wall4->GetComponent<Transform>()->SetRotation(XMConvertToRadians(90.0f), 0.0f, 0.0f);

			auto wall5 = AddGameObject<StageWall>();
			wall5->GetComponent<Transform>()->SetPosition(55.0f, 0.0f, 0.0f);
			wall5->GetComponent<Transform>()->SetRotation(0.0f, XMConvertToRadians(90.0f), 0.0f);

			auto wall6 = AddGameObject<StageWall>();
			wall6->GetComponent<Transform>()->SetPosition(-55.0f, 0.0f, 0.0f);


		}
		catch (...) {
			throw;
		}

		auto mainCamMana = AddGameObject<MainCameraManager>();
		SetSharedGameObject(L"MainCameraManager", mainCamMana);
	}

}
//end basecross
