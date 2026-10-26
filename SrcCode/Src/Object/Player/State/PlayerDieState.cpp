#include "PlayerDieState.h"

#include "../../../Common/Vector3.h"

PlayerDieState::PlayerDieState(
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

void PlayerDieState::OwnStateConditionUpdate(void)
{
	if (isDie) {
		OwnChangeState();
	}
}

void PlayerDieState::Enter(void)
{
	isFlyEnd = false;
	playAnimeDie();
}

void PlayerDieState::Update(void)
{
	if (isAnimeEnd()) { isFlyEnd = true; }
}

void PlayerDieState::Exit(void)
{
}
