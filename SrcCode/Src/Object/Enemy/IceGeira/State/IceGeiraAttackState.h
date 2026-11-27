#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

// 攻撃状態：その場に止まって氷柱を撃つ（移動はしない）
class IceGeiraAttackState : public CharacterStateBase
{
public:

	IceGeiraAttackState(
		int SHOT_NUM,
		int SHOT_INTERVAL,
		int WINDUP_FRAME,
		int RECOVERY_FRAME,

		const Vector3& pos,
		const Vector3* const& playerPos,

		std::function<void(const Vector3&)> lookDir,

		std::function<void(void)> shotIcePillar,

		std::function<void(void)> playAnimeIdle,

		std::function<void(void)> changeStateWarp
	);

	~IceGeiraAttackState()override = default;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// 攻撃の段階
	enum class STEP
	{
		// 予兆
		Windup,

		// 発射中
		Shot,

		// 後隙
		Recovery,
	};

	// 1回の攻撃で撃つ氷柱の数
	const int SHOT_NUM;

	// 氷柱を撃つ間隔
	const int SHOT_INTERVAL;

	// 予兆の長さ
	const int WINDUP_FRAME;

	// 後隙の長さ
	const int RECOVERY_FRAME;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 自身の座標
	const Vector3& pos;

	// プレイヤーの座標
	const Vector3* const& playerPos;

	// 指定方向へ向きを変える関数ポインタ
	const std::function<void(const Vector3&)> lookDir;

	// 氷柱を生成する関数ポインタ（生成自体は IceGeira 側）
	const std::function<void(void)> shotIcePillar;

	// 待機アニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeIdle;

	// ワープ状態へ遷移する関数ポインタ
	const std::function<void(void)> changeStateWarp;

#pragma endregion

	// 現在の段階
	STEP step;

	// 段階ごとの経過フレーム
	float stepTimer;

	// 撃った数
	int shotCount;

	// 各段階の更新
	void UpdateWindup(void);
	void UpdateShot(void);
	void UpdateRecovery(void);
};
