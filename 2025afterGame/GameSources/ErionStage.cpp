/*!
@file ErionStage.cpp
@brief 個人作業用ゲームステージ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void ErionStage::CreateViewLight() {
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



	void ErionStage::OnCreate() {
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

			auto checkpoint = AddGameObject<CheckPoint>();
			checkpoint->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 10.0f);

		}
		catch (...) {
			throw;
		}

		auto mainCamMana = AddGameObject<MainCameraManager>();
		SetSharedGameObject(L"MainCameraManager", mainCamMana);
	}

	void ErionStage::OnUpdate()
	{
		auto& app = App::GetApp();

		//デバッグ用文字
		wstringstream wss(L"");
		wss << "CurrentStage : ErionStage" << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());
	}
}
//end basecross
