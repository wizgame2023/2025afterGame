
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
		wstring path = app->GetDataDirWString();
		wstring modelPath = path + L"Models/";
		wstring texPath = path + L"Textures/";
		wstring uiPlayerPath = path + L"UI/Player/";
		wstring uiTitlePath = path + L"UI/Title/";
		wstring efkPath = path + L"Effects/";

		//Textures //////////////////////////////////////////////
		auto modelTex = modelPath + L"diffuse.png";
		App::GetApp()->RegisterTexture(L"diffuse_TX", modelTex);

		auto strTexture = texPath + L"TestTex.png";
		App::GetApp()->RegisterTexture(L"TestTex", strTexture);

		// Model /////////////////////////////////////////////////
		// Player
		auto modelMesh = MeshResource::CreateStaticModelMesh(modelPath, L"PropellerPlane.bmf");
		app->RegisterResource(L"Sentouki", modelMesh);


		// UI /////////////////////////////////////////////////
		auto numberSprite = uiPlayerPath + L"Number.png";
		App::GetApp()->RegisterTexture(L"Number", numberSprite);
    
		auto hp = uiPlayerPath + L"Hp.png";
		App::GetApp()->RegisterTexture(L"HP", hp);
    
		auto colon = uiPlayerPath + L"Colon.png";
		App::GetApp()->RegisterTexture(L"Colon", colon);
    
		auto startTex = uiTitlePath + L"PressA.png";
		App::GetApp()->RegisterTexture(L"Start_TX", startTex);

		auto remainingRounds = uiPlayerPath + L"RemainingRounds.png";
		App::GetApp()->RegisterTexture(L"RemainingRounds", remainingRounds);
    
		// Effect /////////////////////////////////////////////
		EffectManager::Instance().CreateEfkInterface();
		EffectManager::Instance().RegisterEffect(L"Fire", efkPath + L"Sword.efk");
		
	}
	
	void Scene::OnCreate(){
		try {	
			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);

			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTomokiStage");			
			
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
