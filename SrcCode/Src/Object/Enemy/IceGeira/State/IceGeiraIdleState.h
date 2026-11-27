#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

// 待機状態：その場で浮遊しながらプレイヤーを探す（登場直後のみ使用）
class IceGeiraIdleState : public CharacterStateBase
{
public:

	IceGeiraIdleState(
		float SEARCH_RANGE,

		const Vector3& pos,
		const Vector3* const& playerPos,

		std::function<void(void)> playAnimeIdle,

		std::function<void(void)> changeStateRoar
	);

	~IceGeiraIdleState()override = default;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// 索敵範囲
	const float SEARCH_RANGE;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 自身の座標
	const Vector3& pos;

	// プレイヤーの座標
	const Vector3* const& playerPos;

	// 待機アニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeIdle;

	// おたけび状態へ遷移する関数ポインタ
	const std::function<void(void)> changeStateRoar;

#pragma endregion

	// プレイヤーが索敵範囲に入ったか
	bool IsFindPlayer(void)const;
};
