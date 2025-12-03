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
		ptrCol->SetDrawActive(false);
		ptrCol->SetAfterCollision(AfterCollision::Auto);

		Mat4x4 spanMat1;
		spanMat1.affineTransformation(
			Vec3(0.3f, 0.7f, 0.3f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.7f, 0.0f)
		);

		Mat4x4 spanMat2;
		spanMat2.affineTransformation(
			Vec3(0.45f, 3.3f, 0.45f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XMConvertToRadians(-90.0f), 0.0f),
			Vec3(0.0f, -5.5f, 0.0f)
		);

		//メッシュの描画
		auto ptrDraw = AddComponent<PNTStaticDraw>();

		if (m_tag == L"Desk")
		{
			ptrDraw->SetMeshResource(L"Desk");
			ptrDraw->SetTextureResource(L"DeskTex");
			ptrDraw->SetMeshToTransformMatrix(spanMat1);
		}
		else if (m_tag == L"Chair")
		{
			ptrDraw->SetMeshResource(L"Chair");
			ptrDraw->SetTextureResource(L"ChairTex");
			ptrDraw->SetMeshToTransformMatrix(spanMat2);
		}
		else if (m_tag==L"InvisibleCollision")
		{

		}
		else
		{
			ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
			ptrDraw->SetTextureResource(L"TestTex");
		}
	}
}
//end basecross
