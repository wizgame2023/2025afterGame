/*!
@file BillBoardGauge.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class BillBoardGauge : public BillBoard
	{
	private:
		float m_parsecond;//全体の何パーセント出すか決める
		vector<uint16_t> m_indices;// インデックス情報
	
	public:
		BillBoardGauge(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<GameObject>& actorPtr,
			const wstring& spriteName,
			int layer = 2,
			float pushX = 18.0f,
			float pushY = 18.0f,
			Vec3 scale = Vec3(1.0f),
			Col4 col = Col4(1.0f));
		~BillBoardGauge();

		void OnCreate()override;
		void OnUpdate()override;

		void SetPercent(float parcent);//頂点の更新
	};

}
//end basecross
