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
	MainCameraManager::MainCameraManager(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
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

		// マルチビューかどうか
		IsMultiView(m_sharedName);

		m_mulCam = OnGetDrawCamera(); // カメラの取得
		m_plTrans = m_target->GetComponent<Transform>();
	}

	void MainCameraManager::OnUpdate()
	{
		// プレイヤーのUpベクトル履歴更新

		auto& app = App::GetApp();
		float delta = app->GetElapsedTime();
		static float time = 0;
		time += delta;
		auto& input = InputManager::GetInputManager();

		// 加速しているか
		//bool isAccel = m_player->GetAcceleration();

		bool test = input->GetButton(L"X");

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
		Vec3 newCamPos = LerpV3(currentCamPos, desiredPos, delta * followSpeed);

		// 常にプレイヤーの後ろにカメラを設置する(プレイヤーの角度が変わっても正面が映らないような感じ)
		m_mulCam->SetEye(newCamPos);

		// 加速に合わせて視野角を広げる
		AdjustFov(test);

		// プレイヤーの角度に合わせてカメラも傾く
		m_mulCam->SetUp(Vec3(smoothUp));

		// カメラの注視点
		m_mulCam->SetAt(atPos);

		// デバッグログ
		wstringstream wss(L"");
		wss << "Fov : " << m_mulCam->GetFovY() << "\n";
		wss << "target : " << m_sharedName << "\n";

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());

	}

	void MainCameraManager::AdjustFov(bool isAccel)
	{
		static constexpr float normalFov = 0.8f;
		static constexpr float accelFov = 1.0f;
		
		static float time = 0;
		float delta = App::GetApp()->GetElapsedTime();
		
		static constexpr float lerpSpeed = 0.03f;

		// 一定時間で
		//if (isAccel && time > 1.5f)
		//{
		//	float fov = LerpFlt(m_mulCam->GetFovY(), normalFov, 0.01f);
		//	m_mulCam->SetFovY(fov);
		//	return;
		//}

		// 加速中なら
		if (isAccel)
		{
			time += delta;
			float fov = LerpFlt(m_mulCam->GetFovY(), accelFov, lerpSpeed);
			m_mulCam->SetFovY(fov);
		}
		else
		{
			time = 0;
			float fov = LerpFlt(m_mulCam->GetFovY(), normalFov, lerpSpeed);
			m_mulCam->SetFovY(fov);
		}

	}

	void MainCameraManager::IsMultiView(const wstring& sharedName)
	{
		if (sharedName == L"Player1")
			dynamic_pointer_cast<MultiView>(GetStage()->GetView())->SetTargetIndex(0);
		else if (sharedName == L"Player2")
			dynamic_pointer_cast<MultiView>(GetStage()->GetView())->SetTargetIndex(1);
		else
			m_target = m_stage->GetSharedGameObject<Actor>(sharedName);
	}
}
//end basecross
