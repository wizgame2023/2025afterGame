/*!
@file Bullet.h
@brief ’e‚Ìˆ—
’S“–Ò:O•r—T‘¾
*/


#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross {
	class Bullet :public Actor
	{	
	private:
		bool m_affiliation; // ’e‚ÌŠ‘®(“G‚©–¡•û‚©)

		Vec3 m_pos;
		Vec3 m_Scale;
		Quat m_Qt;

		weak_ptr<Actor> m_parent; // ”­ËŒ³‚Ìƒ|ƒCƒ“ƒ^

	public:
		Bullet(const shared_ptr<Stage>& stagePtr);
		~Bullet();

		void OnCreate()override;
		void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
