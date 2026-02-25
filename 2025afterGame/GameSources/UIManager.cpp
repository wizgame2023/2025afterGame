/*!
@file UIManager.cpp
@brief UI管理の実体
担当：吉田 智貴
*/

#include "stdafx.h"
#include "Project.h"
#include "UIManager.h"

namespace basecross
{
	UIManager::UIManager() :
		m_createUI(false),
		m_minute(0),
		m_second(0),
		m_createUIEnd(false),
		m_enemyCount(4)
	{

	}

	UIManager::~UIManager()
	{

	}

	unique_ptr<UIManager, UIManager::UIManagerDeleter> UIManager::m_UIManager;

	// シングルトンによる生成
	unique_ptr<UIManager, UIManager::UIManagerDeleter>& UIManager::CreateUIManager()
	{
		try
		{
			if (m_UIManager.get() == 0)
			{
				// 自分を作成
				m_UIManager.reset(new UIManager());
			}
			return m_UIManager;
		}
		catch (...)
		{
			throw;
		}

		return m_UIManager;
	}

	// 自分を渡す
	unique_ptr<UIManager, UIManager::UIManagerDeleter>& UIManager::GetUIManager()
	{
		return m_UIManager;
	}

	// 初期化処理
	void UIManager::OnCreate()
	{
		CreateUI();
		CreateRankingUI();
	}

	// 更新
	void UIManager::OnUpdate()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		auto& gameManager = GameManager::GetGameManager();
		auto elapsed = gameManager->GetDeltaTime();
		auto limit  = gameManager->GetTimeLimit();
		auto countDown = gameManager->GetGameStartCountDown();
		auto player = stage->GetSharedGameObject<Player>(L"Player");
		UpdateTime(limit);

		if (gameManager->GetResultDrawActive())
		{
			CreateResult();
		}

