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
		int m_count;
		//オブジェクトのテスト作成
		void CreateTestObject();
		//リングオブジェクトを作成
		void CreateRingObject();
		//壁オブジェクトを作成
		void CreateWallObject();
		//スコアオブジェクト用の作成関数
		void CreateScoreObject();
		//見えないオブジェクト
		void CreateInvisibleCollision();

		void RemoveObject();

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

