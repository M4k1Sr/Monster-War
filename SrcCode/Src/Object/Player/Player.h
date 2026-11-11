#pragma once
#include <functional>

#include "../Common/CharacterBase/CharacterBase.h"

#include "GroundInfo.h"

class EnemyManager;

class Paraseil;
class King_Sword;

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

	void SetGroundRay(std::function<GroundInfo(const Vector3&, float)> func) { groundRay = func; }

private:
	
	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,
		Move,
		ParaseilMove,
		Jump,
		
		SkyRise,

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

		Jump,
		Jumping,
		JumpEnd,

		Slash_Down,
		Slash_Up,
		Slash_End,
		Charge,
		RollingSlash,

		BowStart,
		BowAiming,
		BowEnd,

		React,
		Flying,
		GetUp,
		Death,

		Crouch,
		Hover,

		Float,

		BackFlip,

		Hang,

		Max
	};

	// アニメーション再生速度テーブル
	float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		0.65f,	// Idle

		0.65f,	// Walk
		0.65f,	// Run
		0.65f,	// FastRun

		0.65f,	// Jump
		0.65f,	// Jumping
		0.65f,	// JumpEnd

		0.65f,	// Slash_Down
		0.65f,	// Slash_Up
		0.65f,	// Slash_End
		0.65f,	// Charge
		0.65f,	// RollingSlash

		0.65f,	// BowStart
		0.65f,	// BowAiming
		0.65f,	// BowEnd

		0.65f,	// React
		0.65f,	// Flying
		0.65f,	// GetUp
		0.65f,	// Death

		0.65f,	// Crouch
		0.65f,	// Hover

		0.65f,	// Float

		0.8f,	// BackFlip

		0.65f,	// Hang
	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		true,	// Idle

		true,	// Walk
		true,	// Run
		true,	// FastRun

		false,	// Jump
		true,	// Jumping
		false,	// JumpEnd

		false,	// Slash_Down
		false,	// Slash_Up
		false,	// Slash_End
		false,	// Charge
		false,	// RollingSlash

		false,	// BowStart
		true,	// BowAiming（構え中はループ）
		false,	// BowEnd

		false,	// React
		false,	// Flying
		false,	// GetUp
		false,	// Death（最後のポーズで止める）

		false,	// Crouch（しゃがみ待機ならループ。しゃがむ動作だけなら false）
		true,	// Hover（滞空中ずっと再生するならループ）

		true,	// Float

		false,	// BackFlip

		true,	// Hang
	};
#pragma endregion

#pragma region 真下までのレイ情報などの変数や定数

	// カプセル最下点までのオフセット（60 + 半径30）
	static constexpr float FOOT_OFFSET = 90.0f;
	static constexpr float GROUND_RAY_LENGTH = 5000.0f;

	std::function<GroundInfo(const Vector3&, float)> groundRay;
	GroundInfo groundInfo;

	// 足元から地面までの高さ
	float footHeight = 99999.0f;

#pragma endregion

#pragma region 武器・パラセールなどのクラス

	// キングソード
	King_Sword* king_Sword;

	// パラセール
	Paraseil* paraseil;

#pragma endregion

#pragma region ジャスト回避・ラッシュ攻撃系定義

	// backflipのおり始めのフレーム
	const float FALL_START = 0.4;
	// プレイヤー加速倍率
	const float PLAYER_ACCEL = 1.7f;
	// プレイヤースロー倍率
	const float PLAYER_SLOW = 0.1f;
	// 地面についたフレーム
	const float LAND_RATIO = 0.65f;

	// 敵のスロー倍率
	const float ENEMY_SLOW_SCALE = 0.05f;
	const int HOLD_LONG = 9999;

	// バク転の無敵が切れる再生割合
	const float BACKFLIP_INVINCIBLE_END = 0.85f;
	// ジャスト回避終了直後の猶予フレーム
	const int JUST_END_GRACE_FRAME = 30;

#pragma endregion

	// 初期化処理
	void SubInit(void)override {

		// 待機状態に遷移
		ChangeState(STATE::Idle);
	}

	void SubUpdate(void)override;

	void SubDraw(void)override;

	bool isOpenParaseil;

#pragma region ダメージ関係変数・関数

	// ダメージを受けたか判定
	bool hitReact;

	// 死亡したか
	bool isDie;

	// 自身が吹っ飛び終わったか
	bool isFlyEnd;

	// 無敵検知
	bool isNoDamage;

	// ダメージ
	void Damage(int damage);

	// 死亡した際
	void OnDeath(void)override;

	// 被ダメージを受け付けない状態か
	bool IsInvincible(void) const;

#pragma endregion

#pragma region ロックオン

	// ターゲット座標
	const Vector3* lockOnTarget = nullptr;

	// エネミーマネージャ
	EnemyManager* enemyManager = nullptr;

#pragma endregion

#pragma region ジャスト回避・ラッシュ攻撃判定用変数・関数

	// ジャスト回避判定
	bool isJustDodge;

	// ジャスト回避の更新
	void UpdateJustDodge(void);

	// ジャスト回避の更新遷移
	JUST_PHASE justPhase;

	// ジャスト回避スタート処理
	void StartJustDodgeSlow(void);

	// ジャスト回避終わり処理
	void EndJustDodgeSlow(void);

	// ラッシュ攻撃を始めるか
	bool isRush;

#pragma endregion

#pragma region スカイライズスキル系変数
	
	// パラセール描画判定
	bool isParaseilDraw;

	// 重力付与判定
	bool isGravity;
	
	// ジャンプボタンがホールドされているか検知
	bool isCrouchHold;

#pragma endregion
};