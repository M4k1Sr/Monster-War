#include "IceGeiraRoarState.h"

IceGeiraRoarState::IceGeiraRoarState(
	bool& isPhaseChange,

	std::function<void(void)> playAnimeRoar,
	std::function<bool(void)> isAnimeEnd,
	std::function<void(void)> changeStateMove
) :
	isPhaseChange(isPhaseChange),
	playAnimeRoar(playAnimeRoar),
	isAnimeEnd(isAnimeEnd),
	changeStateMove(changeStateMove)
{
}

void IceGeiraRoarState::OwnStateConditionUpdate(void)
{
	if (isPhaseChange) {
		OwnChangeState();
	}
}

void IceGeiraRoarState::Enter(void)
{
	playAnimeRoar();
}

void IceGeiraRoarState::Update(void)
{
	if (isAnimeEnd) {
		changeStateMove();
	}
}

void IceGeiraRoarState::Exit(void)
{
}
