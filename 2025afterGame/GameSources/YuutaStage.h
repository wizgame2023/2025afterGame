/*!
@file YuutaStage.h
@brief 個人作業用ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class YuutaStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		function<void()> m_Func;

		CsvFile m_objectFile;

	public:
		//構築と破棄
		YuutaStage() :Stage() {}
		virtual ~YuutaStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;


		void CreateDodgeObject();
	};


}
//end basecross
