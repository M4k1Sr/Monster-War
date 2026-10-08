#include "PlayerMoveState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

PlayerMoveState::PlayerMoveState(
	float ACCEL_MAX, float DASH_SPEED_RATE, short DASH_STAMINA_MAX,

	std::function<void(const Vector3&)> moveAccel,

	float& accelMax,

	std::function<void(void)> playAnimeWalk,
	std::function<void(void)> playAnimeRun,
	std::function<void(void)> playAnimeFastRun
) :
	ACCEL_MAX(ACCEL_MAX), DASH_SPEED_RATE(DASH_SPEED_RATE), DASH_STAMINA_MAX(DASH_STAMINA_MAX),

	moveAccel(moveAccel),

	accelMax(accelMax),

	playAnimeWalk(playAnimeWalk),
	playAnimeRun(playAnimeRun),
	playAnimeFastRun(playAnimeFastRun),

	isFastDash(false),
	dashStamina(DASH_STAMINA_MAX),
	isTired(false)
{
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

	// 全力ダッシュフラグを立てる(移動中じゃなければ、全力ダッシュフラグは変えない)
	isFastDash = (isTired) ? false : Input::GetIns().GetInfo(KEY_TYPE::PlayerFastDash).now && inputVec != 0.0f;

	// 移動量の最大値を更新する
	accelMax = (isFastDash) ? ACCEL_MAX * DASH_SPEED_RATE : ACCEL_MAX * inputVec.Length();

	// 最終的に入力があれば加速度に加算する
	if (inputVec != 0.0f) {

		// 移動方向をカメラで回転させる
		inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// 移動
		moveAccel(inputVec);

		// 全力ダッシュスタミナを更新 / アニメーションを更新
		if (isFastDash) {

			// 全力ダッシュしているときはスタミナを減らす
			if (--dashStamina < 0) {
				dashStamina = 0;

				// 息切れ
				isTired = true;
				playAnimeWalk();
				return;
			}

			// 全力ダッシュしているときは全力ダッシュアニメーションにする
			playAnimeFastRun();
		}
		else {
			// ダッシュしていないときは走るアニメーションにする
			playAnimeRun();
		}
	}
}

void PlayerMoveState::Exit(void)
{
	accelMax = ACCEL_MAX;
}

void PlayerMoveState::AlwaysUpdate(void)
{
	// 全力ダッシュしていないときはスタミナを回復させる
	if (!isFastDash) {
		if (++dashStamina > DASH_STAMINA_MAX) {
			dashStamina = DASH_STAMINA_MAX;

			// 息切れ回復
			isTired = false;
		}
	}
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
