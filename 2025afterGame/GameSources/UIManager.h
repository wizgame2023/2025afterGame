/*!
@file UIManager.h
@brief UI管理の宣言
担当：吉田 智貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class HpSprite;
	class UIManager
	{
	private:
		struct UIManagerDeleter
		{
			void operator()(UIManager* p) { delete p; };
		};
		
		// Managerを一つだ保持
		static unique_ptr<UIManager, UIManagerDeleter> m_UIManager;

		bool m_createUI;
		int m_playerHpCurrent;
		int m_playerHpMax;
		int m_minute;
		int m_second;

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

		void CreateUI();

		// プレイヤーHPを取得
		void GetPlayerHP();
		//現在のプレイヤーのHPを取得
		int GetCurrentPlayerHP();
		//現在のプレイヤーの最大HPを取得
		int GetMaxPlayerHP();

		// limitを分と秒に分ける
		void UpdateTime(int limit);
		//現在の分時間を取得
		int GetMinuteTimer();
		//現在の秒時間を取得
		int GetSecondTimer();


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
