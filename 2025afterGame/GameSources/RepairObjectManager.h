/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class RepairObjectManager
	{
	private :
		struct AmmoObjectManagerDeleter
		{
			void operator()(RepairObjectManager* p) { delete p; };
		};

		static unique_ptr<RepairObjectManager, AmmoObjectManagerDeleter> m_repairObjectManager;

	public :
		RepairObjectManager();
		~RepairObjectManager();

		static unique_ptr<RepairObjectManager, AmmoObjectManagerDeleter>& CreateRepairObjectManager();

		static unique_ptr<RepairObjectManager, AmmoObjectManagerDeleter>& GetRepairObjectManager();

		virtual void OnCreate();

		void CreateRepairObject(Vec3 pos, Vec3 rot, Vec3 siz, int tag = 0);

		void RemoveObject(int id);

		void SetVector();

		int m_count;

		std::vector<bool> m_current;
	};
}
//end basecross
