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

		
		static constexpr float m_camDis = 5.0f; // カメラとプレイヤーの距離
		static constexpr float m_followSpeed = 20.0f; // カメラの追従速度
		static constexpr float m_camHeight = 1.5f; // カメラの高さ
		static constexpr float m_atOffset = 10.0f;

		// 履歴の最大値(6で0.1秒のディレイがかかる)
		static constexpr int historyMax = 15;

		wstring m_sharedName = L"Player";

		wstringstream m_debugWss;

		// 傾きの履歴
		std::deque<Vec3> m_plUpHistory;

		// 線形補間関数(Vec3用)
		// 参考 : https://taketakeshi.hatenablog.jp/entry/2025/05/19/205447
		// start : 開始地 end : 終了値 time : 補間係数(0.0f～1.0f)
		// 戻り値 : 補間後の値
		static Vec3 LerpV3(const Vec3& start, const Vec3& end, float time) {
			return start + (end - start) * time;
		}

		// 線形補間関数(float用)
		// start : 開始地 end : 終了値 time : 補間係数(0.0f～1.0f)
		// 戻り値 : 補間後の値
		static float LerpFlt(const float start, const float end, float time) {
			return start + (end - start) * time;
		}

		// 視野角調整
		void AdjustFov(bool isAccel);

		// マルチビューかどうか
		void IsMultiView(const wstring& sharedName);



		// Upベクトルの履歴に追加し、最大値を超えたら削除
		// up : 追加するUpベクトル 
		// historyMax : 履歴の最大数
		void UpdateUpHistory(const Vec3& up, const int historyMax);
		
		// 履歴に保存されたupベクトルの平均値を返す
		// 戻り値 : 平均化されたUpベクトル
		Vec3 CalcUpHistoryAverage() const;

		// 履歴追加と平均化を同時に行う
		// currentUp : 現在のUpベクトル
		// historyMax : 履歴の最大値
		// 戻り値 : 平均化されたUpベクトル
		Vec3 GetSmoothedUp(const Vec3& currentUp, const int historyMax);

		// デバッグログ　複数
		// 使う場合は必ずFlushDebugLogも呼び出すこと
		// name : ログの名前
		// debug : ログに出力する値
		template <typename T>
		void DebugLog(const wstring& name, T debug);

		// デバッグログを使う場合はフレームの最後(OnUpdateの末尾)に呼び出す
		// そうしないと画面に表示されません
		void FlushDebugLog();

		// プレイヤーとカメラの間に障害物があるか
		// from : プレイヤーの位置
		// to : カメラの位置
		bool IsObstructed(const Vec3& from, const Vec3& to);

	public:
		// コンストラクタ
		MainCameraManager(const shared_ptr<Stage>& stagePtr);

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
