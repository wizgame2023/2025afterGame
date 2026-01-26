/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
		ItemObject::ItemObject(const shared_ptr<Stage>& StagePtr,
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
		m_repair(10)
	{
		try
		{

		}
		catch (...)
		{
			throw;
		}
	}

	ItemObject::~ItemObject(){

	}

	void ItemObject::OnCreate()
	{
		Actor::OnCreate();

		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_siz);
		PtrTrans->SetRotation(m_rot);
		PtrTrans->SetPosition(m_pos);

		//メッシュの描画
		/*auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");*/

		//コリジョンの設定
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(false);
		ptrCol->SetAfterCollision(AfterCollision::None); // 物理判定無し

		//ビルボードの生成
		m_billBoard = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"Repair", 2, 0, 0, Vec3(1.5f, 1.5f, 1.5f));
	}

	void ItemObject::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto body = dynamic_pointer_cast<FighterAircraftBase>(obj);
		if (body)
		{
			// BGM、SE用のマネージャー作成
			auto m_audioManager = App::GetApp()->GetXAudio2Manager();
			m_audioManager->Start(L"GetScoreSE", 1, 1.0f);

			auto& score = ScoreObjectManager::GetScoreObjectManager();
			//score->RemoveObject();
			GetStage()->RemoveGameObject<ItemObject>(GetThis<ItemObject>());
		}
	}

	int ItemObject::GetObjectID()
	{
		return m_id;
	}
}
//end basecross
