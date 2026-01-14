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
			auto& game = GameManager::GetGameManager();
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

			auto player = AddGameObject<Player>();
			SetSharedGameObject(L"Player", player);
			int max = 0;

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
		auto& inputMgr = InputManager::GetInputManager();

		auto& obj = StageCreateManager::GetStageCreateManager();

		//十字キー下を押すとシーン遷移
		if (inputMgr->GetDownButton(L"DDown"))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			return;
		}

		//デバッグ用文字
		wstringstream wss(L"");
		wss << "CurrentStage : ErionStage" << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());

		if (m_count < 3)
		{
			obj->CreateItemObject();
			obj->CreateScoreObject();
			m_count++;
		}
	}
}
//end basecross
