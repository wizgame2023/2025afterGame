/*!
@file NumberSprite.h
@brief 数値や値
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	enum class NumberType
	{
		minute,
		second,
		Bullet,
		MaxBullet,
		Score
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
		
	public:
		NumberSprite(
			const shared_ptr<Stage>& stagePtr,
			const wstring& textureName,
			const Vec2& size,
			const Vec3& pos,
			const Vec3& rot = Vec3(0.0f, 0.0f, 0.0f),
			const Col4& color = Col4(1.0f, 1.0f, 1.0f, 1.0f),
			int layer = 1
		);
		virtual ~NumberSprite();//デストラクタ

		void OnCreate() override;
		void OnUpdate() override;         // 更新(必要なら)
		void SetNumber(int number);       // 数値変更
		void SetDigit(int digit);
		void SetMyType(NumberType type);
		void SetDigitCount(int digitCount);
	};
}
//end basecross
