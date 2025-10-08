/*!
@file MainCamera.cpp
@brief プレイヤーのカメラ
担当:佐藤 海斗
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void MainCamera::OnCreate()
	{
		m_stage = GetStage();
		m_mainCamera = OnGetDrawCamera(); // カメラの取得
		m_player = m_stage->GetSharedGameObject<Player>(L"Player");
	}

	void MainCamera::OnUpdate()
	{
		// プレイヤーの位置を取得
		m_plPos = m_player->GetComponent<Transform>()->GetPosition();

		m_mainCamera->SetEye(m_plPos + Vec3(0.0f, 2.0f, -5.0f)); // カメラの位置
		m_mainCamera->SetAt(m_plPos + Vec3(0.0f, 0.0f, 0.0f)); // カメラの注視点
	}
}
//end basecross
