#include "GobKingDieState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"

GobKingDieState::GobKingDieState(
	std::function<void(void)> playAnimeDie
) :
	playAnimeDie(playAnimeDie)
{
}

void GobKingDieState::OwnStateConditionUpdate(void)
{
	OwnChangeState();
}

void GobKingDieState::Enter(void)
{
	playAnimeDie();
}