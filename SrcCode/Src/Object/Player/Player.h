#pragma once

#include "../Common/CharacterBase/CharacterBase.h"

class EnemyManager;

class Player : public CharacterBase
{
public:
	Player();
	~Player()override = default;

	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	// エネミー座標渡し
	void SetEnemyManager(EnemyManager* manager) { enemyManager = manager; }

private:

	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,
		Move,
		Jump,

		AttackSlash,
		AttackRollingSlash,

		BackFlip,

		React,
		Flying,

		Max
	};
	
	// スロー状態定義
	enum class JUST_PHASE
	{
		None,

		EnemySlow,

		AllSlow,

		Rush,
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
		Charge,
		RollingSlash,

		Block,
		BlockIdle,
		BlockHit,

		Flying,
		GetUp,

		JumpStart,
		Jumping,
		JumpEnd,

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
		0.8f,	// BackFlip

		0.65f,	// DrawSword
		0.65f,	// WeaponIdle
		0.65f,	// Slash_Down
		0.65f,	// Slash_Up
		0.65f,	// Slahs_End
		0.65f,	// Charge
		0.65f,	// RollingSlash

		0.65f,	// Block
		0.65f,	// BlockIdle
		0.65f,	// BlockHit

		0.65f,	// Flying
		0.65f,	// GetUp

		0.65f,	// JumpStart
		0.65f,	// Jumping
		0.65f,	// JumpEnd

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
		false,	// Charge
		false,	// RollingSlash

		false,	// Block
		true,	// BlockIdle
		false,	// BlockHit
		false,	// Flying
		false,	// GetUp

		false,	// JumpStart
		true,	// Jumping
		false,	// JumpEnd

	};

	// ラッシュ攻撃するかの猶予フレーム
	const int RUSH_WAIT_MAX = 60;
	// 戻りのイージング
	const int RUSH_EASE_FRAME = 15;   

	// backflipのおり始めのフレーム
	const float FALL_START = 0.4;
	// プレイヤー加速倍率
	const float PLAYER_ACCEL = 2.0f;
	// プレイヤースロー倍率
	const float PLAYER_SLOW = 0.1f;
	// 地面についたフレーム
	const float LAND_RATIO = 0.65f;

	// 敵のスロー倍率
	const float ENEMY_SLOW_SCALE = 0.1f;
	const int HOLD_LONG = 9999;

#pragma endregion

	// 初期化処理
	void SubInit(void)override {

		// 待機状態に遷移
		ChangeState(STATE::Idle);
	}

	void SubUpdate(void)override;

#pragma region ダメージ関係

	// ダメージを受けたか判定
	bool hitReact;

	// 死亡したか
	bool isDie;

	// 敵が吹っ飛び終わったか
	bool isFlyEnd;

	// ダメージ
	void Damage(int damage);

	// 死亡した際
	void OnDeath(void)override;

#pragma endregion

#pragma region ロックオン

	// ターゲット座標
	const Vector3* lockOnTarget = nullptr;

	// エネミーマネージャ
	EnemyManager* enemyManager = nullptr;

#pragma endregion

#pragma region ジャスト回避判定

	// ジャスト回避判定
	bool isJustDodge;

	// スロー演出成功判定
	bool isSlowSuccess;

	// ジャスト回避成功時の演出処理
	void JustDodgeSuccess(void);

	// ジャスト回避がおきたときのスローが終わるまで
	void JustDodgeEnd(void);

	// ラッシュ攻撃したかの判定
	bool isRushWait;

	// ラッシュフレームカウンタ
	int rushWaitCounter;

	// ジャスト回避の更新
	void UpdateJustDodge(void);

	// ジャスト回避の更新遷移
	JUST_PHASE justPhase;

	// ジャスト回避スタート処理
	void StartJustDodgeSlow(void);

	// ジャスト回避終わり処理
	void EndJustDodgeSlow(void);

#pragma endregion

};