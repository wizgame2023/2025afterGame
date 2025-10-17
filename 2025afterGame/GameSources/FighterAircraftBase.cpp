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
	}

	void FighterAircraftBase::OnUpdate()
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

}
//end basecross

