/*!
@file MainCameraManager.cpp
@brief プレイヤーのカメラ
担当:佐藤 海斗
*/

#include "stdafx.h"
#include "Project.h"
#include<vector>

namespace basecross{
	MainCameraManager::MainCameraManager(const shared_ptr<Stage>& stagPtr) :
		MyGameObject(stagPtr)
	{}

	MainCameraManager::MainCameraManager(const shared_ptr<Stage>& stagePtr, const shared_ptr<Actor>& target, const shared_ptr<MultiView>& mulView) :
		MyGameObject(stagePtr),
		m_target(target),
		m_mulView(mulView)

	{}
	
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
		// カメラとプレイヤーの距離
		constexpr float camDis = 7.0f;
		// カメラの高さ
		float camHeight = 3.0f;
		// カメラの追従速度
		constexpr float followSpeed = 20.0f;


		// プレイヤーの情報取得
		m_plPos = m_plTrans.lock()->GetPosition();
		m_plRot = m_plTrans.lock()->GetRotation();
		m_plFwrd = m_plTrans.lock()->GetForward();
		m_plUp = m_plTrans.lock()->GetUp();

		// カメラの現在の位置
		Vec3 currentCamPos = m_mainCamera->GetEye();

		// カメラの位置の調整
		Vec3 camPosOffset = Vec3(0.0f,0.0f,0.0f);

		// 目標カメラ位置
		Vec3 desiredPos = m_plPos - m_plFwrd * camDis + m_plUp * camHeight;

		// 滑らかに補間
		Vec3 newCamPos = Lerp(currentCamPos, desiredPos, delta * followSpeed);

		// 常にプレイヤーの後ろにカメラを設置する(プレイヤーの角度が変わっても正面が映らないような感じ)
		m_mainCamera->SetEye(newCamPos);

		// プレイヤーの角度に合わせてカメラも傾く
		m_mainCamera->SetUp(Vec3(m_plUp));

		// カメラの注視点
		m_mainCamera->SetAt(m_plPos + Vec3(0.0f, 0.0f, 0.0f)); 

		wstringstream wss(L"");
		wss << "UpX : " << m_mainCamera->GetUp().x << " " << "UpY : " << m_mainCamera->GetUp().y << " " << "UpZ : " << m_mainCamera->GetUp().z << " " << endl;
		wss << "CamRot : " << m_plRot.x << " " << m_plRot.y << " " << m_plRot.z << " " << endl;

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());

	}
}
//end basecross
