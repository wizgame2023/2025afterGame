/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	StageWall::StageWall(const shared_ptr<Stage>& StagePtr,
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

	StageWall::~StageWall() {

	}

	void StageWall::OnCreate()
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

		//メッシュの描画
		auto ptrDraw = AddComponent<PNTStaticDraw>();

		AddTag(L"CameraObsNotDiffuse");

		if (m_tag == L"StageWall")
		{
			ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
			ptrDraw->SetTextureResource(L"CloudWall");
		}
		else if (m_tag == L"StageCeiling")
		{
			ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
			ptrDraw->SetTextureResource(L"StageCeiling");
			//ptrDraw->SetTextureResource(L"CloudCeiling");
		}
		else if (m_tag == L"StageFloar")
		{
			ptrDraw->SetMeshResource(L"StageFloar");
			ptrDraw->SetTextureResource(L"StageFloarTex");
			//インスタンスの行列を作成する
			Mat4x4 matrix;
			matrix.affineTransformation(
				Vec3(0.065f, 0.1f, 0.065f),
				Vec3(),
				Vec3(),
				Vec3()
			);
			//ブロックを表示
			ptrDraw->SetMeshToTransformMatrix(matrix);
		}
		else
		{
			ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
			ptrDraw->SetTextureResource(L"TestTex");
		}
	}
}
//end basecross
