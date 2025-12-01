/*!
@file KaitoStage.h
@brief 個人作業用ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class KaitoStage : public Stage {
		//ビューの作成
		void CreateViewLight();

	public:
		//構築と破棄
		KaitoStage() :Stage() {}
		virtual ~KaitoStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate() override;
	};

	class TestCubeKaito : public Actor
	{
		Vec3 m_startPos;
		Vec3 m_startScl;
		
	public:
		TestCubeKaito(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		~TestCubeKaito();
		virtual void OnCreate() override;
		//virtual void OnUpdate() override;
		// 当たり判定
		//virtual void OnCollisionEnter(shared_ptr<GameObject>& obj) override;
	};

}
//end basecross

