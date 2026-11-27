#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

// 移動状態：目的地(moveTarget)まで飛ぶだけ
// 着いた後どこへ行くかは IceGeira 側(nextStateAfterMove)が決める
class IceGeiraMoveState : public CharacterStateBase
{
public:

	IceGeiraMoveState(
		float MOVE_SPEED,
		float ARRIVE_RANGE,

		Vector3& pos,
		const Vector3& moveTarget,

		std::function<void(const Vector3&)> lookDir,

		std::function<void(void)> playAnimeSwim,

		std::function<void(void)> onArrive
	);

	~IceGeiraMoveState()override = default;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// 移動速度
	const float MOVE_SPEED;

	// 到着したとみなす距離
	const float ARRIVE_RANGE;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 自身の座標
	Vector3& pos;

	// 目的地
	const Vector3& moveTarget;

	// 指定方向へ向きを変える関数ポインタ
	const std::function<void(const Vector3&)> lookDir;

	// 泳ぐ(飛行)アニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeSwim;

	// 目的地に着いたときに呼ぶ関数ポインタ
	const std::function<void(void)> onArrive;

#pragma endregion

	// 目的地に着いたか
	bool IsArrive(void)const;
};
