#include "GobKingDieState.h"

#include "../../../../Common/Vector3.h"

GobKingDieState::GobKingDieState(
	bool& isDie,
	bool& isFlyEnd,
	std::function<void(void)> playAnimeDie,

	std::function<bool(void)> isAnimeEnd
) :
	isDie(isDie),
	isFlyEnd(isFlyEnd),
	playAnimeDie(playAnimeDie),
	isAnimeEnd(isAnimeEnd)
{
}

void GobKingDieState::OwnStateConditionUpdate(void)
{
	if (isDie) {
		OwnChangeState();
	}
}

void GobKingDieState::Enter(void)
{
	isFlyEnd = false;
	playAnimeDie();
}

void GobKingDieState::Update(void)
{
	if (isAnimeEnd()) { isFlyEnd = true; }
}

void GobKingDieState::Exit(void)
{
}
