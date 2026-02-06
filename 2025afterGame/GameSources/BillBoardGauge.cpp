/*!
@file BillBoardGauge.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	BillBoardGauge::BillBoardGauge(
		const shared_ptr<Stage>& stagePtr,
		const shared_ptr<FighterAircraftBase>& fighetrAircaftPtr,
		const wstring& spriteName,
		Vec3 scale,
		int layer,
		float pushX,
		float pushY,
		Col4 col,
		int enemyIndex) :
		BillBoard(stagePtr, actorPtr, spriteName, layer, pushX, pushY, scale,col),
		m_indices(vector<uint16_t>()),
		m_parsecond(1.0f),
		m_enemyIndex(enemyIndex),
		m_fighterBase(fighetrAircaftPtr),
		m_spriteMoveFlag(false),
		m_invisibleFlag(false)
	{

	}

	BillBoardGauge::~BillBoardGauge()
	{

	}

	void BillBoardGauge::OnCreate()
	{
		auto ptrTrans = GetComponent<Transform>();
		auto& app = App::GetApp();
		auto scene = app->GetScene<Scene>();
		auto stage = scene->GetActiveStage();


		// 存在していなかったら
		if (!m_fighterBase.expired())
		{
			auto seekPtr = m_fighterBase.lock();
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
			m_drawComp = AddComponent<PCTStaticDraw>();
			m_drawComp->SetMeshResource(m_SquareMeshResource);
			m_drawComp->SetTextureResource(m_textureName);
			SetAlphaActive(true);
			SetDrawLayer(m_layer);
		}
	}

	void BillBoardGauge::OnUpdate()
	{
		if (m_fighterBase.expired())
		{
			RemoveBillBoardGauge();
		}

		float dt = App::GetApp()->GetElapsedTime();

		// 表示要求が来た瞬間
		if (m_invisibleFlag && !m_timerRunning)
		{
			SetDrawActive(false);
			m_timerRunning = true;
			m_delta = 0.0f;
		}

		// タイマー進行中
		if (m_timerRunning)
		{
			m_delta += dt;
			
			// 1秒経過後表示
			if (m_delta >= 6.0f) 
			{
				SetDrawActive(true);
				m_invisibleFlag = false;
				m_timerRunning = false;
			}
		}

		if (m_spriteMoveFlag)
		{
			SetPercent();
		}

		if (!m_fighterBase.expired())
		{
			m_vertices[1].position.x = -0.5f + (1.0f * m_parsecond);
			m_vertices[3].position.x = -0.5f + (1.0f * m_parsecond);
			m_vertices[1].position.y = 0.5f;
			m_vertices[3].position.y = -0.5f;
			auto move = (1.0f * m_parsecond);
			m_vertices[1].textureCoordinate.x = move;
			m_vertices[3].textureCoordinate.x = move;

			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : m_vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}

			m_SquareMeshResource->UpdateVirtexBuffer<VertexPositionColorTexture>(new_vertices);
			
			//
			auto seekPtr = m_fighterBase.lock();
			auto seekPtrTrans = seekPtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = seekPtrTrans->GetPosition();
			Pos.y += m_pushY;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(m_scale);

			m_drawComp = GetComponent<PCTStaticDraw>();
			m_drawComp->SetMeshResource(m_SquareMeshResource);
			m_drawComp->SetTextureResource(m_textureName);

			// 
			if (m_spriteMoveFlag)
			{
				m_drawComp->SetDepthStencilState(DepthStencilState::None);
			}
			else
			{
				m_drawComp->SetDepthStencilState(DepthStencilState::Read);
			}

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);
		}

	}

	//何パーセントテクスチャを出すか決める
	void BillBoardGauge::SetPercent()
	{
		float current = (float)m_fighterBase.lock()->GetHpCurrent();
		float maxHP = (float)m_fighterBase.lock()->GetHpMax();

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


		if(m_parsecond < 0.4f)
		{
			m_color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (m_parsecond < 0.8f)
		{
			m_color = Col4(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			m_color = Col4(0.0f, 1.0f, 0.0f, 1.0f);
		}


		m_drawComp->SetDiffuse(m_color);
	}

	void BillBoardGauge::RemoveBillBoardGauge()
	{
		GetStage()->RemoveGameObject<BillBoardGauge>(GetThis<BillBoardGauge>());
	}

	void BillBoardGauge::SetSpriteMove(bool flag)
	{
		m_spriteMoveFlag = flag;
	}

	void BillBoardGauge::SetInvisible(bool flag)
	{
		m_invisibleFlag = flag;
	}
}
//end basecross
