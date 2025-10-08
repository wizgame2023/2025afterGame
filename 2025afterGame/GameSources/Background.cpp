/*!
@file Background.cpp
@brief 背景
*/

#include "stdafx.h"
#include "Project.h"
#include <string>

namespace basecross{
	//テクスチャキーと座標、角度を定義している、ファイル名は_付けて後に付け足すこと
	const std::map<std::wstring, Vec3>Background::pairs = {
		{L"000_000_000", Vec3(0   ,  0,   +0.5f)},
		{L"000_090_000", Vec3(+0.5f, 0,    0)},
		{L"000_180_000", Vec3(0   ,  0,   -0.5f)},
		{L"000_270_000", Vec3(-0.5f, 0,    0)},
		{L"090_000_000", Vec3(0   , -0.5f, 0)},
		{L"270_000_000", Vec3(0   , +0.5f, 0)},
	};

	void Background::OnCreate()
	{
		for (const auto& pair : pairs)
		{
			//プレーンクラスをベースにする
			auto plane = ObjectFactory::Create<Plane>(GetStage());

			//ライティングは掛けない
			auto planeDrawComp = plane->GetComponent<PTStaticDraw>();
			planeDrawComp->SetTextureResource(pair.first);
			//Zバッファの影響を無効に
			planeDrawComp->SetDepthStencilState(DepthStencilState::None);
			//テクスチャの切れ目を目立たなくする
			planeDrawComp->SetSamplerState(SamplerState::LinearClamp);

			auto planeTransComp = plane->GetComponent<Transform>();
			//テクスチャキーの文字列を角度に変換する(X,Y,Zの順)
			float rotX = XMConvertToRadians(std::stof(pair.first.substr(0, 3)));
			float rotY = XMConvertToRadians(std::stof(pair.first.substr(4, 3)));
			float rotZ = XMConvertToRadians(std::stof(pair.first.substr(8, 3)));
			//念のために視野範囲の限界まで拡大
			const float scale = 707.0f;
			planeTransComp->SetPosition(pair.second * scale);
			planeTransComp->SetRotation(rotX, rotY, rotZ);
			planeTransComp->SetScale(Vec3(scale));
			//スカイキューブを構成するプレーンをBackgroundオブジェクトの子に
			planeTransComp->SetParent(GetThis<GameObject>());

			m_backgrounds.push_back(plane);
		}
	}

	void Background::OnUpdate()
	{
		//常にカメラの座標に追従する
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(GetStage()->GetView()->GetTargetCamera()->GetEye());
	}

	void Background::OnDraw()
	{
		//管理している全ての面を描画
		for (auto& plane : m_backgrounds)
		{
			plane->OnDraw();
		}
	}
}
//end basecross
