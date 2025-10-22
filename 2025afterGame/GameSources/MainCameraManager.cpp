/*!
@file MainCameraManager.cpp
@brief プレイヤーのカメラ
担当:佐藤 海斗
*/

#include "stdafx.h"
#include "Project.h"
#include<vector>
#include<deque>
#include<DirectXMathMatrix.inl>

namespace basecross{
	MainCameraManager::MainCameraManager(const shared_ptr<Stage>& stagPtr) :
		MyGameObject(stagPtr)
	{}

	MainCameraManager::MainCameraManager(
		const shared_ptr<Stage>& stagePtr, 
		const shared_ptr<Actor>& target, 
		const shared_ptr<Camera>& mulCam,
		const wstring& sharedName
	) :
		MyGameObject(stagePtr),
		m_target(target),
		m_mulCam(mulCam),
		m_sharedName(sharedName)
	{}
	
	void MainCameraManager::OnCreate()
	{
		m_stage = GetStage();
		if (m_sharedName == L"Player")
			dynamic_pointer_cast<MultiView>(GetStage()->GetView())->SetTargetIndex(0);
		else
			dynamic_pointer_cast<MultiView>(GetStage()->GetView())->SetTargetIndex(1);

		m_mulCam = OnGetDrawCamera(); // カメラの取得
		//m_target = m_stage->GetSharedGameObject<Actor>(m_sharedName);
		m_plTrans = m_target->GetComponent<Transform>();
	}

	void MainCameraManager::OnUpdate()
	{
		if (m_sharedName == L"Player")
			int a = 0;
		else
			int b = 0;

		// プレイヤーのUpベクトル履歴更新

		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();
		static float time = 0;
		time += delta;

		// カメラとプレイヤーの距離
		constexpr float camDis = 5.0f;
		// カメラの追従速度
		constexpr float followSpeed = 20.0f;
		// カメラの高さ
		constexpr float camHeight = 1.5f;

		// プレイヤーの情報取得
		m_plPos = m_plTrans.lock()->GetPosition();
		m_plRot = m_plTrans.lock()->GetRotation();
		m_plFwrd = m_plTrans.lock()->GetForward();
		m_plUp = m_plTrans.lock()->GetUp();

		// 履歴の最大値(6で0.1秒のディレイがかかる)
		static constexpr int historyMax = 15;

		// ディレイをかけたカメラの傾き制御
		Vec3 smoothUp = GetSmoothedUp(m_plUp, historyMax);

		// カメラの現在の位置
		Vec3 currentCamPos = m_mulCam->GetEye();

		// カメラの位置の調整
		Vec3 camPosOffset = Vec3(0.0f,0.0f,0.0f);

		// 目標カメラ位置
		Vec3 desiredPos = m_plPos - m_plFwrd * camDis + m_plUp * camHeight;

		// 注視点をプレイヤーより少し奥側にする
		Vec3 atPos = m_plPos + m_plFwrd * 10.0f;

		// 滑らかに補間
		Vec3 newCamPos = Lerp(currentCamPos, desiredPos, delta * followSpeed);

		// 常にプレイヤーの後ろにカメラを設置する(プレイヤーの角度が変わっても正面が映らないような感じ)
		m_mulCam->SetEye(newCamPos);


		// プレイヤーの角度に合わせてカメラも傾く
		m_mulCam->SetUp(Vec3(smoothUp));

		// カメラの注視点
		m_mulCam->SetAt(atPos);

		wstringstream wss(L"");
		wss << "Fov : " << m_mulCam->GetFovY() << "\n";
		wss << "target : " << m_sharedName << "\n";

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());

	}

}
//end basecross
