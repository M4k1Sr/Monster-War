#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../../Common/Vector3.h"

struct Vector3;

class GobKingMoveState : public CharacterStateBase
{
public:

	GobKingMoveState(
		float ACCEL_MAX, 
		float SEARCH_RANGE,
		float ATTACK_RANGE,

		std::function<void(const Vector3&)> moveAccel,
		float& accelMax,

		const Vector3& gobPos,
		const Vector3*& playerPos,

		std::function<void(void)> playAnimeWalk
	);

	~GobKingMoveState()override = default;

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

	// 息切れかどうかを返す関数
	bool IsTired(void)const { return isTired; }

private:

#pragma region 定数

	// 非ダッシュ時の最大移動速度
	const float ACCEL_MAX;

	// 索敵範囲
	const float SEARCH_RANGE;
	// 攻撃可能範囲
	const float ATTACK_RANGE;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;

	//ゴブリンキング座標
	const Vector3& gobPos;

	//プレイヤー座標参照
	const Vector3* const& playerPos;

	// 横軸加速度の最大値の参照
	float& accelMax;

	// 歩きアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeWalk;

#pragma endregion

	// 息切れ(1度スタミナが0になったかどうか)
	bool isTired;

	// 移動状態の更新
	void UpdateMove(void);
};