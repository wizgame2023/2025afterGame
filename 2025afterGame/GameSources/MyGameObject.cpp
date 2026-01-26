/*!
@file MyGameObject.cpp
@brief 全てのゲームのオブジェクトの元となるクラス
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	MyGameObject::MyGameObject(const shared_ptr<Stage>& stagePtr) :
		GameObject(stagePtr),
		m_SEVolume(1.0f)
	{

	}

	MyGameObject::~MyGameObject()
	{

	}

	void MyGameObject::OnCreate()
	{

	}

	void MyGameObject::OnUpdate()
	{
		//デルタタイム取得
		m_delta = App::GetApp()->GetElapsedTime();
	}

	wstring MyGameObject::GetBinaryPath() const
	{
		auto& app = App::GetApp();
		wstring path = app->GetDataDirWString();
		return path + L"Bin/";
	}

	void MyGameObject::FlushDebugLog()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(m_debugWss.str());
		m_debugWss.str(L""); // ログをクリア
		m_debugWss.clear();
	}

	void MyGameObject::SetSEVolume(const float volume)
	{
		auto& gameManager = GameManager::GetGameManager();
		gameManager->SetSEVolume(volume);
	}

	float MyGameObject::GetSEVolume() const
	{
		auto& gameManager = GameManager::GetGameManager();
		return gameManager->GetSEVolume();
	}


	// ポーズフラグのゲッタ
	bool MyGameObject::GetPauseFlag()
	{
		return m_pauseFlag;
	}

	// ポーズフラグのセッタ
	void MyGameObject::SetPauseFlag(bool Pause)
	{
		m_pauseFlag = Pause;
		return;
	}

}
//end basecross