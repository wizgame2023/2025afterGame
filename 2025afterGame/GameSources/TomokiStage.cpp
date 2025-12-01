/*!
@file TomokiStage.cpp
@brief 個人作業用ゲームステージ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void TomokiStage::CreateViewLight() {
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



	void TomokiStage::OnCreate() {
		try {
			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();
			auto input = app->GetInputDevice();
			auto& game = GameManager::GetGameManager();
			game->SetGameStartFlag(true);

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
			//player2->GetComponent<Transform>()->SetPosition(Vec3(0.0f, 0.0f, 15.0f));


		}
		catch (...) {
			throw;
		}

		auto mainCamMana = AddGameObject<MainCameraManager>();
		SetSharedGameObject(L"MainCameraManager", mainCamMana);
	}

	void TomokiStage::OnUpdate()
	{
	}

}
//end basecross
