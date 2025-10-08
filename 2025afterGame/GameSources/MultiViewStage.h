/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class MultiViewStage : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		MultiViewStage() :Stage() {}
		virtual ~MultiViewStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

