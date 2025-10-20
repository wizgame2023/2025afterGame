/*!
@file InputManager.cpp
@brief 入力マネージャー
@authors 吉田智貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	unique_ptr<InputManager, InputManager::InputManagerDeleter> InputManager::m_InputManager;

	InputManager::InputManager()
	{
	}

	// シングルトン構築
	unique_ptr<InputManager, InputManager::InputManagerDeleter>& InputManager::CreateInputManager()
	{
		try
		{
			if (m_InputManager.get() == 0)
			{
				//自分自身の構築
				m_InputManager.reset(new InputManager());
				// 初期化
				m_InputManager->Initialize();
			}
			return m_InputManager;
		}
		catch (...)
		{
			throw;
		}
	}

	// シングルトンアクセサ
	unique_ptr<InputManager, InputManager::InputManagerDeleter>& InputManager::GetInputManager()
	{
		try
		{
			if (m_InputManager.get() == 0)
			{
				throw BaseException(
					L"InputManagerがまだ作成されてません",
					L"if (m_InputManager.get() == 0)",
					L"InputManager::GetInputManager()"
				);
			}
			return m_InputManager;
		}
		catch (...)
		{
			throw;
		}
	}

	// 強制破棄
	void InputManager::DeleteInputManager()
	{
		if (m_InputManager.get())
		{
			// 削除処理
			m_InputManager->Discard();
			// 自分自身の破棄
			m_InputManager.reset();
		}
	}

	// 初期化
	void InputManager::Initialize()
	{
		auto& app = App::GetApp();
		auto input = app->GetInputDevice();
		
		m_pad = input.GetControlerVec()[0];

		m_LStick = Vec2(m_pad.fThumbLX, m_pad.fThumbLY);
		m_RStick = Vec2(m_pad.fThumbRX, m_pad.fThumbRY);

		m_LeftTrigger = m_pad.bLeftTrigger;
		m_RightTrigger = m_pad.bRightTrigger;

		// Button
		{
			m_Buttons[L"DUp"] = m_pad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_Buttons[L"DDown"] = m_pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_Buttons[L"DLeft"] = m_pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_Buttons[L"DRight"] = m_pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_Buttons[L"Start"] = m_pad.wButtons & XINPUT_GAMEPAD_START;
			m_Buttons[L"Back"] = m_pad.wButtons & XINPUT_GAMEPAD_BACK;

			m_Buttons[L"LStick"] = m_pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_Buttons[L"RStick"] = m_pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_Buttons[L"L"] = m_pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_Buttons[L"R"] = m_pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_Buttons[L"A"] = m_pad.wButtons & XINPUT_GAMEPAD_A;
			m_Buttons[L"B"] = m_pad.wButtons & XINPUT_GAMEPAD_B;
			m_Buttons[L"X"] = m_pad.wButtons & XINPUT_GAMEPAD_X;
			m_Buttons[L"Y"] = m_pad.wButtons & XINPUT_GAMEPAD_Y;
		}

		// DownButton
		{
			m_DownButtons[L"DUp"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_DownButtons[L"DDown"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_DownButtons[L"DLeft"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_DownButtons[L"DRight"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_DownButtons[L"Start"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_START;
			m_DownButtons[L"Back"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_BACK;

			m_DownButtons[L"LStick"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_DownButtons[L"RStick"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_DownButtons[L"L"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_DownButtons[L"R"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_DownButtons[L"A"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_A;
			m_DownButtons[L"B"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_B;
			m_DownButtons[L"X"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_X;
			m_DownButtons[L"Y"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_Y;
		}

		// UpButton
		{
			m_UpButtons[L"DUp"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_UpButtons[L"DDown"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_UpButtons[L"DLeft"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_UpButtons[L"DRight"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_UpButtons[L"Start"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_START;
			m_UpButtons[L"Back"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_BACK;

			m_UpButtons[L"LStick"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_UpButtons[L"RStick"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_UpButtons[L"L"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_UpButtons[L"R"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_UpButtons[L"A"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_A;
			m_UpButtons[L"B"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_B;
			m_UpButtons[L"X"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_X;
			m_UpButtons[L"Y"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_Y;
		}

		// NowUpdateButton
		{
			m_NowUpdateButtons[L"DUp"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_NowUpdateButtons[L"DDown"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_NowUpdateButtons[L"DLeft"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_NowUpdateButtons[L"DRight"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_NowUpdateButtons[L"Start"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_START;
			m_NowUpdateButtons[L"Back"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_BACK;

			m_NowUpdateButtons[L"LStick"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_NowUpdateButtons[L"RStick"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_NowUpdateButtons[L"L"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_NowUpdateButtons[L"R"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_NowUpdateButtons[L"A"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_A;
			m_NowUpdateButtons[L"B"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_B;
			m_NowUpdateButtons[L"X"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_X;
			m_NowUpdateButtons[L"Y"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_Y;
		}

		// LastButton
		{
			m_LastButtons[L"DUp"] = m_pad.wLastButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_LastButtons[L"DDown"] = m_pad.wLastButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_LastButtons[L"DLeft"] = m_pad.wLastButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_LastButtons[L"DRight"] = m_pad.wLastButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_LastButtons[L"Start"] = m_pad.wLastButtons & XINPUT_GAMEPAD_START;
			m_LastButtons[L"Back"] = m_pad.wLastButtons & XINPUT_GAMEPAD_BACK;

			m_LastButtons[L"LStick"] = m_pad.wLastButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_LastButtons[L"RStick"] = m_pad.wLastButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_LastButtons[L"L"] = m_pad.wLastButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_LastButtons[L"R"] = m_pad.wLastButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_LastButtons[L"A"] = m_pad.wLastButtons & XINPUT_GAMEPAD_A;
			m_LastButtons[L"B"] = m_pad.wLastButtons & XINPUT_GAMEPAD_B;
			m_LastButtons[L"X"] = m_pad.wLastButtons & XINPUT_GAMEPAD_X;
			m_LastButtons[L"Y"] = m_pad.wLastButtons & XINPUT_GAMEPAD_Y;
		}
	}

	// 更新
	void InputManager::Update()
	{
		auto& app = App::GetApp();
		auto input = app->GetInputDevice();
		
		auto pads = input.GetControlerVec();
		bool isPressed0 = pads[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
		bool isPressed1 = pads[1].wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
		bool isPressed = isPressed0 || isPressed1;

		if (isPressed && !m_wasPressed)
		{
			m_padChange = !m_padChange;
		}

		m_pad = pads[m_padChange ? 1 : 0];
		// 前回の押下状態を更新
		m_wasPressed = isPressed;

		m_LStick = Vec2(m_pad.fThumbLX, m_pad.fThumbLY);
		m_RStick = Vec2(m_pad.fThumbRX, m_pad.fThumbRY);

		// Button
		{
			m_Buttons[L"DUp"] = m_pad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_Buttons[L"DDown"] = m_pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_Buttons[L"DLeft"] = m_pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_Buttons[L"DRight"] = m_pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_Buttons[L"Start"] = m_pad.wButtons & XINPUT_GAMEPAD_START;
			m_Buttons[L"Back"] = m_pad.wButtons & XINPUT_GAMEPAD_BACK;

			m_Buttons[L"LStick"] = m_pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_Buttons[L"RStick"] = m_pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_Buttons[L"L"] = m_pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_Buttons[L"R"] = m_pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_Buttons[L"A"] = m_pad.wButtons & XINPUT_GAMEPAD_A;
			m_Buttons[L"B"] = m_pad.wButtons & XINPUT_GAMEPAD_B;
			m_Buttons[L"X"] = m_pad.wButtons & XINPUT_GAMEPAD_X;
			m_Buttons[L"Y"] = m_pad.wButtons & XINPUT_GAMEPAD_Y;
		}

		// DownButton
		{
			m_DownButtons[L"DUp"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_DownButtons[L"DDown"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_DownButtons[L"DLeft"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_DownButtons[L"DRight"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_DownButtons[L"Start"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_START;
			m_DownButtons[L"Back"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_BACK;

			m_DownButtons[L"LStick"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_DownButtons[L"RStick"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_DownButtons[L"L"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_DownButtons[L"R"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_DownButtons[L"A"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_A;
			m_DownButtons[L"B"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_B;
			m_DownButtons[L"X"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_X;
			m_DownButtons[L"Y"] = m_pad.wPressedButtons & XINPUT_GAMEPAD_Y;
		}

		// UpButton
		{
			m_UpButtons[L"DUp"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_UpButtons[L"DDown"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_UpButtons[L"DLeft"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_UpButtons[L"DRight"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_UpButtons[L"Start"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_START;
			m_UpButtons[L"Back"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_BACK;

			m_UpButtons[L"LStick"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_UpButtons[L"RStick"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_UpButtons[L"L"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_UpButtons[L"R"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_UpButtons[L"A"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_A;
			m_UpButtons[L"B"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_B;
			m_UpButtons[L"X"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_X;
			m_UpButtons[L"Y"] = m_pad.wReleasedButtons & XINPUT_GAMEPAD_Y;
		}

		// NowUpdateButton
		{
			m_NowUpdateButtons[L"DUp"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_NowUpdateButtons[L"DDown"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_NowUpdateButtons[L"DLeft"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_NowUpdateButtons[L"DRight"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_NowUpdateButtons[L"Start"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_START;
			m_NowUpdateButtons[L"Back"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_BACK;

			m_NowUpdateButtons[L"LStick"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_NowUpdateButtons[L"RStick"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_NowUpdateButtons[L"L"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_NowUpdateButtons[L"R"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_NowUpdateButtons[L"A"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_A;
			m_NowUpdateButtons[L"B"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_B;
			m_NowUpdateButtons[L"X"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_X;
			m_NowUpdateButtons[L"Y"] = m_pad.wNowUpdateButtons & XINPUT_GAMEPAD_Y;
		}

		// LastButton
		{
			m_LastButtons[L"DUp"] = m_pad.wLastButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_LastButtons[L"DDown"] = m_pad.wLastButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_LastButtons[L"DLeft"] = m_pad.wLastButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_LastButtons[L"DRight"] = m_pad.wLastButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_LastButtons[L"Start"] = m_pad.wLastButtons & XINPUT_GAMEPAD_START;
			m_LastButtons[L"Back"] = m_pad.wLastButtons & XINPUT_GAMEPAD_BACK;

			m_LastButtons[L"LStick"] = m_pad.wLastButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_LastButtons[L"RStick"] = m_pad.wLastButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_LastButtons[L"L"] = m_pad.wLastButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_LastButtons[L"R"] = m_pad.wLastButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_LastButtons[L"A"] = m_pad.wLastButtons & XINPUT_GAMEPAD_A;
			m_LastButtons[L"B"] = m_pad.wLastButtons & XINPUT_GAMEPAD_B;
			m_LastButtons[L"X"] = m_pad.wLastButtons & XINPUT_GAMEPAD_X;
			m_LastButtons[L"Y"] = m_pad.wLastButtons & XINPUT_GAMEPAD_Y;
		}


		wstringstream wss(L"");
		wss << "m_padChange: " << (m_padChange ? 1 : 0);

		auto scene = app->GetScene<Scene>();
		scene->SetDebugString(wss.str());

	}

	// 破棄
	void InputManager::Discard()
	{

	}
}
//end basecross
