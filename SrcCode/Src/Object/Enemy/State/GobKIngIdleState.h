#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

class GobKingIdleState : public CharacterStateBase
{
public:

	GobKingIdleState(std::function<void(void)> playAnimeIdle);

	~GobKingIdleState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

private:

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeIdle;
};