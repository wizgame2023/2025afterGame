/*!
@file TitleStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	TitleStage::TitleStage() :
		m_Transparency(1.0f),
		m_Transparent(true),
		m_elapsedTime(0.0f)
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

	//UI作成
	void TitleStage::CreateUI()
	{
		m_testTitle = AddGameObject<Sprite>(L"Start_TX", Vec2(412.0f, 128.0f), Vec3(0.0f, -210.0f, 0.0f));
	}

	void TitleStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();

			CreateUI();
		}
		catch (...) {
			throw;
		}
	}

	void TitleStage::OnUpdate() 
	{
		auto& app = App::GetApp();
		auto& inputMgr=InputManager::GetInputManager();
		m_elapsedTime = App::GetApp()->GetElapsedTime();

		BlinkUI(m_testTitle);

		//Aボタンを押すとシーン遷移
		if (inputMgr->GetDownButton(L"A"))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSelectStage");
			return;
		}

		//デバッグ用文字
		wstringstream wss(L"");
		wss << "CurrentStage : TitleStage" << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());
	}

	void TitleStage::BlinkUI(shared_ptr<Sprite> blinksprite)
	{
		//透明度を上げる、下げる
		if (m_Transparency > 0.0f && !m_Transparent)
		{
			m_Transparency -= 1.0f * m_elapsedTime;
		}
		if (m_Transparency < 1.0f && m_Transparent)
		{
			m_Transparency += 1.0f * m_elapsedTime;
		}

		//透明度がしきい値を超えた時上がるか下がるかを変更
		if (m_Transparency <= 0.0f)
		{
			m_Transparent = true;
		}
		if (m_Transparency >= 1.0f)
		{
			m_Transparent = false;
		}

		blinksprite->SetColor(Col4(1.0f, 1.0f, 1.0f, m_Transparency));
	}
}
//end basecross
