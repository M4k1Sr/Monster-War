#include "PlayerJumpState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../Common/MotionController/GroundMotionController.h"

PlayerJumpState::PlayerJumpState(
	float JUMP_POWER,

	GroundMotionController& motion,
	
	std::function<void(void)> playAnimeJumpStart,
	std::function<void(void)> playAnimeJumping,
	std::function<void(void)> playAnimeJumpEnd,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle
):
	JUMP_POWER(JUMP_POWER),

	motion(motion),

	playAnimeJumpStart(playAnimeJumpStart),
	playAnimeJumping(playAnimeJumping),
	playAnimeJumpEnd(playAnimeJumpEnd),

	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),

	step(STEP::Start)
{
	// ジャンプ力を設定
	motion.SetJumpSpeed(JUMP_POWER);
}

void PlayerJumpState::OwnStateConditionUpdate(void)
{
	// ジャンプキーのダウントリガーで遷移
	if (!motion.IsGrounded()) { return; }
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

			// 上向きの速度を与える（接地中のみ成功する）
			motion.TryJump();

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
		if (motion.IsGrounded()) {

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