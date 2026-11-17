#include "GobKingDieState.h"

#include "../../../../Object/Common/PhysicsBody/PhysicsBody.h"

#include "../../../../Common/Vector3.h"

GobKingDieState::GobKingDieState(
	bool& isDie,
	bool& isFlyEnd,

	std::function<Vector3(void)> getForward,

	std::function<void(const Vector3&)> setAddVelocity,

	std::function<void(void)> playAnimeDie,

	std::function<bool(void)> isAnimeEnd
) :
	isDie(isDie),
	isFlyEnd(isFlyEnd),
	getForward(getForward),
	setAddVelocity(setAddVelocity),
	playAnimeDie(playAnimeDie),
	isAnimeEnd(isAnimeEnd)
{
}

void GobKingDieState::OwnStateConditionUpdate(void)
{
	if (isDie) {
		OwnChangeState();
	}
}

void GobKingDieState::Enter(void)
{
	isFlyEnd = false;
	playAnimeDie();

	Vector3 back = -getForward();
	back.y = 0.0f;
	back = back.Normalized();

	Vector3 knock = back * 25.0f;
	knock.y = 20.0f;
	setAddVelocity(knock);

}

void GobKingDieState::Update(void)
{
	if (isAnimeEnd()) { isFlyEnd = true; }
}

void GobKingDieState::Exit(void)
{
}
