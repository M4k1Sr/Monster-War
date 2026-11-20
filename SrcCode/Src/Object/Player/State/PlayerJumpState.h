#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;
class GroundMotionController;

class PlayerJumpState : public CharacterStateBase
{
public:

	PlayerJumpState(
		float JUMP_POWER,

		GroundMotionController& motion,

		std::function<void(void)> PlayAnimeJumpStart,
		std::function<void(void)> PlayAnimeJuming,
		std::function<void(void)> PlayAnimeJumpEnd,

		std::function<bool(void)> isAnimeEnd,

		std::function<void(void)> changeStateIdle,
		std::function<void(void)> changeStateMove
	);

	~PlayerJumpState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;

private:

#pragma region 定数

	// ジャンプ力
	const float JUMP_POWER;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 移動制御（ジャンプと接地判定）
	GroundMotionController& motion;

	// ジャンプ開始アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeJumpStart;
	// ジャンプ滞空アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeJumping;
	// 着地アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeJumpEnd;

	// アニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> changeStateIdle;

	// 移動状態に戻る関数のポインタ
	const std::function<void(void)> changeStateMove;

#pragma endregion

	// ステップ
	enum class STEP {

		// ジャンプ開始
		Start,

		// 滞空
		Jumping,

		// 着地
		JumpEnd,
	};

	// 現在ステップ
	STEP step;

	// 移動方向入力取得
	Vector3 InputVec(void)const;

};