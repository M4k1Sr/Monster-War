#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Transform;

struct Vector3;

class PlayerParaseilMoveState : public CharacterStateBase
{
public:

	PlayerParaseilMoveState(
		const float ACCEL_MAX,
		std::function<void(const Vector3&)> moveAccel,
		float& accelMax,
		std::function<void(void)> playAnimeHang
	);

	~PlayerParaseilMoveState()override = default;

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

	// 最大移動速度
	const float ACCEL_MAX;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;

	// 横軸加速度の最大値の参照
	float& accelMax;

	// ハングアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeHang;

#pragma endregion

	// 移動方向入力取得
	Vector3 InputVec(void)const;

	// 移動状態の更新
	void UpdateMove(void);
};