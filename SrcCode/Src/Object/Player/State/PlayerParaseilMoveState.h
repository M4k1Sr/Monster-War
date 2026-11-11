#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Transform;

struct Vector3;

class PlayerParaseilMoveState : public CharacterStateBase
{
public:

	PlayerParaseilMoveState(
		const float ACCEL_MAX,
		const float PARASEIL_OPEN_HEIGHT,
		float& velocityY,
		const float& footHeight,
		const bool& isGround,
		bool& isOpenParaseil,
		std::function<void(const Vector3&)> moveAccel,
		float& accelMax,
		std::function<void(void)> playAnimeHang,
		std::function<void(void)> playAnimeJumpEnd,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(bool)> setParaseilDraw,
		std::function<void(void)> changeStateIdle
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

	// パラセールを開くことができる高さ
	const float PARASEIL_OPEN_HEIGHT;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// Y軸加速度
	float& velocityY;

	// 足元の高さ
	const float& footHeight;

	// 接地判定
	const bool& isGround;

	// パラセールを開いたか
	bool& isOpenParaseil;

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;

	// 横軸加速度の最大値の参照
	float& accelMax;

	// ハングアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeHang;
	// 着地アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeJumpEnd;

	// アニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

	// パラセールの描画設定関数のポインタ
	std::function<void(bool)> setParaseilDraw;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> changeStateIdle;

#pragma endregion

	// 開くフレーム入力の判定
	bool isJustEntered;

	// 移動方向入力取得
	Vector3 InputVec(void)const;

	// ステップ
	enum class STEP {

		// グライド開始
		Glide,

		// 着地
		JumpEnd,
	};

	// 現在ステップ
	STEP step;
};
