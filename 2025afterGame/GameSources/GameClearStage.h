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
	class GameClearStage : public Stage 
	{
		//ビューの作成
		void CreateViewLight();

		void CreateUI();

		//タイトルのスプライト
		shared_ptr<Sprite> m_testTitle;

		//メンバ変数
		float m_Transparency;
		bool m_Transparent;
		float m_elapsedTime;

		// BGM、SE用
		shared_ptr<XAudio2Manager> m_AudioManager;
		shared_ptr<SoundItem> m_bgm;
	public:
		//構築と破棄
		GameClearStage();
		virtual ~GameClearStage();
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		virtual void OnDestroy()override;
	};


}
//end basecross

