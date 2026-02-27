/*!
@file Character.h
@brief ƒLƒƒƒ‰ƒNƒ^[‚È‚Ç
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross{
	class ScoreObject : public Actor {
		Vec3 m_pos;
		Vec3 m_rot;
		Vec3 m_siz;
		int m_id;
		int m_point;
		int m_score;
		int m_targeted; // ‚Ç‚Ì‚­‚ç‚¢‚Ì”‚Ì“G‚É‘_‚í‚ê‚Ä‚¢‚é‚©
		float m_countDown;
		bool m_countDownFlug;
		Vec3 m_uiPos;

		shared_ptr<BillBoard> m_billBoard;
		shared_ptr<NumberSprite> m_number;

	public:
		ScoreObject(const shared_ptr<Stage>& StagePtr,
			const Vec3& Pos,
			const Vec3& Rot,
			const Vec3& Siz,
			const int& ID,
			const int& Point
		);
		virtual ~ScoreObject();

		virtual void OnCreate() override;

		virtual void OnUpdate() override;

		int GetObjectID();

		// ‘_‚í‚ê‚é“G‚Ì”’Ç‰Á
		void Addtargeted();
		// ‘_‚í‚ê‚é“G‚Ì”ƒ}ƒCƒiƒX
		void SubtractTargeted();
		// ‘_‚í‚ê‚é“G‚Ì”‚ÌƒQƒbƒ^
		int GetTargeted();

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
