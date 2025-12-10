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
	};
}
//end basecross
