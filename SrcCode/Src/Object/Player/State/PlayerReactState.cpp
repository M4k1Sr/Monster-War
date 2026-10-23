#include "PlayerReactState.h"

#include "../../../Common/Vector3.h"

PlayerReactState::PlayerReactState(
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

void PlayerReactState::OwnStateConditionUpdate(void)
{
	if (hitReact){
		OwnChangeState();
	}
}

void PlayerReactState::Enter(void)
{
	hitReact = false;
	playAnimeReact();
}

void PlayerReactState::Update(void)
{
	if (isAnimeEnd()) { changeStateIdle(); }
}

void PlayerReactState::Exit(void)
{
	hitReact = false;
}
