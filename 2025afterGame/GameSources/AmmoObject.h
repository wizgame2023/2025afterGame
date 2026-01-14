/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross{
	class AmmoObject : public Actor {
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_siz;
		int m_id;
		int m_reload;

		shared_ptr<BillBoard> m_billBoard;

	public:
		AmmoObject(const shared_ptr<Stage>& StagePtr,
			const Vec3& Pos,
			const Vec3& Rot,
			const Vec3& Siz,
			const int& ID
		);
		virtual ~AmmoObject();

		virtual void OnCreate() override;

		int GetObjectID();

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
