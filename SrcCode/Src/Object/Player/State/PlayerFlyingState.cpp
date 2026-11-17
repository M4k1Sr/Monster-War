#include "PlayerFlyingState.h"

#include "../../../Object/Common/PhysicsBody/PhysicsBody.h"

#include "../../../Common/Vector3.h"

PlayerFlyingState::PlayerFlyingState(
	bool& isImpact,
	bool& isFlyEnd,

	std::function<Vector3(void)> getForward,

	std::function<void(const Vector3&)> setAddVelocity,

	std::function<void(void)> playAnimeFlying,
	std::function<void(void)> playAnimeGetUp,

	std::function<bool(void)> isAnimeEnd
) :
	isImpact(isImpact),
	isFlyEnd(isFlyEnd),
	getForward(getForward),
	setAddVelocity(setAddVelocity),
	playAnimeFlying(playAnimeFlying),
	playAnimeGetUp(playAnimeGetUp),
	isAnimeEnd(isAnimeEnd)
{
}

void PlayerFlyingState::OwnStateConditionUpdate(void)
{
	if (isImpact) {
		OwnChangeState();
	}
}

void PlayerFlyingState::Enter(void)
{
	isFlyEnd = false;
	playAnimeFlying();

	Vector3 back = -getForward();
	back.y = 0.0f;
	back = back.Normalized();

	Vector3 knock = back * 25.0f;
	knock.y = 20.0f;
	setAddVelocity(knock);

}

void PlayerFlyingState::Update(void)
{
	if (isAnimeEnd()) {

		playAnimeGetUp();

		isFlyEnd = true;
	}

}

void PlayerFlyingState::Exit(void)
{
	isImpact = false;
}
