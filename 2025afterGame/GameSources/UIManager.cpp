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
	UIManager::UIManager() :
		m_createUI(false),
		m_initialized(false),
		m_createRankingflag(false),
		m_playerHpCurrent(0),
		m_playerHpMax(0),
		m_enemyHpCurrent(0),
		m_enemyHpMax(0),
		m_minute(0),
		m_second(0),
		m_bulletNumCurrentNow(0),
		m_bulletNumMax(0),
		m_playerScoreCurrent(0),
		m_createUIEnd(false),
		m_deleteUI(false)
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
		CreateUI();
		CreateGaugeUI();
		CreateRankingUI();
	}

	void UIManager::UIManagerCreate()
	{
		OnCreate();
	}

	// XV
	void UIManager::OnUpdate()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		auto& gameManager = GameManager::GetGameManager();
		auto limit  = gameManager->GetTimeLimit();

		GetPlayerHP();
		GetEnemies();

		UpdateTime(limit);

		GetPlayerScore();
		
		if (gameManager->GetGameEnd() && !m_createUIEnd)
		{
			m_score = stage->AddGameObject<NumberSprite>(Vec2(150.0f, 150.0f), Vec3(0.0f, 0.0f, 0.0f));
			m_score->SetMyType(NumberType::Score);
			auto finalscore = stage->AddGameObject<Sprite>(L"Finalscore", Vec2(800.0f, 200.0f), Vec3(0.0f, 200.0f, 0.0f));
			finalscore->SetDrawLayer(2);

			m_createUIEnd = true;
		}

		auto countDown = gameManager->GetGameStartCountDown();

		//if (!m_deleteUI && countDown == 5)
		//{
		//	m_deleteUI = true;

		//	if (m_deleteUI)
		//	{
		//		stage->RemoveGameObject<NumberSprite>(m_countNumber);
		//	}
		//}
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
		auto& gameMana = GameManager::GetGameManager();

		auto hp = stage->AddGameObject<HpSprite>(L"HP", Vec2(30.0f, 5.0f), Vec3(-600.0f, 375.0f, 0.0f));
				
		// 5:6
		auto bullet = stage->AddGameObject<NumberSprite>(Vec2(50.0f, 60.0f),Vec3(-550.0f, 330.0f, 0.0f));
		bullet->SetMyType(NumberType::Bullet);

		auto maxBullet = stage->AddGameObject<Sprite>(L"RemainingRounds",Vec2(150.0f, 75.0f), Vec3(-480.0f, 330.0f, 0.0f));

		auto colon = stage->AddGameObject<Sprite>(L"Colon", Vec2(20.0f, 50.0f), Vec3(500.0f, 370.0f, 0.0f));

		auto minutu = stage->AddGameObject<NumberSprite>(Vec2(50.0f, 50.0f), Vec3(450.0f, 370.0f, 0.0f));
		minutu->SetMyType(NumberType::Minute);

		auto second = stage->AddGameObject<NumberSprite>(Vec2(50.0f, 50.0f), Vec3(600.0f, 370.0f, 0.0f));
		second->SetMyType(NumberType::Second);
		second->SetDigitCount(2);

		auto crown = stage->AddGameObject<Sprite>(L"crown", Vec2(70.0f, 70.0f), Vec3(-600.0f, 205.0f, 0.0f));

		//m_countNumber = stage->AddGameObject<NumberSprite>(Vec2(50.0f, 50.0f), Vec3(0.0f, 0.0f, 0.0f));
		//m_countNumber->SetMyType(NumberType::Count);

		//if(!gameMana->GetCountEnd())
		//{
		//	//stage->RemoveGameObject<NumberSprite>(count);
		//	//App::GetApp()->GetScene<Scene>()->Get
		//	//count->RemoveComponent<NumberSprite>();
		//}

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
		int total = 2;

		for (int i = 0; i < total; i++)
		{

			stage->AddGameObject<RankingUI>(
				Vec3(-450, 200 - i * 50, 0),
				i + 1
			);
		}


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
