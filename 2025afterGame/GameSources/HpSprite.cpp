/*!
@file HpSprite.cpp
@brief HPの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	HpSprite::HpSprite(
		const shared_ptr<Stage>& stagePtr,
		const wstring& textureName,
		const Vec2& size,
		const Vec3& pos,
		const Vec3& rot,
		const Col4& color,
		int layer) :
		Sprite(stagePtr, textureName, size, pos, rot, color, layer),
		m_nowHP(0.0f),
		m_maxHP(0.0f)
	{

	}

	HpSprite::~HpSprite()
	{
	}

	void HpSprite::OnCreate()
	{
		m_width = m_nowHP / m_maxHP * 1.0f;

		float helfSize = 1.0f;

		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(0,                0, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f),  Vec2(0.0f   , 0.0f)) },
			{ VertexPositionColorTexture(Vec3(0,        -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f),  Vec2(0.0f   , 1.0f)) },
			{ VertexPositionColorTexture(Vec3(m_width,		    0, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f),  Vec2(m_width, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(m_width,  -helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f),  Vec2(m_width, 1.0f)) },
		};

		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_clear);
		m_trans = GetComponent<Transform>();
		m_trans->SetScale(0.0f, 0.0f, 0.0f);
		m_trans->SetPosition(m_pos);
		//頂点とインデックスを指定してスプライト作成
		m_drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		m_drawComp->SetSamplerState(SamplerState::LinearWrap);
		m_drawComp->SetTextureResource(m_textureName);
	}


}
//end basecross
