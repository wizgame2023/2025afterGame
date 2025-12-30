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
		m_pauseState(PauseMenuState::False)
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
            spInfo.pos = Vec3(-400.0f, 300.0f + (i * -100), 0.0f);
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
            spInfo.pos = Vec3(0.0f, 300.0f + (i * -100), 0.0f);
            spInfo.leftTopUV = Vec2(0.0f, SettingUVHeight * i);
            spInfo.rightBotUV = Vec2(1.0f, SettingUVHeight * (i + 1));
			PushBackPauseMenuSprite(m_pauseSettingMenuSprites, spInfo);

            spInfo.leftTopUV = Vec2(0.0f, SettingUVHeight * i + 0.5f);
            spInfo.rightBotUV = Vec2(1.0f, SettingUVHeight * (i + 1) + 0.5f);
            PushBackPauseMenuSprite(m_pauseVolumeMenuSprites, spInfo);
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

		// ポーズ中の処理
        if (m_pauseState != PauseMenuState::False)
        {
			// Lスティックの上下入力で選択肢を変更
			UpdateSelection();

			bool isAButtonDown = input->GetNowUpdateButton(L"A"); // Aボタンを押した瞬間を取る
			bool isBButtonDown = input->GetNowUpdateButton(L"B"); // Bボタンを押した瞬間を取る

            if (isAButtonDown)
            {
				SelectDecisionAButton();
            }

			if (isBButtonDown)
			{
				BackBButton();
			}

        }

		DebugLogs();
        FlushDebugLog();
	}

	// =============================================================================================
	// 関数群
	// =============================================================================================

    void PauseMenu::DebugLogs()
    {
		DebugLog(L"\n\n\nPauseState : ", to_wstring(static_cast<int>(m_pauseState)));

		if (m_pauseState == PauseMenuState::MainMenu)
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
		if (m_pauseState == PauseMenuState::SettingMenu)
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
    }

	// ==============================================================================

	void PauseMenu::SelectDecisionAButton()
	{
		bool test = false;
		if (m_pauseState == PauseMenuState::MainMenu)
		{
			switch (m_crntMainSelect)
			{
			case PauseMainMenuSelect::Resume:
				// 再開が選択された場合はポーズ解除
				m_pauseState = PauseMenuState::False;
				break;

			case PauseMainMenuSelect::Restart:
				m_pauseState = PauseMenuState::False;
				// リスタートが選択された場合はゲームステージへ遷移(現在はエラーが出ます)
				PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
				break;

			case PauseMainMenuSelect::Setting:
				// 設定が選択された場合は設定メニューへ遷移
				m_pauseState = PauseMenuState::SettingMenu;
				m_crntSettingSelect = PauseSettingMenuSelect::Volume;
				test = true;
				// メインは非表示、設定メニューは表示
				IsVisibleMenuSprites(m_pauseMainMenuSprites, false);
				IsVisibleMenuSprites(m_pauseSettingMenuSprites, true);
				break;

			case PauseMainMenuSelect::Exit:
				m_pauseState = PauseMenuState::False;
				// 終了が選択された場合はタイトルステージへ遷移
				PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToTitleStage");
				break;

			default:
				m_pauseState = PauseMenuState::False;
				break;
			}
		}
		else if (m_pauseState == PauseMenuState::SettingMenu && test == false)
		{
			switch (m_crntSettingSelect)
			{
			case PauseSettingMenuSelect::Volume:
				// ボリュームメニューへ遷移
				m_pauseState = PauseMenuState::VolumeMenu;
				IsVisibleMenuSprites(m_pauseSettingMenuSprites, false);
				IsVisibleMenuSprites(m_pauseVolumeMenuSprites, true);
				break;
			case PauseSettingMenuSelect::KeyConfig:
				// キーコンフィグメニューへ遷移
				m_pauseState = PauseMenuState::KeyConfigMenu;
				IsVisibleMenuSprites(m_pauseSettingMenuSprites, false);
				//IsVisibleMenuSprites(m_pauseKeyConfigMenuSprites, true);
				break;
			}
		}
	}

	// ==============================================================================

	void PauseMenu::BackBButton()
	{
		switch (m_pauseState)
		{
		case PauseMenuState::MainMenu:
			// メインメニューでBボタンが押された場合はポーズ解除
			m_pauseState = PauseMenuState::False;
			break;

		case PauseMenuState::SettingMenu:
			// 設定メニューでBボタンが押された場合はメインメニューへ戻る
			m_pauseState = PauseMenuState::MainMenu;
			IsVisibleMenuSprites(m_pauseSettingMenuSprites, false);
			IsVisibleMenuSprites(m_pauseVolumeMenuSprites, false);
			IsVisibleMenuSprites(m_pauseMainMenuSprites, true);
			break;

		case PauseMenuState::VolumeMenu:
			// ボリュームメニューでBボタンが押された場合は設定メニューへ戻る
			m_pauseState = PauseMenuState::SettingMenu;
			IsVisibleMenuSprites(m_pauseVolumeMenuSprites, false);
			IsVisibleMenuSprites(m_pauseSettingMenuSprites, true);
			break;

		case PauseMenuState::KeyConfigMenu:
			// キーコンフィグメニューでBボタンが押された場合は設定メニューへ戻る
			m_pauseState = PauseMenuState::SettingMenu;
			IsVisibleMenuSprites(m_pauseKeyConfigMenuSprites, false);
			IsVisibleMenuSprites(m_pauseSettingMenuSprites, true);
			break;

		default:
			m_pauseState = PauseMenuState::False;
			break;
		}
	}

	// ==============================================================================

	void PauseMenu::StartPause()
	{
		m_crntMainSelect = PauseMainMenuSelect::Resume;
		m_pauseState = PauseMenuState::MainMenu;
		IsVisibleMenuSprites(m_pauseMainMenuSprites, true);
		m_pauseBackGroundSprite->OnClear(false);
	}

	// ==============================================================================

	void PauseMenu::UpdateSelection()
	{
		auto& input = InputManager::GetInputManager();
		auto& leftStick = input->GetLStick();

        // スティックが中央付近に戻ったら、フラグをリセット（連続移動防止）
        if (abs(leftStick.y) < 0.5f)
        {
            m_selectChanged = false;
            return;
        }

        // 既に移動していたら何もしない
		if (m_selectChanged) return;

		// スティックの上下入力で選択肢を変更
        int move = 0;

		// 上下の入力検出
		if (leftStick.y < -0.8f) move = 1;
		else if (leftStick.y > 0.8f) move = -1;

        // 入力していなかったら何もしない
        if (move == 0) return;
		else if (m_pauseState == PauseMenuState::MainMenu)
        {
			// 次の選択肢を計算
			int next = static_cast<int>(m_crntMainSelect) + move;
			int max = static_cast<int>(PauseMainMenuSelect::Max) - 1;

			// 範囲外に行かないように補正
			if (next < 0) next = 0;
			else if (next >= max) next = max;

			m_crntMainSelect = static_cast<PauseMainMenuSelect>(next);
			m_selectChanged = true;
        }
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
