#include "GobKingRoarState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"
#include "../../../../Manager/Camera/CurrentCamera.h"

GobKingRoarState::GobKingRoarState(
	float HEAR_RANGE,

	const Vector3& gobPos,
	const Vector3*& playerPos,

	std::function<void(void)> playAnimeRoar,

	std::function<void(void)> changeStateIdle,
	std::function<void(void)> changeStateMove
) :
	gobPos(gobPos),
	playerPos(playerPos),

	HEAR_RANGE(HEAR_RANGE),

	playAnimeRoar(playAnimeRoar),

	changeStateIdle(changeStateIdle),
	changeStateMove(changeStateMove)
{
}

void GobKingRoarState::OwnStateConditionUpdate(void)
{
	// プレイヤーがいなければ何もしない
	if (playerPos == nullptr) { return; }

	// 攻撃範囲外ならばおたけび状態へ遷移
	// 攻撃範囲ならreturn

	// プレイヤー間の距離計算
	// 高さ(y値)は無視
	Vector3 diff = *playerPos - gobPos;
	float distSq = diff.x * diff.x + diff.z * diff.z;

	if (distSq < HEAR_RANGE * HEAR_RANGE)
	{
		OwnChangeState();
	}
}

void GobKingRoarState::Enter(void)
{
}

void GobKingRoarState::Update(void)
{
	// プレイヤー間の距離計算
	// 高さ(y値)は無視
	Vector3 diff = *playerPos - gobPos;
	float distSq = diff.x * diff.x + diff.z * diff.z;

	if (distSq < HEAR_RANGE * HEAR_RANGE)
	{
		playAnimeRoar();
	}
}

void GobKingRoarState::Exit(void)
{
}

void GobKingRoarState::AlwaysUpdate(void)
{
}