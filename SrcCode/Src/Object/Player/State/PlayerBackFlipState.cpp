#include "PlayerBackFlipState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"

PlayerBackFlipState::PlayerBackFlipState(std::function<void(void)> playAnimeIdle) :
	playAnimeBackFlip(playAnimeBackFlip)
{
}

void PlayerBackFlipState::OwnStateConditionUpdate(void)
{
	OwnChangeState();	
}

void PlayerBackFlipState::Enter(void)
{
	playAnimeBackFlip();
}