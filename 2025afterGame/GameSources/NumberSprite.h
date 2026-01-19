/*!
@file NumberSprite.h
@brief 数値や値
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	enum class NumberType
	{
		Minute,
		Second,
		Bullet,
		MaxBullet,
		Score,
		Ranking,
		Count,
	};

	class NumberSprite : public Sprite
	{
	private:
		NumberType m_type;
		vector<shared_ptr<Sprite>> m_digits;
		int m_number;         // 表示する数値
		int m_digit;          // 桁(0〜9)
		int m_prevNumber;     // 初期値はありえない値にしておく
		Vec3 m_pos;
		int m_layer;
		
	public:
		NumberSprite(
			const shared_ptr<Stage>& stagePtr,
			const Vec2& size,
			const Vec3& pos,
			const Vec3& rot = Vec3(0.0f, 0.0f, 0.0f),
			const Col4& color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
			int layer = 1,
			const wstring& textureName = L"Number"

		);
		virtual ~NumberSprite();//デストラクタ

		// 初期化
		void OnCreate() override;
		
		// 更新
		void OnUpdate() override;
		
		// 数値変更
		void SetNumber(int number);

		// 表示する桁数
		void SetDigit(int digit);
		
		// 数値の種類を設定
		void SetMyType(NumberType type);

		// 数字の固定表示桁数を設定する。桁数が不足する場合はゼロ埋めする
		void SetDigitCount(int digitCount);

		virtual void OnDestory();

		void SetLayer(int layer);
	};
}
//end basecross
