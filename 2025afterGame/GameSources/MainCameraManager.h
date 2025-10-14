/*!
@file MainCameraManager.h
@brief プレイヤーのカメラ
担当:佐藤 海斗
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	
	class MainCameraManager : public MyGameObject
	{
		weak_ptr<Transform> m_plTrans;
		shared_ptr<Player> m_player;
		shared_ptr<Stage> m_stage;
		shared_ptr<Camera> m_mainCamera;
		
		Vec3 m_plPos;
		Vec3 m_plRot;
		Vec3 m_plUp;
		Vec3 m_plFwrd;

		// 線形補間関数(滑らかに動かすためのやつ)
		// 参考 : https://taketakeshi.hatenablog.jp/entry/2025/05/19/205447
		// start : 開始地 end : 終了値 time : 補間係数(0.0f～1.0f)
		Vec3 Lerp(const Vec3& start, const Vec3& end, float time) {
			return start + (end - start) * time;
		}

	public:
		MainCameraManager::MainCameraManager(const shared_ptr<Stage>& stagePtr) :
			MyGameObject(stagePtr)
		{}

		~MainCameraManager() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}
//end basecross
