/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class StageCreateManager
	{
	private :
		struct StageCreateManagerDeleter
		{
			void operator()(StageCreateManager* p) { delete p; };
		};

		static unique_ptr<StageCreateManager, StageCreateManagerDeleter> m_stageCreateManager;

	public :
		int m_scoreAnchorCount;
		int m_ammoAnchorCount;
		int m_repairAnchorCount;

		StageCreateManager();
		~StageCreateManager();


		static unique_ptr<StageCreateManager, StageCreateManagerDeleter>& CreateStageCreateManager();

		static unique_ptr<StageCreateManager, StageCreateManagerDeleter>& GetStageCreateManager();

		virtual void OnCreate();

		void CreateStageObject();

		void CreateWallObject();

		void CreateInvisibleCollision();

		void CreateRingObject();

		void CreateScoreObject();

		void CreateItemObject();

		void CreateAmmoObject();

		void SetScoreAnchorCount(int scoreanchorcount);

		void SetAmmoAnchorCount(int ammoanchorcount);

		void SetRepairAnchorCount(int repairanchorcount);

		int GetScoreAnchorCount();

		int GetAmmoAnchorCount();

		int GetRepairAnchorCount();
	};
}
//end basecross
