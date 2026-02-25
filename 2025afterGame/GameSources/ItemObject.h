/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross{
	class ItemObject : public Actor {
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_siz;
		int m_id;
		int m_repair;
		float m_countDown;
		bool m_countDownFlug;
		Vec3 m_uiPos;

		shared_ptr<BillBoard> m_billBoard;
		shared_ptr<Sprite> m_number;

	public:
		ItemObject(const shared_ptr<Stage>& StagePtr,
			const Vec3& Pos,
			const Vec3& Rot,
			const Vec3& Siz,
			const int& ID
		);
		virtual ~ItemObject();

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		int GetObjectID();

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
