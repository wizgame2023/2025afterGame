/*!
@file PauseMenu.cpp
@brief PauseMenu実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	PauseMenu::PauseMenu(const shared_ptr<Stage>& stage) :
		MyGameObject(stage),
		m_crntMainSelect(PauseMainMenuSelect::Resume),
		m_pauseState(PauseMenuState::False),
		m_pauseData(PauseData{ 1.0f, 1.0f, false, L"A", L"RTrigger", L"Y" })
	{
	}

	// =============================================================================================
	// 生成
	// =============================================================================================
	void PauseMenu::OnCreate()
	{
		m_stage = GetStage();
		SpriteInfo spInfo; // 共通設定用のベース
		spInfo.layer = 10; // 全メニュー共通のレイヤー

		// メニューの背景
		spInfo.textureName = L"PauseMenuBackGround_TX";
		spInfo.size = Vec2(700.0f, 700.0f);
		spInfo.pos = Vec3(0.0f, 0.0f, 0.0f);

		// メニュー背景
		m_pauseBackGroundSprite = m_stage->AddGameObject<Sprite>(
			spInfo.textureName,
			spInfo.size,
			spInfo.pos,
			spInfo.layer
		);

		// メインメニュー ------
		spInfo.textureName = L"PauseMenuMain_TX";
		spInfo.size = Vec2(200.0f, 100.0f);
		constexpr float mainUVHeight = 1.0f / 4.0f;

		for (int i = 0; i < 4; i++)
		{
			spInfo.pos = Vec3(0.0f, 280.0f + (i * -150), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, mainUVHeight * i);
			spInfo.rightBotUV = Vec2(1.0f, (mainUVHeight * (i + 1)));
			PushBackPauseMenuSprite(m_pauseMainMenuSprites, spInfo);
		}

		// ボリュームメニュー ------
		spInfo.textureName = L"PauseMenuSetting_TX";
		spInfo.size = Vec2(200.0f, 100.0f);
		constexpr float SettingUVHeight = 1.0f / 4.0f;

		for (int i = 0; i < 2; i++)
		{
			// 音量、キーコンフィグ
			spInfo.pos = Vec3(0.0f, 200.0f + (i * -250), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, SettingUVHeight * i);
			spInfo.rightBotUV = Vec2(1.0f, SettingUVHeight * (i + 1));
			PushBackPauseMenuSprite(m_pauseSettingMenuSprites, spInfo);

			// BGM、SE
			spInfo.leftTopUV = Vec2(0.0f, SettingUVHeight * i + 0.5f);
			spInfo.rightBotUV = Vec2(1.0f, SettingUVHeight * (i + 1) + 0.5f);
			PushBackPauseMenuSprite(m_pauseVolumeMenuSprites, spInfo);
		}

		// ボリュームゲージ ------
		spInfo.textureName = L"PauseMenuVolumeGauge_TX";
		for (int i = 0; i < 2; i++)
		{
			// ボリューム文字列の位置を取得
			Vec3 volumeStringPos = m_pauseVolumeMenuSprites[i]->GetPosition();

			// ゲージ本体
			spInfo.pos = Vec3(0.0f, (volumeStringPos.y - 100), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, 0.0f);
			spInfo.rightBotUV = Vec2(0.75f, 1.0f);
			spInfo.size = Vec2(300.0f, 100.0f);
			PushBackPauseMenuSprite(m_pauseVolumeMenuSprites, spInfo);

			// スライダー部分
			spInfo.pos = Vec3(135.0f, (volumeStringPos.y - 100), 0.0f);
			spInfo.leftTopUV = Vec2(0.75f, 0.0f);
			spInfo.rightBotUV = Vec2(1.0f, 1.0f);
			spInfo.size = Vec2(100.0f, 100.0f);
			PushBackPauseMenuSprite(m_pauseVolumeMenuSprites, spInfo);
		}

		// キーコンフィグメニュー ------
		spInfo.textureName = L"PauseMenuKeyConfig_TX";
		spInfo.size = Vec2(200.0f, 100.0f);
		constexpr float KeyConfigUVHeight = 1.0f / 4.0f;

		for (int i = 0; i < 4; i++)
		{
			spInfo.pos = Vec3(0.0f, 300.0f + (i * -100), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, mainUVHeight * i);
			spInfo.rightBotUV = Vec2(1.0f, (mainUVHeight * (i + 1)));
			PushBackPauseMenuSprite(m_pauseKeyConfigMenuSprites, spInfo);
		}

		// ボタン群 ------
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

		m_gameManager = GameManager::GetGameManager().get();

		// 最初は非表示にしておく
		IsVisibleAllMenuSprites(false);
	}

	// =============================================================================================
	// 更新
	// =============================================================================================
	void PauseMenu::OnUpdate()
	{
		// コントローラーの取得
		auto& input = InputManager::GetInputManager();
		bool isStartButtonDown = input->GetNowUpdateButton(L"Start"); // スタートボタンを押した瞬間を取る

		// ポーズ開始の処理
		if (isStartButtonDown && m_pauseState == PauseMenuState::False)
		{
			StartPause();
		}

		// 非ポーズ中は全メニュー非表示
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
	// 関数群
	// =============================================================================================

	void PauseMenu::DebugLogs()
	{
		DebugLog(L"\n\n\n\nPauseState : ", L"");

		if (m_pauseState == PauseMenuState::MainMenu)
		{
			switch (m_crntMainSelect)
			{
			case PauseMainMenuSelect::Resume:
				DebugLog(L" Current Select: Resume ", L"");
				break;
			case PauseMainMenuSelect::Restart:
				DebugLog(L" Current Select: Restart ", L"");
				break;
			case PauseMainMenuSelect::Setting:
				DebugLog(L" Current Select: Setting ", L"");
				break;
			case PauseMainMenuSelect::Exit:
				DebugLog(L" Current Select: Exit ", L"");
				break;

			}
		}
		else if (m_pauseState == PauseMenuState::SettingMenu)
		{
			switch (m_crntSettingSelect)
			{
			case PauseSettingMenuSelect::Volume:
				DebugLog(L" Current Select: Volume ", L"");
				break;
			case PauseSettingMenuSelect::KeyConfig:
				DebugLog(L" Current Select: KeyConfig ", L"");
				break;
			}
		}
		else if (m_pauseState == PauseMenuState::VolumeMenu)
		{
			switch (m_crntVolumeSelect)
			{
			case PauseVolumeMenuSelect::BGMVolume:
				DebugLog(L" Current Select: BGM ", L"");
				break;
			case PauseVolumeMenuSelect::SEVolume:
				DebugLog(L" Current Select: SE ", L"");
				break;
			}
		}
		else if (m_pauseState == PauseMenuState::KeyConfigMenu)
		{
			switch (m_crntKeyConfigSelect)
			{
			case PauseKeyConfigMenuSelect::UpDownSwap:
				DebugLog(L" Current Select: UpDownSwap ", L"");
				break;
			case PauseKeyConfigMenuSelect::Accel:
				DebugLog(L" Current Select: Accel ", L"");
				break;
			case PauseKeyConfigMenuSelect::Bullet:
				DebugLog(L" Current Select: Bullet ", L"");
				break;
			case PauseKeyConfigMenuSelect::ViewBehind:
				DebugLog(L" Current Select: ViewBehind ", L"");
				break;
			}
		}
		
		float BGMVolume = m_gameManager->GetBGMVolume();
		auto& input = InputManager::GetInputManager();
		auto& pressedButton = input->GetPressedButton();

		DebugLog(L"\nBGMVolume : ", BGMVolume);
		DebugLog(L"SEVolume : ", GetSEVolume());
		DebugLog(pressedButton, L"");
		DebugLog(L"Accel Key : ", m_pauseData.AccelKey);
		DebugLog(L"Bullet Key : ", m_pauseData.BulletKey);
		DebugLog(L"ViewBehind Key : ", m_pauseData.ViewBehindKey);
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
		case PauseMenuState::AccelSetting:
		case PauseMenuState::BulletSetting:
		case PauseMenuState::ViewBehindSetting:	UpdateKeyConfigSettingMenu(*input); break;
		}
	}

	// ==============================================================================

	void PauseMenu::UpdateMainMenu(InputManager& input)
	{
		// Lスティックの上下入力で選択肢を変更
		if(UpdateSelection(m_crntMainSelect, PauseMainMenuSelect::Max))
		{
			// 選択肢が変わった場合の処理
			// 音を鳴らすなど

		}

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");

		if (pressAButton)
		{
			switch (m_crntMainSelect)
			{
			case PauseMainMenuSelect::Resume:
				// 再開が選択された場合はポーズ解除
				m_pauseState = PauseMenuState::False;
				break;

			case PauseMainMenuSelect::Restart:
				m_pauseState = PauseMenuState::False;
				// リスタートが選択された場合はゲームステージへ遷移
				PostEvent(0.0f,
					GetThis<ObjectInterface>(),
					App::GetApp()->GetScene<Scene>(),
					L"ToGameStage");
				break;

			case PauseMainMenuSelect::Setting:
				// 設定が選択された場合は設定メニューへ遷移
				m_pauseState = PauseMenuState::SettingMenu;
				m_crntSettingSelect = PauseSettingMenuSelect::Volume;
				break;

			case PauseMainMenuSelect::Exit:
				m_pauseState = PauseMenuState::False;
				// 終了が選択された場合はタイトルステージへ遷移
				PostEvent(0.0f,
					GetThis<ObjectInterface>(),
					App::GetApp()->GetScene<Scene>(),
					L"ToTitleStage");
				break;

			default:
				// 例外が発生した場合はポーズ解除
				m_pauseState = PauseMenuState::False;
				break;
			}
		}
		else if (pressBButton)
		{
			// メインメニューでBボタンが押された場合はポーズ解除
			m_pauseState = PauseMenuState::False;
		}
	}

	// ==============================================================================

	void PauseMenu::UpdateSettingMenu(InputManager& input)
	{
		// Lスティックの上下入力で選択肢を変更
		if (UpdateSelection(m_crntSettingSelect, PauseSettingMenuSelect::Max))
		{
			// 選択肢が変わった場合の処理
			// 音を鳴らすなど

		}

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");

		if (pressAButton)
		{
			switch (m_crntSettingSelect)
			{
			case PauseSettingMenuSelect::Volume:
				// ボリュームメニューへ遷移
				m_pauseState = PauseMenuState::VolumeMenu;
				break;

			case PauseSettingMenuSelect::KeyConfig:
				// キーコンフィグメニューへ遷移
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
		// Lスティックの上下入力で選択肢を変更
		if (UpdateSelection(m_crntVolumeSelect, PauseVolumeMenuSelect::Max))
		{
			// 選択肢が変わった場合の処理
			// 音を鳴らすなど

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
		float crntVol = 0.0f;
		function<void(float)> volSetter;	// 未定義
		if (m_pauseState == PauseMenuState::BGMSetting)
		{
			crntVol = m_gameManager->GetBGMVolume();
			volSetter = [&](float v) { // ここで関数を定義
				m_pauseVolumeMenuSprites[3]->SetPositionX(-130 + (v * 265));
				return m_gameManager->SetBGMVolume(v); 
			};	
		}
		else if (m_pauseState == PauseMenuState::SESetting)
		{
			crntVol = m_gameManager->GetSEVolume();
			volSetter = [&](float v) { 	// ここで関数を定義
				m_pauseVolumeMenuSprites[5]->SetPositionX(-130 + (v * 265));
				return m_gameManager->SetSEVolume(v); 
			};
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
		// Lスティックの上下入力で選択肢を変更
		if (UpdateSelection(m_crntKeyConfigSelect, PauseKeyConfigMenuSelect::Max))
		{
			// 選択肢が変わった場合の処理
			// 音を鳴らすなど

		}

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");

		if (pressAButton)
		{
			switch (m_crntKeyConfigSelect)
			{
			case PauseKeyConfigMenuSelect::UpDownSwap:
				// 上下移動入れ替え
				//m_pauseData.UpDownSwap = !m_pauseData.UpDownSwap;
				break;

			case PauseKeyConfigMenuSelect::Accel:
				m_pauseState = PauseMenuState::AccelSetting;
				break;

			case PauseKeyConfigMenuSelect::Bullet:
				m_pauseState = PauseMenuState::BulletSetting;
				break;

			case PauseKeyConfigMenuSelect::ViewBehind:
				m_pauseState = PauseMenuState::ViewBehindSetting;
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

		// 何も押されていない状態かBackであれば何もしないで戻る
		if (inputKey == L"" || inputKey == L"Back")
		{
			return;
		}

		bool pressStartButton = input.GetDownButton(L"Start");

		// Startボタンが押された場合はキーコンフィグメニューへ戻る
		if (pressStartButton)
		{
			m_pauseState = PauseMenuState::KeyConfigMenu;
			return;
		}

		function<void(const wstring&)> keySetter;	// 未定義

		// keySetterの定義
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

		// 重複チェック
		bool isDuplicate = false;
		// 加速キー設定でない場合、加速キーと同じなら重複
		if (m_pauseState != PauseMenuState::AccelSetting && inputKey == m_pauseData.AccelKey) isDuplicate = true;
		if (m_pauseState != PauseMenuState::BulletSetting && inputKey == m_pauseData.BulletKey) isDuplicate = true;
		if (m_pauseState != PauseMenuState::ViewBehindSetting && inputKey == m_pauseData.ViewBehindKey) isDuplicate = true;

		if(isDuplicate)
		{
			// 重複があれば音を鳴らすなどの処理
			return;
		}
		else
		{
			// 重複がなければ設定
			keySetter(inputKey);
			// SavePauseData();
		}
	}

	// ==============================================================================

	//void PauseMenu::CreatePauseBinary()
	//{
	//	// バイナリパス取得
	//	wstring binaryPath = GetBinaryPath() + L"PauseMenuData.bin";

	//	// バイナリがあるかを確認
	//	ifstream ifs(binaryPath, ios::binary);

	//	// ないなら生成
	//	if (!ifs)
	//	{
	//		// 初期化
	//		m_pauseData.BGMVolume = m_BGMVolume;
	//		m_pauseData.SEVolume = m_SEVolume;
	//		m_pauseData.UpDownSwap = false;
	//		m_pauseData.BulletKey = L"";

	//		// ofstreamでファイルを生成
	//		ofstream ofs(binaryPath, ios::binary);
	//		ofs.write(reinterpret_cast<const char*>(&m_pauseData), sizeof(m_pauseData));
	//	}

	//}

	// ==============================================================================

	void PauseMenu::MenuVisibleManagement()
	{
		// 現在のポーズメニューの状態に応じて表示・非表示を切り替え
		switch (m_pauseState)
		{
		case PauseMenuState::False:
			// 全メニューを非表示
			IsVisibleAllMenuSprites(false);
			break;

		case PauseMenuState::MainMenu:
			IsVisibleAllMenuSprites(false);
			// 背景とメインメニューを表示
			m_pauseBackGroundSprite->OnClear(false);
			IsVisibleMenuSprites(m_pauseMainMenuSprites, true);
			break;

		case PauseMenuState::SettingMenu:
			IsVisibleAllMenuSprites(false);
			// 設定メニューを表示
			m_pauseBackGroundSprite->OnClear(false);
			IsVisibleMenuSprites(m_pauseSettingMenuSprites, true);
			break;

		case PauseMenuState::VolumeMenu:
			IsVisibleAllMenuSprites(false);
			// ボリュームメニューを表示
			m_pauseBackGroundSprite->OnClear(false);
			IsVisibleMenuSprites(m_pauseVolumeMenuSprites, true);
			break;

		case PauseMenuState::KeyConfigMenu:
			IsVisibleAllMenuSprites(false);
			// キーコンフィグメニューを表示
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
		// それぞれのスプライトに対して透明化処理を行う
		for (const auto& sp : spVec)
		{
			sp->OnClear(!flag);
		}
	}

}
//end basecross
