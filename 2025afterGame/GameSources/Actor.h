/*!
@file Actor.h
@brief ゲーム中の動く3Dオブジェクトの親クラス
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"
#include "AttackCollision.h"

namespace basecross {
	class Actor :public MyGameObject
	{
	protected:
		//攻撃判定
		shared_ptr<AttackCollision> m_attackCol;

	private:

	public:
		Actor(const shared_ptr<Stage>& stagePtr);
		~Actor();

		void OnCreate() override;

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
	};
}
//end basecross
