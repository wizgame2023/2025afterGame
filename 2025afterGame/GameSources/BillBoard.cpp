/*!
@file BillBoard.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	BillBoard::BillBoard(const shared_ptr<Stage>& stagePtr,
		const shared_ptr<GameObject>& actorptr,
		const wstring& spriteName,
		int layer,
		float pushX,
		float pushY,
		const Vec3& scale,
		const Col4& color):
		MyGameObject(stagePtr),
		m_actor(actorptr),
		m_textureName(spriteName),
		m_pushX(pushX),
		m_pushY(pushY),
		m_scale(scale),
		m_color(color),
		m_layer(layer)
	{
	}

	BillBoard::~BillBoard()
	{

	}

	void BillBoard::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		
		// 存在していなかったら
		if (!m_actor.expired())
		{
			auto Seekptr = m_actor.lock();
			auto SeekTransPtr = Seekptr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			Pos.x += m_pushX;
			ptrTrans->SetPosition(Pos);
			ptrTrans->SetScale(m_scale);
			ptrTrans->SetQuaternion(SeekTransPtr->GetQuaternion());

			MeshUtill::CreateSquare(1.0f, m_vertices, m_indices);
			//UV値の変更
			//左上頂点
			m_vertices[0].textureCoordinate = Vec2(0, 0);
			//右上頂点
			m_vertices[1].textureCoordinate = Vec2(1, 0);
			//左下頂点
			m_vertices[2].textureCoordinate = Vec2(0, 1.0f);
			//右下頂点
			m_vertices[3].textureCoordinate = Vec2(1, 1.0f);

			//頂点の型を変えた新しい頂点を作成(こうしないと色がつかない)
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			//// エフェクトの消える現象の解決
			//DrawComp->SetDepthStencilState(DepthStencilState::Read);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
			SetDrawActive(true);
		}
	}

	void BillBoard::OnUpdate() {
		if (m_actor.expired())
		{
			GetStage()->RemoveGameObject<BillBoard>(GetThis<BillBoard>());
		}
		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			//トランスフォーム取得
			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			Pos.x += m_pushX;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);

			auto DrawComp = GetComponent<PCTStaticDraw>();
			DrawComp->SetTextureResource(m_textureName);//テクスチャ更新

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//向きをカメラ目線にする
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);
		}
	}

	void BillBoard::SetBillUV(Vec2 topLeft, Vec2 botRight)
	{
		//UV値の変更
		//左上頂点
		m_vertices[0].textureCoordinate = Vec2(topLeft);
		//右上頂点
		m_vertices[1].textureCoordinate = Vec2(botRight.x, topLeft.y);
		//左下頂点
		m_vertices[2].textureCoordinate = Vec2(topLeft.x, botRight.y);
		//右下頂点
		m_vertices[3].textureCoordinate = Vec2(botRight);

		//新しい頂点に更新
		vector<VertexPositionColorTexture> new_vertices;
		for (auto& v : m_vertices) {
			VertexPositionColorTexture nv;
			nv.position = v.position;
			nv.color = Col4(m_color);
			nv.textureCoordinate = v.textureCoordinate;
			new_vertices.push_back(nv);
		}

		//新しい頂点を使ってメッシュリソースの作成
		m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

		//メッシュの更新
		auto DrawComp = GetComponent<PCTStaticDraw>();
		DrawComp->SetMeshResource(m_SquareMeshResource);
		DrawComp->SetTextureResource(m_textureName);
		DrawComp->SetDiffuse(m_color);
		SetAlphaActive(true);
	}

	//ビルボードのテクスチャ変更
	void BillBoard::ChangeTexture(wstring textureName)
	{
		m_textureName = textureName;
	}

	//サイズのセッター
	void BillBoard::SetScale(Vec3 scale)
	{
		m_scale = scale;
	}

	//出現する高さ変更
	void BillBoard::SetPushY(float pushY)
	{
		m_pushY = pushY;
	}

	void BillBoard::RemoveBill()
	{
		GetStage()->RemoveGameObject<BillBoard>(GetThis<BillBoard>());
	}

}
//end basecross
