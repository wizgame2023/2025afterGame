/*!
@file Player.cpp
@brief プレイヤーなど実体
担当：吉田 智貴
*/

#include "stdafx.h"
#include "Project.h"
#include "FighterAircraftBase.h"

namespace basecross {
	FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& ptrStage) :
		Actor(ptrStage)
	{
	}

	FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale,const shared_ptr<CheckPoint>& startCheckPoint, Col4 color) :
		Actor(stagePtr, pos, rot, scale, color)
		//m_nextCheckPointPos(Vec3(0.0f, 0.0f, 20.0f))
	{
		// 次のチェックポイントの設定
		//SetCheckPoint(startCheckPoint);
		//SetNextCheckPointPos(Vec3(0.0f, 0.0f, 10.0f));
	}

	FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& stagePtr, wstring name, Vec3 pos, Vec3 rot, Vec3 scale, const shared_ptr<CheckPoint>& startCheckPoint, Col4 color):
		Actor(stagePtr, pos, rot, scale, color),
		m_name(name)
		//m_nextCheckPointPos(Vec3(0.0f, 0.0f, 20.0f))
	{
	}

	FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat qt, Vec3 scale,const shared_ptr<CheckPoint>& startCheckPoint, Col4 color):
		Actor(stagePtr,pos,qt,scale,color)
		//m_nextCheckPointPos(Vec3(0.0f, 0.0f, 20.0f))
	{
		// 次のチェックポイントの設定
		//SetCheckPoint(startCheckPoint);
		//SetNextCheckPointPos(Vec3(0.0f, 0.0f, 10.0f));
	}

	FighterAircraftBase::~FighterAircraftBase()
	{

	}

	void FighterAircraftBase::OnCreate()
	{
		Actor::OnCreate();
		ResetParameter();

		auto stage = GetStage();
		auto& scoreManager = ScoreManager::GetScoreManager();

		scoreManager->PushBackFighterBase(GetThis<FighterAircraftBase>());

		// バリア装備
		m_barrier = stage->AddGameObject<Barrier>(GetThis<FighterAircraftBase>());
		// バリア妨害装備
		//m_disableShield = stage->AddGameObject<DisableBarrier>(GetThis<FighterAircraftBase>());
		//m_nextCheckPointPos = Vec3(0.0f, 0.0f, 20.0f);

		// 自分の名前をタグとして追加
		//AddTag(m_name);

	}

	void FighterAircraftBase::OnUpdate()
	{
		Actor::OnUpdate();
		m_pos = GetComponent<Transform>()->GetPosition();
		//// 次のチェックポイントを通り過ぎていないかの処理
		//if (m_pos.z > m_nextCheckPointPos.z)
		//{
		//	// もし、チェックポイントに触れて通り過ぎていなかったらスピード軽減
		//	// 通り過ぎたのがゴールだったら前のチェックポイントの位置に戻る
		//	auto& gameManager = GameManager::GetGameManager();
		//	int checkPointSize = gameManager->GetChackPointsSize();

		//	if (checkPointSize > m_nextCheckPointID + 1)
		//	{
		//		m_speedCurrent -= 1.0f;
		//		auto nextCheckPoint = gameManager->GetCheckPoint(m_nextCheckPointID + 1);
		//		SetCheckPoint(nextCheckPoint);
		//	}
		//	else if(checkPointSize <= m_nextCheckPointID + 1)
		//	{
		//		auto currentCheckPoint = gameManager->GetCheckPoint(m_CurrentCheckPointID);
		//		GetComponent<Transform>()->SetPosition(m_currentCheckPointPos);
		//	}
		//}

		auto& app = App::GetApp();
		auto deltaTime = app->GetElapsedTime();

		Invincible();

	}

	// バリア使用関数
	void FighterAircraftBase::UseBarrier(bool use)
	{
		m_barrier->SetUse(use);
	}

	// バリア妨害使用関数
	void FighterAircraftBase::UseDisableShield()
	{
		m_disableShield->SetUse(true);
	}

	// 弾発射関数
	void FighterAircraftBase::UseBullet()
	{
		// 弾発射
		GetStage()->AddGameObject<Bullet>(GetThis<FighterAircraftBase>());
	}

	// 倒された場合のスコア譲渡処理
	// 第一引数　ぶつかった弾のポインタ 第二引数  譲渡する割合(0.0f~1.0f)
	void FighterAircraftBase::DownTransferScore(const shared_ptr<Bullet>& bullet, float magnification)
	{
		// 球を打ったオブジェクトを確認する
		auto bulletParent = bullet->GetParentObj();
		auto bulletParentlock = bulletParent.lock();

		auto& scoreManager = ScoreManager::GetScoreManager();
		auto plScore = scoreManager->GetPlScore();

		if (bulletParentlock)
		{
			auto bulletParentPlayer = dynamic_pointer_cast<Player>(bulletParentlock);
			auto bulletParentEnemy = dynamic_pointer_cast<Enemy>(bulletParentlock);

			// Playerが倒した時
			if (bulletParentPlayer)
			{
				float tansferScore = scoreManager->GetScore(GetId()) * magnification;
				scoreManager->SubScore(GetId(),tansferScore);
				scoreManager->SetPlScore(plScore + tansferScore);
			}

			// Enemyが倒した時
			if (bulletParentEnemy)
			{
				float transferScore = plScore * magnification;

				// Player から引く
				scoreManager->SubPlScore(transferScore);

				// 弾を撃った Enemy の ID を取得
				int enemyId = bullet->GetOwnerId();

				// Enemy に加算
				scoreManager->AddScore(enemyId, transferScore);
			}
		}
	}

	// 倒された場合のスコア譲渡処理
	// 第一引数　ぶつかったオブジェクトポインタ 第二引数  譲渡する割合(0.0f~1.0f)
	void FighterAircraftBase::DownTransferScoreByCollision(const shared_ptr<FighterAircraftBase>& obj, float magnification)
	{
		auto& scoreManager = ScoreManager::GetScoreManager();
		auto plScore = scoreManager->GetPlScore();

		if (!obj) return;

		// 攻撃者を Fighter として取得
		auto attackerFighter = dynamic_pointer_cast<FighterAircraftBase>(obj);
		if (!attackerFighter) return;

		// Player判定
		auto attackerPlayer = dynamic_pointer_cast<Player>(attackerFighter);
		// Enemy 判定
		auto attackerEnemy = dynamic_pointer_cast<Enemy>(attackerFighter);

		// Player が倒した場合
		if (attackerPlayer)
		{
			float transferScore = scoreManager->GetScore(GetId()) * magnification;

			// 倒された側から減らす
			scoreManager->SubScore(GetId(), transferScore);

			// Player に加算
			scoreManager->SetPlScore(plScore + transferScore);
		}

		// Enemy が倒した場合
		if (attackerEnemy)
		{
			// 倒されたのがPlayerの場合
			// shared_ptr版のthis
			auto victimPlayer = dynamic_pointer_cast<Player>(shared_from_this());
			if (victimPlayer)
			{
				float transferScore = scoreManager->GetPlScore() * magnification;

				// Player から減らす
				scoreManager->SubPlScore(transferScore);

				// Enemy に加算
				scoreManager->AddScore(attackerEnemy->GetId(), transferScore);
			}

			// 倒されたのがPlayerの場合Enemyの場合
			auto victimEnemy = dynamic_pointer_cast<Enemy>(shared_from_this());
			if (victimEnemy && victimEnemy->GetId() != attackerEnemy->GetId())
			{
				float transferScore = scoreManager->GetScore(victimEnemy->GetId()) * magnification;

				// 倒された Enemy から減らす
				scoreManager->SubScore(victimEnemy->GetId(), transferScore);

				// 攻撃 Enemy に加算
				scoreManager->AddScore(attackerEnemy->GetId(), transferScore);
			}
		}
	}

	// 当たり判定(当たった時)
	// 引数１ ぶつかったオブジェクト
	void FighterAircraftBase::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{
		auto fighteBase = dynamic_pointer_cast<FighterAircraftBase>(obj);

		if (fighteBase)
		{
			if (m_invincibleFlag == false)
			{
				// 与えるダメージ
				m_hpCurrent -= 5;

				// 無敵フラグON
				OnInvincibleFlag();
			}

			if (m_hpCurrent <= 0.0f)
			{
				// 敵を倒した時にスコアを譲渡
				DownTransferScoreByCollision(fighteBase, 0.3f);
			}
		}
	}

	// 現在耐久値のゲッタ
	int FighterAircraftBase::GetHpCurrent()
	{
		return m_hpCurrent;
	}

	// 最大耐久値のゲッタ
	int FighterAircraftBase::GetHpMax()
	{
		return m_hpMax;
	}

	//// 現在のエネルギーのゲッタ
	//float FighterAircraftBase::GetEnergyCurrent()
	//{
	//	return m_energyCurrent;
	//}

	//// 最大のエネルギー値のゲッタ
	//float FighterAircraftBase::GetEnergyMax()
	//{
	//	return m_energyMax;
	//}

	//// エネルギー切れになったかのフラグゲッタ
	//bool FighterAircraftBase::GetOutEnergyFlag()
	//{
	//	return m_outEnergyFlag;
	//}

	// バリア使用していいかのフラグゲッタ
	bool FighterAircraftBase::GetBarrierUseFlag()
	{
		return m_barrierUseFlag;
	}

	// 弾を使用していいかのフラグゲッタ
	bool FighterAircraftBase::GetGunUseFlag()
	{
		return m_gunUseFlag;
	}

	// バリア無効化をして以下のフラグゲッタ
	bool FighterAircraftBase::GetDisableShieldFlag()
	{
		return m_disableShieldFlag;
	}

	//// 次のチェックポインタを入れるセッタ
	//void FighterAircraftBase::SetCheckPoint(const shared_ptr<CheckPoint>& nextCheckPoint)
	//{
	//	m_nextCheckPoint = nextCheckPoint;
	//	auto checkPointLock = m_nextCheckPoint.lock();
	//	m_nextCheckPointID++;

	//	// 次のチェックポイント位置取得
	//	m_nextCheckPointPos = checkPointLock->GetPos();
	//}

	//// 現在のエネルギーセッタ
	//void FighterAircraftBase::SetEnergyCurrent(float energyCurrent)
	//{
	//	m_energyCurrent = energyCurrent;
	//}

	//// 自分が通ったチェックポイントのタイムと前の機体のタイムの差のセッタ
	//void FighterAircraftBase::SetTimeCheckPointDifferece(float TimeCheckPointDifferece)
	//{
	//	m_timeCheckPointDifferece = TimeCheckPointDifferece;
	//}

	//// 次のチェックポイントの位置のセッタ
	//void FighterAircraftBase::SetNextCheckPointPos(Vec3 nextCheckPointPos)
	//{
	//	m_currentCheckPointPos = m_nextCheckPointPos;
	//	m_nextCheckPointPos = nextCheckPointPos;
	//	m_nextCheckPointID++;
	//}

	// 現在の弾数のゲッタ
	int FighterAircraftBase::GetBulletNumCurrentNow()
	{
		return m_bulletNumCurrentNow;
	}

	// 現在の弾数のセッタ
	void FighterAircraftBase::SetBulletNumCurrentNow(int bullet)
	{
		m_bulletNumCurrentNow = bullet;
		if (m_bulletNumCurrentNow >= m_bulletNumMax)
		{
			m_bulletNumCurrentNow = m_bulletNumMax;
		}
	}

	// 最大の弾数のゲッタ
	int FighterAircraftBase::GetBulletNumMax()
	{
		return m_bulletNumMax;
	}

	//  復活までの無敵付与時間のゲッタ
	float FighterAircraftBase::GetTimeOfReturn()
	{
		return m_timeOfReturn;
	}

	// 復活までの無敵付与時間
	float FighterAircraftBase::GetTimeOfReturnUnDamage()
	{
		return m_timeOfReturnUnDamage;
	}

	// 現在スコアのゲッタ
	float FighterAircraftBase::GetScoreCurrent()
	{
		return m_scoreCurrent;
	}

	// スコアの追加処理
	void FighterAircraftBase::AddScoreCurrent(int addScore)
	{
		m_scoreCurrent += addScore;
		return;
	}

	// 現在HPのセッタ
	void FighterAircraftBase::SetHPCurrent(float hp)
	{
		m_hpCurrent = hp;

		if (m_hpCurrent >= m_hpMax)
		{
			m_hpCurrent = m_hpMax;
		}
	}

	// idのゲッタ
	int FighterAircraftBase::GetId()
	{
		return m_id;
	}

	// idのセッタ
	void FighterAircraftBase::SetId(int id)
	{
		m_id = id;
	}

	void FighterAircraftBase::ResetParameter()
	{
		auto& scoreManager = ScoreManager::GetScoreManager();
		
		//スコアのリセット
		scoreManager->ResetScore(GetId());
	}

	// 無敵時の処理
	void FighterAircraftBase::Invincible()
	{
		if (m_invincibleFlag)
		{
			m_countTimeOfInvincible += m_delta;
			if (m_timeOfInvincible < m_countTimeOfInvincible)
			{
				// 無敵が切れる
				m_invincibleFlag = false;
				m_countTimeOfInvincible = 0.0f;

				// 点滅用の数値も初期化する
				m_color.w = 1.0f;
				m_countTimeOfBlinking = 0.0f;
			}

			// 無敵状態の時自分自身は点滅する
			DrawBlinking();
		}

		return;
	}

	// 無敵時の点滅処理
	void FighterAircraftBase::DrawBlinking()
	{
		// 無敵状態の時自分自身は点滅する
		m_countTimeOfBlinking += m_delta;
		if (0.3f < m_countTimeOfBlinking)
		{
			if (m_color.w > 0.0f)
			{
				m_color.w = 0.0f;
			}
			else if (m_color.w <= 0.0f)
			{
				m_color.w = 1.0f;
			}

			m_countTimeOfBlinking = 0.0f;
		}

		return;
	}

	bool FighterAircraftBase::GetInvincibleFlag()
	{
		return m_invincibleFlag;
	}

	void FighterAircraftBase::OnInvincibleFlag()
	{
		m_invincibleFlag = true;
		return;

	}
}
//end basecross

