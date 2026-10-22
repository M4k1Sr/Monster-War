#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

class GobKingReactState : public CharacterStateBase
{
public:

	GobKingReactState(
		const bool& hitReact,
		std::function<void(void)> playAnimeReact);

	~GobKingReactState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

private:

#pragma region 定数
#pragma endregion

	// リアクションアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeReact;

	bool hitReact;

};