/*!
@file EffectUpdateDrawManager.cpp
@brief エフェクトの更新と描画を行うクラス
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	class EffectUpdateDrawManager : public MyGameObject;
	//--------------------------------------------------------------------------------------
	EffectUpdateDrawManager::EffectUpdateDrawManager
	(
		const shared_ptr<Stage>& StagePtr
	) :
		GameObject(StagePtr)
	{
	}

	EffectUpdateDrawManager::~EffectUpdateDrawManager()
	{
	}

	void EffectUpdateDrawManager::OnCreate()
	{
		m_manager  = EffectManager::Instance().GetManager();
		m_renderer = EffectManager::Instance().GetRenderer();
		SetAlphaActive(true);
		SetDrawLayer(1);
	}

	void EffectUpdateDrawManager::OnUpdate()
	{
		m_manager->Update();
		auto stage  = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		auto camera = stage->GetView()->GetTargetCamera();
		SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());
	}

	void EffectUpdateDrawManager::OnDraw()
	{
		m_renderer->SetTime(App::GetApp()->GetElapsedTime());

		m_renderer->BeginRendering();

		m_manager->Draw();

		m_renderer->EndRendering();
	}

	void EffectUpdateDrawManager::SetViewProj(const Mat4x4& view, const Mat4x4& proj)
	{
		Matrix44 v, p;
		Mat4x4ToMatrix44(view, v);
		Mat4x4ToMatrix44(proj, p);
		m_renderer->SetCameraMatrix(v);
		m_renderer->SetProjectionMatrix(p);
	}
	
	void EffectUpdateDrawManager::Mat4x4ToMatrix44(const Mat4x4& src, Matrix44& dest)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				dest.Values[i][j] = src(i, j);
			}
		}
	}
}
