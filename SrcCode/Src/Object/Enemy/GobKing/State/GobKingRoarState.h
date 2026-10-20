#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

struct Vector3;

class GobKingRoarState : public CharacterStateBase
{
public:

	GobKingRoarState(
		float SEARCH_RANGE,
		float ATTACK_RANGE,

		const Vector3& gobPos,
		const Vector3*& playerPos,

		std::function<void(void)> playAnimeRoar
	);

	~GobKingRoarState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 定数

	// 索敵範囲
	const float SEARCH_RANGE;
	// 攻撃可能範囲
	const float ATTACK_RANGE;

#pragma endregion

#pragma region 受け取る参照変数・関数

	//ゴブリンキング座標
	const Vector3& gobPos;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

	// おたけびアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeRoar;

#pragma endregion

};