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

			/*for (int i = 0; i < 3; i++)
			{
				game->AddCheckPoint();
				max++;
			}*/

			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			DataDir += L"Stage/";
			////CSVファイルの読み込み
			m_objectFile.SetFileName(DataDir + L"positions.csv");
			m_objectFile.ReadCsv();

			CreateTestObject();

			CreateRingObject();

			//壁の仮生成
			CreateWallObject();
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
	}

	void ErionStage::CreateTestObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;
		//抜き出し
		m_objectFile.GetSelect(ObjectLine, 0, L"Object");
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

			//wstring tag = Tokens[10];

			AddGameObject<TestCsv>(Pos, Rot, Siz);
		}
	}

	void ErionStage::CreateRingObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;
		//抜き出し
		m_objectFile.GetSelect(ObjectLine, 0, L"DashRing");
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

			//wstring tag = Tokens[10];

			AddGameObject<DashRing>(Pos, Rot, Siz);
		}
	}

	void ErionStage::CreateWallObject()
	{
		//オブジェクトの配列
		vector<wstring> ObjectLine;
		//抜き出し
		m_objectFile.GetSelect(ObjectLine, 0, L"StageWall");
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

			//wstring tag = Tokens[10];

			AddGameObject<StageWall>(Pos, Rot, Siz);
		}
	}
}
//end basecross
