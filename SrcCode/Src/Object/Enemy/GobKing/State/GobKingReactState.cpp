#include "GobKingReactState.h"

#include "../../../../Common/Vector3.h"

GobKingReactState::GobKingReactState(
	bool& hitReact,
	bool& isDie,

	std::function<void(void)> playAnimeReact,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateDie,

	std::function<void(void)> changeStateIdle

) :
	hitReact(hitReact),
	isDie(isDie),
	playAnimeReact(playAnimeReact),
	isAnimeEnd(isAnimeEnd),
	changeStateDie(changeStateDie),
	changeStateIdle(changeStateIdle)
{
}

void GobKingReactState::OwnStateConditionUpdate(void)
{
	if (hitReact){
		OwnChangeState();
	}
}

void GobKingReactState::Enter(void)
{
	hitReact = false;
	playAnimeReact();
}

void GobKingReactState::Update(void)
{
	if (isDie) { changeStateDie(); }

	if (isAnimeEnd()) { changeStateIdle(); }
}

void GobKingReactState::Exit(void)
{
	hitReact = false;
}
