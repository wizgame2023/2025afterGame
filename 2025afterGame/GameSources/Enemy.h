/*!
@file StateEnemy.h
@brief 敵のステート処理
担当者：三瓶裕太
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Enemy:public FighterAircraftBase
	{
	private:
		float m_countDebagBulletTime; // デバック用の弾を出す時間計測変数

		float AdjustmentAngle(float angle);	// 角度の調整0~360度までしか出ないようにする

	public:
		Enemy(const shared_ptr<Stage>& obj,const Vec3& pos,const Quat& qt,const Vec3& scale, const shared_ptr<CheckPoint>& startCheckPoint);
		~Enemy();

		void OnCreate()override;
		void OnUpdate()override;
	};

}

//end basecross

