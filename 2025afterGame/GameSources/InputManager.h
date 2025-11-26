/*!
@file InputManager.h
@brief 入力マネージャー
@authors 吉田智貴
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class InputManager
	{
	private:
		struct InputManagerDeleter
		{
			void operator()(InputManager* p) { delete p; }
		};
		static unique_ptr<InputManager, InputManagerDeleter> m_InputManager;

		InputManager();
		virtual ~InputManager() {}

		/*
			L"DUp"    XINPUT_GAMEPAD_DPAD_UP
			L"DDown"  XINPUT_GAMEPAD_DPAD_DOWN
			L"DLeft"  XINPUT_GAMEPAD_DPAD_LEFT
			L"DRight" XINPUT_GAMEPAD_DPAD_RIGHT

			L"Start"  XINPUT_GAMEPAD_START
			L"Back"   XINPUT_GAMEPAD_BACK

			L"LStick" XINPUT_GAMEPAD_LEFT_THUMB
			L"RStick" XINPUT_GAMEPAD_RIGHT_THUMB

			L"L"      XINPUT_GAMEPAD_LEFT_SHOULDER
			L"R"      XINPUT_GAMEPAD_RIGHT_SHOULDER

			L"A"      XINPUT_GAMEPAD_A
			L"B"      XINPUT_GAMEPAD_B
			L"X"      XINPUT_GAMEPAD_X
			L"Y"      XINPUT_GAMEPAD_Y
		*/

		Vec2 m_LStick;
		Vec2 m_RStick;
		map<wstring, bool> m_Buttons;
		map<wstring, bool> m_DownButtons;
		map<wstring, bool> m_UpButtons;
		map<wstring, bool> m_NowUpdateButtons;
		map<wstring, bool> m_LastButtons;

		BYTE m_LeftTrigger;
		BYTE m_RightTrigger;

		CONTROLER_STATE m_pad;

		Vec2 m_LStickP2;
		Vec2 m_RStickP2;
		map<wstring, bool> m_ButtonsP2;
		map<wstring, bool> m_DownButtonsP2;
		map<wstring, bool> m_UpButtonsP2;
		map<wstring, bool> m_NowUpdateButtonsP2;
		map<wstring, bool> m_LastButtonsP2;
		BYTE m_LeftTriggerP2;
		BYTE m_RightTriggerP2;

		CONTROLER_STATE m_pad2;

	public:
		/*!
		@brief 入力マネージャーを作成
		@return 作成した入力マネージャー
		*/
		static unique_ptr<InputManager, InputManagerDeleter>& CreateInputManager();

		/*!
		@brief 入力マネージャーを取得
		@return 入力マネージャー
		*/
		static unique_ptr<InputManager, InputManagerDeleter>& GetInputManager();

		/*!
		@brief 入力マネージャーを削除
		@return なし
		*/
		static void DeleteInputManager();

		/*!
		@brief 入力マネージャーを初期化
		@return なし
		*/
		void Initialize();

		/*!
		@brief 入力マネージャーを更新
		@return なし
		*/
		void Update();

		/*!
		@brief 入力マネージャーを破棄
		@return なし
		*/
		void Discard();

		/*!
		@brief Lスティックの傾きを取得
		@return Lスティックの傾き
		*/
		Vec2 GetLStick()
		{
			return m_LStick;
		}

		/*!
		@brief Rスティックの傾きを取得
		@return Rスティックの傾き
		*/
		Vec2 GetRStick()
		{
			return m_RStick;
		}

		/*!
		@brief ボタンの状態を取得
		@param[in] button 取得したいボタンの名前
		@return ボタンの状態
		*/
		bool GetButton(wstring button)
		{
			auto it = m_Buttons.find(button);
			if (it != m_Buttons.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief ボタンの押下状態を取得
		@param[in] button 取得したいボタンの名前
		@return ボタンの押下状態
		*/
		bool GetDownButton(wstring button)
		{
			auto it = m_DownButtons.find(button);
			if (it != m_DownButtons.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief ボタンの離脱状態を取得
		@param[in] button 取得したいボタンの名前
		@return ボタンの離脱状態
		*/
		bool GetUpButton(wstring button)
		{
			auto it = m_UpButtons.find(button);
			if (it != m_UpButtons.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief ボタンの変更状態を取得
		@param[in] button 取得したいボタンの名前
		@return ボタンの変更状態
		*/
		bool GetNowUpdateButton(wstring button)
		{
			auto it = m_NowUpdateButtons.find(button);
			if (it != m_NowUpdateButtons.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief 前フレームのボタンの状態を取得
		@param[in] button 取得したいボタンの名前
		@return 前フレームのボタンの状態
		*/
		bool GetLastButton(wstring button)
		{
			auto it = m_LastButtons.find(button);
			if (it != m_LastButtons.end())
			{
				return it->second;
			}

			return false;
		}

		BYTE GetLeftTrigger() const
		{
			return m_LeftTrigger;
		}

		BYTE GetRightTrigger() const
		{
			return m_RightTrigger;
		}

		// 2P //////////////////////////////////
		/*!
		@brief Lスティックの傾きを取得
		@return Lスティックの傾き
		*/
		Vec2 GetLStick2()
		{
			return m_LStickP2;
		}

		/*!
		@brief Rスティックの傾きを取得
		@return Rスティックの傾き
		*/
		Vec2 GetRStick2()
		{
			return m_RStickP2;
		}

		/*!
		@brief ボタンの状態を取得
		@param[in] button 取得したいボタンの名前
		@return ボタンの状態
		*/
		bool GetButton2(wstring button)
		{
			auto it = m_ButtonsP2.find(button);
			if (it != m_ButtonsP2.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief ボタンの押下状態を取得
		@param[in] button 取得したいボタンの名前
		@return ボタンの押下状態
		*/
		bool GetDownButton2(wstring button)
		{
			auto it = m_DownButtonsP2.find(button);
			if (it != m_DownButtonsP2.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief ボタンの離脱状態を取得
		@param[in] button 取得したいボタンの名前
		@return ボタンの離脱状態
		*/
		bool GetUpButton2(wstring button)
		{
			auto it = m_UpButtonsP2.find(button);
			if (it != m_UpButtonsP2.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief ボタンの変更状態を取得
		@param[in] button 取得したいボタンの名前
		@return ボタンの変更状態
		*/
		bool GetNowUpdateButton2(wstring button)
		{
			auto it = m_NowUpdateButtonsP2.find(button);
			if (it != m_NowUpdateButtonsP2.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief 前フレームのボタンの状態を取得
		@param[in] button 取得したいボタンの名前
		@return 前フレームのボタンの状態
		*/
		bool GetLastButton2(wstring button)
		{
			auto it = m_LastButtonsP2.find(button);
			if (it != m_LastButtonsP2.end())
			{
				return it->second;
			}

			return false;
		}

		BYTE GetLeftTrigger2() const
		{
			return m_LeftTriggerP2;
		}

		BYTE GetRightTrigger2() const
		{
			return m_RightTriggerP2;
		}

		CONTROLER_STATE GetGamePad() const
		{
			return m_pad;
		}

		CONTROLER_STATE GetGamePad2() const
		{
			return m_pad2;
		}

		bool InputManager::GetDownButton(const wstring& key, int playerIndex)
		{
			if (playerIndex == 0)
			{
				return m_DownButtons[key];
			}
			else if (playerIndex == 1)
			{
				return m_DownButtonsP2[key];
			}
			return false;
		}

	private:
		//コピー禁止
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		//ムーブ禁止
		InputManager(const InputManager&&) = delete;
		InputManager& operator=(const InputManager&&) = delete;
	};
}
//end basecross
