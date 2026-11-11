#include "PlayerJumpState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

PlayerJumpState::PlayerJumpState(
	float JUMP_POWER,

	float& velocityY,
	const bool& isGround,

	std::function<void(const Vector3&)> moveAccel,

	std::function<void(void)> playAnimeJumpStart,
	std::function<void(void)> playAnimeJumping,
	std::function<void(void)> playAnimeJumpEnd,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle
):
	JUMP_POWER(JUMP_POWER),

	velocityY(velocityY),
	isGround(isGround),

	moveAccel(moveAccel),

	playAnimeJumpStart(playAnimeJumpStart),
	playAnimeJumping(playAnimeJumping),
	playAnimeJumpEnd(playAnimeJumpEnd),

	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),

	step(STEP::Start)
{
}

void PlayerJumpState::OwnStateConditionUpdate(void)
{
	// ジャンプキーのダウントリガーで遷移
	if (!isGround) { return; }
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down) { OwnChangeState(); }
}

void PlayerJumpState::Enter(void)
{
	// フラグリセット
	step = STEP::Start;

	// ジャンプ開始アニメを再生
	playAnimeJumpStart();
}

void PlayerJumpState::Update(void)
{
	switch (step) {

	case PlayerJumpState::STEP::Start: {
		// ジャンプ開始

		// アニメーション終了
		if (isAnimeEnd()) {

			// 加速度加算
			velocityY = JUMP_POWER;

			// 滞空アニメーションを再生
			playAnimeJumping();

			// 次のステップへ
			step = STEP::Jumping;
		}

		break;
	}

	case PlayerJumpState::STEP::Jumping: {
		// 滞空

		// 着地
		if (isGround) {

			// 着地のアニメーションを再生
			playAnimeJumpEnd();

			// 次のステップへ
			step = STEP::JumpEnd;
		}

		break;
	}

	case PlayerJumpState::STEP::JumpEnd: {
		// 着地

		// この時点から入力によって再度ジャンプを可能にしておく
		OwnStateConditionUpdate();

		// アニメーション終了
		if (isAnimeEnd()) {

			// 待機状態に戻り終了
			changeStateIdle();
		}

		break;
	}

	}
}