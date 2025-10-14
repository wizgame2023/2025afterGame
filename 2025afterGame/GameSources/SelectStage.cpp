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
		auto& inputMgr=InputManager::GetInputManager();

		if (m_deltaTime >= m_menuMoveCoolDown)
		{
			if (inputMgr->GetDownButton(L"A") && m_stageSelect == 0)
			{
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
			m_deltaTime = 0.0f;
		}
	}
}
//end basecross
