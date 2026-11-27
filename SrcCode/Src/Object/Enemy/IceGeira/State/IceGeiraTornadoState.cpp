#include "IceGeiraTornadoState.h"

IceGeiraTornadoState::IceGeiraTornadoState(
	int CHARGE_FRAME,
	int RECOVERY_FRAME,
	std::function<void(void)> spawnTornado,
	std::function<void(void)> playAnimeIdle,
	std::function<void(void)> changeStateAttack
) :
	CHARGE_FRAME(CHARGE_FRAME),
	RECOVERY_FRAME(RECOVERY_FRAME),
	spawnTornado(spawnTornado),
	playAnimeIdle(playAnimeIdle),
	changeStateAttack(changeStateAttack),
	step(STEP::Charge),
	stepTimer(0.0f)
{
}

void IceGeiraTornadoState::Enter(void)
{
	// TODO
}

void IceGeiraTornadoState::Update(void)
{
	// TODO
}

void IceGeiraTornadoState::Exit(void)
{
	// TODO
}
