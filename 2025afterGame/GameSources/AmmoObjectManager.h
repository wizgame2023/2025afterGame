/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class AmmoObjectManager
	{
	private :
		struct AmmoObjectManagerDeleter
		{
			void operator()(AmmoObjectManager* p) { delete p; };
		};

		static unique_ptr<AmmoObjectManager, AmmoObjectManagerDeleter> m_ammoObjectManager;

	public :
		AmmoObjectManager();
		~AmmoObjectManager();

		static unique_ptr<AmmoObjectManager, AmmoObjectManagerDeleter>& CreateAmmoObjectManager();

		static unique_ptr<AmmoObjectManager, AmmoObjectManagerDeleter>& GetAmmoObjectManager();

		virtual void OnCreate();

		void CreateAmmoObject(Vec3 pos, Vec3 rot, Vec3 siz, int tag = 0);

		void RemoveObject(int id);

		void ResetObject();

		void SetVector();

		int m_count;

		std::vector<bool> m_current;
	};
}
//end basecross
