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
	class Sprite;
	class NumberSprite;
	class UIManager
	{
	private:
		struct UIManagerDeleter
		{
			void operator()(UIManager* p) { delete p; };
		};
		
		// Managerを一つだ保持
		static unique_ptr<UIManager, UIManagerDeleter> m_UIManager;
		shared_ptr<Sprite> m_clon;
		shared_ptr<NumberSprite> m_countNumber;
		shared_ptr<NumberSprite> m_score;
		shared_ptr<NumberSprite> m_secondUI;

		int m_minute;
		int m_second;
		int m_enemyCount;
		bool m_createUI;
		bool m_createUIEnd;

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

		// 初期化
		virtual void OnCreate();
		// 更新
		virtual void OnUpdate();
		// 一つだけ生成した物
		void CreateUI();
		// Spriteで作ったSpriteの生成
		void CreateSprite();
		// NumberSpriteで作られたSpriteの生成
		void CreateNumberSprite();
		// リザルト生成
		void CreateResult();
		// ランキング
		void CreateRankingUI();
		// limitを分と秒に分ける
		void UpdateTime(int limit);
		// 現在の分を取得
		int GetMinuteTimer();
		// 現在の秒を取得
		int GetSecondTimer();
		// リザルトUIが作られたか
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
