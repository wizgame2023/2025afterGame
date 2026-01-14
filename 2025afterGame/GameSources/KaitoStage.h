/*!
@file KaitoStage.h
@brief 個人作業用ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class KaitoStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		wstringstream m_debugWss;

		// BGM、SE用
		shared_ptr<XAudio2Manager> m_AudioManager;
		shared_ptr<SoundItem> m_bgm;
	public:
		//構築と破棄
		KaitoStage() :Stage() {}
		virtual ~KaitoStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate() override;

		template<typename T>
		void DebugLog(const wstring& name, T debug)
		{
			m_debugWss << name << debug << "\n";
		}

		void FlushDebugLog()
		{
			auto& app = App::GetApp();
			auto scene = app->GetScene<Scene>();
			scene->SetDebugString(m_debugWss.str());
			m_debugWss.str(L""); // ログをクリア
			m_debugWss.clear();
		}

	};

	class TestCubeKaito : public Actor
	{
		Vec3 m_startPos;
		Vec3 m_startScl;
		
	public:
		TestCubeKaito(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale);
		~TestCubeKaito();
		virtual void OnCreate() override;
		//virtual void OnUpdate() override;
		// 当たり判定
		//virtual void OnCollisionEnter(shared_ptr<GameObject>& obj) override;
	};

}
//end basecross

