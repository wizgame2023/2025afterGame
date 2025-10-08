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
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->AddView(testView, PtrCamera);

		testView.TopLeftX = 640.0f;
		testView.TopLeftY = 0.0f;

		// カメラ追加
		auto PtrCameraSecond = ObjectFactory::Create<Camera>();

		PtrView->AddView(testView, PtrCameraSecond);

		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		PtrCameraSecond->SetEye(eye);
		PtrCameraSecond->SetAt(at);

		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}



	void MultiViewStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			auto player = AddGameObject<Player>();
			SetSharedGameObject(L"Player", player);
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
