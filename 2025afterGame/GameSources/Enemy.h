/*!
@file StateEnemy.h
@brief “G‚ÌƒXƒe[ƒgˆ—
’S“–ÒFO•r—T‘¾
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Enemy:public FighterAircraftBase
	{
	private:

	public:
		Enemy(const shared_ptr<Stage>& obj,const Vec3& pos,const Quat& qt,const Vec3& scale, const shared_ptr<CheckPoint>& startCheckPoint);
		~Enemy();

		void OnCreate()override;
		void OnUpdate()override;
	};

}

//end basecross

