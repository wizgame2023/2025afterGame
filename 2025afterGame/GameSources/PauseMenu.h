/*!
@file PauseMenu.h
@brief PauseMenuヘッダ
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class PauseMenu : public MyGameObject
	{
		// メインメニューでの選択肢
		enum class PauseMainMenuSelect
		{
			Resume,		// 再開
			Restart,	// リスタート
			Volume,		// ボリューム
			Exit,		// タイトルへ戻る
			Max			// 項目の最大数
		};

		// セッティングメニューの選択肢
		enum class PauseSettingMenuSelect
		{
			Volume,			// ボリューム
			KeyConfig,		// キーコンフィグ
			Max				// 項目の最大数
		};

		// ポーズメニューの状態
		enum class PauseMenuState
		{
			False,
			MainMenu,
			VolumeMenu,
			KeyConfigMenu,
			Max
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
		shared_ptr<Sprite> m_pauseBackGroundSprite;					// ポーズメニューの背景スプライト

		// ============================

		// 現在の選択肢
		PauseMainMenuSelect m_crntSelect;

		// ポーズメニューの状態
		PauseMenuState m_pauseState;

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

		// 全メニューのスプライトを表示するか
		void IsVisibleAllMenuSprites(const bool flag); 

		// メニュー別でスプライトを表示するか
		void IsVisibleMenuSprites(const vector<shared_ptr<Sprite>>& spVec, const bool flag);

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//end basecross
