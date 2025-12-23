/*!
@file PauseMenu.h
@brief PauseMenuヘッダ
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class PauseMenu : public MyGameObject
	{
		enum class PauseMenuState
		{
			Resume,		// 再開
			Restart,	// リスタート
			Volume,		// ボリューム
			Exit,		// タイトルへ戻る
			Max			// 項目の最大数
		};

		// ==============================================================================
		// メンバ変数
		// ==============================================================================

		// ポーズメニュー群============

		vector<shared_ptr<Sprite>> m_pauseMainMenuSprites;// メインメニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseSettingMenuSprites;// 設定メニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseVolumeMenuSprites;// 音量メニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseKeyConfigMenuSprites;// キーコンフィグメニューのスプライトの数

		// ============================

		// 現在の選択肢
		PauseMenuState m_crntSelect;

		// ステージ
		shared_ptr<Stage> m_stage;

		// ==============================================================================
		// メンバ関数
		// ==============================================================================

		// メインメニューのスプライトを追加する
		void PushBackPauseMenuSprite(const wstring& textureName, vector<shared_ptr<Sprite>>& vecSprite, const Vec2& leftTopUV = Vec2(0.0f), const Vec2& rightBotUV = Vec2(1.0f));

	public : 
		PauseMenu(const shared_ptr<Stage>& stage);
		~PauseMenu() {};

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//end basecross
