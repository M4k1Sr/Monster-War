#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

class GobKingIdleState : public CharacterStateBase
{
public:

	GobKingIdleState(
		float SEARCH_RANGE,
		float ATTACK_RANGE,

		const Vector3& gobPos,
		const Vector3*& playerPos,
		std::function<void(void)> playAnimeIdle);

	~GobKingIdleState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

private:

#pragma region 定数

	// 索敵範囲
	const float SEARCH_RANGE;
	// 攻撃可能範囲
	const float ATTACK_RANGE;

#pragma endregion

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeIdle;

	//ゴブリンキング座標
	const Vector3& gobPos;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

};