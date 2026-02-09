/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
		AmmoObject::AmmoObject(const shared_ptr<Stage>& StagePtr,
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
		m_reload(30),
		m_countDown(5.0f),
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

	AmmoObject::~AmmoObject(){

	}

	void AmmoObject::OnCreate()
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
		m_billBoard = GetStage()->AddGameObject<BillBoard>(GetThis<GameObject>(), L"Reload", 2, 0, 0, Vec3(1.5f, 1.5f, 1.5f));
	}

	void AmmoObject::OnUpdate()
	{
		Actor::OnUpdate();

		if (m_countDownFlug)
		{
			m_countDown -= m_delta;
			if (m_countDown <= 0)
			{
				//m_number->RemoveSprite();
				auto& ammomg = AmmoObjectManager::GetAmmoObjectManager();
				ammomg->RemoveObject(m_id);
				GetStage()->RemoveGameObject<AmmoObject>(GetThis<AmmoObject>());
			}
		}
	}

	void AmmoObject::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();
		auto body = dynamic_pointer_cast<FighterAircraftBase>(obj);
		if (body)
		{
			// BGM、SE用のマネージャー作成
			auto m_audioManager = App::GetApp()->GetXAudio2Manager();
			m_audioManager->Start(L"GetScoreSE", 1, 3.0f);

			// 弾UI作成
			//m_number = stage->AddGameObject<Sprite>(L"ReloadString", Vec2(500.0f, 100.0f), Vec3(0.0f, 0.0f, 0.0f));

			int ammo = body->GetBulletNumCurrentNow();
			ammo += m_reload;
			body->SetBulletNumCurrentNow(ammo);

			m_billBoard->RemoveBill();

			m_countDownFlug = true;
		}
	}

	int AmmoObject::GetObjectID()
	{
		return m_id;
	}
}
//end basecross
