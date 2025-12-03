/*!
@file BillBoardGauge.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	BillBoardGauge::BillBoardGauge(
		const shared_ptr<Stage>& stagePtr,
		const shared_ptr<GameObject>& actorPtr,
		const wstring& spriteName,
		int layer,
		float pushX,
		float pushY,
		Vec3 scale,
		Col4 col,
		int enemyIndex) :
		BillBoard(stagePtr, actorPtr, spriteName, layer, pushX, pushY, scale,col),
		m_indices(vector<uint16_t>()),
		m_parsecond(1.0f),
		m_enemyIndex(enemyIndex)
	{

	}

	BillBoardGauge::~BillBoardGauge()
	{

	}

	void BillBoardGauge::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();

		if (!m_actor.expired()) {
			auto SeekPtr = m_actor.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += m_pushY;
			ptrTrans->SetPosition(Pos);
			ptrTrans->SetScale(m_scale);
			ptrTrans->SetQuaternion(SeekTransPtr->GetQuaternion());
			//変更できるスクエアリソースを作成

			//Squareの作成(ヘルパー関数を利用)
			MeshUtill::CreateSquare(1.0f, m_vertices, m_indices);

			//UV値の変更
			//左上頂点
			m_vertices[0].textureCoordinate = Vec2(0.0f, 0.0f);
			//右上頂点
			m_vertices[1].textureCoordinate = Vec2(1.0f * m_parsecond, 0.0f);
			//左下頂点
			m_vertices[2].textureCoordinate = Vec2(0.0f, 1.0f);
			//右下頂点
			m_vertices[3].textureCoordinate = Vec2(1.0f * m_parsecond, 1.0f);

			//頂点の型を変えた新しい頂点を作成
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);//赤
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}

			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, m_indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			DrawComp->SetDepthStencilState(DepthStencilState::Read);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
			SetDrawActive(true);
		}
	}

	void BillBoardGauge::OnUpdate()
	{
		if (m_parsecond <= 0.0f)
		{
			RemoveBillBoardGauge();
			return;
		}

		SetPercent();

		if (!m_actor.expired()) {
			auto seekPtr = m_actor.lock();
			auto seekPtrTrans = seekPtr->GetComponent<Transform>();

			//アニメーション処理/////////////////////////////////////////
			m_vertices[1].position.x = -0.5f + (1.0f * m_parsecond);
			m_vertices[3].position.x = -0.5f + (1.0f * m_parsecond);
			m_vertices[1].position.y = 0.5f;
			m_vertices[3].position.y = -0.5f;
			//UV座標の設定		
			auto move = (1.0f * m_parsecond);
			m_vertices[1].textureCoordinate.x = move;
			m_vertices[3].textureCoordinate.x = move;
			////////////////////////////////////////////////////////////

			//新しい頂点に更新
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);//赤
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//新しい頂点を使ってメッシュリソースの作成
			m_SquareMeshResource->UpdateVirtexBuffer<VertexPositionColorTexture>(new_vertices);

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = seekPtrTrans->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);

			//メッシュの更新
			auto DrawComp = GetComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(m_textureName);
			// エフェクトの消える現象の解決
			DrawComp->SetDepthStencilState(DepthStencilState::Read);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//向きをカメラ目線にする
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}
	}

	//何パーセントテクスチャを出すか決める
	void BillBoardGauge::SetPercent()
	{
		auto& ui = UIManager::GetUIManager();

		// 敵のHP配列を取得
		auto currents = ui->GetCurrentEnemyHP();
		auto maxs = ui->GetMaxEnemyHP();

		// 自分が何番目の敵か
		float current = (float)currents[m_enemyIndex];
		float maxHP = (float)maxs[m_enemyIndex];

		// 割合を作る
		if (maxHP <= 0)
		{
			m_parsecond = 0.0f;
		}
		else
		{
			m_parsecond = current / maxHP;
		}

		m_parsecond = clamp(m_parsecond, 0.0f, 1.0f);
	}


	void BillBoardGauge::RemoveBillBoardGauge()
	{
		GetStage()->RemoveGameObject<BillBoardGauge>(GetThis<BillBoardGauge>());
	}
}
//end basecross
