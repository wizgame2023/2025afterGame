/*!
@file CheckPoint.h
@brief チェックポイント本体のヘッダー
*/

#pragma once
#include "stdafx.h"
#include "MyGameObject.h"

namespace basecross{
	class CheckPoint: public Actor
	{
	public :
		//チェックポイント等のメンバ変数
		float m_raceTime;
		float m_previewTime;
		int m_checkPointID;
		shared_ptr<Transform> m_trans;

	public :
		CheckPoint(const shared_ptr<Stage>& stagePtr);
		~CheckPoint();

		void OnCreate()override;
		void OnUpdate()override;

		//差分の時間を取得
		float GetDifferenceTime();

		//チェックポイントIDの取得
		void SetCheckPointID(int id);

		//次のチェックポイントの取得
		//Vec3 GetNextCheckPoint();

		//接触した場合の処理
		void OnCollisionEnter(shared_ptr<GameObject>& obj)override;
	};
}
//end basecross
