/*!
@file SelectStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class SelectStage : public Stage 
	{
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		SelectStage();
		virtual ~SelectStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		//ステージ選択用メンバ変数
		int m_stageSelect;
	};


}
//end basecross

