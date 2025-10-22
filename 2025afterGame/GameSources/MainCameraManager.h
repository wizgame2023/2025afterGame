/*!
@file MainCameraManager.h
@brief プレイヤーのカメラ
担当:佐藤 海斗
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Player;
	
	class MainCameraManager : public MyGameObject
	{
		weak_ptr<Transform> m_plTrans;
		shared_ptr<Player> m_player;
		shared_ptr<Stage> m_stage;
		shared_ptr<Camera> m_mainCamera;
		shared_ptr<Actor> m_target;
		shared_ptr<Camera> m_mulCam;
		
		Vec3 m_plPos;
		Vec3 m_plRot;
		Vec3 m_plUp;
		Vec3 m_plFwrd;

		wstring m_sharedName = L"Player";

		// 傾きの履歴
		std::deque<Vec3> m_plUpHistory;

		// 線形補間関数(滑らかに動かすためのやつ)
		// 参考 : https://taketakeshi.hatenablog.jp/entry/2025/05/19/205447
		// start : 開始地 end : 終了値 time : 補間係数(0.0f～1.0f)
		// 戻り値 : 補間後の値
		Vec3 Lerp(const Vec3& start, const Vec3& end, float time) {
			return start + (end - start) * time;
		}

		// Upベクトルの履歴に追加し、最大値を超えたら削除
		// up : 追加するUpベクトル 
		// historyMax : 履歴の最大数
		void UpdateUpHistory(const Vec3& up, const int historyMax) {
			m_plUpHistory.push_back(up);
			if (m_plUpHistory.size() > historyMax)
				m_plUpHistory.pop_front();
		}
		
		// 履歴に保存されたupベクトルの平均値を返す
		// 戻り値 : 平均化されたUpベクトル
		Vec3 CalcUpHistoryAverage() const {
			if (m_plUpHistory.empty())
				return Vec3(0.0f, 1.0f, 0.0f);

			Vec3 sum(0.0f, 0.0f, 0.0f);
			for (const auto& v : m_plUpHistory)
				sum += v;

			Vec3 avg = sum / static_cast<float>(m_plUpHistory.size());
			if (avg.length() < 0.00001f)
				avg = Vec3(0.0f, 1.0f, 0.0f);

			return avg.normalize();
		}

		// 履歴追加と平均化を同時に行う
		// currentUp : 現在のUpベクトル
		// historyMax : 履歴の最大値
		// 戻り値 : 平均化されたUpベクトル
		Vec3 GetSmoothedUp(const Vec3& currentUp, const int historyMax) {
			UpdateUpHistory(currentUp, historyMax);
			return CalcUpHistoryAverage();
		}

	public:
		// コンストラクタ
		MainCameraManager(const shared_ptr<Stage>& stagPtr);
		// target : 対象 
		// mulView : どのカメラなのか
		// sharedName : 設定されたSharedGameObjectの名前
		MainCameraManager(
			const shared_ptr<Stage>& stagePtr, 
			const shared_ptr<Actor>& target, 
			const shared_ptr<Camera>& mulView,
			const wstring& sharedName
		);

		~MainCameraManager() {}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}
//end basecross
