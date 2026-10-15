#pragma once

#include "../Common/CharacterBase/CharacterBase.h"

class GobKing : public CharacterBase
{
public:
	GobKing();
	~GobKing()override = default;

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

		Roar,

		Throw_Rock,
		Attack_Swing,

		Hit,

		Flying,
		Death,

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

		Roar,

		DrawSword,
		WeaponIdle,
		Swing,

		ThrowRock,

		HitReact,

		Flying,
		GetUp,

		Max
	};

	// アニメーション再生速度テーブル
	float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		1.0f,	// Idle

		1.0f,	// Walk
		1.0f,	// Run

		1.0f,	// Roar
		
		1.0f,	// DrawSword
		1.0f,	// WeaponIdle
		1.0f,	// Swing

		1.0f,	// ThrowRock

		1.0f,	// HitReact

		1.0f,	// Flying
		1.0f,	// GetUp
	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		true,	// Walk
		true,	// Run

		false,	// Roar

		false,	// DrawSword
		true,	// WeaponIdle
		false,	// Swing

		false,	// TThrowRock

		false,	// HitReact

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