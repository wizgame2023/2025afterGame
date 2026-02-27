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
        m_timerRunning(false),
        m_ignoreRankCrown(false),
        m_ordinalNumberName(L"ST")
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
        m_billboard->SetPushX(0.0f);
        m_billboard->SetScale(m_scale);

        // 王冠追加
        m_iconBillboard = GetStage()->AddGameObject<BillBoard>(actor, L"crown");
        m_iconBillboard->SetTrackingActive(true);
        m_iconBillboard->SetPushY(2.0f); // 少し上
        m_iconBillboard->SetPushX(-1.5f); // 少し上
        m_iconBillboard->SetScale(Vec3(1.0f));

        // th
        m_ordinalNumber = GetStage()->AddGameObject<BillBoard>(actor, m_ordinalNumberName);
        m_ordinalNumber->SetTrackingActive(true);
        m_ordinalNumber->SetPushY(2.0f);
        m_ordinalNumber->SetPushX(1.0f);
        m_ordinalNumber->SetScale(Vec3(1.0f));
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
            m_iconBillboard->SetDrawActive(false);
            
            if (m_number == 1)
            {
                m_ignoreRankCrown = true;
            }

            m_ordinalNumber->SetDrawActive(false);
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
                m_iconBillboard->SetDrawActive(true);
                m_ordinalNumber->SetDrawActive(true);
                m_invisibleFlag = false;
                m_timerRunning = false;
                m_ignoreRankCrown = false;
            }
        }

        // 毎フレーム順位を更新
        UpdateRank();
        ApplyUV();
        ApplyRankColor(m_number);
    }

    void BillBoardNumber::UpdateRank()
    {
        // 自分が追従しているFighterの取得
        auto actor = m_fighterBase.lock();
        if (!actor) return;

        auto& scoreManager = ScoreManager::GetScoreManager();
        auto scores = scoreManager->GetSortedScores();
        // 自分のID取得
        int myId = actor->GetId();

        // 並び順そのままを順位にする
        for (size_t i = 0; i < scores.size(); ++i)
        {
            // scores 配列の中に自分のIDを見つけたら
            if (scores[i].id == myId)
            {
                // 0始まりなので+1して順位変換
                m_number = static_cast<int>(i) + 1;
                break;
            }
        }

        if (m_ordinalNumber)
        {
            m_ordinalNumberName = GetOrdinalSuffix(m_number);
            m_ordinalNumber->ChangeTexture(m_ordinalNumberName);
        }

        if (m_iconBillboard)
        {
            if (!m_ignoreRankCrown)
            {
                m_iconBillboard->SetDrawActive(m_number == 1);
            }
        }
    }

    void BillBoardNumber::ApplyUV()
    {
        if (!m_billboard) return;

        // 数字テクスチャを横に14分割している
        float piece = 1.0f / m_splitCount;

        // 表示する数字のIndex
        int index = m_number;
        Vec2 topLeft(index * piece, 0.0f);
        Vec2 botRight((index + 1) * piece, 1.0f);

        // BillBoardのUVを変更して表示
        m_billboard->SetBillUV(topLeft, botRight);
    }

    wstring BillBoardNumber::GetOrdinalSuffix(int rank)
    {
        // ランキングの順位に応じての割り振り
        switch (rank % 10)
        {
        case 1:
            return L"ST";
        case 2:
            return L"ND";
        case 3:
            return L"RD";
        default:
            return L"TH";
        }
    }

    void BillBoardNumber::SetInvisible(bool flag)
    {
        m_invisibleFlag = flag;
    }


    void BillBoardNumber::ApplyRankColor(int number)
    {
        Col4 color;

        if (number == 1)
        {
            color = Col4(1.0f, 0.85f, 0.2f, 1.0f);
        }
        else if (number == 2)
        {
            color = Col4(0.8f, 0.8f, 0.8f, 1.0f);
        }
        else if (number == 3)
        {
            color = Col4(0.8f, 0.5f, 0.2f, 1.0f);
        }
        else
        {
            color = Col4(1, 1, 1, 1);
        }

        if (m_billboard)
        {
            m_billboard->SetColor(color);
        }
    }
}
