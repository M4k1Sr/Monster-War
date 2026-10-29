#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../Common/Vector3.h"

class PlayerBackFlipCollOperator;

class PlayerBackFlipState : public CharacterStateBase
{
public:

	PlayerBackFlipState(
		float JUST_START_TIME,
		float JUST_END_TIME,

		float BACKFLIP_POWER,
		float UP_POWER,

		Vector3& velocity,

		const bool& isGround,

		std::function<Vector3(void)> getVec,

		PlayerBackFlipCollOperator& collOperator,

		const Vector3* const& lockOnTarget,

		std::function<void(void)> playAnimeBackFlip,
		std::function<float(void)> getAnimeRatio,

		std::function<void(void)> changeStateIdle,

		std::function<void(void)> changeStateAttackSlash

	);

	~PlayerBackFlipState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// 宙返りステップ
	enum class STEP
	{
		// 前隙
		Startup,

		// ジャスト判定発生中
		Active,

		// 後隙
		Recovery
	};

	// 宙返り移動力
	const float BACKFLIP_POWER;
	// 宙返りジャンプ力
	const float UP_POWER;

	// 回避の判定を発生させ始めるアニメーション再生割合
	const float JUST_START_TIME;
	// 回避の判定を発生させ終わるアニメーション再生割合
	const float JUST_END_TIME;

	// スロー演出の範囲
	const float SLOW_START = 0.18f;
	const float SLOW_END = 0.9f;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// プレイヤーのベクトル
	Vector3& velocity;

	// 地面と設置しているか
	const bool& isGround;

	// プレイヤーの現在向いている方向
	std::function<Vector3(void)> getVec;

	// 攻撃の当たり判定管理クラスの参照
	PlayerBackFlipCollOperator& collOperator;

	// 宙返りアニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeBackFlip;

	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> getAnimeRatio;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> changeStateIdle;

	// 攻撃(斬撃)状態に遷移する関数のポインタ
	const std::function<void(void)> changeStateAttackSlash;

#pragma endregion

#pragma region ジャスト回避に必要な変数

	// 回避しているかどうか
	bool isBackFlip;

	// ターゲットがいるか
	const Vector3* const& lockOnTarget;

	// ジャンプ判定
	bool isJumped;

	// 宙返り中か
	bool isAirborne;

	// スロー判定かどうか
	bool isSlow;

	// 回避ステップ
	STEP step;

#pragma endregion

};