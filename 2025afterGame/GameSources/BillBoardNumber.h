#pragma once
#include "stdafx.h"

namespace basecross {

    class Actor;
    class BillBoard;

    class BillBoardNumber : public MyGameObject
    {
    private:
        weak_ptr<FighterAircraftBase> m_fighterBase;
        shared_ptr<BillBoard> m_billboard;
        float m_offsetY;
        Vec3  m_scale;
        int   m_number;      // 表示する順位
        int   m_splitCount; // 数字テクスチャ分割数
        bool m_invisibleFlag;
        bool m_timerRunning;

    public:
        BillBoardNumber(const shared_ptr<Stage>& stage,
        const shared_ptr<FighterAircraftBase>& fighetrAircaftPtr);
        BillBoardNumber::~BillBoardNumber();

        virtual void OnCreate() override;
        virtual void OnUpdate() override;

        // HPがないときの透明にする
        void SetInvisible(bool flag);

    private:
        // 順位計算
        void UpdateRank();
        // 表示反映
        void ApplyUV();
    };

} // namespace basecross
