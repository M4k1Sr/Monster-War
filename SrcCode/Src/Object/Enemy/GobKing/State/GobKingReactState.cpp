#include "GobKingReactState.h"

#include "../../../../Common/Vector3.h"

GobKingReactState::GobKingReactState(
	const bool& hitReact,
	std::function<void(void)> playAnimeReact
) :
	hitReact(hitReact),
	playAnimeReact(playAnimeReact)
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