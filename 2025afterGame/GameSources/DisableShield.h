/*!
@file DisableShield.h
@brief シールドを破壊する処理
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	
	class DisableShield :public Actor
	{
	private:
		float m_sizeMax = 0.0f;      // シールド破壊の最大範囲
		float m_sizeAddSpeed = 3.5f; // シールド破壊の拡大スピード
		Col4 m_Color;

		// 仮でエネルギーの変数を作成、Player側に実装されたらそっちに移行 
		float m_energyDebag = 100.0f;
		float m_energyLost = 50.0f;	// バリア妨害を使用したときの消費 高ければ高い程燃費は悪い

		bool m_use = false; // 使用しているかのフラグ
		bool m_reduction = false; // サイズ縮小フラグ

		weak_ptr<FighterAircraftBase> m_parent; // 親オブジェクト
		shared_ptr<FighterAircraftBase> m_parentLock; // lockをした親オブジェクト

	public:
		DisableShield(const shared_ptr<Stage>& ptrStage, const shared_ptr<FighterAircraftBase>& parent, float sizeMax = 3.0f);
		~DisableShield();

		void OnCreate()override;
		void OnUpdate()override;

		// 親オブジェクトについていく処理
		void FollowMove();

		// 使用時の処理
		void UseProcess();

		// 当たり判定処理
		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;

		bool GetUse();
		void SetUse(bool use);
	};

}
//end basecross
