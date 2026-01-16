/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class RankingUI : public MyGameObject
	{
	private:
		shared_ptr<Transform> m_trans;
		int m_nowIndex;
		wstring m_rankingUI;
		wstring m_playerName;
		wstring m_scoreText;
		wstring m_TextureName;
		int m_layer;
		Vec3 m_pos;
		int m_rankingNumber;

	public:
		RankingUI::RankingUI(
		const shared_ptr<Stage>& stage,	
		const Vec2& size,
		const Vec3& pos,
		const int& m_rankingNumber,
		const Vec3& rot = Vec3(0.0f,0.0f,0.0f),
		const Col4& color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
		const int& layer = 1
		);

		virtual RankingUI::~RankingUI();

		// 初期化
		void OnCreate() override;

		// 更新
		void OnUpdate() override;

		void RankingNumber();
	};

}
//end basecross
