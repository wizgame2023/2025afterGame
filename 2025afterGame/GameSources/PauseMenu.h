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

		struct SpriteInfo
		{
			wstring textureName;
			Vec2 leftTopUV;
			Vec2 rightBotUV;
			Vec3 pos;
			Vec2 size;
			int layer;
		};

		// ==============================================================================
		// メンバ変数
		// ==============================================================================

		// ポーズメニュー群============

		vector<shared_ptr<Sprite>> m_pauseMainMenuSprites;			// メインメニューのスプライトの数
		//vector<shared_ptr<Sprite>> m_pauseSettingMenuSprites;		// 設定メニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseVolumeMenuSprites;		// 音量メニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseKeyConfigMenuSprites;		// キーコンフィグメニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseButtonsSprites;			// ボタンのスプライトの数

		// ============================

		// 現在の選択肢
		PauseMenuState m_crntSelect;

		// ステージ
		shared_ptr<Stage> m_stage;

		// ==============================================================================
		// メンバ関数
		// ==============================================================================

		// メインメニューのスプライトを追加する
		void PushBackPauseMenuSprite(vector<shared_ptr<Sprite>>& vecSprite, const SpriteInfo& spInfo);

	public : 
		PauseMenu(const shared_ptr<Stage>& stage);
		~PauseMenu() {};

		void IsVisibleAllMenuSprites(const bool flag); // 全メニューのスプライトを表示・非表示にする
		void IsVisibleMainMenuSprites(const vector<shared_ptr<Sprite>>& spVec, const bool flag); // メニューのスプライトを表示・非表示にする

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//end basecross
