/*!
@file Actor.cpp
@brief ゲーム中の動く3Dオブジェクトの親クラス
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Actor::Actor(const shared_ptr<Stage>& stagePtr) :
		MyGameObject(stagePtr)
	{

	}

	Actor::~Actor()
	{
	}

	void Actor::OnCreate()
	{
		// 親クラスのOnCreateを呼び出す
		MyGameObject::OnCreate();

		CreateChildObjects();
	}

	void Actor::OnUpdate()
	{
		// 親クラスのOnUpdateを呼び出す
		MyGameObject::OnUpdate();
	}

	// m_posのゲッタ
	Vec3 Actor::GetPos()
	{
		return m_pos;
	}

	// m_qtのゲッタ
	Quat Actor::GetQt()
	{
		return m_qt;
	}

	// m_rotのゲッタ
	Vec3 Actor::GetRot()
	{
		return m_rot;
	}

	// m_scaleのゲッタ
	Vec3 Actor::GetSclce()
	{
		return m_scale;
	}

	// 角度のゲッタ
	// 第一引数　X軸かY軸どちらの軸の角度を取るか
	float Actor::GetAngle(wstring XorY)
	{
		if (XorY == L"X")
		{
			return m_angleX;
		}
		if (XorY == L"Y")
		{
			return m_angleY;
		}

		return 0.0f;
	}

};
//end basecross