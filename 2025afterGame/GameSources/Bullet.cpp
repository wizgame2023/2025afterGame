/*!
@file Bullet.cpp
@brief 弾の実体
担当：三瓶裕太
*/

#include "stdafx.h"
#include "Project.h"
#include "Bullet.h"

namespace basecross {
	Bullet::Bullet(const shared_ptr<Stage>& stagePtr,const shared_ptr<Actor>& parent) :
		Actor(stagePtr),
		m_parent(parent)
	{

	}

	Bullet::~Bullet()
	{

	}

	void Bullet::OnCreate()
	{
		Actor::OnCreate();

		// 親クラスの向いている方向を取得
		auto parentLock = m_parent.lock();
		m_parentForward = parentLock->GetComponent<Transform>()->GetForward();
		auto parentPos = parentLock->GetComponent<Transform>()->GetPosition();


		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(parentPos + (m_parentForward * 1.6f));
		m_trans->SetQuaternion(Quat(0.0f,0.0f,0.0f,-1.0f));
		m_trans->SetScale(Vec3(0.1f));

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetDrawActive(false);
		ptrCol->SetAfterCollision(AfterCollision::None); // 物理判定無し

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"Bullet_Mesh");
		ptrDraw->SetTextureResource(L"BulletTex");

		// 進む方向に向く
		m_qt = SetRotate(m_parentForward);
		m_trans->SetQuaternion(m_qt);
	
		AddTag(L"Bullet");

		// 弾の所属を登録
		shared_ptr<Player> player = dynamic_pointer_cast<Player>(m_parent.lock());
		shared_ptr<Enemy> enemy = dynamic_pointer_cast<Enemy>(m_parent.lock());
		
		if (player) // 味方
		{
			m_affiliation = true;
		}
		else if (enemy) // 敵
		{
			m_affiliation = false;
			// どのEnemyが撃ったか
			m_ownerId = enemy->GetId();
		}

	}

	void Bullet::OnUpdate()
	{
		if (m_pauseFlag) return;

		// 継承元ののUpdate更新
		Actor::OnUpdate();

		// 移動処理
		Move();

		// 射程範囲外にいるならこのオブジェクトは削除される
		if (m_limitLenght < m_limitLenghtCount)
		{
			GetStage()->RemoveGameObject<Bullet>(GetThis<Bullet>());
			return;
		}



		//m_trans->SetQuaternion(qt);

		m_trans->SetPosition(m_pos);
	}

	// 移動用の関数
	void Bullet::Move()
	{
		m_pos = m_trans->GetPosition();

		//SearchTarget();
		//HomingUpdate(m_delta);

		auto addPosVec = (m_speed * m_parentForward) * m_delta;
		m_pos += addPosVec;

		m_limitLenghtCount += abs(addPosVec.x) + abs(addPosVec.y) + abs(addPosVec.z);
	}

	// 当たり判定
	void Bullet::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}

	// 弾の所属のゲッタ
	bool Bullet::GetAffiliation()
	{
		return m_affiliation;
	}

	// ダメージのセッタ
	void Bullet::SetDamage(int damage)
	{
		m_damage = damage;
	}


	// ダメージのゲッタ
	int Bullet::GetDamage()
	{
		return m_damage;
	}

	// 親オブジェクトのゲッタ
	weak_ptr<Actor> Bullet::GetParentObj()
	{
		return m_parent;
	}

	void Bullet::SetOwnerId(int id)
	{
		m_ownerId = id;
	}

	int Bullet::GetOwnerId()
	{
		return m_ownerId;
	}

	void Bullet::SearchTarget()
	{
		// ステージから全オブジェクト取得
		auto stage = GetStage();
		auto objVec = stage->GetGameObjectVec();
		
		// 最良ターゲット探索用
		float bestScore = -FLT_MAX;
		weak_ptr<Enemy> bestTarget;
		
		// 自分の位置と前方
		auto myPos = m_pos;
		auto myForward = m_parentForward;

		// 全オブジェクト走査
		for (auto& obj : objVec)
		{
			// Enemy だけ対象
			auto enemy = dynamic_pointer_cast<Enemy>(obj);
			
			if (!enemy) continue;

			// 所属判定
			if (!m_affiliation && enemy->GetId() == m_ownerId) continue;

			// 敵位置取得
			Vec3 enemyPos = enemy->GetComponent<Transform>()->GetPosition();
			
			// 自分から敵方向
			Vec3 dir = enemyPos - myPos;
			float dist = dir.length();

			// 距離ゼロ防止
			if (dist <= 0.001f) continue;
			
			dir.normalize();

			// スコア計算スコアが高い奴による
			// 距離スコア　近いほと高め
			float distScore = 1.0f - min(dist / 15.0f, 1.0f);
			// 方向スコア　前方ほど高い
			float dirScore = max(dot(myForward, dir), 0.0f);
			// 合成スコア
			float score = distScore * 2.0f + dirScore * 0.5f;
	
			// 最大スコア更新
			if (score > bestScore)
			{
				bestScore = score;
				bestTarget = enemy;
			}

			// 現在の最良ターゲット保存
			m_target = bestTarget;
		}
	}


	void Bullet::HomingUpdate(float dt)
	{
		// ターゲット取得
		auto enemy = m_target.lock();
		if (!enemy) return;

		// 経過時間更新
		m_time += dt;
		// 自分位置
		Vec3 pos = m_pos;
		
		// ターゲット方向ベクトル
		Vec3 targetDir = enemy->GetComponent<Transform>()->GetPosition() - pos; 
		float dist = targetDir.length();

		// 距離ゼロ防止
		if (dist <= 0.0001f) return;
		
		targetDir.normalize(); 

		// ホーミング遅延
		if (m_time <= m_delay) return;

		// 時間経過で旋回強くなる
		float timeFactor = (m_time - m_delay) * m_turnAccel;
		// 近いほど旋回強く
		float distanceFactor = 1.0f - min(dist / 10.0f, 1.0f);
		
		// 角度差,正面なら0、横〜後ろで大
		float dotA = dot(m_parentForward, targetDir);
		float angleFactor = 1.0f - max(dotA, 0.0f);

		// 最終旋回量
		float turn = timeFactor * distanceFactor * angleFactor;
		turn = min(turn, m_maxTurn);

		// 旋回適応
		// 前方ベクトルをターゲット方向へ少し寄せる
		m_parentForward = m_parentForward + targetDir * turn;
		// 正規化
		m_parentForward.normalize(); 
		
		// 回転へ変換
		m_qt = SetRotate(m_parentForward); 
		m_trans->SetQuaternion(m_qt); 
	}


	TestCube::TestCube(const shared_ptr<Stage>& stagePtr,Vec3 pos,Quat qt,Vec3 scale) :
		Actor(stagePtr,pos,qt,scale)
	{

	}

	TestCube::~TestCube()
	{

	}

	void TestCube::OnCreate()
	{
		Actor::OnCreate();

		auto trans = GetComponent<Transform>();
		trans->SetPosition(m_pos);
		trans->SetQuaternion(m_qt);
		trans->SetScale(m_scale);

		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetDrawActive(true);
		ptrCol->SetFixed(true);

		auto ptrDraw = AddComponent<PNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void TestCube::OnUpdate()
	{

	}

	// 当たり判定
	void TestCube::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

	}
}
//end basecross