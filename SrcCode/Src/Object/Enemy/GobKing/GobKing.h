#pragma once

#include "../../Common/CharacterBase/CharacterBase.h"

class GobKing : public CharacterBase
{
public:
	GobKing(
		const Vector3& pos,
		const  int hp
	) :
		CharacterBase(),
		playerPos(nullptr)
	{
		SetGravityFlg(true);
	}

	~GobKing()override = default;

	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	//座標渡し
	void SetPlayerPos(const Vector3* pos) { playerPos = pos; }

private:

#pragma region 定数

	// 聴覚範囲
	const float HEAR_RANGE = 2000.0f;
	// 索敵範囲
	const float SEARCH_RANGE = 1500.0f;
	// 攻撃可能範囲
	const float ATTACK_RANGE = 300.0f;

#pragma endregion

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

		Roar,

		Walk,
		
		Swing,
		Taunt,
		Throw,

		HitReact,

		Flying,

		Max
	};

	// アニメーション再生速度テーブル
	float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		0.5f,	// Idle
	
		0.5f,	// Roar

		0.5f,	// Walk

		0.5f,	// Swing
		0.5f,	// Taunt
		0.5f,	// Throw

		0.5f,	// React

		0.5f,	// Flying

	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		false,	// Roar

		true,	// Walk

		false,	// Swing
		false,	// Taunt
		false,	// Throw

		false,	// React

		false,	// Flying
	};

#pragma endregion

	// 初期化処理
	void SubInit(void)override {

		// 待機状態に遷移
		ChangeState(STATE::Idle);
	}

	void SubUpdate(void)override;

#pragma region 受けとる参照

	//座標参照
	const Vector3* playerPos;

#pragma endregion

	// 死亡した際
	void OnDeath(void)override;

};