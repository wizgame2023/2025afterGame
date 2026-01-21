/*!
@file PauseMenu.cpp
@brief PauseMenuÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	PauseMenu::PauseMenu(const shared_ptr<Stage>& stage) :
		MyGameObject(stage),
		m_crntMainSelect(PauseMainMenuSelect::Resume),
		m_pauseState(PauseMenuState::False)
	{
	}

	// =============================================================================================
	// ¶¬
	// =============================================================================================
	void PauseMenu::OnCreate()
	{
		m_stage = GetStage();
		SpriteInfo spInfo; // ‹¤’Êİ’è—p‚Ìƒx[ƒX
		spInfo.layer = 10; // ‘Sƒƒjƒ…[‹¤’Ê‚ÌƒŒƒCƒ„[

		// ƒƒjƒ…[‚Ì”wŒi
		spInfo.textureName = L"PauseMenuBackGround_TX";
		spInfo.size = Vec2(700.0f, 700.0f);
		spInfo.pos = Vec3(0.0f, 0.0f, 0.0f);

		m_pauseBackGroundSprite = m_stage->AddGameObject<Sprite>(
			spInfo.textureName,
			spInfo.size,
			spInfo.pos,
			spInfo.layer
		);

		// ƒƒCƒ“ƒƒjƒ…[ ------
		spInfo.textureName = L"PauseMenuMain_TX";
		spInfo.size = Vec2(200.0f, 100.0f);
		constexpr float mainUVHeight = 1.0f / 4.0f;

		for (int i = 0; i < 4; i++)
		{
			spInfo.pos = Vec3(0.0f, 300.0f + (i * -100), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, mainUVHeight * i);
			spInfo.rightBotUV = Vec2(1.0f, (mainUVHeight * (i + 1)));
			PushBackPauseMenuSprite(m_pauseMainMenuSprites, spInfo);
		}

		// ƒ{ƒŠƒ…[ƒ€ƒƒjƒ…[ ------
		spInfo.textureName = L"PauseMenuSetting_TX";
		spInfo.size = Vec2(200.0f, 100.0f);
		constexpr float SettingUVHeight = 1.0f / 4.0f;

		for (int i = 0; i < 2; i++)
		{
			spInfo.pos = Vec3(0.0f, 300.0f + (i * -100), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, SettingUVHeight * i);
			spInfo.rightBotUV = Vec2(1.0f, SettingUVHeight * (i + 1));
			PushBackPauseMenuSprite(m_pauseSettingMenuSprites, spInfo);

			spInfo.leftTopUV = Vec2(0.0f, SettingUVHeight * i + 0.5f);
			spInfo.rightBotUV = Vec2(1.0f, SettingUVHeight * (i + 1) + 0.5f);
			PushBackPauseMenuSprite(m_pauseVolumeMenuSprites, spInfo);
		}

		// ƒ{ƒ^ƒ“ŒQ ------
		spInfo.textureName = L"Buttons_TX";
		spInfo.size = Vec2(80.0f, 80.0f);
		constexpr float buttonsUV = 1.0f / 4.0f;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				spInfo.pos = Vec3(300.0f + (i * 100), 300.0f + (j * -100), 0.0f);
				spInfo.leftTopUV = Vec2(buttonsUV * i, buttonsUV * j);
				spInfo.rightBotUV = Vec2(buttonsUV * (i + 1), buttonsUV * (j + 1));
				PushBackPauseMenuSprite(m_pauseButtonsSprites, spInfo);
			}
		}

		m_pauseState = PauseMenuState::False;

		// Å‰‚Í”ñ•\¦‚É‚µ‚Ä‚¨‚­
		IsVisibleAllMenuSprites(false);
	}

	// =============================================================================================
	// XV
	// =============================================================================================
	void PauseMenu::OnUpdate()
	{
		// ƒRƒ“ƒgƒ[ƒ‰[‚Ìæ“¾
		auto& input = InputManager::GetInputManager();
		bool isStartButtonDown = input->GetNowUpdateButton(L"Start"); // ƒXƒ^[ƒgƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½uŠÔ‚ğæ‚é

		// ƒ|[ƒYŠJn‚Ìˆ—
		if (isStartButtonDown && m_pauseState == PauseMenuState::False)
		{
			StartPause();
		}

		// ”ñƒ|[ƒY’†‚Í‘Sƒƒjƒ…[”ñ•\¦
		if (m_pauseState == PauseMenuState::False)
		{
			IsVisibleAllMenuSprites(false);
			return;
		}

		UpdatePauseMenu();

		MenuVisibleManagement();

		DebugLogs();
		FlushDebugLog();
	}

	// =============================================================================================
	// ŠÖ”ŒQ
	// =============================================================================================

	void PauseMenu::DebugLogs()
	{
		DebugLog(L"\n\n\n\nPauseState : ", to_wstring(static_cast<int>(m_pauseState)));

		if (m_pauseState == PauseMenuState::MainMenu)
		{
			switch (m_crntMainSelect)
			{
			case PauseMainMenuSelect::Resume:
				DebugLog(L" Current Select: Resume ", to_wstring(static_cast<int>(m_crntMainSelect)));
				break;
			case PauseMainMenuSelect::Restart:
				DebugLog(L" Current Select: Restart ", to_wstring(static_cast<int>(m_crntMainSelect)));
				break;
			case PauseMainMenuSelect::Setting:
				DebugLog(L" Current Select: Setting ", to_wstring(static_cast<int>(m_crntMainSelect)));
				break;
			case PauseMainMenuSelect::Exit:
				DebugLog(L" Current Select: Exit ", to_wstring(static_cast<int>(m_crntMainSelect)));
				break;

			}
		}
		else if (m_pauseState == PauseMenuState::SettingMenu)
		{
			switch (m_crntSettingSelect)
			{
			case PauseSettingMenuSelect::Volume:
				DebugLog(L" Current Select: Volume ", to_wstring(static_cast<int>(m_crntSettingSelect)));
				break;
			case PauseSettingMenuSelect::KeyConfig:
				DebugLog(L" Current Select: KeyConfig ", to_wstring(static_cast<int>(m_crntSettingSelect)));
				break;
			}
		}
		else if (m_pauseState == PauseMenuState::VolumeMenu)
		{
			switch (m_crntVolumeSelect)
			{
			case PauseVolumeMenuSelect::BGMVolume:
				DebugLog(L" Current Select: BGM ", to_wstring(static_cast<int>(m_crntVolumeSelect)));
				break;
			case PauseVolumeMenuSelect::SEVolume:
				DebugLog(L" Current Select: SE ", to_wstring(static_cast<int>(m_crntVolumeSelect)));
				break;
			}
		}
		else if (m_pauseState == PauseMenuState::KeyConfigMenu)
		{
			switch (m_crntKeyConfigSelect)
			{
			case PauseKeyConfigMenuSelect::UpDownSwap:
				DebugLog(L" Current Select: UpDownSwap ", to_wstring(static_cast<int>(m_crntKeyConfigSelect)));
				break;
			case PauseKeyConfigMenuSelect::Bullet:
				DebugLog(L" Current Select: Bullet ", to_wstring(static_cast<int>(m_crntKeyConfigSelect)));
				break;
			case PauseKeyConfigMenuSelect::ViewBehind:
				DebugLog(L" Current Select: ViewBehind ", to_wstring(static_cast<int>(m_crntKeyConfigSelect)));
				break;
			}
		}
		auto& gameManager = GameManager::GetGameManager();
		float BGMVolume = gameManager->GetBGMVolume();
		DebugLog(L"\nBGMVolume : ", BGMVolume);
		DebugLog(L"SEVolume : ", GetSEVolume());
	}

	// ==============================================================================

	void PauseMenu::UpdatePauseMenu()
	{
		auto& input = InputManager::GetInputManager();

		switch (m_pauseState)
		{
		case PauseMenuState::MainMenu:		UpdateMainMenu(*input); break;
		case PauseMenuState::SettingMenu:	UpdateSettingMenu(*input); break;
		case PauseMenuState::VolumeMenu:	UpdateVolumeMenu(*input); break;
		case PauseMenuState::BGMSetting:
		case PauseMenuState::SESetting:		UpdateVolumeSettingMenu(*input); break;
		case PauseMenuState::KeyConfigMenu:	UpdateKeyConfigMenu(*input); break;
		}
	}

	// ==============================================================================

	void PauseMenu::UpdateMainMenu(InputManager& input)
	{
		// LƒXƒeƒBƒbƒN‚Ìã‰º“ü—Í‚Å‘I‘ğˆ‚ğ•ÏX
		if(UpdateSelection(m_crntMainSelect, PauseMainMenuSelect::Max))
		{
			// ‘I‘ğˆ‚ª•Ï‚í‚Á‚½ê‡‚Ìˆ—
			// ‰¹‚ğ–Â‚ç‚·‚È‚Ç

		}

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");

		if (pressAButton)
		{
			switch (m_crntMainSelect)
			{
			case PauseMainMenuSelect::Resume:
				// ÄŠJ‚ª‘I‘ğ‚³‚ê‚½ê‡‚Íƒ|[ƒY‰ğœ
				m_pauseState = PauseMenuState::False;
				break;

			case PauseMainMenuSelect::Restart:
				m_pauseState = PauseMenuState::False;
				// ƒŠƒXƒ^[ƒg‚ª‘I‘ğ‚³‚ê‚½ê‡‚ÍƒQ[ƒ€ƒXƒe[ƒW‚Ö‘JˆÚ
				PostEvent(0.0f,
					GetThis<ObjectInterface>(),
					App::GetApp()->GetScene<Scene>(),
					L"ToGameStage");
				break;

			case PauseMainMenuSelect::Setting:
				// İ’è‚ª‘I‘ğ‚³‚ê‚½ê‡‚Íİ’èƒƒjƒ…[‚Ö‘JˆÚ
				m_pauseState = PauseMenuState::SettingMenu;
				m_crntSettingSelect = PauseSettingMenuSelect::Volume;
				break;

			case PauseMainMenuSelect::Exit:
				m_pauseState = PauseMenuState::False;
				// I—¹‚ª‘I‘ğ‚³‚ê‚½ê‡‚Íƒ^ƒCƒgƒ‹ƒXƒe[ƒW‚Ö‘JˆÚ
				PostEvent(0.0f,
					GetThis<ObjectInterface>(),
					App::GetApp()->GetScene<Scene>(),
					L"ToTitleStage");
				break;

			default:
				// —áŠO‚ª”­¶‚µ‚½ê‡‚Íƒ|[ƒY‰ğœ
				m_pauseState = PauseMenuState::False;
				break;
			}
		}
		else if (pressBButton)
		{
			// ƒƒCƒ“ƒƒjƒ…[‚ÅBƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½ê‡‚Íƒ|[ƒY‰ğœ
			m_pauseState = PauseMenuState::False;
		}
	}

	// ==============================================================================

	void PauseMenu::UpdateSettingMenu(InputManager& input)
	{
		// LƒXƒeƒBƒbƒN‚Ìã‰º“ü—Í‚Å‘I‘ğˆ‚ğ•ÏX
		if (UpdateSelection(m_crntSettingSelect, PauseSettingMenuSelect::Max))
		{
			// ‘I‘ğˆ‚ª•Ï‚í‚Á‚½ê‡‚Ìˆ—
			// ‰¹‚ğ–Â‚ç‚·‚È‚Ç

		}

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");

		if (pressAButton)
		{
			switch (m_crntSettingSelect)
			{
			case PauseSettingMenuSelect::Volume:
				// ƒ{ƒŠƒ…[ƒ€ƒƒjƒ…[‚Ö‘JˆÚ
				m_pauseState = PauseMenuState::VolumeMenu;
				break;

			case PauseSettingMenuSelect::KeyConfig:
				// ƒL[ƒRƒ“ƒtƒBƒOƒƒjƒ…[‚Ö‘JˆÚ
				m_pauseState = PauseMenuState::KeyConfigMenu;
				break;
			}
		}
		else if (pressBButton)
		{
			m_pauseState = PauseMenuState::MainMenu;
		}
	}

	// ==============================================================================

	void PauseMenu::UpdateVolumeMenu(InputManager& input)
	{
		// LƒXƒeƒBƒbƒN‚Ìã‰º“ü—Í‚Å‘I‘ğˆ‚ğ•ÏX
		if (UpdateSelection(m_crntVolumeSelect, PauseVolumeMenuSelect::Max))
		{
			// ‘I‘ğˆ‚ª•Ï‚í‚Á‚½ê‡‚Ìˆ—
			// ‰¹‚ğ–Â‚ç‚·‚È‚Ç

		}

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");

		if (pressAButton)
		{
			switch (m_crntVolumeSelect)
			{
				case PauseVolumeMenuSelect::BGMVolume:
					m_pauseState = PauseMenuState::BGMSetting;
					break;

				case PauseVolumeMenuSelect::SEVolume:
					m_pauseState = PauseMenuState::SESetting;
					break;
			}
		}
		else if (pressBButton)
		{
			m_pauseState = PauseMenuState::SettingMenu;
		}

	}

	// ==============================================================================

	void PauseMenu::UpdateVolumeSettingMenu(InputManager& input)
	{
		auto& gameManager = GameManager::GetGameManager();
		float crntVol = 0.0f;
		function<void(float)> volSetter;	// –¢’è‹`
		if (m_pauseState == PauseMenuState::BGMSetting)
		{
			crntVol = gameManager->GetBGMVolume();
			volSetter = [&](float v) { return gameManager->SetBGMVolume(v); };	// ‚±‚±‚ÅŠÖ”‚ğ’è‹`
		}
		else if (m_pauseState == PauseMenuState::SESetting)
		{
			crntVol = gameManager->GetSEVolume();
			volSetter = [&](float v) { return gameManager->SetSEVolume(v); };	// ‚±‚±‚ÅŠÖ”‚ğ’è‹`
		}

		auto leftStick = input.GetLStick();
		if (leftStick != Vec2(0.0f))
		{
			float clampedVol = clamp(crntVol + leftStick.x * 0.01f, 0.0f, 1.0f);
			volSetter(clampedVol);
		}

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");
		if (pressAButton || pressBButton)
		{
			m_pauseState = PauseMenuState::VolumeMenu;

			// SavePauseData();
		}

	}

	// ==============================================================================

	void PauseMenu::UpdateKeyConfigMenu(InputManager& input)
	{
		// LƒXƒeƒBƒbƒN‚Ìã‰º“ü—Í‚Å‘I‘ğˆ‚ğ•ÏX
		if (UpdateSelection(m_crntKeyConfigSelect, PauseKeyConfigMenuSelect::Max))
		{
			// ‘I‘ğˆ‚ª•Ï‚í‚Á‚½ê‡‚Ìˆ—
			// ‰¹‚ğ–Â‚ç‚·‚È‚Ç

		}

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");

		if (pressAButton)
		{
			switch (m_crntKeyConfigSelect)
			{
			case PauseKeyConfigMenuSelect::UpDownSwap:
				break;

			case PauseKeyConfigMenuSelect::Bullet:
				break;

			case PauseKeyConfigMenuSelect::ViewBehind:
				break;
			}
		}
		else if (pressBButton)
		{
			m_pauseState = PauseMenuState::SettingMenu;
		}

	}

	// ==============================================================================

	void PauseMenu::UpdateKeyConfigSettingMenu(InputManager& input)
	{
		const wstring inputKey = input.GetPressedButton();

		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½Ô‚ï¿½Backï¿½Å‚ï¿½ï¿½ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Å–ß‚ï¿½
		if (inputKey == L"" || inputKey == L"Back")
		{
			return;
		}

		bool pressStartButton = input.GetDownButton(L"Start");

		// Startï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½ê‡ï¿½ÍƒLï¿½[ï¿½Rï¿½ï¿½ï¿½tï¿½Bï¿½Oï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½Ö–ß‚ï¿½
		if (pressStartButton)
		{
			m_pauseState = PauseMenuState::KeyConfigMenu;
			return;
		}

		function<void(const wstring&)> keySetter;	// ï¿½ï¿½ï¿½ï¿½`

		// keySetterï¿½Ì’ï¿½`
		switch (m_pauseState)
		{
		case PauseMenuState::AccelSetting:
			keySetter = [&](const wstring& k) { 
				m_pauseData.AccelKey = k;
				m_gameManager->SetAccelKey(k); 
			};
			break;
		case PauseMenuState::BulletSetting:
			keySetter = [&](const wstring& k) {
				m_pauseData.BulletKey = k;
				m_gameManager->SetBulletKey(k);
			};
			break;
		case PauseMenuState::ViewBehindSetting:
			keySetter = [&](const wstring& k) {
				m_pauseData.ViewBehindKey = k;
				m_gameManager->SetViewBehindKey(k); 
			};
			break;
		}

		// ï¿½dï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
		bool isDuplicate = false;
		// ï¿½ï¿½ï¿½ï¿½ï¿½Lï¿½[ï¿½İ’ï¿½Å‚È‚ï¿½ï¿½ê‡ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½Lï¿½[ï¿½Æ“ï¿½ï¿½ï¿½ï¿½È‚ï¿½dï¿½ï¿½
		if (m_pauseState != PauseMenuState::AccelSetting && inputKey == m_pauseData.AccelKey) isDuplicate = true;
		if (m_pauseState != PauseMenuState::BulletSetting && inputKey == m_pauseData.BulletKey) isDuplicate = true;
		if (m_pauseState != PauseMenuState::ViewBehindSetting && inputKey == m_pauseData.ViewBehindKey) isDuplicate = true;

		if(isDuplicate)
		{
			// ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î‰ï¿½ï¿½ï¿½Â‚ç‚·ï¿½È‚Ç‚Ìï¿½ï¿½ï¿½
			return;
		}
		else
		{
			// ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½Îİ’ï¿½
			keySetter(inputKey);
			// SavePauseData();
		}
	}

	// ==============================================================================

	//void PauseMenu::CreatePauseBinary()
	//{
	//	// ƒoƒCƒiƒŠƒpƒXæ“¾
	//	wstring binaryPath = GetBinaryPath() + L"PauseMenuData.bin";

	//	// ƒoƒCƒiƒŠ‚ª‚ ‚é‚©‚ğŠm”F
	//	ifstream ifs(binaryPath, ios::binary);

	//	// ‚È‚¢‚È‚ç¶¬
	//	if (!ifs)
	//	{
	//		// ‰Šú‰»
	//		m_pauseData.BGMVolume = m_BGMVolume;
	//		m_pauseData.SEVolume = m_SEVolume;
	//		m_pauseData.UpDownSwap = false;
	//		m_pauseData.BulletKey = L"";

	//		// ofstream‚Åƒtƒ@ƒCƒ‹‚ğ¶¬
	//		ofstream ofs(binaryPath, ios::binary);
	//		ofs.write(reinterpret_cast<const char*>(&m_pauseData), sizeof(m_pauseData));
	//	}

	//}

	// ==============================================================================

	void PauseMenu::MenuVisibleManagement()
	{
		// Œ»İ‚Ìƒ|[ƒYƒƒjƒ…[‚Ìó‘Ô‚É‰‚¶‚Ä•\¦E”ñ•\¦‚ğØ‚è‘Ö‚¦
		switch (m_pauseState)
		{
		case PauseMenuState::False:
			// ‘Sƒƒjƒ…[‚ğ”ñ•\¦
			IsVisibleAllMenuSprites(false);
			break;

		case PauseMenuState::MainMenu:
			IsVisibleAllMenuSprites(false);
			// ”wŒi‚ÆƒƒCƒ“ƒƒjƒ…[‚ğ•\¦
			m_pauseBackGroundSprite->OnClear(false);
			IsVisibleMenuSprites(m_pauseMainMenuSprites, true);
			break;

		case PauseMenuState::SettingMenu:
			IsVisibleAllMenuSprites(false);
			// İ’èƒƒjƒ…[‚ğ•\¦
			m_pauseBackGroundSprite->OnClear(false);
			IsVisibleMenuSprites(m_pauseSettingMenuSprites, true);
			break;

		case PauseMenuState::VolumeMenu:
			IsVisibleAllMenuSprites(false);
			// ƒ{ƒŠƒ…[ƒ€ƒƒjƒ…[‚ğ•\¦
			m_pauseBackGroundSprite->OnClear(false);
			IsVisibleMenuSprites(m_pauseVolumeMenuSprites, true);
			break;

		case PauseMenuState::KeyConfigMenu:
			IsVisibleAllMenuSprites(false);
			// ƒL[ƒRƒ“ƒtƒBƒOƒƒjƒ…[‚ğ•\¦
			m_pauseBackGroundSprite->OnClear(false);
			IsVisibleMenuSprites(m_pauseKeyConfigMenuSprites, true);
			break;
		}
	}

	// ==============================================================================

	void PauseMenu::StartPause()
	{
		SetPauseFlag(true);
		m_crntMainSelect = PauseMainMenuSelect::Resume;
		m_pauseState = PauseMenuState::MainMenu;
		IsVisibleMenuSprites(m_pauseMainMenuSprites, true);
		m_pauseBackGroundSprite->OnClear(false);
	}

	// ==============================================================================

	void PauseMenu::PushBackPauseMenuSprite(vector<shared_ptr<Sprite>>& vecSprite, const SpriteInfo& spInfo)
	{
		auto sprite = m_stage->AddGameObject<Sprite>(
			spInfo.textureName,
			spInfo.size,
			spInfo.pos,
			spInfo.layer
		);

		sprite->SetUVRect(spInfo.leftTopUV, spInfo.rightBotUV);

		vecSprite.push_back(sprite);
	}

	// ==============================================================================

	void PauseMenu::IsVisibleAllMenuSprites(const bool flag)
	{
		m_pauseBackGroundSprite->OnClear(!flag);
		IsVisibleMenuSprites(m_pauseMainMenuSprites,flag);
		IsVisibleMenuSprites(m_pauseSettingMenuSprites, flag);
		IsVisibleMenuSprites(m_pauseVolumeMenuSprites, flag);
		IsVisibleMenuSprites(m_pauseKeyConfigMenuSprites, flag);
		IsVisibleMenuSprites(m_pauseButtonsSprites, flag);
	}

	// ==============================================================================

	void PauseMenu::IsVisibleMenuSprites(const vector<shared_ptr<Sprite>>& spVec, const bool flag)
	{
		// ‚»‚ê‚¼‚ê‚ÌƒXƒvƒ‰ƒCƒg‚É‘Î‚µ‚Ä“§–¾‰»ˆ—‚ğs‚¤
		for (const auto& sp : spVec)
		{
			sp->OnClear(!flag);
		}
	}

}
//end basecross
