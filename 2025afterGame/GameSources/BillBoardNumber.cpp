#include "stdafx.h"
#include "Project.h"

namespace basecross {

    BillBoardNumber::BillBoardNumber(
        const shared_ptr<Stage>& stage,
        const shared_ptr<FighterAircraftBase>& fighetrAircaftPtr
    )
        : MyGameObject(stage),
        m_fighterBase(fighetrAircaftPtr),
        m_offsetY(2.0f),
        m_scale(Vec3(1.0f, 2.0f, 1.0f)),
        m_number(0),
        m_splitCount(14),
        m_invisibleFlag(false),
        m_timerRunning(false)
    {
    }

    BillBoardNumber::~BillBoardNumber()
    {

    }

    void BillBoardNumber::OnCreate()
    {
        auto actor = m_fighterBase.lock();
        if (!actor) return;

        // Billboard作成
        m_billboard = GetStage()->AddGameObject<BillBoard>(actor, L"Number");

        m_billboard->SetTrackingActive(true);
        m_billboard->SetPushY(m_offsetY);
        m_billboard->SetScale(m_scale);

        UpdateRank();
        ApplyUV();
    }

    void BillBoardNumber::OnUpdate()
    {
        if (m_fighterBase.expired())
        {
            GetStage()->RemoveGameObject<BillBoardNumber>(GetThis<BillBoardNumber>());
            return;
        }

        float dt = App::GetApp()->GetElapsedTime();

        // 表示要求が来た瞬間
        if (m_invisibleFlag && !m_timerRunning)
        {
            m_billboard->SetDrawActive(false);
            m_timerRunning = true;
            m_delta = 0.0f;
        }

        // タイマー進行中
        if (m_timerRunning)
        {
            m_delta += dt;

            // 1秒経過後表示
            if (m_delta >= 6.0f)
            {
                m_billboard->SetDrawActive(true);
                m_invisibleFlag = false;
                m_timerRunning = false;
            }
        }

        // 毎フレーム順位を更新
        UpdateRank();
        ApplyUV();
    }

    void BillBoardNumber::UpdateRank()
    {
        auto actor = m_fighterBase.lock();
        if (!actor) return;

        auto& scoreManager = ScoreManager::GetScoreManager();
        auto scores = scoreManager->GetSortedScores();

        int myId = actor->GetId();

        // 並び順そのままを順位にする
        for (size_t i = 0; i < scores.size(); ++i)
        {
            if (scores[i].id == myId)
            {
                m_number = static_cast<int>(i) + 1;
                break;
            }
        }
    }

    void BillBoardNumber::ApplyUV()
    {
        if (!m_billboard) return;

        float piece = 1.0f / m_splitCount;

        int index = m_number;
        Vec2 topLeft(index * piece, 0.0f);
        Vec2 botRight((index + 1) * piece, 1.0f);

        m_billboard->SetBillUV(topLeft, botRight);
    }

    void BillBoardNumber::SetInvisible(bool flag)
    {
        m_invisibleFlag = flag;
    }

}
