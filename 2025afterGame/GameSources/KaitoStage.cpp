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

			

			// ゲームマネージャ
			auto& game = GameManager::CreateGameManager();
			game->SetGameStartFlag(true);
			game->ResetCheckPoint();

			auto& scrMana = ScoreManager::CreateScoreManager();
			
			scrMana->CreateHighScoreBinary();
			// playerは0固定だけど、一応宣言
			scrMana->SetID(0);
			scrMana->SetID(1);
			scrMana->SetID(2);

			scrMana->AddPlScore(50);
			scrMana->AddScore(1, 150);
			scrMana->AddScore(2, 300);

			scrMana->SaveHighScoreBinary();

			//背景
			AddGameObject<Background>();

			m_player = AddGameObject<Player>();
			SetSharedGameObject(L"Player", m_player);

			AddGameObject<TestCubeKaito>(Vec3(0.0f, 0.0f, 10.0f), Vec3(10.0f, 10.0f, 1.0f));

			auto plPos = m_player->GetComponent<Transform>()->GetPosition();
			EffectManager::Instance().PlayEffect(L"Fire", Vec3(plPos));

			auto mainCamMana = AddGameObject<MainCameraManager>();
			SetSharedGameObject(L"MainCameraManager", mainCamMana);

			auto& gameManager = GameManager::GetGameManager();
			gameManager->AddCheckPoint();
			auto startCheckPoint = gameManager->GetCheckPoint(0);

			auto enemy = AddGameObject<Enemy>(Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, m_player);

			// BGM、SE用のマネージャー作成
			//m_audioManager = App::GetApp()->GetXAudio2Manager();
			//m_bgm = m_audioManager->Start(L"StageBGM", XAUDIO2_LOOP_INFINITE, gameManager->GetBGMVolume());
			
			// これがないとエフェクトが表示されない()
			AddGameObject<EffectUpdateDrawManager>();
		}

		catch (...) {
			throw;
		}
	}

	void KaitoStage::OnUpdate()
	{		
		auto& scrMana = ScoreManager::GetScoreManager();

		//m_bgm->m_SourceVoice->SetVolume(GameManager::GetGameManager()->GetBGMVolume());

		auto& input = InputManager::GetInputManager();
		auto ptrMana = App::GetApp()->GetXAudio2Manager();

		if (input->GetDownButton(L"X"))
		{
			ptrMana->Start(L"ShotSE", 0, m_player->GetSEVolume());
		}

		scrMana->AddPlScore(1);

		//scrMana->SetPlScore(scrMana->GetPlScore() + 1);
		//DebugLog(L"\n\n\n\n\nPLScore : ", scrMana->GetPlScore());
		//DebugLog(L"Enemy1Score : ", scrMana->GetScore(L"Enemy1"));
		//DebugLog(L"fileScore : ", scrMana->LoadHighScoreBinary());
		auto SortedScores = scrMana->GetSortedScores();
		for (size_t i = 0; i < SortedScores.size(); i++)
		{
			DebugLog(L"\nRank ", i + 1);
			DebugLog(L" ID : ", SortedScores[i].id);
			DebugLog(L" Score : ", SortedScores[i].crntScore);
		}
		FlushDebugLog();
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

		//auto ptrCol = AddComponent<CollisionObb>();
		//ptrCol->SetDrawActive(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		
		SetAlphaActive(true);

		// カメラを邪魔しえるオブジェクトのタグ(透明化処理はしない)
		AddTag(L"CameraObsDiffuse");
	}
}
//end basecross
