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

		CsvFile m_objectFile;
		//オブジェクトのテスト作成
		void CreateTestObject();

	public:
		//構築と破棄
		ErionStage() : Stage(){}
		virtual ~ErionStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};


}
//end basecross

