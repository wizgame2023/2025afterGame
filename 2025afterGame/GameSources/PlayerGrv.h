/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class PlayerGrv : public MyGameObject
	{
		shared_ptr<CollisionSphere> m_collision;

		int m_flyCount;
		Vec3 m_bindPos = Vec3(0, -.5, 0);
		float m_defaultRadius = .1f;

		bool m_isGrounded;

	public :
		PlayerGrv(const shared_ptr<Stage>& stagePtr);
		PlayerGrv(const shared_ptr<Stage>& stagePtr, float collidecnt);
		~PlayerGrv();

		bool GetLand();
		void SetBindPos(Vec3 pos);
		void SetCollScale(float scale);

		void OnCreate() override;
		void OnUpdate() override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& other) override;
	};
}
//end basecross
