/*!
@file BillBoard.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class BillBoard : public MyGameObject
	{
	protected:
		weak_ptr<GameObject> m_actor;
		wstring m_textureName;
		int m_layer;
		float m_pushX;
		float m_pushY;
		Vec3 m_scale;
		Col4 m_color;
		bool m_trackingActive;

		// 空のvectorを用意
		vector<VertexPositionNormalTexture> m_vertices;
		vector<uint16_t> m_indices;//インデックス情報
		shared_ptr<MeshResource> m_SquareMeshResource;


	public:
		BillBoard(const shared_ptr<Stage>& ptrStage,
			const shared_ptr<GameObject>& actorPtr,
			const wstring& spriteName,
			int layer = 2,
			float pushX = 18.0f,
			float pushY = 18.0f,
			const Vec3& scale = Vec3(1.0f),
			const Col4& color = Col4(1.0f));
		virtual ~BillBoard();

		// 初期化
		virtual void OnCreate() override;
		// 更新
		virtual void OnUpdate() override;

		Quat Billboard(const Vec3& Line);

		void SetBillUV(Vec2 topLeft, Vec2 botRight);

		//スプライト変更
		void ChangeTexture(wstring spriteName);

		//サイズ変更
		void SetScale(Vec3 scale);

		//出現する高さ変更
		void SetPushY(float pushY);

		//出現する高さ変更
		void SetPushX(float pushX);

		// ビルボードの削除
		void RemoveBill();

		// 追跡するBillBordにするか
		void SetTrackingActive(bool flag);
	};
}
//end basecross
