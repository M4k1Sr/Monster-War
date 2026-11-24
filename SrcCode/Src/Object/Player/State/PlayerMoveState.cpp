#include "PlayerMoveState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../Common/MotionController/GroundMotionController.h"

PlayerMoveState::PlayerMoveState(
	float ACCEL_MAX, 
	float UNDASH_SPEED_RATE, float DASH_SPEED_RATE,
	short DASH_STAMINA_MAX,

	bool& isTired,

	GroundMotionController& motion,

	std::function<bool(float)> useStamina,

	std::function<void(void)> playAnimeWalk,
	std::function<void(void)> playAnimeRun,
	std::function<void(void)> playAnimeFastRun
) :
	ACCEL_MAX(ACCEL_MAX), 
	UNDASH_SPEED_RATE(UNDASH_SPEED_RATE),DASH_SPEED_RATE(DASH_SPEED_RATE),
	DASH_STAMINA_MAX(DASH_STAMINA_MAX),

	isTired(isTired),

	motion(motion),

	useStamina(useStamina),

	playAnimeWalk(playAnimeWalk),
	playAnimeRun(playAnimeRun),
	playAnimeFastRun(playAnimeFastRun),

	isFastDash(false)
{
	isTired = false;
	// 通常移動の最大速度を設定
	motion.SetMoveSpeed(ACCEL_MAX);
}

void PlayerMoveState::OwnStateConditionUpdate(void)
{
	// 移動入力があれば、自分の状態に遷移
	if (InputVec() != 0.0f) { OwnChangeState(); }
}

void PlayerMoveState::Enter(void)
{
	// 非ダッシュ
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerFastDash).now) { playAnimeFastRun(); }
	// ダッシュ
	else { playAnimeRun(); }
}

void PlayerMoveState::Update(void)
{
	// 移動方向入力を取得
	Vector3 inputVec = InputVec();

	if (isTired) {
		UNDASH_SPEED_RATE = 0.2f;
	}
	else {
		UNDASH_SPEED_RATE = 1.0f;
	}

	// 全力ダッシュフラグを立てる(移動中じゃなければ、全力ダッシュフラグは変えない)
	isFastDash = (isTired) ? false : Input::GetIns().GetInfo(KEY_TYPE::PlayerFastDash).now && inputVec != 0.0f;

	// 移動量の最大値を更新する
	motion.SetMoveSpeed((isFastDash) ? ACCEL_MAX * DASH_SPEED_RATE * inputVec.Length() : ACCEL_MAX * UNDASH_SPEED_RATE * inputVec.Length());

	// 最終的に入力があれば加速度に加算する
	if (inputVec != 0.0f) {

		// 移動方向をカメラで回転させる
		inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// 移動
		motion.ApplyInput(inputVec);

		// 移動状態の更新(スタミナにより状態変化)
		UpdateMove();
	}
}

void PlayerMoveState::Exit(void)
{
	motion.SetMoveSpeed(ACCEL_MAX);
}

void PlayerMoveState::AlwaysUpdate(void)
{
}

Vector3 PlayerMoveState::InputVec(void) const
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

void PlayerMoveState::UpdateMove(void)
{
	// 全力ダッシュスタミナを更新 / アニメーションを更新
	if (isFastDash) {

		// 全力ダッシュしているときはスタミナを減らす
		// 足りなければ全力ダッシュをやめる（息切れはPlayer側のUseStaminaで設定される）
		if (!useStamina(1.0f)) {
			isFastDash = false;
			playAnimeWalk();
			return;
		}		

		// 全力ダッシュしているときは全力ダッシュアニメーションにする
		playAnimeFastRun();
	}
	else {

		if (isTired) {

			// 息切れしているときは歩くアニメーションにする
			playAnimeWalk();
			return;
		}

		// ダッシュしていないときは走るアニメーションにする
		playAnimeRun();
	}

}
