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
		InitButtonTypeMap();
		CreateUI();
		CreateRankingUI();
	}

	// 更新
	void UIManager::OnUpdate()
	{
		auto& gameManager = GameManager::GetGameManager();

		// リスタート検知
		static bool lastGameEnd = false;
		bool nowGameEnd = gameManager->GetGameEnd();

		// リスタート
		if (lastGameEnd && !nowGameEnd)
		{
			ForceRefreshOperationUI();
		}
		lastGameEnd = nowGameEnd;

		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		auto limit = gameManager->GetTimeLimit();
		auto countDown = gameManager->GetGameStartCountDown();

		UpdateTime(limit);

		if (gameManager->GetResultDrawActive())
		{
			CreateResult();
		}

		if (countDown == 5)
		{
			m_countNumber->OnDestory();
		}

		UpdateOperationUI();
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

		// スプライトの
		SpriteUV();
		CreateKeyConfigSprite();
	}

	void UIManager::CreateSprite()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		// コロンの作成
		m_clon = stage->AddGameObject<Sprite>(L"Number", Vec2(50.0f, 100.0f), Vec3(500.0f, 350.0f, 0.0f));
		m_clon->SetDigit(11);

		// +の作成
		auto plus = stage->AddGameObject<Sprite>(L"Number", Vec2(50.0f, 100.0f), Vec3(380.0f, -320.0f, 0.0f));
		plus->SetDigit(12);

		// 上昇の作成
		auto x = 240.0f * 0.8;
		auto y = 200.0f * 0.8;

		auto up = stage->AddGameObject<Sprite>(L"UP", Vec2(x, y), Vec3(550, -330.0f, 0));

		// バックスラッシュの作成
		auto backslash = stage->AddGameObject<Sprite>(L"Number", Vec2(40.0f, 80.0f), Vec3(-410.0f, 320.0f, 0.0f));
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
		auto bullet = stage->AddGameObject<NumberSprite>(Vec2(40.0f, 80.0f), Vec3(-460.0f, 320.0f, 0.0f));
		bullet->SetMyType(NumberType::Bullet);	
		bullet->SetDigitCount(2);

		// 弾の最大数の作成
		auto maxBullet = stage->AddGameObject<NumberSprite>(Vec2(40.0f, 80.0f), Vec3(-330.0f, 320.0f, 0.0f));
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

		//for (int i = 0; i < m_enemyCount + playerCount; i++)
		//{
		//	auto obj = stage->AddGameObject<RankingUI>(
		//		Vec3(500, 200 - i * 50, 0),
		//		i + 1,
		//		!playerOnlyRanking,
		//		!rankingDraw
		//	);

		//	obj->SetScoreUIDraw(false);
		//}
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

	void UIManager::SpriteUV()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		constexpr float buttonsUV = 1.0f / 4.0f;
		Vec2 size(60.0f, 60.0f);

		for (int i = 0; i < ButtonsTypeCount; i++)
		{
			int rows = i / 4;
			int cols = i % 4;

			Vec2 lt(buttonsUV * cols, buttonsUV * rows);
			Vec2 rb(buttonsUV * (cols + 1), buttonsUV * (rows + 1));

			auto sp = stage->AddGameObject<Sprite>(
				L"Buttons_TX",
				size,
				Vec3(0, 0, 0),
				1
			);

			sp->SetUVRect(lt, rb);
			sp->OnClear(true);

			m_buttonSprites[i] = sp;
		}
	}

	void UIManager::Show(const wstring& buttonName, const Vec3& pos)
	{
		auto it = m_buttonTypeMap.find(buttonName);
		if (it == m_buttonTypeMap.end()) return;

		int index = static_cast<int>(it->second);
		m_buttonSprites[index]->SetPosition(pos);
		m_buttonSprites[index]->OnClear(false);
	}

	void UIManager::Hide(const wstring& buttonName)
	{
		auto it = m_buttonTypeMap.find(buttonName);
		if (it == m_buttonTypeMap.end()) return;

		int index = static_cast<int>(it->second);
		m_buttonSprites[index]->OnClear(true);
	}

	void UIManager::HideAll()
	{
		for (auto& sp : m_buttonSprites)
		{
			if (sp) sp->OnClear(true);
		}
	}

	void UIManager::InitButtonTypeMap()
	{
		m_buttonTypeMap[L"A"] = ButtonsType::A;
		m_buttonTypeMap[L"B"] = ButtonsType::B;
		m_buttonTypeMap[L"X"] = ButtonsType::X;
		m_buttonTypeMap[L"Y"] = ButtonsType::Y;
		m_buttonTypeMap[L"L"] = ButtonsType::LB;
		m_buttonTypeMap[L"R"] = ButtonsType::RB;
		m_buttonTypeMap[L"LTrigger"] = ButtonsType::LT;
		m_buttonTypeMap[L"RTrigger"] = ButtonsType::RT;
		m_buttonTypeMap[L"Back"] = ButtonsType::Back;
		m_buttonTypeMap[L"Start"] = ButtonsType::Start;
		m_buttonTypeMap[L"LStick"] = ButtonsType::LS;
		m_buttonTypeMap[L"RStick"] = ButtonsType::RS;
		m_buttonTypeMap[L"DUp"] = ButtonsType::Up;
		m_buttonTypeMap[L"DRight"] = ButtonsType::Right;
		m_buttonTypeMap[L"DDown"] = ButtonsType::Down;
		m_buttonTypeMap[L"DLeft"] = ButtonsType::Left;
	}

	void UIManager::UpdateOperationUI()
	{
		auto& game = GameManager::GetGameManager();

		auto accel = game->GetAccelKey();
		auto bullet = game->GetBulletKey();
		auto view = game->GetViewBehindKey();

		auto accelPos = Vec3(450.0f, -320.0f, 0.0f);
		auto bulletPos = Vec3(50.0f, -320.0f, 0.0f);
		// auto viewPos = Vec3(480.0f, -320.0f, 0.0f);
		auto stickPos = Vec3(250.0f, -320.0f, 0.0f);
		auto DownPos = Vec3(310.0f, -320.0f, 0.0f);

		if (accel != m_lastAccelKey)
		{
			if (!m_lastAccelKey.empty())
			{
				Hide(m_lastAccelKey);
			}

			Show(accel, accelPos);
			m_lastAccelKey = accel;
		}

		if (bullet != m_lastBulletKey)
		{
			if (!m_lastBulletKey.empty())
			{
				Hide(m_lastBulletKey);
			}

			Show(bullet, bulletPos);
			m_lastBulletKey = bullet;
		}

		//if (view != m_lastViewKey)
		//{
		//	if (!m_lastViewKey.empty())
		//	{
		//		Hide(m_lastViewKey);
		//	}

		//	Show(view, viewPos);
		//	m_lastViewKey = view;
		//}

		// Hide(m_lastViewKey);
		Show(L"LStick", stickPos);
		Show(L"DDown", DownPos);

	}

	void UIManager::ForceRefreshOperationUI()
	{
		// 以前表示していたキーキャッシュを消す
		m_lastAccelKey.clear();
		m_lastBulletKey.clear();
		m_lastViewKey.clear();

		// 念のため全ボタン非表示
		HideAll();
	}

	void UIManager::PushBackSprite()
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();

		constexpr int menuCount = static_cast<int>(KeyConfigSelect::Max);
		constexpr float uvHeight = 1.0f / menuCount;

		Vec2 size(200.0f, 100.0f);

		for (int i = 0; i < menuCount; i++)
		{
			// 縦分割UV
			Vec2 lt(0.0f, uvHeight * i);
			Vec2 rb(1.0f, uvHeight * (i + 1));

			auto sprite = stage->AddGameObject<Sprite>(
				L"PauseMenuKeyConfig_TX",
				size,
				Vec3(0, 0, 0),
				1
			);

			sprite->SetUVRect(lt, rb);
			sprite->OnClear(true);

			m_keyConfigSprites.push_back(sprite);
		}
	}

	void UIManager::ShowSprite(KeyConfigSelect type)
	{
		int index = static_cast<int>(type);
		m_keyConfigSprites[index]->OnClear(false);
	}

	void UIManager::HideSprite(KeyConfigSelect type)
	{
		int index = static_cast<int>(type);
		m_keyConfigSprites[index]->OnClear(true);
	}

	void UIManager::SetSpritePos(KeyConfigSelect type, const Vec3& pos)
	{
		int index = static_cast<int>(type);
		m_keyConfigSprites[index]->GetComponent<Transform>()->SetPosition(pos);
	}

	void UIManager::CreateKeyConfigSprite()
	{
		if (m_keyConfigSprites.empty())
		{
			PushBackSprite();
		}

		SetSpritePos(KeyConfigSelect::Bullet, Vec3(150, -320.0f, 0));
		ShowSprite(KeyConfigSelect::Bullet);
	}

	void UIManager::ForceRefreshKeyConfigSprite()
	{
		m_keyConfigSprites.clear();

		constexpr int count = static_cast<int>(KeyConfigSelect::Max);

		for (int i = 0; i < count; i++)
		{
			HideSprite(static_cast<KeyConfigSelect>(i));
		}
	}
}
//end basecross
