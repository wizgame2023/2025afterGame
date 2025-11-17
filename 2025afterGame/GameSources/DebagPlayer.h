/*!
@file DebagPlayer.h
@brief デバック用のプレイヤー
担当：三瓶 裕太
*/

#pragma once
#include "stdafx.h"
#include "Actor.h"
#include "Barrier.h"
#include "Bullet.h"
#include "CheckPoint.h"

namespace basecross {

	class DebagPlayer : public FighterAircraftBase
	{
	private:
		shared_ptr<Barrier> m_barrier;
		shared_ptr<Bullet> m_bullet;
		shared_ptr<CheckPoint> m_checkPoint;

		Vec3 m_velocity;
		Vec3 m_checkPointPos;

		float m_angleSpeed;
		float m_rollSpeed;
		bool m_acceleration;

		Quat m_initialQuat;

		bool m_prevDDown;
		int  m_playerIndex;
		bool m_aButton;

		float m_yawSpeed;
		Quat  m_targetQuat;

		Vec3 m_respawnPos;

	public:
		DebagPlayer::DebagPlayer(const shared_ptr<Stage>& ptrStage);
		DebagPlayer::~DebagPlayer();

		void OnCreate() override;
		void OnUpdate() override;
		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;

	};
}
//end basecross

