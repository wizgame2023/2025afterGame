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
        m_pos(pos),
        m_layer(layer)
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
        SetLayer(99);

        SetDrawLayer(m_layer);
    }

    void NumberSprite::OnUpdate()
    {
        auto& gameManger = GameManager::GetGameManager();
        auto& uiManager = UIManager::GetUIManager();
        auto currentHP = uiManager->GetCurrentPlayerHP();
        auto& scoreManager = ScoreManager::GetScoreManager();

        auto countDown = gameManger->GetGameStartCountDown();

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
        case NumberType::Count:
            value = countDown;
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
        if (m_digits.size() != str.size())
        {
            // 既存の桁を削除
            for (auto& obj : m_digits)
            {
                obj->MyDestroy();
            }
            m_digits.clear();
            m_digits.reserve(str.size());

            // 新しい桁Spriteを生成
            for (int i = 0; i < str.size(); i++) 
            {
                float x = m_pos.x + (i * m_size.x) - totalWidth + m_size.x;

                x = round(x);

                Vec3 digitPos = Vec3(
                    x,
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

            m_digits[i]->SetDigit(value);
        }
    }

    void NumberSprite::SetMyType(NumberType type)
    {
        m_type = type;
    }

    void NumberSprite::SetDigitCount(int digitCount)
    {
        m_digit = digitCount;   // ←桁数を指定（最低桁数）
    }

    void NumberSprite::OnDestory()
    {
        GetStage()->RemoveGameObject<NumberSprite>(GetThis<NumberSprite>());
    }

    void NumberSprite::SetLayer(int layer)
    {
        m_layer = layer;
    }
}
//end basecross
