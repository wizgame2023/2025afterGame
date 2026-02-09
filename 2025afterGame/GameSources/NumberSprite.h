/*!
@file NumberSprite.h
@brief 数値や値
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	// スプライトの値を特定にしている
	enum class NumberType
	{
		None,
		Minute,
		Second,
		Bullet,
		MaxBullet,
		Score,
		Count,
		RankingNumber,
	};

	class NumberSprite : public Sprite
	{
	private:
		NumberType m_type;
		weak_ptr<FighterAircraftBase> m_fightBase;
		vector<shared_ptr<Sprite>> m_digits;
		// 表示する数値
		int m_number;
		int m_digit;          // 桁(0〜9)
		int m_prevNumber;     // 初期値はありえない値にしておく
		Vec3 m_pos;
		int m_layer;
		int m_rankingNumber;
		int m_value;
		
		// 数字の更新フラグ
		bool m_numberUpDater;

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
		// SetSignedNumber
		void SetSignedNumber(int symbol,int number);
		// 数値の種類を設定
		void SetMyType(NumberType type);
		// 数字の固定表示桁数を設定する。桁数が不足する場合はゼロ埋めする
		void SetDigitCount(int digitCount);
		// 削除
		virtual void OnDestory();
		// UIが生成された時のカウント
		void AddRankingNumberCount(int number);
		// Numberの更新のセッター
		void SetNumberUpdateFlag(bool flag);
		// Numberの更新のゲッター
		bool GetNumberUpdateFlag();

		void SetNumberLayer(int number);
	};
}
//end basecross
