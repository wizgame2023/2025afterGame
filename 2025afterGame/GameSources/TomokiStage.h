/*!
@file TomokiStage.h
@brief 個人作業用ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class TomokiStage : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		TomokiStage() :Stage() {}
		virtual ~TomokiStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

