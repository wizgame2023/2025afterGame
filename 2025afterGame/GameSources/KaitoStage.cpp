/*!
@file KaitoStage.cpp
@brief 個人作業用ゲームステージ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void KaitoStage::CreateViewLight() {
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



	void KaitoStage::OnCreate() {
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

			AddGameObject<TestCubeKaito>(Vec3(0.0f, 0.0f, 10.0f), Vec3(10.0f,10.0f,1.0f));

		}
		catch (...) {
			throw;
		}

		auto mainCamMana = AddGameObject<MainCameraManager>();
		SetSharedGameObject(L"MainCameraManager", mainCamMana);
	}

	// ==============================================================================
	// kaitostage末尾
	// ==============================================================================

	TestCubeKaito::TestCubeKaito(
		const shared_ptr<Stage>& stagePtr,
		Vec3 pos, Vec3 scale) :
		Actor(stagePtr, pos, Vec3(0.0f), scale, Col4(1.0f, 0.0f, 0.0f, 1.0f))
	{}

	TestCubeKaito::~TestCubeKaito()
	{}

	void TestCubeKaito::OnCreate()
	{
		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
		trans->SetScale(m_scale);

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		
		SetAlphaActive(true);

		// カメラを邪魔しえるオブジェクトのタグ(透明化処理はしない)
		AddTag(L"CameraObsDiffuse");
	}
}
//end basecross
