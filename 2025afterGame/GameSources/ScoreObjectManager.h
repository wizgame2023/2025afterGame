/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class ScoreObjectManager
	{
	private :
		struct ScoreObjectManagerDeleter
		{
			void operator()(ScoreObjectManager* p) { delete p; };
		};

		static unique_ptr<ScoreObjectManager, ScoreObjectManagerDeleter> m_scoreObjectManager;

	public :
		ScoreObjectManager();
		~ScoreObjectManager();

		static unique_ptr<ScoreObjectManager, ScoreObjectManagerDeleter>& CreateScoreObjectManager();

		static unique_ptr<ScoreObjectManager, ScoreObjectManagerDeleter>& GetScoreObjectManager();

		virtual void OnCreate();

		void CreateScoreObject(Vec3 pos, Vec3 rot, Vec3 siz, int tag = 0);

		void RemoveObject(int id);

		// オブジェクトの配置処理を全て初期化する関数
		void ResetObject();

		void SetVector();

		int m_count;

		std::vector<bool> m_current;
	};
}
//end basecross
