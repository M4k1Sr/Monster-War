#pragma once

#include "../Common/CharacterBase/CharacterBase.h"

class Player : public CharacterBase
{
public:
	Player();
	~Player()override = default;

	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

private:

	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,
		Move,
		Jump,

		AttackSlash,

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Idle,

		Walk,
		Run,
		FastRun,
		WalkBack,
		StepRight,
		StepLeft,

		InPlaceJump,
		BackFlip,

		DrawSword,
		WeaponIdle,
		Slash_Down,
		Slash_Up,
		Slash_End,

		Block,
		BlockIdle,
		BlockHit,

		Flying,
		GetUp,

		Max
	};

	// アニメーション再生速度テーブル
	float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		0.65f,	// Idle

		0.65f,	// Walk
		0.65f,	// Run
		0.65f,	// FastRun
		0.65f,	// WalkBack
		0.65f,	// StepRight
		0.65f,	// StepLeft

		1.0f,	// InPlaceJump
		1.0f,	// BackFlip

		0.65f,	// DrawSword
		0.65f,	// WeaponIdle
		0.65f,	// Slash_Down
		0.65f,	// Slash_Up
		0.65f,	// Slahs_End

		0.65f,	// Block
		0.65f,	// BlockIdle
		0.65f,	// BlockHit

		0.65f,	// Flying
		0.65f,	// GetUp

	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		true,	// Walk
		true,	// Run
		true,	// FastRun
		true,	// WalkBack
		true,	// StepRight
		true,	// StepLeft
		false,	// InPlaceJump
		false,	// BackFlip

		false,	// DrawSword
		true,	// WeaponIdle
		false,	// Slash_Down
		false,	// Slash_Up
		false,	// Slahs_End

		false,	// Block
		true,	// BlockIdle
		false,	// BlockHit
		false,	// Flying
		false,	// GetUp

	};

#pragma endregion

	// 初期化処理
	void SubInit(void)override {

		// 待機状態に遷移
		ChangeState(STATE::Idle);
	}

	void SubUpdate(void)override;

};