		// カウントダウンが0になったら消える
		if (countDown == 5)
		{
			m_countNumber->OnDestory();
		}
	}

	// 自分自身の破棄処理
	void UIManager::DeleteUIManager()
	{
		// 自分自身の破棄
		m_UIManager.reset();
	}

	void UIManager::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		auto& gameMana = GameManager::GetGameManager();

		auto hpwaku = stage->AddGameObject<HpSprite>(L"HP_WAKU", Vec2(30.0f, 5.8f), Vec3(-600.0f, 375.0f, 0.0f));
		auto hp = stage->AddGameObject<HpSprite>(L"HP", Vec2(30.0f, 5.0f), Vec3(-600.0f, 375.0f, 0.0f));
		hp->SetSpriteMove(true);

		auto Ballet = stage->AddGameObject<HpSprite>(L"Ballet",Vec2(15,10),Vec3(-625.0f, 320.0f, 0.0f));

		// スプライト
		CreateSprite();
		// ナンバースプライト
		CreateNumberSprite();

		auto pauseMenu = stage->AddGameObject<PauseMenu>();
	}

	void UIManager::CreateSprite()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		// コロンの作成
		m_clon = stage->AddGameObject<Sprite>(L"Number", Vec2(50.0f, 100.0f), Vec3(500.0f, 350.0f, 0.0f));
		m_clon->SetDigit(11);

		// バックスラッシュの作成
		auto backslash = stage->AddGameObject<Sprite>(L"Number", Vec2(40.0f, 80.0f), Vec3(-400.0f, 320.0f, 0.0f));
		backslash->SetDigit(10);

		// 自分のスコアの後ろに置く背景
		auto backGraund = stage->AddGameObject<Sprite>(L"PauseMenuBackGround_TX", Vec2(310.0f, 80.0f), Vec3(-445.0f, 200.0f, 0.0f));
		backGraund->SetColor(Col4(1.0f, 1.0f, 1.0f, 0.5f));
		backGraund->SetDrawLayer(-1);
	}

	void UIManager::CreateNumberSprite()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		int plusNumber = 12;

		// カウントダウン
		m_countNumber = stage->AddGameObject<NumberSprite>(Vec2(50.0f, 100.0f), Vec3(0.0f, 0.0f, 0.0f));
		m_countNumber->SetDigitCount(1);
		m_countNumber->SetMyType(NumberType::Count);

		// 制限時間(分)
		auto minutu = stage->AddGameObject<NumberSprite>(Vec2(50.0f, 100.0f), Vec3(445.0f, 350.0f, 0.0f));
		minutu->SetMyType(NumberType::Minute);

		// 制限時間(秒)
		m_secondUI = stage->AddGameObject<NumberSprite>(Vec2(50.0f, 100.0f), Vec3(600.0f, 350.0f, 0.0f));
		m_secondUI->SetMyType(NumberType::Second);
		m_secondUI->SetDigitCount(2);

		// 弾UI作成
		auto bullet = stage->AddGameObject<NumberSprite>(Vec2(40.0f, 80.0f), Vec3(-450.0f, 320.0f, 0.0f));
		bullet->SetMyType(NumberType::Bullet);	
		bullet->SetDigitCount(2);

		// 弾の最大数の作成
		auto maxBullet = stage->AddGameObject<NumberSprite>(Vec2(40.0f, 80.0f), Vec3(-320.0f, 320.0f, 0.0f));
		maxBullet->SetMyType(NumberType::MaxBullet);
	}

	void UIManager::CreateResult()
	{
		auto& gameManager = GameManager::GetGameManager();
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		// リザルト生成
		if (gameManager->GetGameEnd() && !m_createUIEnd)
		{
			auto backGraund = stage->AddGameObject<Sprite>(L"PauseMenuBackGround_TX", Vec2(600.0f, 750.0f), Vec3(0.0f, 0.0f, 0.0f));
			backGraund->SetDrawLayer(1);
			backGraund->SetColor(Col4(1.0f, 1.0f, 1.0f, 1.0f));

			auto finalscore = stage->AddGameObject<Sprite>(L"Finalscore", Vec2(420.0f, 140.0f), Vec3(0.0f, 250.0f, 0.0f));
			finalscore->SetDrawLayer(4);

			auto pushA = stage->AddGameObject<Sprite>(L"Start_TX", Vec2(309.0f, 96.0f), Vec3(0.0f, -260.0f, 0.0f));

			bool playerOnly = false;
			bool rankingDraw = true;

			for (int i = 0; i < m_enemyCount + 1; i++)
			{
				auto obj = stage->AddGameObject<RankingUI>(
					Vec3(-50, 100 - i * 60, 0),
					i + 1,
					playerOnly,
					rankingDraw
				);

				obj->SetLayer(2);
			}

			m_createUIEnd = true;
		}
	}

	void UIManager::CreateRankingUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		bool playerOnlyRanking = true;
		bool rankingDraw = true;
		int playerCount = 1;

		for (int i = 0; i < playerCount; i++)
		{
			auto obj = stage->AddGameObject<RankingUI>(
				Vec3(-500, 200 - i * 50, 0),
				i + 1,
				playerOnlyRanking,
				rankingDraw
			);
		}

		for (int i = 0; i < m_enemyCount + playerCount; i++)
		{
			auto obj = stage->AddGameObject<RankingUI>(
				Vec3(500, 200 - i * 50, 0),
				i + 1,
				!playerOnlyRanking,
				!rankingDraw
			);

			obj->SetScoreUIDraw(false);
		}
	}

	void UIManager::UpdateTime(int limit)
	{
		m_minute = limit / 60;
		m_second = limit % 60;
	}

	int UIManager::GetMinuteTimer()
	{
		return m_minute;
	}

	int UIManager::GetSecondTimer()
	{
		return m_second;
	}

	void UIManager::SetCreateUIFlag(bool flag)
	{
		m_createUIEnd = flag;
	}
}
//end basecross
