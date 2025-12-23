/*!
@file PauseMenu.cpp
@brief PauseMenuŽÀ‘Ì
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

		for (int i = 0; i < 4; i++)
		{
			PushBackPauseMenuSprite(L"PauseMenuMain_TX", m_pauseMainMenuSprites, Vec2(0.0f, 0.25f * i), Vec2(1.0f, (0.25f * i) + 0.25f));

			m_pauseMainMenuSprites[i]->GetComponent<Transform>()->SetPosition(Vec3(0.0f, 300.0f + (i * -100), 0.0f));
		}
	}

	void PauseMenu::OnUpdate()
	{

	}

	void PauseMenu::PushBackPauseMenuSprite(const wstring& textureName, vector<shared_ptr<Sprite>>& vecSprite, const Vec2& leftTopUV, const Vec2& rightBotUV)
	{
		// ‹¤’ÊƒŒƒCƒ„[
		constexpr int layer = 10;

		auto sprite = m_stage->AddGameObject<Sprite>(
			textureName,
			Vec2(80.0f, 100.0f),
			Vec3(0.0f),
			layer
		);

		sprite->SetUVRect(leftTopUV, rightBotUV);

		vecSprite.push_back(sprite);
	}
}
//end basecross
