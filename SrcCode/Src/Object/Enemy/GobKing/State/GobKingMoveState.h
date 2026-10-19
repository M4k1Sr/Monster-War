#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

class GobKingMoveState : public CharacterStateBase
{
public:

	GobKingMoveState(
		float ACCEL_MAX, 
		std::function<void(const Vector3&)> moveAccel,
		float& accelMax,
		std::function<void(void)> playAnimeWalk,
		std::function<void(void)> playAnimeRun
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

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;

	// 横軸加速度の最大値の参照
	float& accelMax;

	// 歩きアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeWalk;
	// 走りアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeRun;

#pragma endregion

	// 息切れ(1度スタミナが0になったかどうか)
	bool isTired;

	// 移動状態の更新
	void UpdateMove(void);
};