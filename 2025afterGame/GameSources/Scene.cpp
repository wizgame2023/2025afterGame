
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	Scene::~Scene()
	{

	}

	void Scene::CreateResource()
	{
		// 各種フォルダパスを取得
		auto& app = App::GetApp();
		const wstring path = app->GetDataDirWString();
		const wstring& modelPath = path + L"Models/";
		const wstring& texPath = path + L"Textures/";
		const wstring& uiPlayerPath = path + L"UI/Player/";
		const wstring& uiTitlePath = path + L"UI/Title/";
		const wstring& uiMenuPath = path + L"UI/Menu/";
		const wstring& uiScorePath = path + L"UI/Score/";
		const wstring& uiStagePath = path + L"UI/Stage/";
		const wstring& efkPath = path + L"Effects/";
		const wstring& soundPath = path + L"Sounds/";

		//Textures //////////////////////////////////////////////
		auto modelTex = modelPath + L"diffuse.png";
		App::GetApp()->RegisterTexture(L"diffuse_TX", modelTex);

		auto strTexture = texPath + L"TestTex.png";
		App::GetApp()->RegisterTexture(L"TestTex", strTexture);

		modelTex = modelPath + L"desk.png";
		App::GetApp()->RegisterTexture(L"DeskTex", modelTex);

		modelTex = modelPath + L"Chair.png";
		App::GetApp()->RegisterTexture(L"ChairTex", modelTex);

		strTexture = texPath + L"StageWall.png";
		App::GetApp()->RegisterTexture(L"StageWall", strTexture);

		strTexture = texPath + L"StageCeiling.jpg";
		App::GetApp()->RegisterTexture(L"StageCeiling", strTexture);

		modelTex = modelPath + L"floar.png";
		App::GetApp()->RegisterTexture(L"StageFloarTex", modelTex);

		modelTex = modelPath + L"bed.png";
		App::GetApp()->RegisterTexture(L"BedTex", modelTex);

		// Model /////////////////////////////////////////////////
		// Player
		auto modelMesh = MeshResource::CreateStaticModelMesh(modelPath, L"PropellerPlane.bmf");
		app->RegisterResource(L"Sentouki", modelMesh);

		modelMesh = MeshResource::CreateStaticModelMesh(modelPath, L"desk.bmf");
		app->RegisterResource(L"Desk", modelMesh);

		modelMesh = MeshResource::CreateStaticModelMesh(modelPath, L"Chair.bmf");
		app->RegisterResource(L"Chair", modelMesh);

		modelMesh = MeshResource::CreateStaticModelMesh(modelPath, L"floar.bmf");
		app->RegisterResource(L"StageFloar", modelMesh);

		modelMesh = MeshResource::CreateStaticModelMesh(modelPath, L"bed.bmf");
		app->RegisterResource(L"Bed", modelMesh);


		// UI /////////////////////////////////////////////////
		auto numberSprite = uiPlayerPath + L"Number.png";
		App::GetApp()->RegisterTexture(L"Number", numberSprite);
    
		auto hp = uiPlayerPath + L"Hp.png";
		App::GetApp()->RegisterTexture(L"HP", hp);
    
		auto colon = uiPlayerPath + L"Colon.png";
		App::GetApp()->RegisterTexture(L"Colon", colon);
    
		auto startTex = uiTitlePath + L"PressA.png";
		App::GetApp()->RegisterTexture(L"Start_TX", startTex);

		startTex = uiTitlePath + L"rogo.png";
		App::GetApp()->RegisterTexture(L"rogo_TX", startTex);

		auto remainingRounds = uiPlayerPath + L"RemainingRounds.png";
		App::GetApp()->RegisterTexture(L"RemainingRounds", remainingRounds);

		auto ScoreLow = uiScorePath + L"bear.png";
		App::GetApp()->RegisterTexture(L"Bear", ScoreLow);


		// MenuUI ///////////////////////
		auto pauseMenuMain = uiMenuPath + L"PauseMenuMain.png";
		App::GetApp()->RegisterTexture(L"PauseMenuMain_TX", pauseMenuMain);

		auto pauseMenuVolume = uiMenuPath + L"PauseMenuSetting.png";
		App::GetApp()->RegisterTexture(L"PauseMenuSetting_TX", pauseMenuVolume);
    
		auto buttonsTex = uiMenuPath + L"Buttons.png";
		App::GetApp()->RegisterTexture(L"Buttons_TX", buttonsTex);

		auto menuBackground = uiMenuPath + L"PauseMenuBackGround.png";
		App::GetApp()->RegisterTexture(L"PauseMenuBackGround_TX", menuBackground);

		// StageUI////////////////////////

		auto stageTex = uiStagePath + L"GameStart.png";
		App::GetApp()->RegisterTexture(L"GameStart_TX", stageTex);

		uiStagePath + L"GameEnd.png";
		App::GetApp()->RegisterTexture(L"GameEnd_TX", stageTex);


		// Effect /////////////////////////////////////////////
		EffectManager::Instance().CreateEfkInterface();
		EffectManager::Instance().RegisterEffect(L"Fire", efkPath + L"Sword.efk");
		

		//BGM
		wstring BGMPath = soundPath + L"BGM/";
		app->RegisterWav(L"TitleBGM", BGMPath + L"Title.wav");
		app->RegisterWav(L"StageBGM", BGMPath + L"Stage.wav");

		//SE
		wstring SEPath = soundPath + L"SE/";
		app->RegisterWav(L"ButtonPushSE", SEPath + L"ButtonPush.wav");
		app->RegisterWav(L"CountDownSE", SEPath + L"CountDown.wav");
		app->RegisterWav(L"DengerSE", SEPath + L"Denger.wav");
		app->RegisterWav(L"HealSE", SEPath + L"Heal.wav");
		app->RegisterWav(L"ReloadSE", SEPath + L"Reload.wav"); 
		app->RegisterWav(L"ResultSE", SEPath + L"Result.wav");
		app->RegisterWav(L"SpeedUpSE", SEPath + L"SpeedUp.wav");
		app->RegisterWav(L"GetScoreSE", SEPath + L"GetScore.wav");
		app->RegisterWav(L"ShotSE", SEPath + L"Shot.wav");
	}
	
	void Scene::OnCreate(){
		try {	
			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);

			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");			
			
			CreateResource();

			// ゲームマネージャー作成
			GameManager::CreateGameManager();
		}
		catch (...) {
			throw;
		}
	}

	void Scene::OnUpdate()
	{
		SceneBase::OnUpdate();

		// ゲームマネージャー更新
		GameManager::GetGameManager()->OnUpdate();
	}



	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
		if (event->m_MsgStr == L"ToMultiViewStage") {
			//マルチビューのアクティブステージ設定
			ResetActiveStage<MultiViewStage>();
		}
		if (event->m_MsgStr == L"ToTitleStage") {
			//タイトルステージのアクティブステージ設定
			ResetActiveStage<TitleStage>();
		}
		if (event->m_MsgStr == L"ToSelectStage") {
			//セレクトステージのアクティブステージ設定
			ResetActiveStage<SelectStage>();
		}
		//デバッグ用ステージ
		if (event->m_MsgStr == L"ToErionStage") {
			//セレクトステージのアクティブステージ設定
			ResetActiveStage<ErionStage>();
		}
		//デバッグ用ステージ
		if (event->m_MsgStr == L"ToTomokiStage") {
			//セレクトステージのアクティブステージ設定
			ResetActiveStage<TomokiStage>();
		}
	}

}
//end basecross
