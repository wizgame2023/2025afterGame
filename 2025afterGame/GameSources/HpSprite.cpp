/*!
@file HpSprite.cpp
@brief HPの実体
担当：吉田 智貴
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
		m_Rate(0.0f),
		m_size(size),
		m_pos(pos),
		m_hpWidth(0.0f)
	{

	}

	HpSprite::~HpSprite()
	{
	}

	void HpSprite::OnCreate()
	{
		m_clear = true;
		// uWidthに元の値を記録
		vector<VertexPositionColorTexture> m_vertices = { // 頂点データ
		//             座標                         , 頂点色,  UV座標
		{Vec3(0,                +m_size.y * 0.5f, 0), m_color, Vec2(0.0f, 0.0f)},
		{Vec3(m_size.x * 0.5f,  +m_size.y * 0.5f, 0), m_color, Vec2(1.0f, 0.0f)},
		{Vec3(0,                -m_size.y * 0.5f, 0), m_color, Vec2(0.0f, 1.0f)},
		{Vec3(m_size.x * 0.5f,  -m_size.y * 0.5f, 0), m_color, Vec2(1.0f, 1.0f)}, };

		vector<uint16_t> m_indices = { // 頂点インデックス（頂点のつなげ順）
			0, 1, 2, // ←これで一つのポリゴン(三角形)
			2, 1, 3  // ←こっちも
		};

		m_trans = GetComponent<Transform>();
		m_trans->SetScale(Vec3(m_size.x, m_size.y, 0.0f));
		m_trans->SetPosition(m_pos);
		// 頂点とインデックスを指定してスプライト作成
		// メッシュの作成
		m_drawComp = AddComponent<PCTSpriteDraw>(m_vertices, m_indices);
		m_drawComp->SetSamplerState(SamplerState::LinearWrap);
		m_drawComp->SetTextureResource(m_textureName);
		m_drawComp->SetDiffuse(m_color);

		SetAlphaActive(m_clear);
		SetDrawLayer(m_layer);
	}

	void HpSprite::OnUpdate()
	{
		if (m_spriteMoveFlag)
		{
			UpdateHpSprite();
		}
	}

	void HpSprite::UpdateHpSprite()
	{
		auto hpMin = 0.0f;
		auto deltaTime = App::GetApp()->GetElapsedTime();
		auto stage = App::GetApp()->GetScene<Scene>()->GetActiveStage();
		m_fightBase = stage->GetSharedGameObject<Player>(L"Player");
		auto fightBase = m_fightBase.lock();
		auto currentHP = fightBase->GetHpCurrent();
		auto maxHP = fightBase->GetHpMax();

		// 体力の割合
		m_Rate = static_cast<float>(currentHP) / static_cast<float>(maxHP);
		m_Rate = clamp(m_Rate, hpMin, 1.0f);

		// 割合×横サイズ
		m_hpWidth = m_size.x * m_Rate;

		// 比率でスケーリング（横方向のみ縮む）
		m_trans->SetScale(Vec3(m_hpWidth, m_size.y, 1.0f));
		m_trans->SetPosition(Vec3(m_pos));

		if (m_Rate >= 0.8f)
		{
			m_color = Col4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		else if (m_Rate >= 0.4f)
		{
			m_color = Col4(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			m_color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		}

		m_drawComp->SetDiffuse(m_color);
	}

	void HpSprite::SetSpriteMove(bool flag)
	{
		m_spriteMoveFlag = flag;
	}

}
//end basecross
