#include "GobKingIdleState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"

GobKingIdleState::GobKingIdleState(
	float SEARCH_RANGE,
	float ATTACK_RANGE,

	const Vector3& gobPos,
	const Vector3*& playerPos,
	std::function<void(void)> playAnimeIdle
):
	gobPos(gobPos),
	playerPos(playerPos),

	SEARCH_RANGE(SEARCH_RANGE),
	ATTACK_RANGE(ATTACK_RANGE),

	playAnimeIdle(playAnimeIdle)
{
}

void GobKingIdleState::OwnStateConditionUpdate(void)
{
	if (playerPos == nullptr) { OwnChangeState(); }

	Vector3 diff = *playerPos - gobPos;
	float distSq = diff.x * diff.x + diff.z * diff.z;

	// õ“G”ÍˆÍ‚ÌŠOA‚Ü‚½‚ÍUŒ‚”ÍˆÍ‚Ì“à‘¤ ¨ ‘Ò‹@
	if (distSq >= SEARCH_RANGE * SEARCH_RANGE)
	{
		OwnChangeState();
		return;
	}
}

void GobKingIdleState::Enter(void)
{
	playAnimeIdle();
}