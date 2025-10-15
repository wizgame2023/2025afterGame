/*!
@file SelectStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	SelectStage::SelectStage() :
		m_stageSelect(0),
		m_deadZone(0.2f),
		m_deltaTime(0.0f),
		m_menuMoveCoolDown(0.2f)
	{
	}

	SelectStage::~SelectStage()
	{
	}

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void SelectStage::CreateViewLight() {
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



	void SelectStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
		}
		catch (...) {
			throw;
		}
	}

	void SelectStage::OnUpdate() 
	{
		auto& app = App::GetApp();
		auto& inputMgr=InputManager::GetInputManager();
		auto& game = GameManager::GetGameManager();
		float elapsed = game->GetDeltaTime();

		m_deltaTime += elapsed;

		if (m_deltaTime >= m_menuMoveCoolDown)
		{
			if (inputMgr->GetLStick().x > m_deadZone)
			{
				m_stageSelect++;
				m_deltaTime = 0.0f;
			}
			else if (inputMgr->GetLStick().x < -m_deadZone)
			{
				m_stageSelect--;
				m_deltaTime = 0.0f;
			}

			if (inputMgr->GetDownButton(L"A"))
			{
				switch (m_stageSelect)
				{
				case 0:
					PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
					m_deltaTime = 0.0f;
					break;

				default:
					m_deltaTime = 0.0f;
					break;
				}
			}
		}

		wstringstream wss(L"");
		wss << "CurrentStage : SelectStage" << endl;
		wss << "SelectStage : " << m_stageSelect << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());
	}
}
//end basecross
