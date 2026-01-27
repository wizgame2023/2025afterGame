/*!
@file TitleStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	GameClearStage::GameClearStage() :
		m_Transparency(1.0f),
		m_Transparent(true),
		m_elapsedTime(0.0f)
	{
	}

	GameClearStage::~GameClearStage()
	{
	}

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameClearStage::CreateViewLight() {
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

	//UI作成
	void GameClearStage::CreateUI()
	{
		AddGameObject<Sprite>(L"rogo_TX", Vec2(1024.0f, 512.0f), Vec3(0.0f, 100.0f, 0.0f));
	}

	void GameClearStage::OnCreate() {
		// BGM、SE用のマネージャー作成
		m_AudioManager = App::GetApp()->GetXAudio2Manager();
		m_bgm = m_AudioManager->Start(L"TitleBGM", XAUDIO2_LOOP_INFINITE, 0.9f);

		try {
			//ビューとライトの作成
			CreateViewLight();

			CreateUI();
		}
		catch (...) {
			throw;
		}
	}

	void GameClearStage::OnUpdate() 
	{
		auto& app = App::GetApp();
		auto& inputMgr=InputManager::GetInputManager();
		m_elapsedTime = App::GetApp()->GetElapsedTime();

		//Aボタンを押すとシーン遷移
		if (inputMgr->GetDownButton(L"A"))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			return;
		}
	}

	// 消去される際の処理
	void GameClearStage::OnDestroy()
	{
		m_AudioManager->Stop(m_bgm);
	}
}
//end basecross
