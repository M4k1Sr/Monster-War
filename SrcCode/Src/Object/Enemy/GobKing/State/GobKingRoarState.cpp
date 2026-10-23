#include "GobKingRoarState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/InputManager.h"
#include "../../../../Manager/Camera/CurrentCamera.h"

GobKingRoarState::GobKingRoarState(
	float HEAR_RANGE,

	const Vector3& gobPos,
	const Vector3*& playerPos,

	std::function<void(void)> playAnimeRoar,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle,
	std::function<void(void)> changeStateMove
) :
	gobPos(gobPos),
	playerPos(playerPos),

	HEAR_RANGE(HEAR_RANGE),

	playAnimeRoar(playAnimeRoar),

	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),
	changeStateMove(changeStateMove),
	hasRoared(false)
{
}

void GobKingRoarState::OwnStateConditionUpdate(void)
{
	// 一度吠えたら再発見では吠えない
	if (hasRoared) { return; }

	// 聴覚範囲に入ったらおたけび状態へ
	if (IsInHearRange()) {
		OwnChangeState();
	}
}

void GobKingRoarState::Enter(void)
{
	hasRoared = true;
 	playAnimeRoar();
}

void GobKingRoarState::Update(void)
{
	// 雄たけびアニメが終わったら追跡へ
	// 聴覚範囲外なら待機状態へ
	if (isAnimeEnd()) {
		if (IsInHearRange()) { 
			hasRoared = true;	// 一度きりならばここをtrueに変更
			changeStateMove(); 
		}
		else {
			hasRoared = true;	// 一度きりならばここをtrueに変更
			changeStateIdle();
		}
	}
}

void GobKingRoarState::Exit(void)
{
}

void GobKingRoarState::AlwaysUpdate(void)
{
}

bool GobKingRoarState::IsInHearRange(void) const
{
	if (playerPos == nullptr) { return false; }

	// 高さ(y値)は無視
	const Vector3 diff = *playerPos - gobPos;
	const float distSq = diff.x * diff.x + diff.z * diff.z;

	return distSq < HEAR_RANGE * HEAR_RANGE;
}