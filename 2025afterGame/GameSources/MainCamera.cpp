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

		// プレイヤーの位置に合わせてカメラを移動
		SetEye(Vec3(m_plPos.x, m_plPos.y + 3.0f, m_plPos.z - 5.0f));
		SetAt(m_plPos);
	}
}
//end basecross
