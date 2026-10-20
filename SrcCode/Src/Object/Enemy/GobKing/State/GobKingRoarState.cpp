#include "GobKingRoarState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"
#include "../../../../Manager/Camera/CurrentCamera.h"

GobKingRoarState::GobKingRoarState(
	float SEARCH_RANGE,
	float ATTACK_RANGE,

	const Vector3& gobPos,
	const Vector3*& playerPos,

	std::function<void(void)> playAnimeRoar
) :
	gobPos(gobPos),
	playerPos(playerPos),

	SEARCH_RANGE(SEARCH_RANGE),
	ATTACK_RANGE(ATTACK_RANGE),

	playAnimeRoar(playAnimeRoar)
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

	if (distSq > ATTACK_RANGE * ATTACK_RANGE &&
		distSq < SEARCH_RANGE * SEARCH_RANGE)
	{
		OwnChangeState();
	}
}

void GobKingRoarState::Enter(void)
{
	//playAnimeWalk();
}

void GobKingRoarState::Update(void)
{
}

void GobKingRoarState::Exit(void)
{
}

void GobKingRoarState::AlwaysUpdate(void)
{
}