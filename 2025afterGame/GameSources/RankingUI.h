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
		Vec3 m_pos;
		int m_rankingNumber;   // 自分は何位か
		wstring m_textureName;
		shared_ptr<NumberSprite> m_rankUI;
		shared_ptr<NumberSprite> m_scoreUI;
		shared_ptr<Sprite> m_nameSprite;

	public:
		RankingUI::RankingUI(
		const shared_ptr<Stage>& stage,	
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
	};

}
//end basecross
