/*!
@file Player.cpp
@brief ƒvƒŒƒCƒ„[‚È‚ÇÀ‘Ì
’S“–F‹g“c ’q‹M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	FighterAircraftBase::FighterAircraftBase(const shared_ptr<Stage>& ptrStage) :
		Actor(ptrStage)
	{
	}

	FighterAircraftBase::~FighterAircraftBase()
	{

	}

	void FighterAircraftBase::OnCreate()
	{
		Actor::OnCreate();		
	}

	void FighterAircraftBase::OnUpdate()
	{

	}

}
//end basecross

