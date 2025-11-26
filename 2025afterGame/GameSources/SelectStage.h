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
		virtual ~SelectStage();
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		virtual wstring NextStage(int nextstage,wstring gotostage);

		//ステージ選択用メンバ変数
		int m_stageSelect;
		//コントローラーのデッドゾーン変数
		float m_deadZone;
		//メニュー画面の時間を測定
		float m_deltaTime;
		float m_menuMoveCoolDown;
		//次のステージを指定
		wstring m_nextStage;
	};


}
//end basecross

