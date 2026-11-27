#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

class IceGeiraMoveState : public CharacterStateBase
{
public:

	// 移動パターン
	enum class MOVE_PATTERN
	{
		Horizontal,	// 平行移動
		Vertical,	// 垂直移動
	};

	IceGeiraMoveState(
		float MOVE_SPEED,
		float ARRIVE_RANGE,

		Vector3& pos,
		const Vector3& moveTarget,
		const MOVE_PATTERN& movePattern,

		std::function<void(const Vector3&)> lookDir,

		std::function<void(void)> playAnimeSwim,

		std::function<void(void)> onArrive,

		std::function<float(void)> getTimeScale
	);

	~IceGeiraMoveState()override = default;

	// 状態遷移後1度行う初期化処理
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

	// 移動パターン
	const MOVE_PATTERN& movePattern;

	// 指定方向へ向きを変える関数ポインタ
	const std::function<void(const Vector3&)> lookDir;

	// 泳ぐ(飛行)アニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeSwim;

	// 目的地に着いたときに呼ぶ関数ポインタ
	const std::function<void(void)> onArrive;

	// 時間倍率を呼ぶ関数ポインタ
	const std::function<float(void)> getTimeScale;

#pragma endregion

	// 平行移動の更新
	void UpdateHorizontal(void);
	// 垂直移動の更新
	void UpdateVertical(void);

	// 目的地に着いたか（移動パターンによって判定する軸を変える）
	bool IsArrive(void)const;
};