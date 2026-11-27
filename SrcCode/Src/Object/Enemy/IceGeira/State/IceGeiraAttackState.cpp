#include "IceGeiraAttackState.h"

#include "../../../../Common/Vector3.h"

IceGeiraAttackState::IceGeiraAttackState(
	int SHOT_NUM,
	int SHOT_INTERVAL,
	int WINDUP_FRAME,
	int RECOVERY_FRAME,
	const Vector3& pos,
	const Vector3* const& playerPos,
	std::function<void(const Vector3&)> lookDir,
	std::function<void(void)> shotIcePillar,
	std::function<void(void)> playAnimeIdle,
	std::function<void(void)> changeStateWarp
) :
	SHOT_NUM(SHOT_NUM),
	SHOT_INTERVAL(SHOT_INTERVAL),
	WINDUP_FRAME(WINDUP_FRAME),
	RECOVERY_FRAME(RECOVERY_FRAME),
	pos(pos),
	playerPos(playerPos),
	lookDir(lookDir),
	shotIcePillar(shotIcePillar),
	playAnimeIdle(playAnimeIdle),
	changeStateWarp(changeStateWarp),
	step(STEP::Windup),
	stepTimer(0.0f),
	shotCount(0)
{
}

void IceGeiraAttackState::Enter(void)
{
	// TODO
}

void IceGeiraAttackState::Update(void)
{
	// TODO
}

void IceGeiraAttackState::Exit(void)
{
	// TODO
}

void IceGeiraAttackState::UpdateWindup(void)
{
	// TODO
}

void IceGeiraAttackState::UpdateShot(void)
{
	// TODO
}

void IceGeiraAttackState::UpdateRecovery(void)
{
	// TODO
}
