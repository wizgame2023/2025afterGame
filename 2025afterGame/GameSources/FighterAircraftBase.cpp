/*!
@file Player.cpp
@brief プレイヤーなど実体
担当：吉田 智貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& ptrStage) :
		Actor(ptrStage)
	{
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
		m_disableShield = stage->AddGameObject<DisableShield>(GetThis<FighterAircraftBase>());
	}

	void FighterAircraftBase::OnUpdate()
	{

	}

	// バリア使用関数
	void FighterAircraftBase::UseBarrier(bool use)
	{
		if (m_energyCurrent > 0.0f)
		{
			m_barrier->SetUse(use);
		}
	}

	// バリア妨害使用関数
	void FighterAircraftBase::UseDisableShield()
	{
		float energyLost = 30.0f; // エネルギーを消費する量
		if (m_energyCurrent >= energyLost)
		{
			m_energyCurrent -= energyLost; // エネルギー消費		
			m_disableShield->SetUse(true);
		}
	}

	// 弾発射関数
	void FighterAircraftBase::UseBullet()
	{
		float energyLost = 30.0f; // エネルギーを消費する量
		if (m_energyCurrent >= energyLost)
		{
			m_energyCurrent -= energyLost; // エネルギー消費
			GetStage()->AddGameObject<Bullet>(GetThis<FighterAircraftBase>());
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

	// 現在のエネルギーのゲッタ
	float FighterAircraftBase::GetEnergyCurrent()
	{
		return m_energyCurrent;
	}

	// 最大のエネルギー値のゲッタ
	float FighterAircraftBase::GetEnergyMax()
	{
		return m_energyMax;
	}

	// エネルギー切れになったかのフラグゲッタ
	bool FighterAircraftBase::GetOutEnergyFlag()
	{
		return m_outEnergyFlag;
	}

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

	// 現在のエネルギーセッタ
	void FighterAircraftBase::SetEnergyCurrent(float energyCurrent)
	{
		m_energyCurrent = energyCurrent;
	}

}
//end basecross

