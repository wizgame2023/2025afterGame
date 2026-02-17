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
        shared_ptr<BillBoard> m_iconBillboard;
        shared_ptr<BillBoard> m_ordinalNumber;
        float m_offsetY;
        Vec3 m_scale;
        // 表示する順位
        int  m_number;      
        // 数字テクスチャ分割数
        int  m_splitCount; 
        // 透明にするかのフラグ
        bool m_invisibleFlag;
        // タイマーを動かすかのフラグ
        bool m_timerRunning;
        // 順位表示の無効化
        bool m_ignoreRankCrown;
        wstring m_ordinalNumberName;

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
        // ランキングの順位に応じてTextureの名前を返す
        wstring GetOrdinalSuffix(int rank);
    };

} // namespace basecross
