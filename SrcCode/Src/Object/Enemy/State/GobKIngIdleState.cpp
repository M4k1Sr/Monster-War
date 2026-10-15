#include "GobKingIdleState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"

GobKingIdleState::GobKingIdleState(std::function<void(void)> playAnimeIdle) :
	playAnimeIdle(playAnimeIdle)
{
}

void GobKingIdleState::OwnStateConditionUpdate(void)
{
	Vector3 vec = Vector3();

	// ˆÚ“®“ü—Í‚ª‚È‚¯‚ê‚ÎA©•ª‚Ìó‘Ô‚É‘JˆÚ
	if (vec == 0.0f) { OwnChangeState(); }
}

void GobKingIdleState::Enter(void)
{
	playAnimeIdle();
}