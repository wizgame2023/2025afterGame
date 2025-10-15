/*!
@file TitleStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	TitleStage::TitleStage()
	{
	}

	TitleStage::~TitleStage()
	{
	}

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void TitleStage::CreateViewLight() {
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



	void TitleStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
		}
		catch (...) {
			throw;
		}
	}

	void TitleStage::OnUpdate() 
	{
		auto& app = App::GetApp();
		auto& inputMgr=InputManager::GetInputManager();

		if (inputMgr->GetDownButton(L"A"))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSelectStage");
		}

		wstringstream wss(L"");
		wss << "CurrentStage : TitleStage" << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());
	}
}
//end basecross
