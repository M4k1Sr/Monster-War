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

	std::function<void(void)> changeStateIdle,
	std::function<void(void)> changeStateMove
):
	JUMP_POWER(JUMP_POWER),

	motion(motion),

	playAnimeJumpStart(playAnimeJumpStart),
	playAnimeJumping(playAnimeJumping),
	playAnimeJumpEnd(playAnimeJumpEnd),

	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),
	changeStateMove(changeStateMove),

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
	// 移動方向入力を取得
	Vector3 inputVec = InputVec();

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

		// 移動量の最大値を更新する
		motion.SetMoveSpeed(10.0f *  inputVec.Length());

		// 最終的に入力があれば加速度に加算する
		if (inputVec != 0.0f) {

			// 移動方向をカメラで回転させる
			inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

			// 移動
			motion.ApplyInput(inputVec);
		}

		// 着地
		if (motion.IsGrounded()) {

			// 入力があれば着地アニメを待たずに移動へ
			if (InputVec() != 0.0f) {
				changeStateMove();
				break;
			}

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

Vector3 PlayerJumpState::InputVec(void) const
{
	// 返却用一時変数
	Vector3 vec = Vector3();

	// コントローラーの入力を取得
	vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
		vec.Normalize();
	}

	return vec;
}