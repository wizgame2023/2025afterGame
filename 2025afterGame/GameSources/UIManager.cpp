/*!
@file UIManager.cpp
@brief UIŠÇ—‚ÌÀ‘Ì
’S“–F‹g“c ’q‹M
*/

#include "stdafx.h"
#include "Project.h"
#include "UIManager.h"

namespace basecross
{
	UIManager::UIManager():
		m_createUI(false),
		m_initialized(false),
		m_playerHpCurrent(0),
		m_playerHpMax(0),
		m_enemyHpCurrent(0),
		m_enemyHpMax(0),
		m_minute(0),
		m_second(0),
		m_bulletNumCurrentNow(0),
		m_bulletNumMax(0),
		m_playerScoreCurrent(0)
	{

	}

	UIManager::~UIManager()
	{

	}

	unique_ptr<UIManager, UIManager::UIManagerDeleter> UIManager::m_UIManager;

	// ƒVƒ“ƒOƒ‹ƒgƒ“‚É‚æ‚é¶¬
	unique_ptr<UIManager, UIManager::UIManagerDeleter>& UIManager::CreateUIManager()
	{
		try
		{
			if (m_UIManager.get() == 0)
			{
				// ©•ª‚ğì¬
				m_UIManager.reset(new UIManager());

				// ‰Šú‰»
				m_UIManager->OnCreate();
			}
			return m_UIManager;
		}
		catch (...)
		{
			throw;
		}

		return m_UIManager;
	}

	// ©•ª‚ğ“n‚·
	unique_ptr<UIManager, UIManager::UIManagerDeleter>& UIManager::GetUIManager()
	{
		return m_UIManager;
	}


	// ‰Šú‰»ˆ—
	void UIManager::OnCreate()
	{
	}

	// XV
	void UIManager::OnUpdate()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		auto& gameManager = GameManager::GetGameManager();
		auto limit  = gameManager->GetTimeLimit();

		// OnCreate‚¾‚ÆScene‚æ‚è‘¬‚¢‚Ì‚ÅƒGƒ‰[‚ªã‚Ì•û‚¾‚Æo‚é
		if (dynamic_pointer_cast<TitleStage>(stage) != nullptr || dynamic_pointer_cast<SelectStage>(stage) != nullptr) return;

		GetPlayerHP();
		GetEnemies();

		if (m_createUI == false)
		{	
			CreateUI();
		}

		if (!m_initialized)
		{
			CreateGaugeUI();
			m_initialized = true;
		}

		// CreateRankingUI();

		UpdateTime(limit);

		GetPlayerScore();
	}

	// ©•ª©g‚Ì”jŠüˆ—
	void UIManager::DeleteUIManager()
	{
		// ©•ª©g‚Ì”jŠü
		m_UIManager.reset();
	}

	void UIManager::CreateUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		auto hp = stage->AddGameObject<HpSprite>(L"HP", Vec2(30.0f, 5.0f), Vec3(-600.0f, 375.0f, 0.0f));
				
		auto bullet = stage->AddGameObject<NumberSprite>(Vec2(40.0f, 40.0f),Vec3(-550.0f, 330.0f, 0.0f));
		bullet->SetMyType(NumberType::Bullet);

		auto maxBullet = stage->AddGameObject<Sprite>(L"RemainingRounds",Vec2(200.0f, 60.0f), Vec3(-480.0f, 330.0f, 0.0f));
		
		auto score = stage->AddGameObject<NumberSprite>(Vec2(50.0f,50.0f),Vec3(600.0f, 370.0f, 0.0f));
		score->SetMyType(NumberType::Score);

		auto pauseMenu = stage->AddGameObject<PauseMenu>();

		m_createUI = true;
	}

	void UIManager::CreateGaugeUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		for (int i = 0; i < m_enemies.size(); i++)
		{
			auto enemyBillBoard = stage->AddGameObject<BillBoardGauge>(m_enemies[i], L"HP", 3, 2.0f, 1.5f, Vec3(2.0f, 0.2f, 5.0f),Col4(1.0f),i);
			m_enemyGauges.push_back(enemyBillBoard);
		}
	}

	void UIManager::CreateRankingUI()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		const int playerCount = 1;

		// auto rankingUI = stage->AddGameObject<RankingUI>(Vec2(50.0f, 50.0f), Vec3(0.0f, 100.0f, 0.0f), m_enemies.size() + playerCount);

		wstringstream wss;
		wss << m_enemies.size() + playerCount;

		scene->SetDebugString(wss.str());
	}

	void UIManager::GetPlayerHP()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto activeStage = scene->GetActiveStage();
		auto objets = activeStage->GetGameObjectVec();

		for (auto obj : objets)
		{
			auto player = dynamic_pointer_cast<Player>(obj);
		
			if (player)
			{
				m_playerHpCurrent = player->GetHpCurrent();
				m_playerHpMax = player->GetHpMax();
				m_bulletNumCurrentNow = player->GetBulletNumCurrentNow();
				m_bulletNumMax = player->GetBulletNumMax();
				m_playerScoreCurrent = player->GetScoreCurrent();
			}
		}
	}
	
	void UIManager::GetEnemies()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto activeStage = scene->GetActiveStage();
		auto objets = activeStage->GetGameObjectVec();

		m_enemies.clear();
		m_enemyHpCurrent.clear();
		m_enemyHpMax.clear();

		for (auto obj : objets)
		{
			auto enemy = dynamic_pointer_cast<Enemy>(obj);
			
			if (enemy)
			{
				m_enemies.push_back(enemy);
				m_enemyHpCurrent.push_back(enemy->GetHpCurrent());
				m_enemyHpMax.push_back(enemy->GetHpMax());
			}
		}
	}

	int UIManager::GetCurrentPlayerHP()
	{
		return m_playerHpCurrent;
	}

	int UIManager::GetMaxPlayerHP()
	{
		return m_playerHpMax;
	}

	vector<int> UIManager::GetCurrentEnemyHP()
	{
		return m_enemyHpCurrent;
	}

	vector<int> UIManager::GetMaxEnemyHP()
	{
		return m_enemyHpMax;
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

	int UIManager::GetBulletNumCurrentNow()
	{
		return m_bulletNumCurrentNow;
	}

	int UIManager::GetBulletNumMax()
	{
		return m_bulletNumMax;
	}

	int UIManager::GetPlayerScore()
	{
		return m_playerScoreCurrent;
	}
}
//end basecross
