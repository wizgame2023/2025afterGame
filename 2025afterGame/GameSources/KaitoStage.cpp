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
			auto& game = GameManager::GetGameManager();
			game->SetGameStartFlag(true);
			game->ResetCheckPoint();

			auto& scrMana = ScoreManager::CreateScoreManager();
			
			scrMana->CreateHighScoreBinary();
			scrMana->SetID(L"Player");
			scrMana->SetID(L"Enemy1");
			scrMana->SetID(L"Enemy2");

			scrMana->SetPlScore(scrMana->GetPlScore() + 200);
			scrMana->SetScore(L"Enemy1", scrMana->GetScore(L"Enemy1") + 400);
			scrMana->SetScore(L"Enemy2", scrMana->GetScore(L"Enemy2") + 100);

			scrMana->SaveHighScoreBinary();

			//背景
			AddGameObject<Background>();

			auto player = AddGameObject<Player>();
			SetSharedGameObject(L"Player", player);

			AddGameObject<TestCubeKaito>(Vec3(0.0f, 0.0f, 10.0f), Vec3(10.0f, 10.0f, 1.0f));

			auto plPos = player->GetComponent<Transform>()->GetPosition();
			EffectManager::Instance().PlayEffect(L"Fire", Vec3(plPos));

			auto mainCamMana = AddGameObject<MainCameraManager>();
			SetSharedGameObject(L"MainCameraManager", mainCamMana);

			auto& gameManager = GameManager::GetGameManager();
			gameManager->AddCheckPoint();
			auto startCheckPoint = gameManager->GetCheckPoint(0);

			auto enemy = AddGameObject<Enemy>(Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);

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
		//scrMana->SetPlScore(scrMana->GetPlScore() + 1);
		DebugLog(L"\n\n\n\n\nPLScore : ", scrMana->GetPlScore());
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
