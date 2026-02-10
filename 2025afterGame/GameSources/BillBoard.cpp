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
		m_layer(layer),
		m_trackingActive(false)
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
			auto seekPtr = m_actor.lock();
			auto seekPtrTrans = seekPtr->GetComponent<Transform>();
			auto pos = seekPtrTrans->GetPosition();
			pos.x += m_pushX;
			pos.y += m_pushY;
			ptrTrans->SetPosition(pos);
			ptrTrans->SetScale(m_scale);
			ptrTrans->SetQuaternion(seekPtrTrans->GetQuaternion());
			//Squareの作成(ヘルパー関数を利用)
			MeshUtill::CreateSquare(1.0f, m_vertices, m_indices);
			
			//UV値の変更
			//左上頂点
			m_vertices[0].textureCoordinate = Vec2(0.0f, 0.0f);
			//右上頂点
			m_vertices[1].textureCoordinate = Vec2(1.0f, 0.0f);
			//左下頂点
			m_vertices[2].textureCoordinate = Vec2(0.0f, 1.0f);
			//右下頂点
			m_vertices[3].textureCoordinate = Vec2(1.0f, 1.0f);

			//頂点の型を変えた新しい頂点を作成
			// PCTを使いたいので頂点からPosition、Color、UVだけ取って
			// new_verticesに入れている
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices)
			{
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}

			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			// メッシュ作成
			auto drawComp = AddComponent<PCTStaticDraw>();
			drawComp->SetMeshResource(m_SquareMeshResource);
			drawComp->SetTextureResource(m_textureName);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
		}
	}

	void BillBoard::OnUpdate()
	{
		//　actorがいなかったら
		if (m_actor.expired())
		{
			RemoveBill();
		}

		if(!m_actor.expired() && m_trackingActive)
		{
			auto seekPtr = m_actor.lock();
			auto seekPtrTrans = seekPtr->GetComponent<Transform>();

			//トランスフォーム取得
			auto ptrTrans = GetComponent<Transform>();
			auto pos = seekPtrTrans->GetPosition();
			pos.y += m_pushY;
			ptrTrans->SetPosition(pos);
			ptrTrans->SetScale(m_scale);

			auto DrawComp = GetComponent<PCTStaticDraw>();
			DrawComp->SetTextureResource(m_textureName);//テクスチャ更新

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//向きをカメラ目線にする
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			ptrTrans->SetQuaternion(Qt);
		}


		// actorがいたら
		if (!m_actor.expired() && !m_trackingActive) {
			auto seekPtr = m_actor.lock();
			auto seekPtrTrans = seekPtr->GetComponent<Transform>();

			//トランスフォーム取得
			auto ptrTrans= GetComponent<Transform>();
			auto pos = ptrTrans->GetPosition();
			ptrTrans->SetPosition(pos);
			ptrTrans->SetScale(m_scale);

			auto DrawComp = GetComponent<PCTStaticDraw>();
			DrawComp->SetTextureResource(m_textureName);//テクスチャ更新

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//向きをカメラ目線にする
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			ptrTrans->SetQuaternion(Qt);
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

	Quat BillBoard::Billboard(const Vec3& Line)
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

	void BillBoard::SetPushX(float pushX)
	{
		m_pushX = pushX;
	}

	void BillBoard::RemoveBill()
	{
		GetStage()->RemoveGameObject<BillBoard>(GetThis<BillBoard>());
	}

	void BillBoard::SetTrackingActive(bool flag)
	{
		m_trackingActive = flag;
	}
}
//end basecross
