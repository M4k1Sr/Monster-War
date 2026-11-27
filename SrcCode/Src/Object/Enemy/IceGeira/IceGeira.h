#pragma once

#include "../../Common/CharacterBase/CharacterBase.h"

#include <vector>

#include "State/IceGeiraMoveState.h"

class IceGeira : public CharacterBase
{
public:
	IceGeira(
		const Vector3& pos,
		const int initHp
	) :
		CharacterBase(),
		playerPos(nullptr),
		hitReact(false),
		isDie(false),
		isFlyEnd(false),
		phase(1),
		maxHp(initHp),
		isPhaseChange(false),
		moveTarget(Vector3()),
		nextStateAfterMove(STATE::Idle),
		prevState(STATE::Idle),
		isVisible(true)
	{
		invincibleFrame = INVINCIBLE_FRAME_MAX;
		hp = initHp;
	}

	~IceGeira()override = default;

	// 読み込み
	void Load(void)override;

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

	//座標渡し
	void SetPlayerPos(const Vector3* pos) { playerPos = pos; }

private:

#pragma region 定数

	// 索敵範囲
	const float SEARCH_RANGE = 6000.0f;

	// ダメージを受けた際の無敵猶予フレーム
	static constexpr int INVINCIBLE_FRAME_MAX = 30;

	// 移動速度
	const float MOVE_SPEED = 10.0f;
	// 目的地に到着したとみなす距離
	const float ARRIVE_RANGE = 100.0f;

	// 登場時の初期座標
	const Vector3 SPAWN_POS = Vector3(0.0f, 800.0f, -5000.0f);
	// 登場時の目的地
	const Vector3 ENTRY_POS = Vector3(0.0f, 800.0f, -1500.0f);

	// プレイヤーを中心にポイントがどれだけ離れているか
	const float PATROL_DISTANCE = 4500;

	// プレイヤーから離れた位置に出現する距離
	const float WARP_DISTANCE = 2000.0f;
	// 出現する高さ
	const float WARP_HEIGHT = 800.0f;
	// 消えている時間
	const int VANISH_FRAME = 60;

	// 1回の攻撃で撃つ氷柱の数
	const int SHOT_NUM = 3;
	// 氷柱を撃つ間隔
	const int SHOT_INTERVAL = 30;
	// 攻撃の予兆の長さ
	const int ATTACK_WINDUP_FRAME = 60;
	// 攻撃の後隙の長さ
	const int ATTACK_RECOVERY_FRAME = 60;

	// 竜巻を発生させる定位置
	const Vector3 TORNADO_POS = Vector3(0.0f, 1200.0f, 0.0f);
	// 竜巻の溜めの長さ
	const int TORNADO_CHARGE_FRAME = 90;
	// 竜巻の後隙の長さ
	const int TORNADO_RECOVERY_FRAME = 60;

#pragma endregion

	// 状態定義
	enum class STATE
	{
		None = -1,

		Idle,
		Move,

		Warp,

		Roar,

		Attack,
		Tornado,

		React,

		Die,

		Max
	};

#pragma region アニメーション関係定義

	// アニメーションタイプ定義
	enum class ANIME_TYPE
	{
		None = -1,

		Damage,

		Death,

		Idle,

		Roar,

		Swim,

		Max
	};

	// アニメーション再生速度テーブル
	float ANIME_SPEED_TABLE[(int)ANIME_TYPE::Max] =
	{
		0.5f,	// Damage
		0.5f,	// Death
		0.5f,	// Idle
		0.5f,	// Swim
		0.5f,	// Roar
	};

	// アニメーションループ再生フラグテーブル
	const bool ANIME_LOOP_TABLE[(int)ANIME_TYPE::Max] =
	{
		false,	// Damage
		false,	// Death
		false,	// Roar
		true,	// Idle
		true,	// Swim
	};

#pragma endregion

	// 初期化処理
	void SubInit(void)override {

		// 奥から登場 → 着いたら待機
		trans.pos = SPAWN_POS;
		moveTarget = ENTRY_POS;
		nextStateAfterMove = STATE::Idle;

		ChangeState(STATE::Move);
	}

	void SubUpdate(void)override;

	// スキンメッシュ用の頂点シェーダー（骨8本影響）を使う
	SHADER_VERTEX_TYPE ShaderVertexType(void)const override { return SHADER_VERTEX_TYPE::Frame8; }

	void SubDraw(void)override;

#pragma region 受けとる参照

	//座標参照
	const Vector3* playerPos;

	// ダメージを受けたか判定
	bool hitReact;

	// 死亡したか
	bool isDie;

	// 墜落し終わったか
	bool isFlyEnd;

#pragma endregion

#pragma region 状態管理用

	// 現在のフェーズ(1 or 2)
	int phase;
	// 最大HP(第2フェーズ判定用)
	int maxHp;
	// 第2フェーズ移行リクエスト
	bool isPhaseChange;

	// ルートのポイント
	std::vector<Vector3> routePoint[4];

	// 今向かっているポイント
	Vector3 routeIndex;

	// 移動の目的地
	Vector3 moveTarget;
	// 移動後に遷移する状態
	STATE nextStateAfterMove;

	// リアクション前の状態
	STATE prevState;

	// 表示するか
	bool isVisible;

	// 移動するルートの作成
	void CreatePatrolRoute(void);

#pragma endregion

#pragma region ダメージ・死亡

	// ダメージ
	void Damage(int damage);

	// 死亡した際
	void OnDeath(void)override;

#pragma endregion

#pragma region ステートから呼ぶ関数

	// 動きのパターン
	IceGeiraMoveState::MOVE_PATTERN movePattern;

	// 指定方向を向く
	void LookDir(const Vector3& dir);
	// 表示切り替え
	void SetVisible(bool visible) { isVisible = visible; }

	// 氷柱を生成
	void ShotIcePillar(void);
	// 竜巻を生成
	void SpawnTornado(void);

	// Move到着時：nextStateAfterMove へ遷移
	void OnMoveArrive(void);
	// Roar終了時：phaseを見て目的地と次の状態を決めてMoveへ
	void OnRoarEnd(void);
	// Warp終了時：プレイヤー付近を目的地にしてMoveへ（着いたらAttack）
	void OnWarpEnd(void);
	// React終了時：ボスの向きから座標分離れた場所にワープホールに行く
	void OnReactEnd(void);
#pragma endregion
};
