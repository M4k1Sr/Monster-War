#include "IceGeiraRoarState.h"

IceGeiraRoarState::IceGeiraRoarState(
	int ROAR_FRAME,
	bool& isPhaseChange,
	std::function<void(void)> playAnimeRoar,
	std::function<void(void)> onRoarEnd
) :
	ROAR_FRAME(ROAR_FRAME),
	isPhaseChange(isPhaseChange),
	playAnimeRoar(playAnimeRoar),
	onRoarEnd(onRoarEnd),
	roarTimer(0.0f)
{
}

void IceGeiraRoarState::OwnStateConditionUpdate(void)
{
	// TODO
}

void IceGeiraRoarState::Enter(void)
{
	// TODO
}

void IceGeiraRoarState::Update(void)
{
	// TODO
}

void IceGeiraRoarState::Exit(void)
{
	// TODO
}
