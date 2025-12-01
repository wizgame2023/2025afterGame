/*!
@file TitleStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class TitleStage : public Stage 
	{
		//ビューの作成
		void CreateViewLight();

		void CreateUI();

		//タイトルのスプライト
		shared_ptr<Sprite> m_testTitle;

		//メンバ変数
		float m_Transparency;
		bool m_Transparent;

	public:
		//構築と破棄
		TitleStage();
		virtual ~TitleStage();
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void BlinkUI(shared_ptr<Sprite> blinksprite);
	};


}
//end basecross

