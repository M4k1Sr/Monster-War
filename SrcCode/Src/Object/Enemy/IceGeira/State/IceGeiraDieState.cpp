#include "IceGeiraDieState.h"

IceGeiraDieState::IceGeiraDieState(
	const bool& isDie,
	bool& isFlyEnd,
	const bool& isGround,
	std::function<void(bool)> setGravityEnabled,
	std::function<void(void)> playAnimeDeath,
	std::function<bool(void)> isAnimeEnd
) :
	isDie(isDie),
	isFlyEnd(isFlyEnd),
	isGround(isGround),
	setGravityEnabled(setGravityEnabled),
	playAnimeDeath(playAnimeDeath),
	isAnimeEnd(isAnimeEnd)
{
}

void IceGeiraDieState::OwnStateConditionUpdate(void)
{
	// TODO
}

void IceGeiraDieState::Enter(void)
{
	// TODO
}

void IceGeiraDieState::Update(void)
{
	// TODO
}

void IceGeiraDieState::Exit(void)
{
	// TODO
}
