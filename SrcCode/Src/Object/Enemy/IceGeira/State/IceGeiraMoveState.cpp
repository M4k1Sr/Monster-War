#include "IceGeiraMoveState.h"

#include "../../../../Common/Vector3.h"

IceGeiraMoveState::IceGeiraMoveState(
	float MOVE_SPEED,
	float ARRIVE_RANGE,
	Vector3& pos,
	const Vector3& moveTarget,
	std::function<void(const Vector3&)> lookDir,
	std::function<void(void)> playAnimeSwim,
	std::function<void(void)> onArrive
) :
	MOVE_SPEED(MOVE_SPEED),
	ARRIVE_RANGE(ARRIVE_RANGE),
	pos(pos),
	moveTarget(moveTarget),
	lookDir(lookDir),
	playAnimeSwim(playAnimeSwim),
	onArrive(onArrive)
{
}

void IceGeiraMoveState::Enter(void)
{
	// TODO
}

void IceGeiraMoveState::Update(void)
{
	// TODO
}

void IceGeiraMoveState::Exit(void)
{
	// TODO
}

bool IceGeiraMoveState::IsArrive(void) const
{
	// TODO
	return false;
}
