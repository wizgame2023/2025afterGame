/*!
@file PauseMenu.cpp
@brief PauseMenu実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	PauseMenu::PauseMenu(const shared_ptr<Stage>& stage) :
		MyGameObject(stage),
		m_crntSelect(PauseMenuState::Resume)
	{
	}

    void PauseMenu::OnCreate()
    {
        m_stage = GetStage();
        SpriteInfo spInfo; // 共通設定用のベース
        spInfo.layer = 10; // 全メニュー共通のレイヤー

        // --- 1. メインメニュー ---
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

        // --- 2. ボリュームメニュー ---
        spInfo.textureName = L"PauseMenuVolume_TX";
        spInfo.size = Vec2(200.0f, 100.0f);
        constexpr float volumeUVHeight = 1.0f / 3.0f;

        for (int i = 0; i < 3; i++)
        {
            spInfo.pos = Vec3(0.0f, 300.0f + (i * -100), 0.0f);
            spInfo.leftTopUV = Vec2(0.0f, volumeUVHeight * i);
            spInfo.rightBotUV = Vec2(1.0f, volumeUVHeight * (i + 1));
            PushBackPauseMenuSprite(m_pauseVolumeMenuSprites, spInfo);
        }

        // --- 3. ボタン群 ---
        spInfo.textureName = L"Buttons_TX";
        spInfo.size = Vec2(80.0f, 80.0f); // ボタンは少し小さくする、などの変更が楽！
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

		IsVisibleMainMenuSprites(m_pauseMainMenuSprites, false);
    }
	void PauseMenu::OnUpdate()
	{

	}

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

	void PauseMenu::IsVisibleAllMenuSprites(const bool flag)
	{
		IsVisibleMainMenuSprites(m_pauseMainMenuSprites,flag);
		IsVisibleMainMenuSprites(m_pauseVolumeMenuSprites, flag);
		IsVisibleMainMenuSprites(m_pauseButtonsSprites, flag);
	}

    void PauseMenu::IsVisibleMainMenuSprites(const vector<shared_ptr<Sprite>>& spVec, const bool flag)
    {
        for (const auto& sp : spVec)
        {
            sp->OnClear(!flag);
        }
    }

}
//end basecross
