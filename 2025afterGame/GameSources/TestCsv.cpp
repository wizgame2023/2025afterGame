/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	TestCsv::TestCsv(const shared_ptr<Stage>& StagePtr,
		const Vec3& Pos,
		const Vec3& Rot,
		const Vec3& Siz,
		const wstring& Tag
	) :
		GameObject(StagePtr),
		m_pos(Pos),
		m_rot(Rot),
		m_siz(Siz),
		m_tag(Tag)
	{
		try
		{

		}
		catch (...)
		{
			throw;
		}
	}

	TestCsv::~TestCsv(){

	}

	void TestCsv::OnCreate()
	{
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_siz);
		PtrTrans->SetRotation(m_rot);
		PtrTrans->SetPosition(m_pos);

		//コリジョン
		auto ptrCol = AddComponent<CollisionObb>();
		ptrCol->SetFixed(true);
		ptrCol->SetDrawActive(true);
		ptrCol->SetAfterCollision(AfterCollision::Auto);

		Mat4x4 spanMat;
		

		//メッシュの描画
		auto ptrDraw = AddComponent<PNTStaticDraw>();
		

		if (m_tag == L"Desk")
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"Desk");
			ptrDraw->SetTextureResource(L"DeskTex");

			spanMat.affineTransformation(
				Vec3(0.29f, 3.0f, 0.62f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(0.0f, -6.0f, 0.0f)
			);

			ptrDraw->SetMeshToTransformMatrix(spanMat);

		}
		else if (m_tag == L"Chair")
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"Chair");
			ptrDraw->SetTextureResource(L"ChairTex");

			spanMat.affineTransformation(
				Vec3(0.45f, 3.3f, 0.45f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
				Vec3(0.0f, -5.5f, 0.0f)
			);

			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else if (m_tag == L"Bed")
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"Bed");
			ptrDraw->SetTextureResource(L"BedTex");

			spanMat.affineTransformation(
				Vec3(0.15f, 1.0f, 0.3f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(0.0f, -1.0f, 0.0f)
			);

			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else if (m_tag == L"Shelf")
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"Shelf");
			ptrDraw->SetTextureResource(L"ShelfTex");

			spanMat.affineTransformation(
				Vec3(0.11f, 0.16f, 0.2f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(0.0f, -0.54f, 0.0f)
			);

			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else if (m_tag == L"BookShelf")
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"Shelf");
			ptrDraw->SetTextureResource(L"ShelfTex");

			spanMat.affineTransformation(
				Vec3(0.11f, 0.16f, 0.2f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
				Vec3(0.0f, -0.54f, 0.0f)
			);

			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else if (m_tag == L"Clock")
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"Clock");
			ptrDraw->SetTextureResource(L"ClockTex");

			spanMat.affineTransformation(
				Vec3(0.2f, 0.2f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, XMConvertToRadians(90.0f), 0.0f),
				Vec3(0.5f, -0.6f, 0.0f)
			);

			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else if (m_tag == L"House")
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"House");
			ptrDraw->SetTextureResource(L"HouseTex");

			spanMat.affineTransformation(
				Vec3(0.2f, 0.2f, 0.43f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, -0.15f)
			);

			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else if (m_tag == L"Basket")
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"Basket");
			ptrDraw->SetTextureResource(L"BasketTex");

			spanMat.affineTransformation(
				Vec3(0.3f, 0.3f, 0.3f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);

			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else
		{
			AddTag(L"CameraObsDiffuse");
			SetAlphaActive(true);

			ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
			ptrDraw->SetTextureResource(L"TestTex");
		}
	}
}
//end basecross
