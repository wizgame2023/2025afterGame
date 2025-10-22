/*!
@file ErionStage.h
@brief 個人作業用ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class ErionStage : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		ErionStage();
		virtual ~ErionStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		//生成フラグ
		bool m_setupFlug;
	};


}
//end basecross

