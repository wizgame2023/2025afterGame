/*!
@file Barrier.h
@brief バリアの処理
担当者:三瓶裕太
*/


#pragma once
#include "stdafx.h"

namespace basecross {
	class Barrier :public Actor
	{
	private:
		bool m_affiliation; // 自分の所属(敵か味方か)
		bool m_use = false; // バリアが発動しているか
		bool m_useBefore = false; // ひとつ前のバリアが発動しているかのフラグ
		bool m_StartExpansion = false; // 拡大処理をするかのフラグ
		bool m_EndReduction = false; // 縮小処理をするかのフラグ
		float m_sizePercent = 0.0f; // m_sizeを最大(1.0f)とした割合サイズ

		// 仮でエネルギーの変数を作成、Player側に実装されたらそっちに移行 
		float m_energyDebag = 100.0f; 
		float m_energyEfficiency = 1.0f; // バリア使用時の燃費 高ければ高い程悪い
		float m_energyLost = 10.0f;	// バリア使用時に弾が当たった時の消費エネルギー 高ければ高い程悪い

		float m_barrierLenght = 2.0f; // Playerからのバリアの直線距離

		weak_ptr<FighterAircraftBase> m_parent; // 発射元のポインタ
		shared_ptr<FighterAircraftBase> m_parentLock; // 発射元のポインタをLockして受け取った変数

	public:
		Barrier(const shared_ptr<Stage>& stagePtr,const shared_ptr<FighterAircraftBase>& parent);
		~Barrier();

		void OnCreate()override;
		void OnUpdate()override;

		// 開始時の拡大処理
		void StartExpansion();
		// 終了時の縮小処理
		void EndReduction();

		// 今の使用状態が前のフレームから変わったかを確認する処理
		void CheckUse();

		// 親クラスによって所属を決める処理
		void DecideAffiliation();
		// 親オブジェクトについていく処理
		void FollowMove();
		// バリアを使うことによって起きるエネルギーを消費する処理
		void EnergyConsumption();

		// 当たり判定処理
		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;




		void SetUse(bool use);
		bool GetUse();
	};
}
//end basecross
