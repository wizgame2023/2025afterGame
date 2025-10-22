/*!
@file MultiViewStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void MultiViewStage::CreateViewLight() {
		const Vec3 eye(0.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<MultiView>();
		Viewport testView;
		testView.TopLeftX = 0.0f;
		testView.TopLeftY = 0.0f;
		testView.Width = 640.0f;
		testView.Height = 800.0f;
		testView.MinDepth = 0.0f;
		testView.MaxDepth = 1.0f;

		//ビューのカメラの設定
		m_camera1 = ObjectFactory::Create<Camera>();
		PtrView->AddView(testView, m_camera1);

		testView.TopLeftX = 640.0f;
		testView.TopLeftY = 0.0f;

		// カメラ追加
		m_camera2 = ObjectFactory::Create<Camera>();

		PtrView->AddView(testView, m_camera2);

		m_camera1->SetEye(eye);
		m_camera1->SetAt(at);
		m_camera2->SetEye(-eye);
		m_camera2->SetAt(at);

		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}



	void MultiViewStage::OnCreate() {
		try {
			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();

			wstring player1NumSharedName = L"Player";
			wstring player2NumSharedName = L"Player2";

			auto backgroundPath = path + L"Backgrounds/";
			for (const auto& keyName : Background::pairs) {
				app->RegisterTexture(keyName.first, backgroundPath + keyName.first + L".bmp");
			}

			//背景
			AddGameObject<Background>();

			//ビューとライトの作成
			CreateViewLight();
			auto player1 = AddGameObject<Player>();
			auto player2 = AddGameObject<Player>();
			player2->GetComponent<Transform>()->SetPosition(Vec3(5.0f, 0.0f, -1.0f));

			SetSharedGameObject(player1NumSharedName, player1);
			SetSharedGameObject(player2NumSharedName, player2);

			auto mainCamMana1 = AddGameObject<MainCameraManager>(player1, m_camera1, player1NumSharedName);

			auto mainCamMana2 = AddGameObject<MainCameraManager>(player2, m_camera2, player2NumSharedName);

		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
