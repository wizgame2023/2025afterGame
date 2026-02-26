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
	class GameStage : public Stage {
		//ビューの作成
		void CreateViewLight();

		CsvFile m_objectFile;

		shared_ptr<Player> m_player;

		// BGM、SE用
		shared_ptr<XAudio2Manager> m_audioManager;
		shared_ptr<SoundItem> m_bgm;
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDestroy()override;

		int m_count;
	};


}
//end basecross

