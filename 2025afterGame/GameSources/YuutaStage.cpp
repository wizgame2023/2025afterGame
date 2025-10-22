/*!
@file YuutaStage.cpp
@brief 個人作業用ゲームステージ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void YuutaStage::CreateViewLight() {
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



	void YuutaStage::OnCreate() {
		try {
			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();

			auto backgroundPath = path + L"Backgrounds/";
			for (const auto& keyName : Background::pairs) {
				app->RegisterTexture(keyName.first, backgroundPath + keyName.first + L".bmp");
			}

			//ビューとライトの作成
			CreateViewLight();

			//背景
			AddGameObject<Background>();

			auto player = AddGameObject<Player>();
			SetSharedGameObject(L"Player", player);

		}
		catch (...) {
			throw;
		}

		// バリア妨害のテスト処理
		//auto testBullet = AddGameObject<Bullet>();
		//AddGameObject<DisableShield>(testBullet);

		// テストでバリア生成
		auto testCube = AddGameObject<TestCube>(Vec3(+3.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.5f));
		// Playerの親クラスがFighterAircraftBaseになっていないのでそれ待ちのコメントアウト
		//auto barrier = AddGameObject<Barrier>(GetSharedGameObject<Player>(L"Player"));
		//SetSharedGameObject(L"Barrier", barrier);



		auto mainCamMana = AddGameObject<MainCameraManager>();
		SetSharedGameObject(L"MainCameraManager", mainCamMana);
	}

	void YuutaStage::OnUpdate()
	{
		// テストのために弾を出す
		auto bButton = InputManager::GetInputManager()->GetDownButton(L"B");
		if (bButton)
		{
			AddGameObject<Bullet>(GetSharedGameObject<Player>(L"Player"));
		}

		// テストのためにバリアをオンにする
		auto yButton = InputManager::GetInputManager()->GetDownButton(L"Y");
		if (yButton)
		{
			GetSharedGameObject<Barrier>(L"Barrier")->SetUse(true);
		}


	}

}
//end basecross
