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
		float m_sizeAddSpeed = 0.0f; // シールド破壊の拡大スピード

		bool m_use = false; // 使用しているかのフラグ
		bool m_reduction = false; // サイズ縮小フラグ

	public:
		DisableShield(const shared_ptr<Stage>& ptrStage);
		~DisableShield();

		void OnCreate()override;
		void OnUpdate()override;

		bool GetUse();
		void SetUse(bool use);
	};

}
//end basecross
