/*!
@file MyGameObject.h
@brief 全てのゲームのオブジェクトの元となるクラス
*/


#pragma once
#include "stdafx.h"

namespace basecross {
	class MyGameObject :public GameObject
	{	
	private:
		bool m_PauseFlag = false;

	protected:
		float m_delta = 0.0f;

		// =======================↓海斗作成↓========================== //
	private:
		// デバッグ用文字列ストリーム
		wstringstream m_debugWss;

		// SE音量
		float m_SEVolume;

	protected:
		// デバッグログ　
		// 使う場合は必ずFlushDebugLogも呼び出すこと
		// name : ログの名前
		// debug : ログに出力する値
		template<typename T>
		void DebugLog(const wstring& name, T debug)
		{
			m_debugWss << name << debug << "\n";
		}

		// デバッグログを使う場合はフレームの最後(OnUpdateの末尾)に呼び出す
		// そうしないと画面に表示されません
		void FlushDebugLog();

		// バイナリパスの取得
		wstring GetBinaryPath() const;

	public:
		void SetSEVolume(const float volume);
		float GetSEVolume() const;
		// =======================↑海斗作成↑========================== //

	public:
		MyGameObject(const shared_ptr<Stage>& stagePtr);
		~MyGameObject();

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		// ポーズフラグのゲッタセッタ
		virtual bool GetPauseFlag();
		virtual void SetPauseFlag(bool Pause);
	};
}
//end basecross
