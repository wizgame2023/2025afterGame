/*!
@file MainCameraManager.h
@brief プレイヤーのカメラ
担当:佐藤 海斗
*/

#pragma once
#include "stdafx.h"

namespace basecross{	
	// ==============================================================================
	// MainCameraManagerクラス
	// ==============================================================================

	class MainCameraManager : public MyGameObject
	{
		// ==============================================================================
		// enum
		// ==============================================================================

		// ==============================================================================
		// 構造体
		// ==============================================================================

		// ターゲットの情報(構造体)
		struct PlayerInfo {
			Vec3 pos;	// 位置
			Vec3 rot;	// 回転
			Vec3 up;	// 上方向ベクトル
			Vec3 fwrd;	// 前方向ベクトル
		};

		// 障害物ヒット結果(構造体)
		struct ObstructionHitResult{
			Vec3 hitPos;
			float hitLength;
			bool hit;
			Col4 diffuseColor;
		};

		// ==============================================================================
		// メンバ変数
		// ==============================================================================


		PlayerInfo m_plInfo;

		// スマートポインタ関係
		weak_ptr<Transform> m_plTrans;
		shared_ptr<Stage> m_stage;
		shared_ptr<Actor> m_target;
		shared_ptr<Camera> m_mulCam;
		
		// SharedGameObjectの名前
		wstring m_sharedName = L"Player";

		// 傾きの履歴
		deque<Vec3> m_plUpHistory;

		// カメラ関係の変数
		Vec3 m_camPos;
		Vec3 m_atPos;

		bool m_onceFlag = false;

		// ==============================================================================
		// メンバ定数
		// ==============================================================================
		
		static constexpr float m_camDis = 5.0f; // カメラとプレイヤーの距離
		static constexpr float m_followSpeed = 20.0f; // カメラの追従速度
		static constexpr float m_camHeight = 1.5f; // カメラの高さ
		static constexpr float m_atOffset = 10.0f; // 注視点のオフセット距離
		
		static constexpr int historyMax = 15;// 履歴の最大値(6で0.1秒のディレイがかかる)

		// ==============================================================================
		// 関数
		// ==============================================================================

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
		// isAccel : 加速中かどうか
		void AdjustFov(bool isAccel);

		// マルチビューかどうか
		void CheckMultiView(const wstring& sharedName);

		// カメラが前方を映すか後方を映すか
		// isButton : 押されたボタンの真偽
		void SetCameraNormalBehindMode(bool isButton);

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

		// 障害物がカメラの機能を邪魔していないかを見る
		void UpdateCameraObstruction();

		// プレイヤーとカメラの間に障害物があるか
		// from : プレイヤーの位置
		// to : カメラの位置
		ObstructionHitResult TestCameraObstruction(const Vec3& from, const Vec3& to, const shared_ptr<GameObject>& obj);

		// プレイヤーとカメラの間に障害物があるか
		// from : プレイヤーの位置
		// to : カメラの位置
		// 戻り値 : 障害物に当たっているかどうか
		//bool TestCameraDiffuseObj(const Vec3& from, const Vec3& to, const shared_ptr<GameObject>& obj);

		//// 障害物の走査と収集
		//vector<ObstructionHitResult> CollectObstructionHits(const Vec3& from, const Vec3& to);

		//// 障害物に当たっているときのカメラ位置の更新
		//void UpdateCameraPositionObstruction(const ObstructionHitResult& result);

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
	// ==============================================================================
	// MainCameraManagerクラス末尾
	// ==============================================================================

	// カメラを邪魔するオブジェクトを判定するクラス群
	/*
	// ======================================================================================================================
	
	// ==============================================================================
	// カメラを邪魔するオブジェクトを判定する基底クラス
	// ==============================================================================
	class CameraBlocker
	{
	public:
		// 最終的なカメラ位置を返す関数
		// plPos : プレイヤー位置
		// tgtCamPos : 本来行きたいカメラ位置
		// stage : オブジェクト検索用
		virtual Vec3 ResolveObstruction(const Vec3& plPos, const Vec3& tgtCamPos, const shared_ptr<Stage>& stage) = 0;
		virtual ~CameraBlocker() = default;
	};

	// ==============================================================================
	// カメラを近づけるためのクラス
	// ==============================================================================
	class CameraCloser : public CameraBlocker
	{
		struct HitResult {
			Vec3 hitPos;
			float hitLength;
			bool hit;
		};

		// 内部処理用関数
		HitResult TestObstruction(const Vec3& from, const Vec3& to, const shared_ptr<MyGameObject>& obj);

	public:
		virtual Vec3 ResolveObstruction(const Vec3& plPos, const Vec3& tgtCamPos, const shared_ptr<Stage>& stage) override;
		virtual ~CameraCloser() = default;
	};

	// ==============================================================================
	// オブジェクトを半透明にするクラス
	// ==============================================================================
	class CameraFader : public CameraBlocker
	{
	public:
		virtual Vec3 ResolveObstruction(const Vec3& plPos, const Vec3& tgtCamPos, const shared_ptr<Stage>& stage) override;
		virtual ~CameraFader() = default;
	};

	// ======================================================================================================================
	*/

	// ==============================================================================
	// WindEffectImageAnimationクラス
	// ==============================================================================

	//class WindEffectImageAnimation : public MyGameObject
	//{

	//public:
	//	WindEffectImageAnimation(const shared_ptr<Stage>& stagePtr) :
	//		MyGameObject(stagePtr)
	//	{}

	//	~WindEffectImageAnimation()
	//	{}

	//	virtual void OnCreate() override;
	//	virtual void OnUpdate() override;

	//};

	// ==============================================================================
	// WindEffectImageAnimationクラス末尾
	// ==============================================================================

}
//end basecross
