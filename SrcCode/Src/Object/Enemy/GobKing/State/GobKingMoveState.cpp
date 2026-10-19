#include "GobKingMoveState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"
#include "../../../../Manager/Camera/CurrentCamera.h"

GobKingMoveState::GobKingMoveState(
	float ACCEL_MAX,
	float SEARCH_RANGE,
	float ATTACK_RANGE,

	std::function<void(const Vector3&)> moveAccel,

	float& accelMax,

	const Vector3& gobPos,
	const Vector3*& playerPos,

	std::function<void(void)> playAnimeWalk,
	std::function<void(void)> playAnimeRun
) :
	gobPos(gobPos),
	playerPos(playerPos),

	ACCEL_MAX(ACCEL_MAX),
	SEARCH_RANGE(SEARCH_RANGE),
	ATTACK_RANGE(ATTACK_RANGE),

	moveAccel(moveAccel),

	accelMax(accelMax),

	playAnimeWalk(playAnimeWalk),
	playAnimeRun(playAnimeRun),

	isTired(false)
{
}

void GobKingMoveState::OwnStateConditionUpdate(void)
{
	// プレイヤーがいなければ何もしない
	if (playerPos == nullptr) { return; }

	// 攻撃範囲外ならば移動状態へ遷移
	// 攻撃範囲ならreturn

	// プレイヤー間の距離計算
	// 高さ(y値)は無視
	Vector3 diff = *playerPos - gobPos;
	float distSq = diff.x * diff.x + diff.z * diff.z;

	if (distSq > ATTACK_RANGE * ATTACK_RANGE &&
		distSq < SEARCH_RANGE * SEARCH_RANGE)
	{
		OwnChangeState();
	}
}

void GobKingMoveState::Enter(void)
{
	//playAnimeWalk();
}

void GobKingMoveState::Update(void)
{
	// プレイヤーがいなければ何もしない
	if (playerPos == nullptr) { return; }

	// プレイヤーへの方向ベクトル（高さは無視して水平だけ）
	Vector3 dir = *playerPos - gobPos;
	dir.y = 0.0f;

	// ほぼ重なってるときは正規化で0割りになるので何もしない
	float lenSq = dir.x * dir.x + dir.z * dir.z;
	if (lenSq < 0.0001f) { return; }

	// 正規化（単位ベクトルにする）
	dir = dir.Normalized();   // 関数名は自分のVector3に合わせて

	// 移動量の最大値を設定
	accelMax = ACCEL_MAX;

	// その方向に加速
	moveAccel(dir);

	// 移動状態の更新(スタミナにより状態変化)
	UpdateMove();

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
	// 歩くアニメーションにする
	playAnimeWalk();

	//// ダッシュしていないときは走るアニメーションにする
	//playAnimeRun();

}
