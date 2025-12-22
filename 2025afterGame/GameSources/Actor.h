/*!
@file Actor.h
@brief ゲーム中の動く3Dオブジェクトの親クラス
*/

#pragma once
#include "stdafx.h"
#include "AttackCollision.h"

namespace basecross {
	class Actor :public MyGameObject
	{
	protected:
		//攻撃判定
		shared_ptr<AttackCollision> m_attackCol;

		shared_ptr<Transform> m_trans; // Transform

		Vec3 m_pos;		// 位置
		Vec3 m_scale;	// サイズ
		Vec3 m_rot;     // 回転度

		Quat m_qt;  // クォータニオン

		Col4 m_color; // オブジェクトの色

		float m_angleX = 0.0f; // 向いている方向X軸
		float m_angleY = 0.0f; // 向いている方向Y軸


	private:

	public:
		Actor(const shared_ptr<Stage>& stagePtr);
		Actor(const shared_ptr<Stage>& stagePtr, const Vec3& pos, const Vec3& rot, const Vec3& scale, const Col4& color = Col4(1.0f));
		Actor(const shared_ptr<Stage>& stagePtr, const Vec3& pos, const Quat& qt, const Vec3& scale, const Col4& color = Col4(1.0f));
		~Actor();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		// 判定用の子オブジェクトを生成し、Actorに紐付ける
		virtual void CreateChildObjects()
		{
			auto stage = GetStage();

			m_attackCol = stage->AddGameObject<AttackCollision>();	
			auto ptrColTrans = m_attackCol->GetComponent<Transform>();
			auto colPos = ptrColTrans->GetPosition();
			ptrColTrans->SetParent(dynamic_pointer_cast<GameObject>(GetThis<Actor>()));
			ptrColTrans->SetPosition(Vec3(colPos.x, colPos.y, 1.0f));
		}

		virtual void DestroyChildObjects()
		{
			if (m_attackCol)
			{
				auto stage = GetStage();
				if (stage)
				{
					stage->RemoveGameObject<AttackCollision>(m_attackCol);
				}
				m_attackCol.reset();
			}
		}

		// 攻撃判定のポインタを取得
		shared_ptr<AttackCollision> GetAttackPtr() 
		{
			return m_attackCol;
		}

		// m_posのゲッタ
		Vec3 GetPos();
		// m_qtのゲッタ
		Quat GetQt();
		// m_rotのゲッタ
		Vec3 GetRot();
		// m_sclceのゲッタ
		Vec3 GetSclce();
		// m_colorのゲッタ
		Col4 GetColor();

		// 角度のゲッタ
		// 第一引数　X軸かY軸どちらの軸の角度を取るか
		float GetAngle(wstring XorY);

	};
}
//end basecross
