/*!
@file MainCameraManager.cpp
@brief プレイヤーのカメラ
担当:佐藤 海斗
*/

#include "stdafx.h"
#include "Project.h"
#include<vector>

namespace basecross{

	void MainCameraManager::OnCreate()
	{
		m_stage = GetStage();
		m_mainCamera = OnGetDrawCamera(); // カメラの取得
		m_player = m_stage->GetSharedGameObject<Player>(L"Player");
		m_plTrans = m_player->GetComponent<Transform>();
	}

	void MainCameraManager::OnUpdate()
	{
		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();
		float time = 0;
		time += delta;

		// プレイヤーの位置と回転を取得
		m_plPos = m_plTrans.lock()->GetPosition();
		m_plRot = m_plTrans.lock()->GetRotation();
		m_plUp = m_plTrans.lock()->GetUp();
		// カメラの位置の調整
		Vec3 camPosOffset = Vec3(0.0f, 2.0f, -5.0f);

		// 常にプレイヤーの後ろにカメラを設置する(プレイヤーの角度が変わっても正面が映らないような感じ)

		// プレイヤーの角度に合わせてカメラも回転
		m_mainCamera->SetUp(Vec3(m_plUp));
		
		m_mainCamera->SetEye(m_plPos + camPosOffset); // カメラの位置
		m_mainCamera->SetAt(m_plPos + Vec3(0.0f, 0.0f, 0.0f)); // カメラの注視点

		wstringstream wss(L"");
		wss << "UpX : " << m_mainCamera->GetUp().x << " " << "UpY : " << m_mainCamera->GetUp().y << " " << "UpZ : " << m_mainCamera->GetUp().z << " " << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());

	}
}
//end basecross
