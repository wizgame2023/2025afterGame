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
	// ==============================================================================
	// MainCameraManagerクラス
	// ==============================================================================

	// コンストラクタ1(ソロのみ)
	MainCameraManager::MainCameraManager(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
	{}

	// コンストラクタ2(マルチ対応)
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

	// ==============================================================================
	// 生成
	// ==============================================================================

	void MainCameraManager::OnCreate()
	{
		// 必要だったらMyGameObjectのOnCreateを呼ぶ
		//MyGameObject::OnCreate();

		m_stage = GetStage();

		// マルチビューかどうか
		CheckMultiView(m_sharedName);

		m_mulCam = OnGetDrawCamera(); // カメラの取得
		m_plTrans = m_target->GetComponent<Transform>();
		
	}

	// ==============================================================================
	// 更新
	// ==============================================================================

	void MainCameraManager::OnUpdate()
	{
		MyGameObject::OnUpdate();

		// コントローラーの取得
		auto& input = InputManager::GetInputManager();

		// 加速しているか
		//bool isAccel = m_player->GetAcceleration();

		// ボタンの状態の取得(デバッグ用になると思われる)
		bool isAButton = input->GetButton(L"A"); // Aボタンの状態
		bool isYButton = input->GetButton(L"Y"); // Yボタンの状態
		bool isYButtonDownUp = input->GetNowUpdateButton(L"Y");// Yボタンを押した瞬間と離した瞬間を取る

		// プレイヤーの情報取得
		m_plInfo.pos = m_plTrans.lock()->GetPosition();
		m_plInfo.rot = m_plTrans.lock()->GetRotation();
		m_plInfo.fwrd = m_plTrans.lock()->GetForward();
		m_plInfo.up = m_plTrans.lock()->GetUp();


		// ディレイをかけたカメラの傾き制御
		Vec3 smoothUp = GetSmoothedUp(m_plInfo.up, historyMax);

		// カメラの現在の位置
		Vec3 currentCamPos = m_mulCam->GetEye();

		// カメラが前方を映すか後方を映すか
		SetCameraNormalBehindMode(isYButton);


		// 加速に合わせて視野角を広げる
		AdjustFov(isAButton);

		// 障害物がカメラの機能を邪魔していないかを見る
		UpdateCameraObstruction();

		// 滑らかに補間		Yボタンを押した(カメラを後ろに向かせた)瞬間は補間OFF
		Vec3 newCamPos = isYButtonDownUp ?
			m_camPos :
			LerpV3(currentCamPos, m_camPos, m_delta * m_followSpeed);
		
		// カメラの最終的な設定
		m_mulCam->SetUp(Vec3(smoothUp));// プレイヤーの角度に合わせてカメラも傾く
		m_mulCam->SetEye(newCamPos);// 常にプレイヤーの後ろにカメラを設置する
		m_mulCam->SetAt(m_atPos);// カメラの注視点の設定

		// デバッグログ
		//DebugLog(L"CameraPosX:", m_mulCam->GetEye().x);
		//DebugLog(L"CameraPosY:", m_mulCam->GetEye().y);
		//DebugLog(L"CameraPosZ:", m_mulCam->GetEye().z);
		//FlushDebugLog();
	}

	// ==============================================================================
	// 関数
	// ==============================================================================

	void MainCameraManager::AdjustFov(bool isAccel)
	{
		static constexpr float normalFov = 0.8f;
		static constexpr float accelFov = 1.0f;
		static constexpr float lerpSpeed = 0.03f;
		
		//static float time = 0;

		// 一定時間で視野角を戻す(念のため残す)
		//if (isAccel && time > 1.5f)
		//{
		//	float fov = LerpFlt(m_mulCam->GetFovY(), normalFov, 0.01f);
		//	m_mulCam->SetFovY(fov);
		//	return;
		//}

		// 加速中なら広げる
		if (isAccel)
		{
			//time += m_delta;
			float fov = LerpFlt(m_mulCam->GetFovY(), accelFov, lerpSpeed);
			m_mulCam->SetFovY(fov);
		}
		else
		{
			//time = 0;
			float fov = LerpFlt(m_mulCam->GetFovY(), normalFov, lerpSpeed);
			m_mulCam->SetFovY(fov);
		}

	}

	// ==============================================================================

	void MainCameraManager::CheckMultiView(const wstring& sharedName)
	{
		if (sharedName == L"Player1")
			dynamic_pointer_cast<MultiView>(GetStage()->GetView())->SetTargetIndex(0);
		else if (sharedName == L"Player2")
			dynamic_pointer_cast<MultiView>(GetStage()->GetView())->SetTargetIndex(1);
		else
			m_target = m_stage->GetSharedGameObject<Actor>(sharedName);
	}

	// ==============================================================================

	void MainCameraManager::UpdateUpHistory(const Vec3& up, const int historyMax) {
		// 履歴に追加
		m_plUpHistory.push_back(up);

		// 最大値を超えたら先頭を削除
		if (m_plUpHistory.size() > historyMax)
			m_plUpHistory.pop_front();
	}

	// ==============================================================================

	Vec3 MainCameraManager::CalcUpHistoryAverage() const {
		// こうしないとエラーで死ぬ(ゼロ除算になるから)
		if (m_plUpHistory.empty())
			return Vec3(0.0f, 1.0f, 0.0f);

		// 履歴の平均を取る
		Vec3 sum(0.0f, 0.0f, 0.0f);
		for (const auto& v : m_plUpHistory)
			sum += v;

		// キューに入っている数で割る
		Vec3 avg = sum / static_cast<float>(m_plUpHistory.size());

		// 長さが0に近いときは強制的に上向き(こっちもゼロ除算対策)
		if (avg.length() < 0.00001f)
			avg = Vec3(0.0f, 1.0f, 0.0f);

		// 正規化して返す
		return avg.normalize();
	}

	// ==============================================================================

	Vec3 MainCameraManager::GetSmoothedUp(const Vec3& currentUp, const int historyMax) {
		// 履歴の更新
		UpdateUpHistory(currentUp, historyMax);

		// 平均を計算して返す
		return CalcUpHistoryAverage();
	}

	// ==============================================================================

	void MainCameraManager::UpdateCameraObstruction()
	{
		float min = 9999999.9f;
		Vec3 bestHitPos = m_camPos;

		auto objVec = m_stage->GetGameObjectVec();

		for (auto obj : objVec)
		{
			auto result = TestCameraObstruction(m_plInfo.pos, m_camPos, obj);

			if (result.hit && result.hitLength < min)
			{
				min = result.hitLength;
				bestHitPos = result.hitPos;
			}
		}

		if (min < 9999999.9f)
		{
			m_camPos = bestHitPos;
		}
	}

	// ==============================================================================

	MainCameraManager::ObstructionHitResult MainCameraManager::TestCameraObstruction(const Vec3& from, const Vec3& to, const shared_ptr<GameObject>& obj)
	{
		ObstructionHitResult result;
		result.hitPos = Vec3(0.0f);
		result.hitLength = 9999999.9f;
		result.hit = false;
		result.diffuseColor = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		auto obstacles = dynamic_pointer_cast<GameObject>(obj);
		bool isCamObsDif = obstacles->FindTag(L"CameraObsDiffuse");
		bool isCamObsNotDif = obstacles->FindTag(L"CameraObsNotDiffuse");

		// タグが付いていない場合は無視
		if (!obstacles || (!isCamObsDif && !isCamObsNotDif))
			return result;

		auto ptrDraw = obstacles->GetComponent<SmBaseDraw>();
		TRIANGLE triangle;
		size_t triangleNum;
		ptrDraw->HitTestStaticMeshSegmentTriangles(from, to, result.hitPos, triangle, triangleNum); 

		Col4 currentDiffuse = ptrDraw->GetDiffuse();
		float currentAlpha = currentDiffuse.w;

		if (result.hitPos != Vec3(0.0f))
		{
			if (isCamObsNotDif)
			{
				Vec3 playerToHit = result.hitPos - from;
				result.hitLength = abs(playerToHit.x) + abs(playerToHit.y) + abs(playerToHit.z);
				result.hit = true;
			}
			// 透明化処理をする場合はSetAlphaActioveを<<絶対に>>trueにすること
			if (isCamObsDif)
			{
				float newAlpha = currentAlpha - 0.03f;
				newAlpha = clamp(newAlpha, 0.3f, 1.0f);

				result.diffuseColor = Col4(1.0f, 1.0f, 1.0f, newAlpha);
			}
		}
		else
		{
			if (isCamObsDif)
			{
				result.diffuseColor = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		if (isCamObsDif)
			ptrDraw->SetDiffuse(result.diffuseColor);


		return result;
	}

	// ==============================================================================

	//bool TestCameraDiffuseObj(const Vec3& from, const Vec3& to, const shared_ptr<GameObject>& obj)
	//{
	//	auto obstacles = dynamic_pointer_cast<GameObject>(obj);
	//	Vec3 hitPos = Vec3(0.0f);

	//	if (obstacles != nullptr || obstacles->FindTag(L"CameraObsNotDiffuse"))
	//		return false;

	//	auto ptrDraw = obstacles->GetComponent<SmBaseDraw>();
	//	TRIANGLE triangle;
	//	size_t triangleNum;
	//	ptrDraw->HitTestStaticMeshSegmentTriangles(from, to, hitPos, triangle, triangleNum); 

	//	if (hitPos != Vec3(0.0f))
	//	{
	//		Vec3 playerToHit = hitPos - from;
	//		hitLength = abs(playerToHit.x) + abs(playerToHit.y) + abs(playerToHit.z);
	//		result.hit = true;
	//	}

	//	return result;
	//}

	// ==============================================================================

	void MainCameraManager::SetCameraNormalBehindMode(bool isButton)
	{
		// カメラ位置と注視点の計算
		if (isButton)
		{
			m_camPos = m_plInfo.pos + m_plInfo.fwrd * m_camDis + m_plInfo.up * m_camHeight;
			m_atPos = m_plInfo.pos - m_plInfo.fwrd * m_atOffset;
		}
		else
		{
			m_camPos = m_plInfo.pos - m_plInfo.fwrd * m_camDis + m_plInfo.up * m_camHeight;
			m_atPos =  m_plInfo.pos + m_plInfo.fwrd * m_atOffset;
		}
	}

	// ==============================================================================

	// ==============================================================================
	// MainCameraManagerクラス末尾
	// ==============================================================================
}
//end basecross
