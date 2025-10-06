/*!
@file MainCamera.h
@brief ƒvƒŒƒCƒ„[‚ÌƒJƒƒ‰
’S“–:²“¡ ŠC“l
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	
	class MainCamera : public MyGameObject
	{
		shared_ptr<Player> m_player;
		shared_ptr<Stage> m_stage;
		shared_ptr<Camera> m_mainCamera;

		Vec3 m_plPos;

	public:
		MainCamera::MainCamera() :
			MyGameObject(m_stage)
		{}

		~MainCamera() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void SetEye(const Vec3& eye)
		{
			m_mainCamera->SetEye(eye);
		}

		void SetAt(const Vec3& at)
		{
			m_mainCamera->SetAt(at);
		}
	};
}
//end basecross
