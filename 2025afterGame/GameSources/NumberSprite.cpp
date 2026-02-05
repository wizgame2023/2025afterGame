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
        m_layer(layer),
        m_rankingNumber(0),
        m_value(0)
    {
    }

    NumberSprite::~NumberSprite()
    {
    }

    void NumberSprite::OnCreate()
    {
        SetDrawLayer(m_layer);
    }

    void NumberSprite::OnUpdate()
    {
        auto& gameManger = GameManager::GetGameManager();
        auto& uiManager = UIManager::GetUIManager();
        auto currentHP = uiManager->GetCurrentPlayerHP();
        auto& scoreManager = ScoreManager::GetScoreManager();

        auto countDown = gameManger->GetGameStartCountDown();

        if (m_type == NumberType::None)
        {
            return;
        }

        switch (m_type)
        {
        case NumberType::Bullet:
            m_value = uiManager->GetBulletNumCurrentNow();
            break;
        case NumberType::Minute:
            m_value = uiManager->GetMinuteTimer();
            break;
        case NumberType::Second:
            m_value = uiManager->GetSecondTimer();
            break;
        case NumberType::MaxBullet:
            m_value = uiManager->GetBulletNumMax();
            break;
        case NumberType::Score:
            m_value = scoreManager->GetPlScore();
			break;
        case NumberType::Count:
            m_value = 4 - countDown;
            break;
        case NumberType::RankingNumber:
            m_value = m_rankingNumber;
            break;
        }

        if (m_value >= 0)
        {
            SetNumber(m_value);
        }
    }
    
    void NumberSprite::SetNumber(int number)
    {
        m_number = number;
        int n = max(0, number);

        // 表示する桁数
        int digitCount =
            (m_digit > 0) ? m_digit :
            max(1, (int)to_string(n).size());

        // 数が違えば作り直す
        if ((int)m_digits.size() != digitCount)
        {
            for (auto& d : m_digits)
            {
                if (d) d->MyDestroy();
            }
            m_digits.clear();
            m_digits.reserve(digitCount);

            float totalWidth = m_size.x * digitCount;

            for (int i = 0; i < digitCount; ++i)
            {
                float x = m_pos.x + (i * m_size.x) - totalWidth + m_size.x;

                auto digitSprite = GetStage()->AddGameObject<Sprite>(
                    m_textureName,
                    m_size,
                    Vec3(round(x), m_pos.y, m_pos.z),
                    m_rot,
                    m_color,
                    m_layer
                );
                m_digits.push_back(digitSprite);
            }
        }

        // 下位桁から数字を入れる

        if (!m_numberUpDater)
        {
            for (int i = digitCount - 1; i >= 0; --i)
            {
                int digit = n % 10;
                n /= 10;

                m_digits[i]->SetDigit(digit);
            }
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
        // 子の桁Spriteを全て破棄
        for (auto& digit : m_digits)
        {
            if (digit)
            {
                digit->MyDestroy();
            }
        }
        m_digits.clear();

        // 自分自身を破棄
        GetStage()->RemoveGameObject<NumberSprite>(GetThis<NumberSprite>());
    }

    void NumberSprite::AddRankingNumberCount(int number)
    {
        m_rankingNumber += number;
    }

    void NumberSprite::SetNumberUpdateFlag(bool flag)
    {
        m_numberUpDater = flag;
    }

    bool NumberSprite::GetNumberUpdateFlag()
    {
        return m_numberUpDater;
    }

    void NumberSprite::SetNumberLayer(int number)
    {
        m_layer = number;
    }
}
//end basecross
