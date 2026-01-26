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
		m_score(0)
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

	void ScoreObject::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto& gameManager = GameManager::GetGameManager();
		auto& score = ScoreObjectManager::GetScoreObjectManager();
		auto& scoreMana = ScoreManager::GetScoreManager();
		auto body = dynamic_pointer_cast<FighterAircraftBase>(obj);
		auto player = dynamic_pointer_cast<Player>(obj);
		auto enemy = dynamic_pointer_cast<Enemy>(obj);


		if (body)
		{
			// BGM、SE用のマネージャー作成
			auto m_audioManager = App::GetApp()->GetXAudio2Manager();
			m_audioManager->Start(L"GetScoreSE", 0, 1.0f);

			score->RemoveObject(m_id);
			body->AddScoreCurrent(m_score);
			gameManager->RemoveScoreObjectCout();
			GetStage()->RemoveGameObject<ScoreObject>(GetThis<ScoreObject>());
			m_billBoard = nullptr;
			//GetStage()->RemoveGameObject<BillBoard>(m_billBoard);
		}

		if (player)
		{
			auto plScore = scoreMana->GetPlScore();
			scoreMana->SetPlScore(plScore + m_score);
		}

		if (enemy)
		{
			auto enemyScore = scoreMana->GetPlScore();
			scoreMana->SetScore(L"Enemy1", scoreMana->GetScore(L"Enemy1") + m_score);
		}

	}

	int ScoreObject::GetObjectID()
	{
		return m_id;
	}
}
//end basecross
