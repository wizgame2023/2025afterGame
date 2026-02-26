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
		bool m_affiliation; // 弾の所属(敵はfalse味方はtrue)

		weak_ptr<Actor> m_parent; // 発射元のポインタ

		Vec3 m_parentForward; // 親オブジェクトの向いている方向

		int m_damage = 10; // ダメージ量

		float m_limitLenght = 30.0f; // 制限射程
		float m_limitLenghtCount = 0.0f; // 制限射程

		float m_speed = 15.0f; // 弾のスピード

		int m_ownerId;   // 撃ったやつのID

		float m_time = 0.0f;
		float m_delay = 0.1f;        // 初動直進
		float m_turnAccel = 3.0f;    // 曲がり加速
		float m_maxTurn = 0.01f;     // 最大旋回量

		weak_ptr<Enemy> m_target;

	public:
		Bullet(const shared_ptr<Stage>& stagePtr,const shared_ptr<Actor>& parent);
		~Bullet();

		void OnCreate()override;
		void OnUpdate()override;

		// 移動用の関数
		void Move();

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;

		// 弾の所属のゲッタ
		bool GetAffiliation();
		// ダメージのセッタ
		void SetDamage(int damage);
		// ダメージのゲッタ
		int GetDamage();

		// 親オブジェクトのゲッタ
		weak_ptr<Actor> GetParentObj();

		// どのIDが所有者セッター
		void SetOwnerId(int id);
		// どのIDかゲッター
		int  GetOwnerId();
		// ターゲット探索
		void SearchTarget();
		// ホーミング旋回
		void HomingUpdate(float dt);
	};

	class TestCube :public Actor
	{
	private:
		bool m_affiliation; // 弾の所属(敵か味方か)

	public:
		TestCube(const shared_ptr<Stage>& stagePtr, Vec3 pos, Quat qt, Vec3 scale);
		~TestCube();

		void OnCreate()override;
		void OnUpdate()override;

		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};

}
//end basecross
