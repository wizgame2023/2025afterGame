/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Sprite::Sprite(
		const shared_ptr<Stage>& stagePtr,
		const wstring& textureName,
		const Vec2& size,
		const Vec3& pos,
		const Vec3& rot,
		const Col4& color,
		int layer) :
		MyGameObject(stagePtr),
		m_textureName(textureName),
		m_size(size),
		m_pos(pos),
		m_rot(rot),
		m_color(color),
		m_layer(layer),
		m_updateFlag(true)
	{
	}

	Sprite::Sprite(
		const shared_ptr<Stage>& stagePtr,
		const wstring& textureName,
		const Vec2& size,
		const Vec3& pos,
		int layer) :
		MyGameObject(stagePtr),
		m_textureName(textureName),
		m_size(size),
		m_pos(pos),
		m_rot(0.0f),
		m_color(1.0f),
		m_layer(layer),
		m_updateFlag(true)
	{
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::OnCreate()
	{
		// ポリゴンの自作
		Col4 color(1, 1, 1, 1); // ポリゴンの色
		const float w = 200.0f; // ポリゴンの幅
		const float h = 100.0f; // ポリゴンの高さ
		vector<VertexPositionColorTexture> m_vertices = { // 頂点データ
			//             座標                          ,頂点色,     UV座標
			{Vec3(-m_size.x * 0.5f, +m_size.y * 0.5f, 0), color, Vec2(0.0f, 0.0f)}, // 0
			{Vec3(+m_size.x * 0.5f, +m_size.y * 0.5f, 0), color, Vec2(1.0f, 0.0f)}, // 1
			{Vec3(-m_size.x * 0.5f, -m_size.y * 0.5f, 0), color, Vec2(0.0f, 1.0f)}, // 2
			{Vec3(+m_size.x * 0.5f, -m_size.y * 0.5f, 0), color, Vec2(1.0f, 1.0f)}, // 3
		};

		vector<uint16_t> m_indices = { // 頂点インデックス（頂点のつなげ順）
			0, 1, 2, // ←これで一つのポリゴン(三角形)
			2, 1, 3  // ←こっちも
		};

		m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, m_indices); // スプライト用のドローコンポーネント
		m_drawComp->SetTextureResource(m_textureName);//テクスチャの名前指定
		m_drawComp->SetSamplerState(SamplerState::LinearWrap); // テクスチャを繰り返して貼り付ける設定
		m_drawComp->SetDiffuse(m_color); // ポリゴンを色を設定する

		m_trans = GetComponent<Transform>();
		m_trans->SetPosition(m_pos);
		m_trans->SetRotation(m_rot);


		// アルファブレンド(透過処理)を有効にする
		SetAlphaActive(true); // true:透過を有効、false:透過を無効

		SetDrawLayer(m_layer);

	}

	void Sprite::OnClear(bool OnOff)
	{
		m_clear = OnOff;
		if (OnOff == true)//オンなら
		{
			SetDrawActive(false);
		}
		if (OnOff == false)//オフなら
		{
			SetDrawActive(true);
		}
	}


	//テクスチャを変更する
	void Sprite::SetTexture(wstring texture)
	{
		m_drawComp->SetTextureResource(texture);//テクスチャの名前指定
	}

	//カラーの数値を変更する
	void Sprite::SetColor(Col4 color)
	{
		m_color = color;
		m_drawComp->SetDiffuse(m_color);
	}

	//アップデートするかの処理
	void Sprite::SetUpdateFlag(bool onOff)
	{
		m_updateFlag = onOff;
	}

	//カラーの数値を取得させる
	Col4 Sprite::GetColor()
	{
		return m_color;
	}

	//自分自身を消去する
	void Sprite::MyDestroy()
	{
		GetStage()->RemoveGameObject<Sprite>(GetThis<Sprite>());
	}

	// 位置のセッター、ゲッター
	// -----------------------------

	void Sprite::SetPosition(Vec3 pos)
	{
		m_trans->SetPosition(pos);
	}

	void Sprite::SetPositionX(float X)
	{
		auto& pos = m_trans->GetPosition();
		m_trans->SetPosition(X, pos.y, pos.z);
	}

	void Sprite::SetPositionY(float Y)
	{
		auto& pos = m_trans->GetPosition();
		m_trans->SetPosition(pos.x, Y, pos.z);
	}

	void Sprite::SetPositionZ(float Z)
	{
		auto& pos = m_trans->GetPosition();
		m_trans->SetPosition(pos.x, pos.y, Z);
	}

	Vec3 Sprite::GetPosition() const
	{
		return m_trans->GetPosition();
	}

	float Sprite::GetPositionX() const
	{
		return m_trans->GetPosition().x;
	}

	float Sprite::GetPositionY() const
	{
		return m_trans->GetPosition().y;
	}

	float Sprite::GetPositionZ() const
	{
		return m_trans->GetPosition().z;
	}

	void Sprite::SetRotate(Vec3 rotate)
	{
		m_trans->SetRotation(rotate);
	}

	void Sprite::SetScale(Vec3 scl)
	{
		m_trans->SetScale(scl);
	}

	Vec3 Sprite::GetScale()
	{
		return m_trans->GetScale();
	}

	void Sprite::SetUVRect(const Vec2& topLeft, const Vec2& botRight)
	{
		vector<VertexPositionColorTexture> vertices = {
		{Vec3(-m_size.x * 0.5f, +m_size.y * 0.5f, 0), m_color, topLeft},
		{Vec3(+m_size.x * 0.5f, +m_size.y * 0.5f, 0), m_color, Vec2(botRight.x, topLeft.y)},
		{Vec3(-m_size.x * 0.5f, -m_size.y * 0.5f, 0), m_color, Vec2(topLeft.x, botRight.y)},
		{Vec3(+m_size.x * 0.5f, -m_size.y * 0.5f, 0), m_color, botRight},
		};

		vector<uint16_t> indices = { 0, 1, 2, 2, 1, 3 };

		// 一旦コンポーネントを削除して作り直す
		RemoveComponent<PCTSpriteDraw>();
		m_drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		m_drawComp->SetTextureResource(m_textureName);
		m_drawComp->SetSamplerState(SamplerState::LinearWrap);
		m_drawComp->SetDiffuse(m_color);

		SetAlphaActive(true);

	};

	void Sprite::SetDigit(int digit)
	{
		digit = clamp(digit, 0, 9);

		float piece = 1.0f / 10.0f;
		float u0 = piece * digit;
		float u1 = piece * (digit + 1);

		SetUVRect(Vec2(u0, 0.0f), Vec2(u1, 1.0f));
	}

}
//end basecross
