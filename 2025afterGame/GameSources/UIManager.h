/*!
@file UIManager.h
@brief UI管理の宣言
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class UIManager
	{
	private:
		struct UIManagerDeleter
		{
			void operator()(UIManager* p) { delete p; };
		};
		
		// Managerを一つだ保持
		static unique_ptr<UIManager, UIManagerDeleter> m_UIManager;

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

		// プレイヤーHPをUIに渡す
		void GetPlayerHP();

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
