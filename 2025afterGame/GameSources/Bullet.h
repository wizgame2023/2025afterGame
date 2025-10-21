/*!
@file Bullet.h
@brief 弾の処理
担当者:三瓶裕太
*/


#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross {
	class Bullet :public Actor
	{	
	private:
		bool m_affiliation; // 弾の所属(敵か味方か)

		weak_ptr<Actor> m_parent; // 発射元のポインタ

		Vec3 m_parentForward; // 親オブジェクトの向いている方向

		float m_limitLenght = 30.0f; // 制限射程
		float m_limitLenghtCount = 0.0f; // 制限射程

		float m_speed = 15.0f; // 弾のスピード

	public:
		Bullet(const shared_ptr<Stage>& stagePtr,const shared_ptr<Actor>& parent);
		~Bullet();

		void OnCreate()override;
		void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};

	class TestCube :public Actor
	{
	private:
		bool m_affiliation; // 弾の所属(敵か味方か)

		Vec3 m_pos;
		Vec3 m_Scale;
		Quat m_Qt;

	public:
		TestCube(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat qt, Vec3 scale);
		~TestCube();

		void OnCreate()override;
		void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};

}
//end basecross
