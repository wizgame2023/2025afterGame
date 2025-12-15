/*!
@file YuutaStage.cpp
@brief 個人作業用ゲームステージ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void YuutaStage::CreateViewLight() {
		const Vec3 eye(0.0f, 8.0f, -30.0f);
		const Vec3 at(0.0f,0.0f,10.0f);
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



	void YuutaStage::OnCreate() {
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

			// 正式のプレイヤー	
			//auto player = AddGameObject<Player>();
			//SetSharedGameObject(L"Player", player);

			// デバック用のプレイヤー作成
			auto player = AddGameObject<DebagPlayer>();
			SetSharedGameObject(L"Player", player);

		}
		catch (...) {
			throw;
		}

		auto& gameManager = GameManager::GetGameManager();
		gameManager->AddCheckPoint();
		//gameManager->AddCheckPoint();
		//gameManager->AddCheckPoint();

		auto startCheckPoint = gameManager->GetCheckPoint(0);
		// 敵が出ているかテスト処理
		//auto debagPlayer =  dynamic_pointer_cast<Actor>(GetSharedGameObject<DebagPlayer>(L"Player"));
		auto player = dynamic_pointer_cast<Actor>(GetSharedGameObject<DebagPlayer>(L"Player"));
		auto enemy = AddGameObject<Enemy>(Vec3(10.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.25f), startCheckPoint, player);

		// バリア妨害のテスト処理
		//auto testBullet = AddGameObject<Bullet>();
		//AddGameObject<DisableBarrier>(testBullet);

		// テストでバリア生成
		//auto testCube = AddGameObject<TestCube>(Vec3(+3.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(0.5f));
		//// Playerの親クラスがFighterAircraftBaseになっていないのでそれ待ちのコメントアウト
		//auto barrier = AddGameObject<Barrier>(GetSharedGameObject<Player>(L"Player"));
		//SetSharedGameObject(L"Barrier", barrier);

		// テスト用のオブジェクト
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		DataDir += L"Stage/";
		//CSVファイルの読み込み
		m_objectFile.SetFileName(DataDir + L"testdodge.csv");
		m_objectFile.ReadCsv();

		// 障害物のオブジェクト生成
		CreateDodgeObject();



		AddGameObject<TestCube>(Vec3(10.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<TestCube>(Vec3(-10.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<TestCube>(Vec3(0.0f, 0.0f, -10.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<TestCube>(Vec3(0.0f, 0.0f, 10.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));


		//auto mainCamMana = AddGameObject<MainCameraManager>();
		//SetSharedGameObject(L"MainCameraManager", mainCamMana);
	}

	void YuutaStage::OnUpdate()
	{
		if (m_Func) {
			m_Func();
			m_Func = nullptr;
		}
		// テストのために弾を出す
		auto bButton = InputManager::GetInputManager()->GetDownButton(L"B");
		if (bButton)
		{
			AddGameObject<Bullet>(GetSharedGameObject<Player>(L"Player"));
		}

		// テストのためにバリアをオンにする
		//auto yButton = InputManager::GetInputManager()->GetDownButton(L"X");
		//if (yButton)
		//{
		//	GetSharedGameObject<Barrier>(L"Barrier")->SetUse(true);
		//}
		//auto yButton = InputManager::GetInputManager()->GetDownButton(L"Y");
		//if (yButton)
		//{
		//	GetSharedGameObject<Barrier>(L"Barrier")->SetUse(false);
		//}


	}

	// 障害物生成処理
	void YuutaStage::CreateDodgeObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;
		vector<wstring> dodgeAnchorLine;
		//抜き出し
		m_objectFile.GetSelect(ObjectLine, 0, L"Object");
		m_objectFile.GetSelect(dodgeAnchorLine, 0, L"DodgeAnchor");

		for (auto& v : dodgeAnchorLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			wstring Tag = Tokens[10];

			auto obstacle = AddGameObject<TestCube>(Pos, Quat(0.0f,0.0f,0.0f,1.0f), Siz);
			obstacle->AddTag(L"ObstacleRoute"); // 障害物ルートタグを追加
		}

		for (auto& v : ObjectLine)
		{
			//オブジェクトの作成
			vector<wstring> Tokens;
			Util::WStrToTokenVector(Tokens, v, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);


			Vec3 Rot;
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());

			Vec3 Siz(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);

			wstring Tag = Tokens[10];

			AddGameObject<TestCsv>(Pos, Rot, Siz, Tag);
		}
	}


}
//end basecross
