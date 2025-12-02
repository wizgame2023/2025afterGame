/*!
@file BillBoard.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
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
		
		Quat Billboard(const Vec3& Line)
		{
			Vec3 Temp = Line;
			Mat4x4 RotMatrix;
			Vec3 DefUp(0, 1.0f, 0);
			Vec2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.length() < 0.1f)
			{
				DefUp = Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix.inverse();
			Quat Qt;
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
			return Qt;
		}

		void SetBillUV(Vec2 topLeft, Vec2 botRight);
		
		//スプライト変更
		void ChangeTexture(wstring spriteName);

		//サイズ変更
		void SetScale(Vec3 scale);

		//出現する高さ変更
		void SetPushY(float pushY);

		// ビルボードの削除
		void RemoveBill();
	};
}
//end basecross
