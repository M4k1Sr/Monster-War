#include "GobKingReactState.h"

#include "../../../../Common/Vector3.h"

GobKingReactState::GobKingReactState(
	bool& hitReact,
	std::function<void(void)> playAnimeReact,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle

) :
	hitReact(hitReact),
	playAnimeReact(playAnimeReact),
	isAnimeEnd(isAnimeEnd),
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
	if (isAnimeEnd()) { changeStateIdle(); }
}

void GobKingReactState::Exit(void)
{
	hitReact = false;
}
