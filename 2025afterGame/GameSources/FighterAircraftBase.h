/*!
@file FighterAircraftBaseBase.h
@brief プレイヤーなど
担当：吉田 智貴
*/

#pragma once
#include "stdafx.h"
#include "Actor.h"

namespace basecross{
	class FighterAircraftBase : public Actor
	{
	protected:
		// パラメーター
		int m_hpCurrent;	   // 耐久値
		int m_hpMax;		   // 最大耐久値
		float m_speedAdd;	   // スピードの加速度
		float m_speedCurrent;  // スピード
		float m_speedMax;	   // 最大スピード

		//エネルギー
		float m_energyCurrent; // 現在のエネルギー
		float m_energyMax; // 最大のエネルギー値
		float m_energyCharging; // エネルギー回復
		float m_recastEnergyChargingCurrent; // 回復に移行する必要時間を計測する変数
		float m_recastEnergyChargingMax;	 // 回復に移行するまでの必要経過時間

		bool m_outEnergyFlag; // エネルギー切れになったかのフラグ(そうなったらMaxになるまで使えない)
		bool m_barrierUseFlag;    // バリア使用してよいかのフラグ
		bool m_gunUseFlag;		  // 弾を発射してよいかのフラグ
		bool m_disableShieldFlag; // バリア無効化をしてよいかのフラグ 

		// 今後,ストリップストリーム,DRSを実装予定
		
	public:
		FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& ptrStage);
		FighterAircraftBase::~FighterAircraftBase();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		// 現在耐久値のゲッタ
		int GetHpCurrent();
		// 最大耐久値のゲッタ
		int GetHpMax();
		// 現在のエネルギーのゲッタ
		float GetEnergyCurrent();
		// 最大のエネルギー値のゲッタ
		float GetEnergyMax();
		// エネルギー切れになったかのフラグゲッタ
		bool GetOutEnergyFlag();
		// バリア使用していいかのフラグゲッタ
		bool GetBarrierUseFlag();
		// 弾を使用していいかのフラグゲッタ
		bool GetGunUseFlag();
		// バリア無効化をして以下のフラグゲッタ
		bool GetDisableShieldFlag();

		// 現在のエネルギーセッタ
		void SetEnergyCurrent(float energyCurrent);
	};
}
//end basecross

