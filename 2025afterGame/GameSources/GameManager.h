/*!
@file GameManaegr.h
@brief ゲーム内部を管理するマネージャー
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameManager
	{
	private:
		// 削除処理
		struct GameManagerDeleter
		{
			void operator()(GameManager* p) { delete p; };
		};

		static unique_ptr<GameManager, GameManagerDeleter> m_GameManager;

		float m_deltaTime = 0.0f; // 次のフレームに移動するまでの時間
		int m_gameStageNow = 0; // 現在のゲームステージの変数

		bool m_gameStartFlag = false; // ゲームが開始しているかのフラグ
		float m_timeGamePlaying = 0.0f; // ゲームの経過時間

	public:
		//構築と破棄
		GameManager();
		virtual ~GameManager();

		// ゲームマネージャーを作成
		static unique_ptr<GameManager, GameManagerDeleter>& CreateGameManager();

		// ゲームマネージャーを取得
		static unique_ptr<GameManager, GameManagerDeleter>& GetGameManager();

		// 破棄処理
		void DeleteGameManager();

		// 子マネージャー破棄処理
		void DeleteChildManager();


		//初期化
		virtual void OnCreate();
		virtual void OnUpdate();	
		
		// デルタタイムのゲッタ
		float GetDeltaTime();

		// gameStageNowのゲッタ
		int GetGameStageNow();
		// gameStageNowのセッタ
		void SetGameStageNow(int gameStageNow);

		// m_gameStartFlagのゲッタ
		bool GetGameStartFlag();
		// m_gameStartFlagのセッタ
		void SetGameStartFlag(bool gameStartFlag);

		// m_timeGamePlayingのゲッタ
		float GetTimeGamePlaying();

	};


}
//end basecross

