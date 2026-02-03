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
		// ==============================================================================
		// enum群
		// ==============================================================================

		// メインメニューでの選択肢
		enum class PauseMainMenuSelect
		{
			Resume,		// 再開
			Restart,	// リスタート
			Setting,	// 設定
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

		// ボリュームメニューの選択肢
		enum class PauseVolumeMenuSelect
		{
			BGMVolume,		// BGM音量
			SEVolume,		// SE音量
			Max				// 項目の最大数
		};

		// キーコンフィグメニューの選択肢
		enum class PauseKeyConfigMenuSelect
		{
			UpDownSwap,		// 上下反転
			Accel,			// 加速
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
			AccelSetting,
			BulletSetting,
			ViewBehindSetting,
			Max
		};

		// ボタンのスプライトの種類
		enum class ButtonsType : int
		{
			A, B, X, Y,
			LB,	RB,	LT,	RT,
			Back, Start,
			LS,	RS,
			Up,	Right, Down, Left,
			Max
		};

		// ==============================================================================
		// 構造体群
		// ==============================================================================

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
			wstring AccelKey;
			wstring BulletKey;
			wstring ViewBehindKey;
		};

		// ==============================================================================
		// メンバ変数
		// ==============================================================================

		// 定数群================================

		const Vec3 m_normalScale = Vec3(1.0f, 1.0f, 1.0f);    // 通常のスケール
		const Vec3 m_selectionScale = Vec3(1.3f, 1.3f, 1.0f); // 選択中のスケール
		static constexpr int ButtonsTypeCount = 16;           // ボタンの種類数

		// ======================================
		
		// ポーズメニュースプライト群============

		vector<shared_ptr<Sprite>> m_pauseMainMenuSprites;			// メインメニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseSettingMenuSprites;		// 設定メニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseVolumeMenuSprites;		// 音量メニューのスプライトの数
		vector<shared_ptr<Sprite>> m_pauseKeyConfigMenuSprites;		// キーコンフィグメニューのスプライトの数
		shared_ptr<Sprite> m_pauseBackGroundSprite;					// ポーズメニューの背景スプライト
		// ボタンのスプライトの数
		array<shared_ptr<Sprite>, ButtonsTypeCount> m_pauseButtonsSprites;
		// [自分用メモ]arrayは<型, 要素数>の形で宣言(この場合はshared_ptr<Sprite>型の16個の要素配列)
		
		// ボタン名と種類のマップ
		unordered_map<wstring, ButtonsType> m_buttonTypeMap; 
		// [自分用メモ]mapは<キー, 値>の形で宣言(この場合はwstring型とButtonsType型のマップ)
		// m_buttonTypeMap[L"A"]でButtonsType::Aが取れるように設定する必要がある
		// データ駆動と呼ぶらしい(画面に対してこうしろ、と命令するのではなく
		//						　データが変わったから画面を変える、という考え方)

		// ======================================

		// 現在の選択肢群========================

		PauseMainMenuSelect m_crntMainSelect;			// 現在の選択肢(Main)
		PauseSettingMenuSelect m_crntSettingSelect;		// 現在の選択肢(Setting)
		PauseVolumeMenuSelect m_crntVolumeSelect;		// 現在の選択肢(Volume)
		PauseKeyConfigMenuSelect m_crntKeyConfigSelect;	// 現在の選択肢(KeyConfig)

		// ======================================
		

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

		// スプライトを追加する(vector専用)
		void PushBackPauseMenuSprite(vector<shared_ptr<Sprite>>& vecSprite, const SpriteInfo& spInfo);

		// ボタンの辞書登録
		void InitButtonTypeMap();

		// ボタンの可視管理
		void SetShowAndPosButtons(const wstring& buttonsName, const Vec3& setPos);

		// ポーズが始まった瞬間の処理
		void StartPause();

		// メニューの可視管理
		void MenuVisibleManagement();

		// ポーズメニューの更新	==========================================

		// 全メニュー更新の管理
		void UpdatePauseMenu();

		// 各メニューの更新
		void UpdateMainMenu(InputManager& input);
		void UpdateSettingMenu(InputManager& input);
		void UpdateVolumeMenu(InputManager& input);
		void UpdateVolumeSettingMenu(InputManager& input);
		void UpdateKeyConfigMenu(InputManager& input);
		void UpdateKeyConfigSettingMenu(InputManager& input);
		
		// ポーズメニューの更新	==========================================

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

		// ==============================================================================
		// テンプレート関数
		// ==============================================================================
		
	private:
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
			constexpr float deadZone = 0.5;

			if (leftStick.y > deadZone)	  move = -1; // 上
			else if (leftStick.y < deadZone) move = 1;  // 下

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

		// 選ばれている選択肢のスケールを変更
		template<typename Se, typename Max>
		void ScalingSelectedSprite(vector<shared_ptr<Sprite>>& spVec, const Se& crntSelect, const Max& selectMax)
		{
			for (int i = 0; i < static_cast<int>(selectMax); i++)
			{
				spVec[i]->SetScale(m_normalScale);
			}
			spVec[static_cast<int>(crntSelect)]->SetScale(m_selectionScale);
		}

		// 選択肢が変わった時の処理
		template<typename T, typename EnumMax>
		void HandleMenuSelection(vector<shared_ptr<Sprite>>& spVec, T& crntSelect, EnumMax max) {
			if (UpdateSelection(crntSelect, max)) {
				// 選択が変わった時だけスケーリングを更新
				ScalingSelectedSprite(spVec, crntSelect, max);

				// あとは音を鳴らすなどの処理を入れる
			}
		}

	};
}
//end basecross
