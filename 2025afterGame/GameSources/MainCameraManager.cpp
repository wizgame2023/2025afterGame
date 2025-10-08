/*!
@file MainCameraManager.cpp
@brief プレイヤーのカメラ
担当:佐藤 海斗
*/

#include "stdafx.h"
#include "Project.h"

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

		// プレイヤーの位置と回転を取得
		m_plPos = m_plTrans.lock()->GetPosition();
		m_plRot = m_plTrans.lock()->GetRotation();

		// カメラの位置の調整
		Vec3 camPosOffset;

		// 常にプレイヤーの後ろにカメラを設置する(プレイヤーの角度が変わっても正面が映らないような感じ)



		m_mainCamera->SetEye(m_plPos + camPosOffset); // カメラの位置
		m_mainCamera->SetAt(m_plPos + Vec3(0.0f, 0.0f, 0.0f)); // カメラの注視点
	}
}
//end basecross
