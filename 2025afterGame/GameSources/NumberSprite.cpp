/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"
class ScoreManager;
namespace basecross {
    NumberSprite::NumberSprite(
        const shared_ptr<Stage>& stagePtr,
        const Vec2& size,
        const Vec3& pos,
        const Vec3& rot,
        const Col4& color,
        int layer,
        const wstring& textureName
    ) :
        Sprite(stagePtr, textureName, size, pos, rot, color, layer),
        m_number(0),
        m_digit(0),
        m_prevNumber(-1),
        m_pos(pos)
    {
    }

    NumberSprite::~NumberSprite()
    {
    }

    void NumberSprite::OnCreate()
    {
        Sprite::OnCreate();
        m_trans->SetPosition(m_pos.x, m_pos.y,m_pos.z);
        SetDrawActive(false);
    }

    void NumberSprite::OnUpdate()
    {
        auto& gameManger = GameManager::GetGameManager();
        auto& uiManager = UIManager::GetUIManager();
        auto currentHP = uiManager->GetCurrentPlayerHP();
        auto& scoreManager = ScoreManager::GetScoreManager();

        int value = 0;

        switch (m_type)
        {
        case NumberType::Bullet:
            value = uiManager->GetBulletNumCurrentNow();
            break;
        case NumberType::Minute:
            value = uiManager->GetMinuteTimer();
            break;
        case NumberType::Second:
            value = uiManager->GetSecondTimer();
            break;
        case NumberType::MaxBullet:
            value = uiManager->GetBulletNumMax();
            break;
        case NumberType::Score:
			value = uiManager->GetPlayerScore();
			break;
        case NumberType::Ranking:
            value = 100;
            break;
        }

        if (value >= 0)
        {
            SetNumber(value);
        }
    }

    void NumberSprite::SetNumber(int number)
    {
        m_number = number;

        wstring str;

        // m_digit が 1以上なら「ゼロ埋めして桁数固定」
        if (m_digit > 0)
        {
            int n = max(0, number);  // マイナスはとりあえず0扱い
            str.clear();

            // とりあえず 「m_digit 桁」ぶん 0埋めして作る（下位桁から）
            for (int i = 0; i < m_digit; ++i)
            {
                int d = n % 10;
                wchar_t ch = L'0' + d;
                // 先頭に詰めていく
                str.insert(str.begin(), ch);
                n /= 10;
            }

            // もし number が m_digit より大きい桁数だった場合（例: m_digit=2 で number=123）
            // 余った桁をさらに前に足す
            while (n > 0)
            {
                int d = n % 10;
                wchar_t ch = L'0' + d;
                str.insert(str.begin(), ch);
                n /= 10;
            }
        }
        else
        {
            // そのまま文字列化
            str = to_wstring(number);
        }

        float totalWidth = m_size.x * str.size();

        // 桁数が変わったらスプライト作り直し
        if (m_digits.size() != str.size()) {
            // 既存の桁を削除
            for (auto& obj : m_digits) {
                obj->MyDestroy();
            }
            m_digits.clear();
            m_digits.reserve(str.size());

            // 新しい桁Spriteを生成
            for (int i = 0; i < str.size(); i++) {
                Vec3 digitPos = Vec3(
                    m_pos.x + (i * m_size.x) - totalWidth + m_size.x,
                    m_pos.y,
                    m_pos.z
                );

                auto digitSprite = GetStage()->AddGameObject<Sprite>(
                    m_textureName,
                    m_size,
                    digitPos,
                    m_rot,
                    m_color,
                    m_layer
                );
                m_digits.push_back(digitSprite);
            }
        }

        // UVで数字部分を切り出す
        for (int i = 0; i < str.size(); i++)
        {
            int value = str[i] - L'0';
            value = clamp(value, 0, 9);

            const float texWidth = 512.0f;   // 数字テクスチャの横幅(px)
            const float digitCount = 10.0f;

            float digitWidth = texWidth / digitCount;   // 51.2px

            // ピクセル境界にスナップ
            float px0 = round(digitWidth * value);
            float px1 = round(digitWidth * (value + 1));

            // UVに正規化
            float u0 = px0 / texWidth;
            float u1 = px1 / texWidth;

            // 隣の数字がにじまないように 0.5px 内側へ
            float halfPixel = 1.0f / texWidth;
            u0 += halfPixel;
            u1 -= halfPixel;

            m_digits[i]->SetUVRect(Vec2(u0, 0.0f), Vec2(u1, 1.0f));
        }
    }

    void NumberSprite::SetDigit(int digit)
    {
        digit = clamp(digit, 0, 9);

        float piece = 1.0f / 10.0f;
        float u0 = piece * digit;
        float u1 = piece * (digit + 1);

      SetUVRect(Vec2(u0, 0.0f), Vec2(u1, 1.0f));
    }

    void NumberSprite::SetMyType(NumberType type)
    {
        m_type = type;
    }

    void NumberSprite::SetDigitCount(int digitCount)
    {
        m_digit = digitCount;   // ←桁数を指定（最低桁数）
    }
}
//end basecross
