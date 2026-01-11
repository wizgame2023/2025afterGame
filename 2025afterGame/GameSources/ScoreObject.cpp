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
		const int& ID
	) :
		Actor(StagePtr),
		m_pos(Pos),
		m_rot(Rot),
		m_siz(Siz),
		m_id(ID),
		m_score(10)
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

		//メッシュの描画
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");

		//コリジョンの設定
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(true);
		ptrCol->SetAfterCollision(AfterCollision::None); // 物理判定無し
	}

	void ScoreObject::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto body = dynamic_pointer_cast<FighterAircraftBase>(obj);
		if (body)
		{
			auto& score = ScoreObjectManager::GetScoreObjectManager();
			//score->RemoveObject();
			body->AddScoreCurrent(m_score);
			GetStage()->RemoveGameObject<ScoreObject>(GetThis<ScoreObject>());
		}
	}

	int ScoreObject::GetObjectID()
	{
		return m_id;
	}
}
//end basecross
