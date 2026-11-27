#include "IceGeiraWarpState.h"

#include "../../../../Common/Vector3.h"

IceGeiraWarpState::IceGeiraWarpState(
	float WARP_DISTANCE,
	float WARP_HEIGHT,
	int VANISH_FRAME,
	Vector3& pos,
	const Vector3* const& playerPos,
	std::function<void(bool)> setVisible,
	std::function<void(void)> playAnimeSwim,
	std::function<void(void)> onWarpEnd
) :
	WARP_DISTANCE(WARP_DISTANCE),
	WARP_HEIGHT(WARP_HEIGHT),
	VANISH_FRAME(VANISH_FRAME),
	pos(pos),
	playerPos(playerPos),
	setVisible(setVisible),
	playAnimeSwim(playAnimeSwim),
	onWarpEnd(onWarpEnd),
	step(STEP::Enter),
	stepTimer(0.0f)
{
}

void IceGeiraWarpState::Enter(void)
{
	// TODO
}

void IceGeiraWarpState::Update(void)
{
	// TODO
}

void IceGeiraWarpState::Exit(void)
{
	// TODO
}

Vector3 IceGeiraWarpState::CalcWarpPos(void) const
{
	// TODO
	return Vector3();
}

void IceGeiraWarpState::UpdateEnter(void)
{
	// TODO
}

void IceGeiraWarpState::UpdateVanish(void)
{
	// TODO
}

void IceGeiraWarpState::UpdateExit(void)
{
	// TODO
}
