/*!
@file UIManager.h
@brief UI管理の宣言
担当：吉田 智貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Enemy;
	class BillBoardGauge;
	class NumberSprite;
	class Sprite;
	class UIManager
	{
	private:
		struct UIManagerDeleter
		{
			void operator()(UIManager* p) { delete p; };
		};
		
		// Managerを一つだ保持
		static unique_ptr<UIManager, UIManagerDeleter> m_UIManager;
		vector<shared_ptr<Enemy>> m_enemies;
		vector<shared_ptr<BillBoardGauge>> m_enemyGauges;
		shared_ptr<NumberSprite> m_countNumber;
		shared_ptr<Sprite> m_clon;
		shared_ptr<NumberSprite> m_score;
		shared_ptr<NumberSprite> m_secondUI;

		bool m_createUI;
		bool m_initialized;
		bool m_createRankingflag;
		int m_playerHpCurrent;
		int m_playerHpMax;
		vector<int> m_enemyHpCurrent;
		vector<int> m_enemyHpMax;
		int m_minute;
		int m_second;
		int m_bulletNumCurrentNow;
		int m_bulletNumMax;
		int m_playerScoreCurrent;

		bool m_createUIEnd;
		bool m_deleteUI;

	public:
		//構築と破棄
		UIManager();
		virtual ~UIManager();

		// ゲームマネージャーを作成
		static unique_ptr<UIManager, UIManagerDeleter>& CreateUIManager();

		// ゲームマネージャーを取得
		static unique_ptr<UIManager, UIManagerDeleter>& GetUIManager();
		
		// 破棄処理
		void DeleteUIManager();

		// 子マネージャー破棄処理
		void DeleteChildManager();


		//初期化
		virtual void OnCreate();
		virtual void OnUpdate();

		void UIManagerCreate();

		// 一つだけ生成した物
		void CreateUI();
		void CreateSprite();
		void CreateNumberSprite();
		// ランキング
		void CreateRankingUI();

		// プレイヤーHPを取得
		void GetPlayerHP();
		// ステージにいる敵の数を取得
		void GetEnemies();
		//現在のプレイヤーのHPを取得
		int GetCurrentPlayerHP();
		//現在のプレイヤーの最大HPを取得
		int GetMaxPlayerHP();
		// 現在の敵のHPを取得
		vector<int> GetCurrentEnemyHP();
		// 現在の敵の最大HPを取得
		vector<int> GetMaxEnemyHP();
		// limitを分と秒に分ける
		void UpdateTime(int limit);
		//現在の分時間を取得
		int GetMinuteTimer();
		//現在の秒時間を取得
		int GetSecondTimer();

		//現在の弾数を取得
		int GetBulletNumCurrentNow();
		//最大の弾数を取得
		int GetBulletNumMax();

		// 現在のPlayerのスコアを取得
		int GetPlayerScore();

		void SetCreateUIFlag(bool flag);

	private:
		//コピー禁止
		UIManager(const UIManager&) = delete;
		UIManager& operator=(const UIManager&) = delete;
		//ムーブ禁止
		UIManager(const UIManager&&) = delete;
		UIManager& operator=(const UIManager&&) = delete;

	};
}
//end basecross
