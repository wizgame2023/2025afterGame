
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

		auto Modeltex = modelPath + L"diffuse.png";
		App::GetApp()->RegisterTexture(L"diffuse_TX", Modeltex);

		auto strTexture = texPath + L"TestTex.png";
		App::GetApp()->RegisterTexture(L"TestTex", strTexture);

		// Player
		auto ModelMesh = MeshResource::CreateStaticModelMesh(modelPath, L"Sentouki.bmf");
		app->RegisterResource(L"Sentouki", ModelMesh);

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
			ResetActiveStage<YuutaStage>();
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
	}

}
//end basecross
