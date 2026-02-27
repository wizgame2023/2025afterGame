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
		weak_ptr<FighterAircraftBase> m_fighterBase;
		//全体の何パーセント出すか決める
		float m_parsecond;
		// インデックス情報
		vector<uint16_t> m_indices;
		int m_enemyIndex;
		vector<VertexPositionColorTexture> m_newVertices;
		shared_ptr<GameObject> actorPtr;
		bool m_spriteMoveFlag;
		// 表示
		bool m_invisibleFlag;
		// タイマー動作中
		bool  m_timerRunning = false;
		shared_ptr<PCTStaticDraw> m_drawComp;
		weak_ptr<BillBoardGauge> m_hpFrame;

	public:
		BillBoardGauge(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<FighterAircraftBase>& fighetrAircaftPtr,
			const wstring& spriteName,
			Vec3 scale,
			int layer = 2,
			float pushX = 1.0f,
			float pushY = 1.0f,
			Col4 col = Col4(1.0f),
			int enemyIndex = 0);
		~BillBoardGauge();

		void OnCreate()override;
		void OnUpdate()override;

		// HP割合の作成
		void SetPercent();
		// ビルボードの削除
		void RemoveBillBoardGauge();
		// HPが動くのかそれとも枠なのか
		void SetSpriteMove(bool flag);
		// HPがないときの透明にする
		void SetInvisible(bool flag);
	};

}
//end basecross
