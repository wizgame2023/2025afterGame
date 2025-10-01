/*!
@file AttackCollision.h
@brief Player攻撃の実体
担当：吉田 智貴
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	enum class AttackType {
		Player,
		Enemy
	};

	// 攻撃判定から相手に伝える情報
	struct HitInfo {
		AttackType Type;
		int Damage;
		bool HitOnce;

		//コンストラクタ
		HitInfo::HitInfo(AttackType attacktype,
			int damage,
			bool hitonce
		)
		{
			this->Type = attacktype;
			this->Damage = damage;
			this->HitOnce = hitonce;
		}
		HitInfo::HitInfo() {
			this->Type = AttackType::Player;
			this->Damage = 0;
			this->HitOnce = false;
		}
	};

	class AttackCollision : public MyGameObject
	{
		shared_ptr<CollisionCapsule> m_collision;

		// 攻撃のスペック
		HitInfo m_info;

		// 当たったらtrueになるやつ
		bool m_moveContact;

		//持続時間
		float m_ActiveTime;

	public:
		AttackCollision::AttackCollision(const shared_ptr<Stage>& stagePtr);
		AttackCollision::~AttackCollision();

		void OnCreate() override;
		void OnUpdate() override;

		// CollisionCapsuleの取得
		shared_ptr<CollisionCapsule> GetCollisionPtr()
		{
			return m_collision;
		}

		// 判定の半径を変える
		// 引数 拡縮率
		void SetCollScale(float scale) {
			m_collision->SetMakedRadius(scale);
		}
		// 判定の高さを変える
		// 引数 拡縮率
		void SetCollHeight(float scale)
		{
			m_collision->SetMakedHeight(scale);
		}

		// コリジョンを有効化して、一定時間後に無効化する
		// 引数 有効にしておく時間
		void ActivateCollision(float activetime)
		{
			m_ActiveTime = activetime;
		}

		//攻撃判定を相手に当てた時に呼ばれる関数
		HitInfo& GetHitInfo()
		{
			return m_info;
		}
		
		// 攻撃判定を更新する
		// HitInfo 攻撃の情報をまとめた構造体
		void SetHitInfo(HitInfo info)
		{
			this->m_info = info;
		}

		// フラグを取得する関数
		bool GetMoveContact();
		
		// フラグを変更する関数
		// 引数 フラグ
		void SetMoveContact(bool mc);

	};

}
//end basecross
