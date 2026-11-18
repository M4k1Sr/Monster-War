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

	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle
) :
	isImpact(isImpact),
	isFlyEnd(isFlyEnd),
	getForward(getForward),
	setAddVelocity(setAddVelocity),
	playAnimeFlying(playAnimeFlying),
	playAnimeGetUp(playAnimeGetUp),
	isAnimeEnd(isAnimeEnd),
	changeStateIdle(changeStateIdle),
	step()
{
}

void PlayerFlyingState::OwnStateConditionUpdate(void)
{
	// インパクトのある攻撃を受けたら遷移
	if (isImpact) {
		OwnChangeState();
	}
}

void PlayerFlyingState::Enter(void)
{
	// 吹っ飛ぶ
	step = FLYING_STEP::Flying;

	isFlyEnd = false;
	playAnimeFlying();

	Vector3 back = -getForward();
	back.y = 0.0f;
	back = back.Normalized();

	Vector3 knock = back * 20.0f;
	knock.y = 25.0f;
	setAddVelocity(knock);

}

void PlayerFlyingState::Update(void)
{
	// ステップ別更新
	switch (step) {

	case PlayerFlyingState::FLYING_STEP::Flying: {
		// 前隙

		if (isAnimeEnd()) {

			playAnimeGetUp();

			isFlyEnd = true;

			step = FLYING_STEP::GetUp;

		}

		break;
	}
	case PlayerFlyingState::FLYING_STEP::GetUp: {

		if (isAnimeEnd()) {

			changeStateIdle();

		}
		
		break;
	}

	}
}

void PlayerFlyingState::Exit(void)
{
	isImpact = false;
}
