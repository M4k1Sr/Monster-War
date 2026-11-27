#include "IceGeiraIdleState.h"

#include "../../../../Common/Vector3.h"

IceGeiraIdleState::IceGeiraIdleState(
	float SEARCH_RANGE,
	const Vector3& pos,
	const Vector3* const& playerPos,
	std::function<void(void)> playAnimeIdle,
	std::function<void(void)> changeStateRoar
) :
	SEARCH_RANGE(SEARCH_RANGE),
	pos(pos),
	playerPos(playerPos),
	playAnimeIdle(playAnimeIdle),
	changeStateRoar(changeStateRoar)
{
}

void IceGeiraIdleState::Enter(void)
{
	playAnimeIdle();
}

void IceGeiraIdleState::Update(void)
{
	playAnimeIdle();

	// プレイヤーを発見したらRoarに遷移
	// 二度とIdleになることはない
	if (IsFindPlayer()) {
		changeStateRoar();
	}
}

void IceGeiraIdleState::Exit(void)
{
	// 特になし
}

bool IceGeiraIdleState::IsFindPlayer(void) const
{
	if (playerPos == nullptr) { return false; }

	// 高さ(y値)は無視
	const Vector3 diff = *playerPos - pos;
	const float distSq = diff.x * diff.x + diff.z * diff.z;

	return distSq < SEARCH_RANGE * SEARCH_RANGE;
}
