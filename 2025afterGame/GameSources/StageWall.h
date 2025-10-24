/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class StageWall :public Actor
	{
	public:
		StageWall();
		~StageWall();

		void OnCreate()override;
		void OnUpdate()override;

	};
}
//end basecross
