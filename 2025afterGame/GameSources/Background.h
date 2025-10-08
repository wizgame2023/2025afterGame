/*!
@file Background.h
@brief ”wŒi
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Background : public GameObject
	{
	public :
		//‚±‚±‚ÅƒeƒNƒXƒ`ƒƒƒL[‚Æ”z’uÀ•W‚ğŠi”[
		static const std::map<std::wstring, Vec3> pairs;

	private :
		//”wŒi‚Ì6–Ê‚ğŠÇ—
		std::vector<std::shared_ptr<GameObject>> m_backgrounds;
	public :
		Background(const shared_ptr<Stage>& stage)
			: GameObject(stage)
		{
		}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
	};
}
//end basecross
