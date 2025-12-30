/*!
@file MainCameraManager.cpp
@brief ƒvƒŒƒCƒ„[‚ÌƒJƒƒ‰
’S“–:²“¡ ŠC“l
*/

#include "stdafx.h"
#include "Project.h"
#include<vector>
#include<deque>
#include<DirectXMathMatrix.inl>

namespace basecross{
	// ==============================================================================
	// MainCameraManagerƒNƒ‰ƒX
	// ==============================================================================

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^1(ƒ\ƒ‚Ì‚İ)
	MainCameraManager::MainCameraManager(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
	{}

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^2(ƒ}ƒ‹ƒ`‘Î‰)
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
	// ¶¬
	// ==============================================================================

	void MainCameraManager::OnCreate()
	{
		// •K—v‚¾‚Á‚½‚çMyGameObject‚ÌOnCreate‚ğŒÄ‚Ô
		//MyGameObject::OnCreate();

		m_stage = GetStage();

		// ƒ}ƒ‹ƒ`ƒrƒ…[‚©‚Ç‚¤‚©
		CheckMultiView(m_sharedName);

		m_mulCam = OnGetDrawCamera(); // ƒJƒƒ‰‚Ìæ“¾
		m_plTrans = m_target->GetComponent<Transform>();
		
	}

	// ==============================================================================
	// XV
	// ==============================================================================

	void MainCameraManager::OnUpdate()
	{
		MyGameObject::OnUpdate();

		// ƒRƒ“ƒgƒ[ƒ‰[‚Ìæ“¾
		auto& input = InputManager::GetInputManager();

		// ‰Á‘¬‚µ‚Ä‚¢‚é‚©
		//bool isAccel = m_player->GetAcceleration();

		// ƒ{ƒ^ƒ“‚Ìó‘Ô‚Ìæ“¾(ƒfƒoƒbƒO—p‚É‚È‚é‚Æv‚í‚ê‚é)
		bool isAButton = input->GetButton(L"A"); // Aƒ{ƒ^ƒ“‚Ìó‘Ô
		bool isYButton = input->GetButton(L"Y"); // Yƒ{ƒ^ƒ“‚Ìó‘Ô
		bool isYButtonDownUp = input->GetNowUpdateButton(L"Y");// Yƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½uŠÔ‚Æ—£‚µ‚½uŠÔ‚ğæ‚é

		// ƒvƒŒƒCƒ„[‚Ìî•ñæ“¾
		m_plInfo.pos = m_plTrans.lock()->GetPosition();
		m_plInfo.rot = m_plTrans.lock()->GetRotation();
		m_plInfo.fwrd = m_plTrans.lock()->GetForward();
		m_plInfo.up = m_plTrans.lock()->GetUp();


		// ƒfƒBƒŒƒC‚ğ‚©‚¯‚½ƒJƒƒ‰‚ÌŒX‚«§Œä
		Vec3 smoothUp = GetSmoothedUp(m_plInfo.up, historyMax);

		// ƒJƒƒ‰‚ÌŒ»İ‚ÌˆÊ’u
		Vec3 currentCamPos = m_mulCam->GetEye();

		// ƒJƒƒ‰‚ª‘O•û‚ğ‰f‚·‚©Œã•û‚ğ‰f‚·‚©
		SetCameraNormalBehindMode(isYButton);


		// ‰Á‘¬‚É‡‚í‚¹‚Ä‹–ìŠp‚ğL‚°‚é
		AdjustFov(isAButton);

		// áŠQ•¨‚ªƒJƒƒ‰‚Ì‹@”\‚ğ×–‚‚µ‚Ä‚¢‚È‚¢‚©‚ğŒ©‚é
		UpdateCameraObstruction();

		// ŠŠ‚ç‚©‚É•âŠÔ		Yƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½(ƒJƒƒ‰‚ğŒã‚ë‚ÉŒü‚©‚¹‚½)uŠÔ‚Í•âŠÔOFF
		Vec3 newCamPos = isYButtonDownUp ?
			m_camPos :
			LerpV3(currentCamPos, m_camPos, m_delta * m_followSpeed);
		
		// ƒJƒƒ‰‚ÌÅI“I‚Èİ’è
		m_mulCam->SetUp(Vec3(smoothUp));// ƒvƒŒƒCƒ„[‚ÌŠp“x‚É‡‚í‚¹‚ÄƒJƒƒ‰‚àŒX‚­
		m_mulCam->SetEye(newCamPos);// í‚ÉƒvƒŒƒCƒ„[‚ÌŒã‚ë‚ÉƒJƒƒ‰‚ğİ’u‚·‚é
		m_mulCam->SetAt(m_atPos);// ƒJƒƒ‰‚Ì’‹“_‚Ìİ’è

		// ƒfƒoƒbƒOƒƒO
		//DebugLog(L"CameraPosX:", m_mulCam->GetEye().x);
		//DebugLog(L"CameraPosY:", m_mulCam->GetEye().y);
		//DebugLog(L"CameraPosZ:", m_mulCam->GetEye().z);
		//FlushDebugLog();
	}

	// ==============================================================================
	// ŠÖ”
	// ==============================================================================

	void MainCameraManager::AdjustFov(bool isAccel)
	{
		static constexpr float normalFov = 0.8f;
		static constexpr float accelFov = 1.0f;
		static constexpr float lerpSpeed = 0.03f;
		
		//static float time = 0;

		// ˆê’èŠÔ‚Å‹–ìŠp‚ğ–ß‚·(”O‚Ì‚½‚ßc‚·)
		//if (isAccel && time > 1.5f)
		//{
		//	float fov = LerpFlt(m_mulCam->GetFovY(), normalFov, 0.01f);
		//	m_mulCam->SetFovY(fov);
		//	return;
		//}

		// ‰Á‘¬’†‚È‚çL‚°‚é
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
		// ï¿½ï¿½ï¿½ï¿½É’Ç‰ï¿½
		m_plUpHistory.push_back(up);

		// ï¿½Å‘ï¿½lï¿½ğ’´‚ï¿½ï¿½ï¿½ï¿½ï¿½æ“ªï¿½ï¿½íœ
		if (m_plUpHistory.size() > historyMax)
			m_plUpHistory.pop_front();
	}

	// ==============================================================================

	Vec3 MainCameraManager::CalcUpHistoryAverage() const {
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ÆƒGï¿½ï¿½ï¿½[ï¿½Åï¿½ï¿½ï¿½(ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½É‚È‚ï¿½ï¿½ï¿½ï¿½á‚¤ï¿½ï¿½ï¿½ï¿½)
		if (m_plUpHistory.empty())
			return Vec3(0.0f, 1.0f, 0.0f);

		// ï¿½ï¿½ï¿½ï¿½Ì•ï¿½ï¿½Ï‚ï¿½ï¿½ï¿½
		Vec3 sum(0.0f, 0.0f, 0.0f);
		for (const auto& v : m_plUpHistory)
			sum += v;

		// ï¿½Lï¿½ï¿½ï¿½[ï¿½É“ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½é—šï¿½ï¿½Ìï¿½ï¿½Å•ï¿½ï¿½Ï‚ï¿½ï¿½ï¿½
		Vec3 avg = sum / static_cast<float>(m_plUpHistory.size());

		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½0ï¿½É‹ß‚ï¿½ï¿½Æ‚ï¿½ï¿½Í‹ï¿½ï¿½ï¿½ï¿½Iï¿½Éï¿½ï¿½ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½Îï¿½)
		if (avg.length() < 0.00001f)
			avg = Vec3(0.0f, 1.0f, 0.0f);

		// ï¿½ï¿½ï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½Ä•Ô‚ï¿½
		return avg.normalize();
	}

	// ==============================================================================

	Vec3 MainCameraManager::GetSmoothedUp(const Vec3& currentUp, const int historyMax) {
		// ï¿½ï¿½ï¿½ï¿½ÌXï¿½V
		UpdateUpHistory(currentUp, historyMax);

		// ï¿½ï¿½ï¿½Ï‚ï¿½vï¿½Zï¿½ï¿½ï¿½Ä•Ô‚ï¿½
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

		// ƒ^ƒO‚ª•t‚¢‚Ä‚¢‚È‚¢ê‡‚Í–³‹
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
			// “§–¾‰»ˆ—‚ğ‚·‚éê‡‚ÍSetAlphaActiove‚ğ<<â‘Î‚É>>true‚É‚·‚é‚±‚Æ
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
		// ƒJƒƒ‰ˆÊ’u‚Æ’‹“_‚ÌŒvZ
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
	// MainCameraManagerƒNƒ‰ƒX––”ö
	// ==============================================================================
}
//end basecross
