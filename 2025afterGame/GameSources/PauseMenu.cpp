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
		// m_pauseMainMenu[0] : 再開
		// m_pauseMainMenu[1] : リスタート
		// m_pauseMainMenu[2] : 設定
		// m_pauseMainMenu[3] : タイトル

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
			spInfo.pos = Vec3(0.0f, 260.0f + (i * -150), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, mainUVHeight * i);
			spInfo.rightBotUV = Vec2(1.0f, (mainUVHeight * (i + 1)));
			PushBackPauseMenuSprite(m_pauseMainMenuSprites, spInfo);
		}

		// m_pauseSettingMenu[0] : 音量
		// m_pauseSettingMenu[1] : キーコンフィグ

		// 設定メニュー ------
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

		// m_pauseVolumeMenu[0] : BGM文字列
		// m_pauseVolumeMenu[1] : SE文字列
		// m_pauseVolumeMenu[2] : BGMゲージ本体
		// m_pauseVolumeMenu[3] : BGMゲージスライダー
		// m_pauseVolumeMenu[4] : SEゲージ本体
		// m_pauseVolumeMenu[5] : SEゲージスライダー

		// ボリュームゲージ ------
		spInfo.textureName = L"PauseMenuVolumeGauge_TX";
		constexpr float gaugeWidth = 300.0f;
		constexpr float gaugeHeight = gaugeWidth / 3.0f;
		// 正方形にする
		constexpr float sliderWidth = gaugeHeight; 
		constexpr float sliderHeight = sliderWidth;
		for (int i = 0; i < 2; i++)
		{
			// ボリューム文字列の位置を取得
			Vec3 volumeStringPos = m_pauseVolumeMenuSprites[i]->GetPosition();

			// ゲージ本体
			spInfo.pos = Vec3(0.0f, (volumeStringPos.y - 100), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, 0.0f);
			spInfo.rightBotUV = Vec2(0.75f, 1.0f);
			spInfo.size = Vec2(gaugeWidth, 100.0f);
			PushBackPauseMenuSprite(m_pauseVolumeMenuSprites, spInfo);

			// ゲージの右端の位置を取得(backで直前に追加されたデータを見る)
			auto& gaugeSp = m_pauseVolumeMenuSprites.back();
			auto rightEdgePos = gaugeSp->GetPositionX() + gaugeSp->GetSpritePixel().x * 0.49f;

			// スライダー部分
			spInfo.pos = Vec3(rightEdgePos, (volumeStringPos.y - 100), 0.0f);
			spInfo.leftTopUV = Vec2(0.75f, 0.0f);
			spInfo.rightBotUV = Vec2(1.0f, 1.0f);
			spInfo.size = Vec2(100.0f, 100.0f);
			PushBackPauseMenuSprite(m_pauseVolumeMenuSprites, spInfo);
		}

		// m_pauseKeyConfigMenu[0] : 上下反転文字列
		// m_pauseKeyConfigMenu[1] : 加速文字列
		// m_pauseKeyConfigMenu[2] : 弾発射文字列
		// m_pauseKeyConfigMenu[3] : 背面視点文字列

		// キーコンフィグメニュー ------
		spInfo.textureName = L"PauseMenuKeyConfig_TX";
		spInfo.size = Vec2(200.0f, 100.0f);
		constexpr float KeyConfigUVHeight = 1.0f / 4.0f;

		for (int i = 0; i < 4; i++)
		{
			spInfo.pos = Vec3(0.0f, 260.0f + (i * -150), 0.0f);
			spInfo.leftTopUV = Vec2(0.0f, mainUVHeight * i);
			spInfo.rightBotUV = Vec2(1.0f, (mainUVHeight * (i + 1)));
			PushBackPauseMenuSprite(m_pauseKeyConfigMenuSprites, spInfo);
		}

		// ボタン群 ------
		spInfo.textureName = L"Buttons_TX";
		spInfo.size = Vec2(80.0f, 80.0f);
		constexpr float buttonsUV = 1.0f / 4.0f;

		for (int i = 0; i < static_cast<int>(ButtonsType::Max); i++)
		{
			int rows = i / 4;	// 行 : 0, 1, 2, 3, 0, 1, 2, 3 ...
			int cols = i % 4;	// 列 : 0, 0, 0, 0, 1, 1, 1, 1 ...


			spInfo.leftTopUV = Vec2(buttonsUV * cols, buttonsUV * rows);
			spInfo.rightBotUV = Vec2(buttonsUV * (cols + 1), buttonsUV * (rows + 1));
			spInfo.pos = Vec3(0.0f, 0.0f, 0.0f); // 仮置き
			
			// arrayに格納
			auto buttonSp = m_stage->AddGameObject<Sprite>(
				spInfo.textureName,
				spInfo.size,
				spInfo.pos,
				spInfo.layer
			);
			buttonSp->SetUVRect(spInfo.leftTopUV, spInfo.rightBotUV);
			buttonSp->OnClear(true);
			
			m_pauseButtonsSprites[i] = buttonSp;
		}

		m_pauseState = PauseMenuState::False;


		// ボタンの種類マップの初期化
		InitButtonTypeMap();

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
		//bool testX = input->GetNowUpdateButton(L"X"); // デバッグ用
		//if (testX)
		//{
		//	m_pauseBackGroundSprite->RemoveSprite();
		//}

		// ポーズ開始の処理
		if (isStartButtonDown && m_pauseState == PauseMenuState::False)
		{
			StartPause();
		}

		MenuVisibleManagement();

		// 非ポーズ中は全メニュー非表示
		if (m_pauseState == PauseMenuState::False)
		{
			return;
		}

		UpdatePauseMenu();


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
		
		auto& game = GameManager::CreateGameManager();
		float BGMVolume = game->GetBGMVolume();
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
		HandleMenuSelection(m_pauseMainMenuSprites, m_crntMainSelect, PauseMainMenuSelect::Max);

		bool pressAButton = input.GetDownButton(L"A");
		bool pressBButton = input.GetDownButton(L"B");

		if (pressAButton)
		{
			switch (m_crntMainSelect)
			{
			case PauseMainMenuSelect::Resume:
				// 再開が選択された場合はポーズ解除
				m_pauseState = PauseMenuState::False;
				SetPauseFlag(false);
				break;

			case PauseMainMenuSelect::Restart:
				m_pauseState = PauseMenuState::False;
				SetPauseFlag(false);
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
				SetPauseFlag(false);
				// 終了が選択された場合はタイトルステージへ遷移
				PostEvent(0.0f,
					GetThis<ObjectInterface>(),
					App::GetApp()->GetScene<Scene>(),
					L"ToTitleStage");
				break;

			default:
				// 例外が発生した場合はポーズ解除
				m_pauseState = PauseMenuState::False;
				SetPauseFlag(false);
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
		HandleMenuSelection(m_pauseSettingMenuSprites, m_crntSettingSelect, PauseSettingMenuSelect::Max);

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
		HandleMenuSelection(m_pauseVolumeMenuSprites, m_crntVolumeSelect, PauseVolumeMenuSelect::Max);

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
		auto& game = GameManager::CreateGameManager();
		float crntVol = 0.0f;
		function<void(float)> volSetter;	// 未定義

		// ゲージとスライダーの取得
		shared_ptr<Sprite> gauge;
		shared_ptr<Sprite> slider;

		// volSetterの定義
		if (m_pauseState == PauseMenuState::BGMSetting)
		{
			crntVol = game->GetBGMVolume();
			gauge = m_pauseVolumeMenuSprites[2];
			slider = m_pauseVolumeMenuSprites[3];
			volSetter = [&](float v) { // ここで関数を定義
				float width = gauge->GetSpritePixel().x;
				float leftEdge = gauge->GetPositionX() - (width * 0.48f);
				float rightEdge = gauge->GetPositionX() + (width * 0.48f);
				float clampedSliderX = clamp(leftEdge + (v * width), leftEdge, rightEdge);
				slider->SetPositionX(clampedSliderX);
				return game->SetBGMVolume(v); 
			};	
		}
		else if (m_pauseState == PauseMenuState::SESetting)
		{
			crntVol = game->GetSEVolume();
			gauge = m_pauseVolumeMenuSprites[4];
			slider = m_pauseVolumeMenuSprites[5];
			volSetter = [&](float v) { 	// ここで関数を定義
				float width = gauge->GetSpritePixel().x;
				float leftEdge = gauge->GetPositionX() - (width * 0.48f);
				float rightEdge = gauge->GetPositionX() + (width * 0.48f);
				float clampedSliderX = clamp(leftEdge + (v * width), leftEdge, rightEdge);
				slider->SetPositionX(clampedSliderX);
				return game->SetSEVolume(v);
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
		HandleMenuSelection(m_pauseKeyConfigMenuSprites, m_crntKeyConfigSelect, PauseKeyConfigMenuSelect::Max);

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
		auto& game = GameManager::CreateGameManager();

		// keySetterの定義
		switch (m_pauseState)
		{
		case PauseMenuState::AccelSetting:
			keySetter = [&](const wstring& k) { 
				m_pauseData.AccelKey = k;
				game->SetAccelKey(k);
				auto& accelStringPos = m_pauseKeyConfigMenuSprites[1]->GetPosition();
				SetShowAndPosButtons(k, accelStringPos + Vec3(200.0f, 0.0f, 0.0f));
			};
			break;
		case PauseMenuState::BulletSetting:
			keySetter = [&](const wstring& k) {
				m_pauseData.BulletKey = k;
				game->SetBulletKey(k);
			};
			break;
		case PauseMenuState::ViewBehindSetting:
			keySetter = [&](const wstring& k) {
				m_pauseData.ViewBehindKey = k;
				game->SetViewBehindKey(k);
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
		static PauseMenuState lastState = PauseMenuState::Max;

		// 前フレームとステートが違っていたら処理を行う
		if (m_pauseState != lastState)
		{
			// 背景は非ポーズ状態以外常に表示
			m_pauseBackGroundSprite->OnClear(m_pauseState == PauseMenuState::False);

			// 各グループについて、現在のステートと一致するときだけ true にする
			{
				// ポーズがMainMenu状態のときだけメインメニューを表示
				IsVisibleMenuSprites(m_pauseMainMenuSprites,		(m_pauseState == PauseMenuState::MainMenu));

				IsVisibleMenuSprites(m_pauseSettingMenuSprites,		(m_pauseState == PauseMenuState::SettingMenu));

				IsVisibleMenuSprites(m_pauseVolumeMenuSprites,		(m_pauseState == PauseMenuState::VolumeMenu ||
																	 m_pauseState == PauseMenuState::BGMSetting ||
																	 m_pauseState == PauseMenuState::SESetting));

				IsVisibleMenuSprites(m_pauseKeyConfigMenuSprites,	(m_pauseState == PauseMenuState::KeyConfigMenu ||
																	 m_pauseState == PauseMenuState::AccelSetting ||
																	 m_pauseState == PauseMenuState::BulletSetting ||
																	 m_pauseState == PauseMenuState::ViewBehindSetting));


			}

			// 大きさの初期化処理
			switch (m_pauseState)
			{
			case PauseMenuState::MainMenu:
				ScalingSelectedSprite(m_pauseMainMenuSprites, m_crntMainSelect, PauseMainMenuSelect::Max);
				break;
			case PauseMenuState::SettingMenu:
				ScalingSelectedSprite(m_pauseSettingMenuSprites, m_crntSettingSelect, PauseSettingMenuSelect::Max);
				break;
			case PauseMenuState::VolumeMenu:
				ScalingSelectedSprite(m_pauseVolumeMenuSprites, m_crntVolumeSelect, PauseVolumeMenuSelect::Max);
				break;
			case PauseMenuState::KeyConfigMenu:
				ScalingSelectedSprite(m_pauseKeyConfigMenuSprites, m_crntKeyConfigSelect, PauseKeyConfigMenuSelect::Max);
				break;
			}
		}

		lastState = m_pauseState;
	}

	// ==============================================================================

	void PauseMenu::StartPause()
	{
		SetPauseFlag(true);
		m_crntMainSelect = PauseMainMenuSelect::Resume;
		m_pauseState = PauseMenuState::MainMenu;
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

		// array 用の処理
		//for (auto& sp : m_pauseButtonsSprites) {
		//	if (sp) sp->OnClear(!flag);
		//}
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

	// ==============================================================================

	void PauseMenu::InitButtonTypeMap()
	{
		m_buttonTypeMap[L"A"]			= ButtonsType::A;
		m_buttonTypeMap[L"B"]			= ButtonsType::B;
		m_buttonTypeMap[L"X"]			= ButtonsType::X;
		m_buttonTypeMap[L"Y"]			= ButtonsType::Y;
		m_buttonTypeMap[L"LB"]			= ButtonsType::LB;
		m_buttonTypeMap[L"RB"]			= ButtonsType::RB;
		m_buttonTypeMap[L"LTrigger"]	= ButtonsType::LT;
		m_buttonTypeMap[L"RTrigger"]	= ButtonsType::RT;
		m_buttonTypeMap[L"Back"]		= ButtonsType::Back;
		m_buttonTypeMap[L"Start"]		= ButtonsType::Start;
		m_buttonTypeMap[L"LStick"]		= ButtonsType::LS;
		m_buttonTypeMap[L"RStick"]		= ButtonsType::RS;
		m_buttonTypeMap[L"DUp"]			= ButtonsType::Up;
		m_buttonTypeMap[L"DRight"]		= ButtonsType::Right;
		m_buttonTypeMap[L"DDown"]		= ButtonsType::Down;
		m_buttonTypeMap[L"DLeft"]		= ButtonsType::Left;
	}

	// ==============================================================================

	void PauseMenu::SetShowAndPosButtons(const wstring& buttonsName, const Vec3& setPos)
	{
		// ボタン種類を取得
		auto it = m_buttonTypeMap.find(buttonsName);

		// 見つかったら表示位置を設定して表示
		if (it != m_buttonTypeMap.end())
		{
			ButtonsType type = it->second;
			int index = static_cast<int>(type);
			if (index >= 0 && index < m_pauseButtonsSprites.size())
			{
				m_pauseButtonsSprites[index]->SetPosition(setPos);
				m_pauseButtonsSprites[index]->OnClear(false); // 表示
			}
		}
	}
}
//end basecross
