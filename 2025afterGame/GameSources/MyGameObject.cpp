/*!
@file MyGameObject.cpp
@brief 全てのゲームのオブジェクトの元となるクラス
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	MyGameObject::MyGameObject(const shared_ptr<Stage>& stagePtr) :
		GameObject(stagePtr)
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