#include "GobKingMoveState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"
#include "../../../../Manager/Camera/CurrentCamera.h"

GobKingMoveState::GobKingMoveState(
	float ACCEL_MAX,

	std::function<void(const Vector3&)> moveAccel,

	float& accelMax,

	std::function<void(void)> playAnimeWalk,
	std::function<void(void)> playAnimeRun
) :
	ACCEL_MAX(ACCEL_MAX),

	moveAccel(moveAccel),

	accelMax(accelMax),

	playAnimeWalk(playAnimeWalk),
	playAnimeRun(playAnimeRun),

	isTired(false)
{
}

void GobKingMoveState::OwnStateConditionUpdate(void)
{
	//// 移動入力があれば、自分の状態に遷移
	//if (move != 0.0f) { OwnChangeState(); }
}

void GobKingMoveState::Enter(void)
{
	playAnimeWalk();
}

void GobKingMoveState::Update(void)
{
	//// 移動方向入力を取得
	//Vector3 inputVec = InputVec();

	//// 移動量の最大値を更新する
	//accelMax = ACCEL_MAX * inputVec.Length();

	//// 移動方向をカメラで回転させる
	//inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

	//// 移動
	//moveAccel(inputVec);

	//// 移動状態の更新(スタミナにより状態変化)
	//UpdateMove();

}

void GobKingMoveState::Exit(void)
{
	accelMax = ACCEL_MAX;
}

void GobKingMoveState::AlwaysUpdate(void)
{
	// 息切れ回復
	isTired = false;
}

void GobKingMoveState::UpdateMove(void)
{
	//// 息切れしているときは歩くアニメーションにする
	//playAnimeWalk();

	//// ダッシュしていないときは走るアニメーションにする
	//playAnimeRun();

}
