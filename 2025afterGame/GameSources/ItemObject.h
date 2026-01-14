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

		shared_ptr<BillBoard> m_billBoard;

	public:
		ItemObject(const shared_ptr<Stage>& StagePtr,
			const Vec3& Pos,
			const Vec3& Rot,
			const Vec3& Siz,
			const int& ID
		);
		virtual ~ItemObject();

		virtual void OnCreate() override;

		int GetObjectID();

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
