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
		int m_enemyIndex;
		vector<VertexPositionColorTexture> m_newVertices;

	public:
		BillBoardGauge(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<GameObject>& actorPtr,
			const wstring& spriteName,
			int layer = 2,
			float pushX = 18.0f,
			float pushY = 18.0f,
			Vec3 scale = Vec3(1.0f),
			Col4 col = Col4(1.0f),
			int enemyIndex = 0);
		~BillBoardGauge();

		void OnCreate()override;
		void OnUpdate()override;

		void SetPercent();
		void RemoveBillBoardGauge();
	};

}
//end basecross
