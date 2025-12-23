/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class ObstaclesDodge : public Actor {
	public:
		ObstaclesDodge(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 rot, Vec3 scale, Col4 color = Col4(0.0f));
		~ObstaclesDodge();

		void OnCreate();
	};
}