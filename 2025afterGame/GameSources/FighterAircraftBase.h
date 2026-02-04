/*!
@file FighterAircraftBase.h
@brief プレイヤーなど
担当：吉田 智貴
*/

#pragma once
#include "stdafx.h"
#include "Actor.h"
#include "PlayerGrv.h"

namespace basecross{
	class Barrier;
	class DisableBarrier;
	class Bullet;
	class FighterAircraftBase : public Actor
	{
	protected:
		shared_ptr<Barrier> m_barrier = nullptr; // バリアのポインタ
		shared_ptr<DisableBarrier> m_disableShield = nullptr; // バリア妨害のポインタ

		// 名前
		wstring m_name;

		// パラメーター
		int m_hpCurrent = 0;			// 耐久値
		int m_hpMax = 0;				// 最大耐久値
		float m_speedAdd = 0.0f;		// スピードの加速度
		float m_speedCurrent = 3.0f;	// スピード
		float m_speedMax = 0.0f;		// 最大スピード

		// 弾関係
		int m_bulletNumCurrentNow = 30; // 現在の球数
		int m_bulletNumMax = 30;		// 最大の球数

		// スコア関係
		int m_scoreCurrent = 0;		 // スコアの現在値
		int m_scoreMax = 99999999;   // スコアの最大値

		// 復活関係
		float m_timeOfReturnUnDamage = 0.0f; // 復活までの無敵付与時間
		float m_timeOfReturn = 0.0f;		   // 倒されてからの復活の時間

		////エネルギー
		//float m_energyCurrent = 0.0f;				// 現在のエネルギー
		//float m_energyMax = 0.0f;					// 最大のエネルギー値
		//float m_energyCharging = 0.0f;				// エネルギー回復
		//float m_recastEnergyChargingCurrent = 0.0f; // 回復に移行する必要時間を計測する変数
		//float m_recastEnergyChargingMax = 0.0f;		// 回復に移行するまでの必要経過時間

		//bool m_outEnergyFlag = false;			// エネルギー切れになったかのフラグ(そうなったらMaxになるまで使えない)
		bool m_barrierUseFlag = false;			// バリア使用してよいかのフラグ
		bool m_gunUseFlag = true;				// 弾を発射してよいかのフラグ
		bool m_disableShieldFlag = true;		// バリア無効化をしてよいかのフラグ 

		// 今後,ストリップストリーム,DRSを実装予定
		int m_nextCheckPointID = 0;
		int m_CurrentCheckPointID = 0;
		//weak_ptr<CheckPoint> m_nextCheckPoint;	  // 次のチェックポイントのポインタ
		//float m_timeCheckPointDifferece = 0.0f;	  // 自分が通ったチェックポイントのタイムと前の機体のタイムの差
		//Vec3 m_nextCheckPointPos = Vec3(0.0f);	  // 次のチェックポイントの位置
		//Vec3 m_currentCheckPointPos = Vec3(0.0f);	  // 次のチェックポイントの位置

		shared_ptr<PlayerGrv> m_playerGrv;

	public:
		FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& ptrStage);
		FighterAircraftBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, const shared_ptr<CheckPoint>& startCheckPoint, Col4 color = Col4(1.0f));
		FighterAircraftBase(const shared_ptr<Stage>& stagePtr, wstring name,Vec3 pos, Vec3 rot, Vec3 scale, const shared_ptr<CheckPoint>& startCheckPoint, Col4 color = Col4(1.0f));
		FighterAircraftBase(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat qt, Vec3 scale,const shared_ptr<CheckPoint>& startCheckPoint, Col4 color = Col4(1.0f));
		FighterAircraftBase::~FighterAircraftBase();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		// バリア使用関数
		void UseBarrier(bool use);
		// バリア妨害使用関数
		void UseDisableShield();
		// 弾発射関数
		void UseBullet();

		// 倒された場合のスコア譲渡処理
		void DownTransferScore(const shared_ptr<Bullet>& bullet,float magnification);

		// 当たり判定(当たった時)
		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;

		// 現在耐久値のゲッタ
		int GetHpCurrent();
		// 最大耐久値のゲッタ
		int GetHpMax();
		//// 現在のエネルギーのゲッタ
		//float GetEnergyCurrent();
		//// 最大のエネルギー値のゲッタ
		//float GetEnergyMax();
		//// エネルギー切れになったかのフラグゲッタ
		//bool GetOutEnergyFlag();
		// バリア使用していいかのフラグゲッタ
		bool GetBarrierUseFlag();
		// 弾を使用していいかのフラグゲッタ
		bool GetGunUseFlag();
		// バリア無効化をして以下のフラグゲッタ
		bool GetDisableShieldFlag();

		//// 次のチェックポインタを入れるセッタ
		//void SetCheckPoint(const shared_ptr<CheckPoint>& nextChackPoint);

		//// 現在のエネルギーセッタ
		//void SetEnergyCurrent(float energyCurrent);
		//// 自分が通ったチェックポイントのタイムと前の機体のタイムの差のセッタ
		//void SetTimeCheckPointDifferece(float timeCheckPointDifferece);
		//// 次のチェックポイントの位置のセッタ
		//void SetNextCheckPointPos(Vec3 nextCheckPointPos);

		// 現在の弾数のゲッタ
		int GetBulletNumCurrentNow();
		// 現在の弾数のゲッタ
		void SetBulletNumCurrentNow(int bullet);
		// 最大の弾数のゲッタ
		int GetBulletNumMax();
		//  復活までの無敵付与時間のゲッタ
		float GetTimeOfReturn();
		// 復活までの無敵付与時間
		float GetTimeOfReturnUnDamage();
		// 現在スコアのゲッタ
		float GetScoreCurrent();
		// 現在スコアの追加処理
		void AddScoreCurrent(int addScore);
		// 現在HPのセッタ
		void SetHPCurrent(float hp);

		virtual void CreateChildObjects()
		{
			auto stage = GetStage();

			m_playerGrv = stage->AddGameObject<PlayerGrv>();
			m_playerGrv->GetComponent<Transform>()->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));
		}
	};
}
//end basecross

