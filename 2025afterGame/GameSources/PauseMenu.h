/*!
@file PauseMenu.h
@brief PauseMenuヘッダ
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class InputManager;
	class PauseMenu : public MyGameObject
	{
		// メインメニューでの選択肢
		enum class PauseMainMenuSelect
		{
			Resume,		// 再開
			Restart,	// リスタート
			Setting,		// 設定
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

		enum class PauseVolumeMenuSelect
		{
			BGMVolume,		// BGM音量
			SEVolume,		// SE音量
			Max				// 項目の最大数
		};

		enum class PauseKeyConfigMenuSelect
		{
			UpDownSwap,		// 上下反転
			Bullet,			// 弾丸
			ViewBehind,		// 背面視点
			Max				// 項目の最大数
		};

		// ポーズメニューの状態
		enum class PauseMenuState
		{
			False,
			MainMenu,
			SettingMenu,
			VolumeMenu,
			KeyConfigMenu,
			BGMSetting,
			SESetting,
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

		struct PauseData
		{
			float BGMVolume;
			float SEVolume;
			bool UpDownSwap;
			wstring BulletKey;
			wstring ViewBehindKey;
		};

		// ==============================================================================
		// メンバ変数
		// ==============================================================================

		// ポーズメニュー群============

		vector<shared_ptr<Sprite>> m_pauseMainMenuSprites;			// メインメニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseSettingMenuSprites;		// 設定メニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseVolumeMenuSprites;		// 音量メニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseKeyConfigMenuSprites;		// キーコンフィグメニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseButtonsSprites;			// ボタンのスプライトの数
		shared_ptr<Sprite> m_pauseBackGroundSprite;					// ポーズメニューの背景スプライト

		// ============================

		// 現在の選択肢(Main)
		PauseMainMenuSelect m_crntMainSelect;

		// 現在の選択肢(Setting)
		PauseSettingMenuSelect m_crntSettingSelect;

		// 現在の選択肢(Volume)
		PauseVolumeMenuSelect m_crntVolumeSelect;

		// 現在の選択肢(KeyConfig)
		PauseKeyConfigMenuSelect m_crntKeyConfigSelect;

		// ポーズメニューの状態
		PauseMenuState m_pauseState;

		// 設定
		PauseData m_pauseData;

		// ステージ
		shared_ptr<Stage> m_stage;

		// 選択が変わったかどうか
		bool m_selectChanged = false;

		// ==============================================================================
		// メンバ関数
		// ==============================================================================

		// メインメニューのスプライトを追加する
		void PushBackPauseMenuSprite(vector<shared_ptr<Sprite>>& vecSprite, const SpriteInfo& spInfo);

		// 選択肢が変わったかどうか
		template<typename T>
		bool UpdateSelection(T& crntSelect, T maxEnum)
		{
			auto& input = InputManager::GetInputManager();
			auto leftStick = input->GetLStick();

			// スティックが中立に戻ったらフラグをリセット
			if (abs(leftStick.y) < 0.5f) {
				m_selectChanged = false;
				return false;
			}

			// すでに動かした後なら何もしない
			if (m_selectChanged) return false;

			// enum を int に変換して計算
			int current = static_cast<int>(crntSelect);
			int max = static_cast<int>(maxEnum);
			int move = 0;

			if (leftStick.y > 0.8f)	  move = -1; // 上
			else if (leftStick.y < -0.8f) move = 1;  // 下

			if (move != 0) {
				current += move;
				// ループ処理
				if (current < 0) current = max - 1;
				if (current >= max) current = 0;

				// 計算結果を元の enum 型に戻して保存
				crntSelect = static_cast<T>(current);
				m_selectChanged = true;
				
				return true; // 選択が変わった瞬間
			}

			return false;
		}

		// ポーズ開始
		void StartPause();

		// メニューの可視管理
		void MenuVisibleManagement();

		// ポーズメニューの更新
		void UpdatePauseMenu();

		// 各メニューの更新
		void UpdateMainMenu(InputManager& input);
		void UpdateSettingMenu(InputManager& input);
		void UpdateVolumeMenu(InputManager& input);
		void UpdateKeyConfigMenu(InputManager& input);
		void UpdateVolumeSettingMenu(InputManager& input);

		// デバッグログ群
		void DebugLogs();

	public : 
		PauseMenu(const shared_ptr<Stage>& stage);
		~PauseMenu() {};

		// 全メニューのスプライトを表示するか
		void IsVisibleAllMenuSprites(const bool flag); 

		// メニュー別でスプライトを表示するか
		void IsVisibleMenuSprites(const vector<shared_ptr<Sprite>>& spVec, const bool flag);

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		//void CreatePauseBinary();
		//void SavePauseBinary();
		//PauseData LoadPauseBinary();
	};

}
//end basecross
