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
		m_playerHpCurrent(0),
		m_playerHpMax(0),
		m_enemyHpCurrent(0),
		m_enemyHpMax(0),
		m_minute(0),
		m_second(0),
		m_bulletNumCurrentNow(0),
		m_bulletNumMax(0)
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
		if (dynamic_pointer_cast<TomokiStage>(stage) == nullptr) return;

		GetPlayerHP();
		GetEnemyHP();

		if (m_createUI == false)
		{	
			CreateUI();
		}

		UpdateTime(limit);
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
				
		auto bullet = stage->AddGameObject<NumberSprite>(L"Number",Vec2(40.0f, 40.0f),Vec3(-550.0f, 330.0f, 0.0f));
		bullet->SetMyType(NumberType::Bullet);

		auto maxBullet = stage->AddGameObject<NumberSprite>(L"Number",Vec2(40.0f,40.0f),Vec3(-430.0f, 330.0f, 0.0f));
		maxBullet->SetMyType(NumberType::MaxBullet);
		
		auto minuteTimer = stage->AddGameObject<NumberSprite>(L"Number",Vec2(50.0f,50.0f),Vec3(470.0f, 375.0f, 0.0f));
		minuteTimer->SetMyType(NumberType::minute);
		
		auto secondTimer = stage->AddGameObject<NumberSprite>(L"Number",Vec2(50.0f,50.0f),Vec3(600.0f, 375.0f, 0.0f));
		secondTimer->SetMyType(NumberType::second);
		secondTimer->SetDigitCount(2);

		auto colon = stage->AddGameObject<Sprite>(L"Colon", Vec2(16.0f, 43.0f));
		colon->SetPosition(Vec3(515.0f, 375.0f, 0.0f));

		auto enemyBillBoad = stage->AddGameObject<BillBoardGauge>(m_enemy,L"HP", 3, 2.0f, 2.0f, Vec3(2.0f, 0.5f, 5.0f));
		enemyBillBoad->SetPercent(1.0f);

		m_createUI = true;
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
			}
		}
	}

	void UIManager::GetEnemyHP()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto activeStage = scene->GetActiveStage();
		auto objets = activeStage->GetGameObjectVec();

		for (auto obj : objets)
		{
			auto enemy = dynamic_pointer_cast<Enemy>(obj);
			
			if (enemy)
			{
				m_enemy = enemy;
				m_enemyHpCurrent = enemy->GetHpCurrent();
				m_enemyHpMax = enemy->GetHpMax();
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
}
//end basecross
