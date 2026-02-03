/*!
@file ScoreManager.h
@brief スコアマネージャー
担当:佐藤 海斗
*/

#pragma once
#include "stdafx.h"
#include "fstream"

namespace basecross {
	// ==============================================================================
	// ScoreManagerクラス
	// ==============================================================================

	// ================================================================================================ //
	// ScoreManagerの使い方																				//
	// インスタンスの取得:																				//
	//		使いたい場所で auto& scoreManager = ScoreManager::GetScoreManager(); を実行します。			//
	//		(※まだ作成されていない可能性がある場合は CreateScoreManager() を使用してください。)		//
	// メソッドの呼び出し:																				//
	//		scoreManager->SetScore(...) のようにアロー演算子で各機能にアクセスします。					//
	// IDの登録（重要）:																				//
	//		新しいプレイヤーやNPCのスコアを扱う前には、必ず SetID(L"名前") で登録を行ってください。		//
	//		登録されていないIDに対してスコアを設定・取得しようとしても反映されません。					//
	// ================================================================================================ //

	class FighterAircraftBase;

	class ScoreManager
	{
	public:
		struct ScoreInfo
		{
			wstring id;		// スコアを持っているプレイヤー,NPCの名前
			int crntScore = 0;		// 現在のスコア
		};

	private:
		// 削除処理
		struct ScoreManagerDeleter
		{
			void operator()(ScoreManager* p) { delete p; };
		};

		// ==============================================================================
		// メンバ変数
		// ==============================================================================

		static unique_ptr<ScoreManager, ScoreManagerDeleter> m_scoreManager;

		// スコア
		vector<ScoreInfo> m_scores;

		// スプライト
		vector<shared_ptr<Sprite>> m_numberSprite;

		// ステージ
		shared_ptr<Stage> m_stage;

		// 戦闘機を管理するための配列
		vector<weak_ptr<FighterAircraftBase>> m_fighterBases;

		// 定数

		// playerのID
		const wstring& PLAYER_ID = L"Player";
		
		// ==============================================================================
		// 関数
		// ==============================================================================

		// スコアバイナリファイルパス取得
		wstring GetBinaryPath() const;

		// ヘルパー関数

		// IDからスコア情報を探す
		ScoreInfo* FindScoreInfo(const wstring& id);
		// 読み取りのみ
		const ScoreInfo* FindScoreInfo(const wstring& id) const;


	public:

		// ランキングソート
		vector<ScoreInfo> GetSortedScores() const;

		// player専用のセッター
		void SetPlScore(int score)
		{
			SetScore(PLAYER_ID, score);
		}
		
		// player専用のゲッター
		int GetPlScore() const
		{
			return GetScore(PLAYER_ID);
		}

		// コンストラクタ
		ScoreManager();
		// デストラクタ
		~ScoreManager() {};

		// スコアマネージャーを作成
		static unique_ptr<ScoreManager, ScoreManager::ScoreManagerDeleter>& CreateScoreManager();

		// 自分を渡す
		static unique_ptr<ScoreManager, ScoreManager::ScoreManagerDeleter>& GetScoreManager();

		// ID設定
		void SetID(const wstring& id);

		// スコア更新
		// 引数 : 変動するスコア
		void SetScore(const wstring& id, int score);

		// 現在のスコア取得
		int GetScore(const wstring& id) const;

		// スコア初期化
		void ResetScore(const wstring& id);

		// スコアバイナリ生成
		void CreateHighScoreBinary();

		// スコアバイナリ書き込み
		void SaveHighScoreBinary();

		// スコアバイナリ読み込み
		int LoadHighScoreBinary();

		// 戦闘機配列に対し戦闘機の中身を追加する
		void PushBackFighterBase(const shared_ptr<FighterAircraftBase>& fighterAircraftBase);
	};

}
//end basecross

