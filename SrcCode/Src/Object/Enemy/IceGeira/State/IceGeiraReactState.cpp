#include "IceGeiraReactState.h"

IceGeiraReactState::IceGeiraReactState(
	bool& hitReact,
	const bool& isDie,
	std::function<void(void)> playAnimeDamage,
	std::function<bool(void)> isAnimeEnd,
	std::function<void(void)> returnPrevState
) :
	hitReact(hitReact),
	isDie(isDie),
	playAnimeDamage(playAnimeDamage),
	isAnimeEnd(isAnimeEnd),
	returnPrevState(returnPrevState)
{
}

void IceGeiraReactState::OwnStateConditionUpdate(void)
{
	// TODO
}

void IceGeiraReactState::Enter(void)
{
	// TODO
}

void IceGeiraReactState::Update(void)
{
	// TODO
}

void IceGeiraReactState::Exit(void)
{
	// TODO
}
