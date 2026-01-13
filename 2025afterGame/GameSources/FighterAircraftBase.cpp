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

		auto stage = GetStage();

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

		if (bulletParentlock)
		{
			auto bulletParentFighter = dynamic_pointer_cast<FighterAircraftBase>(bulletParentlock);

			// 譲渡するスコアの計算(基本的には10%譲渡する)
			float transferScore = m_scoreCurrent * magnification;
			m_scoreCurrent -= transferScore;

			if (bulletParentFighter)
			{
				bulletParentFighter->AddScoreCurrent(transferScore);
			}
		}
	}

	// 当たり判定(当たった時)
	// 引数１ ぶつかったオブジェクト
	void FighterAircraftBase::OnCollisionEnter(shared_ptr<GameObject>& obj)
	{

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
	}

}
//end basecross

