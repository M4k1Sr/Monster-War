#include "PlayerParaseilMoveState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../Common/ActorBase/ActorBase.h"

PlayerParaseilMoveState::PlayerParaseilMoveState(
	const float ACCEL_MAX,

	std::function<void(const Vector3&)> moveAccel,

	float& accelMax,

	std::function<void(void)> playAnimeHang
) :
	ACCEL_MAX(ACCEL_MAX),

	moveAccel(moveAccel),

	accelMax(accelMax),

	playAnimeHang(playAnimeHang)
{
}


void PlayerParaseilMoveState::OwnStateConditionUpdate(void)
{
	// 移動入力があれば、自分の状態に遷移
	if (InputVec() != 0.0f) { OwnChangeState(); }
}

void PlayerParaseilMoveState::Enter(void)
{
}

void PlayerParaseilMoveState::Update(void)
{
	// 移動方向入力を取得
	Vector3 inputVec = InputVec();

	// 最終的に入力があれば加速度に加算する
	if (inputVec != 0.0f) {

		// 移動方向をカメラで回転させる
		inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// 移動
		moveAccel(inputVec);

		// 移動状態の更新(スタミナにより状態変化)
		UpdateMove();
	}
}

void PlayerParaseilMoveState::Exit(void)
{
	accelMax = ACCEL_MAX;
}

void PlayerParaseilMoveState::AlwaysUpdate(void)
{
}

Vector3 PlayerParaseilMoveState::InputVec(void) const
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

void PlayerParaseilMoveState::UpdateMove(void)
{
}
