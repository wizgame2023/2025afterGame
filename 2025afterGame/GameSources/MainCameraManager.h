/*!
@file MainCameraManager.h
@brief ƒvƒŒƒCƒ„[‚ÌƒJƒƒ‰
’S“–:²“¡ ŠC“l
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
