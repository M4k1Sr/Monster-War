#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

struct Vector3;

class GobKingRoarState : public CharacterStateBase
{
public:

	GobKingRoarState(
		float HEAR_RANGE,

		const Vector3& gobPos,
		const Vector3*& playerPos,

		std::function<void(void)> playAnimeRoar,

		std::function<void(void)> changeStateIdle,
		std::function<void(void)> changeStateMove

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

	// 聴覚範囲
	const float HEAR_RANGE;

#pragma endregion

#pragma region 受け取る参照変数・関数

	//ゴブリンキング座標
	const Vector3& gobPos;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

	// おたけびアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeRoar;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> changeStateIdle;
	// 移動状態に遷移する関数のポインタ
	const std::function<void(void)> changeStateMove;

#pragma endregion

};