/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
		ScoreObject::ScoreObject(const shared_ptr<Stage>& StagePtr,
		const Vec3& Pos,
		const Vec3& Rot,
		const Vec3& Siz,
		const int& ID,
		const int& Point
	) :
		Actor(StagePtr),
		m_pos(Pos),
		m_rot(Rot),
		m_siz(Siz),
		m_id(ID),
		m_point(Point),
		m_score(0),
		m_countDown(2.0f),
		m_countDownFlug(false)
	{
		try
		{

		}
		catch (...)
		{
			throw;
		}
	}

	ScoreObject::~ScoreObject(){

	}

	void ScoreObject::OnCreate()
	{
		Actor::OnCreate();

		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_siz);
		PtrTrans->SetRotation(m_rot);
		PtrTrans->SetPosition(m_pos);

		auto& gameManager = GameManager::GetGameManager();
		gameManager->AddscoreObjecCout();
		//メッシュの描画
		/*auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");*/

		//コリジョンの設定
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(false);
		ptrCol->SetAfterCollision(AfterCollision::None); // 物理判定無し

		if (m_point == 0)
		{
			//ビルボードの生成、スコアの設定
			m_billBoard = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"Bear", 2, 0, 0, Vec3(1.5f, 1.5f, 1.5f));
			m_score = 10;
		}
		else if (m_point == 1)
		{
			//ビルボードの生成、スコアの設定
			m_billBoard = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"Car", 2, 0, 0, Vec3(1.5f, 1.5f, 1.5f));
			m_score = 30;
		}
		else if (m_point == 2)
		{
			//ビルボードの生成、スコアの設定
			m_billBoard = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"Racingcar", 2, 0, 0, Vec3(1.5f, 1.5f, 1.5f));
			m_score = 50;
		}
	}

	void ScoreObject::OnUpdate()
	{
		Actor::OnUpdate();

		if (m_countDownFlug)
		{
			m_countDown -= m_delta;
			if (m_countDown <= 0)
			{
				m_number->OnDestory();
				GetStage()->RemoveGameObject<ScoreObject>(GetThis<ScoreObject>());
				m_billBoard = nullptr;
			}
		}
	}

	void ScoreObject::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		if (!m_countDownFlug)
		{
			auto& gameManager = GameManager::GetGameManager();
			auto& score = ScoreObjectManager::GetScoreObjectManager();
			auto& scoreMana = ScoreManager::GetScoreManager();
			auto body = dynamic_pointer_cast<FighterAircraftBase>(obj);
			auto player = dynamic_pointer_cast<Player>(obj);
			auto enemy = dynamic_pointer_cast<Enemy>(obj);
			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			auto stage = scene->GetActiveStage();
			int plusNumber = 12;


			if (body)
			{
				// BGM、SE用のマネージャー作成
				auto m_audioManager = App::GetApp()->GetXAudio2Manager();
				m_audioManager->Start(L"GetScoreSE", 0, 1.0f);
				score->RemoveObject(m_id);
				body->AddScoreCurrent(m_score);
				gameManager->RemoveScoreObjectCout();
				GetStage()->RemoveGameObject<BillBoard>(m_billBoard);
			}

			if (player)
			{
				auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
				auto plScore = scoreMana->GetPlScore();
				scoreMana->SetPlScore(plScore + m_score);
				m_billBoard->RemoveBill();
				// スコアUI作成
				m_number = stage->AddGameObject<NumberSprite>(Vec2(40.0f, 80.0f), Vec3(0.0f, 0.0f, 0.0f));
				m_number->SetSignedNumber(plusNumber, m_score);
				m_countDownFlug = true;
			}

			if (enemy)
			{
				auto enemyID = enemy->GetId();
				scoreMana->AddScore(enemyID, m_score);
				GetStage()->RemoveGameObject<ScoreObject>(GetThis<ScoreObject>());
				m_billBoard = nullptr;
				m_countDownFlug = true;
			}
		}
	}

	int ScoreObject::GetObjectID()
	{
		return m_id;
	}
}
//end basecross
