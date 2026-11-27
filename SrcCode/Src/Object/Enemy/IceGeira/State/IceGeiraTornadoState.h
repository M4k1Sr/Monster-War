#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

// 竜巻状態：定位置で竜巻を生成する（第2フェーズのみ）
// 定位置への移動は MoveState が担当し、着いたらこの状態に入る
class IceGeiraTornadoState : public CharacterStateBase
{
public:

	IceGeiraTornadoState(
		int CHARGE_FRAME,
		int RECOVERY_FRAME,

		std::function<void(void)> spawnTornado,

		std::function<void(void)> playAnimeIdle,

		std::function<void(void)> changeStateAttack
	);

	~IceGeiraTornadoState()override = default;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// 竜巻の段階
	enum class STEP
	{
		// 溜め
		Charge,

		// 生成
		Spawn,

		// 後隙
		Recovery,
	};

	// 溜めの長さ
	const int CHARGE_FRAME;

	// 後隙の長さ
	const int RECOVERY_FRAME;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 竜巻を生成する関数ポインタ（生成自体は IceGeira 側）
	const std::function<void(void)> spawnTornado;

	// 待機アニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeIdle;

	// 攻撃状態へ遷移する関数ポインタ
	const std::function<void(void)> changeStateAttack;

#pragma endregion

	// 現在の段階
	STEP step;

	// 段階ごとの経過フレーム
	float stepTimer;
};
