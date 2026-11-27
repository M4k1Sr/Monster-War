#include "IceGeira.h"

#include "../../../Application/Application.h"

#include "../../../Utility/Utility.h"

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../../Common/CharacterBase/CharacterBase.h"

#include "../../Common/Collider/CapsuleCollider.h"

#include "../../Common/PhysicsBody/PhysicsBody.h"
#include "../../Common/MotionController/GroundMotionController.h"

#include "../../Common/Shader/DefaultShader.h"

#include "../../../Scene/SceneManager.h"

#include "State/IceGeiraIdleState.h"
#include "State/IceGeiraAttackState.h"
#include "State/IceGeiraMoveState.h"
#include "State/IceGeiraRoarState.h"
#include "State/IceGeiraWarpState.h"
#include "State/IceGeiraTornadoState.h"
#include "State/IceGeiraReactState.h"
#include "State/IceGeiraDieState.h"


void IceGeira::Load(void)
{

#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetColliderMobility(COLLIDER_MOBILITY::Movable);

	// 物理挙動の有効化
	EnablePhysics();

	//重力無効
	GetPhysicsBody()->SetGravityEnabled(false);

	// 物理挙動
	GetPhysicsBody()->SetMode(PhysicsBody::MotionMode::Character);

	// 当たり判定による押し出しを有効にする
	SetContactResponseEnabled(true);

	// 物理衝突時の重さを設定
	GetPhysicsBody()->SetMass(500.0f);

	// 墜落時に跳ねすぎない
	GetPhysicsBody()->SetRestitution(0.2f);

	// 移動制御を生成
	GroundMotionController* groundMotionController = new GroundMotionController();

	// モデル制御情報を設定
	groundMotionController->SetTransform(&trans);
	// 接地判定の参照先を設定
	groundMotionController->SetGroundedSource(&isGround);
	// 加速度を設定
	groundMotionController->SetGroundAcceleration(2.0f);
	groundMotionController->SetAirAcceleration(2.0f);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Enemy/IceGeira");

	// モデルのスケール設定 
	trans.scale = 3;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, 0.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

#pragma endregion


#pragma region アニメーション読み込み

	// アニメーションコントローラーの生成
	CreateAnimationController();

	// アニメーションの読み込み
	AddInFbxAnimation((int)ANIME_TYPE::Max, ANIME_SPEED_TABLE, ANIME_LOOP_TABLE);

#pragma endregion


#pragma region コライダーの生成

	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Enemy,
			Vector3(0.0f, 0.0f, 60.0f) * trans.scale,
			Vector3(0.0f, 0.0f, -60.0f) * trans.scale,
			25.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region 状態設定

	// 待機状態
	AddState(
		STATE::Idle,
		new IceGeiraIdleState(
			SEARCH_RANGE,
			trans.pos,
			playerPos,
			[&]() { AnimePlay(ANIME_TYPE::Idle); },
			[&]() { ChangeState(STATE::Roar); }
		)
	);

	// 移動状態
	AddState(
		STATE::Move,
		new IceGeiraMoveState(
			MOVE_SPEED,
			ARRIVE_RANGE,
			trans.pos,
			moveTarget,
			movePattern,
			[&](const Vector3& dir) { LookDir(dir); },
			[&]() { AnimePlay(ANIME_TYPE::Swim); },
			[&]() { OnMoveArrive(); },
			[&]() { return GetTimeScale(); }
		)
	);

	//// ワープ状態
	//AddState(
	//	STATE::Warp,
	//	new IceGeiraWarpState(
	//		WARP_DISTANCE,
	//		WARP_HEIGHT,
	//		VANISH_FRAME,
	//		trans.pos,
	//		playerPos,
	//		[&](bool visible) { SetVisible(visible); },
	//		[&]() { AnimePlay(ANIME_TYPE::Swim); },
	//		[&]() { OnWarpEnd(); }
	//	)
	//);

	// おたけび状態
	AddState(
		STATE::Roar,
		new IceGeiraRoarState(
			isPhaseChange,
			[&]() { AnimePlay(ANIME_TYPE::Roar, false); },
			std::bind(&IceGeira::IsAnimeEnd, this),
			[&]() { OnRoarEnd(); }
		)
	);

	//// 攻撃(氷柱)状態
	//AddState(
	//	STATE::Attack,
	//	new IceGeiraAttackState(
	//		SHOT_NUM,
	//		SHOT_INTERVAL,
	//		ATTACK_WINDUP_FRAME,
	//		ATTACK_RECOVERY_FRAME,
	//		trans.pos,
	//		playerPos,
	//		[&](const Vector3& dir) { LookDir(dir); },
	//		[&]() { ShotIcePillar(); },
	//		[&]() { AnimePlay(ANIME_TYPE::Idle); },
	//		[&]() { ChangeState(STATE::Warp); }
	//	)
	//);

	//// 竜巻状態
	//AddState(
	//	STATE::Tornado,
	//	new IceGeiraTornadoState(
	//		TORNADO_CHARGE_FRAME,
	//		TORNADO_RECOVERY_FRAME,
	//		[&]() { SpawnTornado(); },
	//		[&]() { AnimePlay(ANIME_TYPE::Idle); },
	//		[&]() { ChangeState(STATE::Attack); }
	//	)
	//);

	//// リアクション状態
	//AddState(
	//	STATE::React,
	//	new IceGeiraReactState(
	//		hitReact,
	//		isDie,
	//		[&]() { AnimePlay(ANIME_TYPE::Damage); },
	//		std::bind(&IceGeira::IsAnimeEnd, this),
	//		[&]() { ChangeState(prevState); }
	//	)
	//);

	//// 死亡(墜落)状態
	//AddState(
	//	STATE::Die,
	//	new IceGeiraDieState(
	//		isDie,
	//		isFlyEnd,
	//		isGround,
	//		[&](bool enabled) { GetPhysicsBody()->SetGravityEnabled(enabled); },
	//		[&]() { AnimePlay(ANIME_TYPE::Death); },
	//		std::bind(&IceGeira::IsAnimeEnd, this)
	//	)
	//);

	//// ----- 割り込み：リアクション -----
	//RegisterStateTransition(STATE::Idle, STATE::React);
	//RegisterStateTransition(STATE::Move, STATE::React);
	//RegisterStateTransition(STATE::Attack, STATE::React);
	//RegisterStateTransition(STATE::Tornado, STATE::React);

	//// ----- 割り込み：第2フェーズ移行(おたけび) -----
	//RegisterStateTransition(STATE::Move, STATE::Roar);
	//RegisterStateTransition(STATE::Attack, STATE::Roar);
	//RegisterStateTransition(STATE::Warp, STATE::Roar);

	//// ----- 割り込み：死亡 -----
	//RegisterStateTransition(STATE::Idle, STATE::Die);
	//RegisterStateTransition(STATE::Move, STATE::Die);
	//RegisterStateTransition(STATE::Warp, STATE::Die);
	//RegisterStateTransition(STATE::Roar, STATE::Die);
	//RegisterStateTransition(STATE::Attack, STATE::Die);
	//RegisterStateTransition(STATE::Tornado, STATE::Die);
	//RegisterStateTransition(STATE::React, STATE::Die);

#pragma endregion

}

void IceGeira::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (ownTag != COLLIDER_TAG::Enemy) { return; }

	// 無敵中は無視
	if (invincibleFrame > 0) { return; }

	// プレイヤーの切り付け攻撃
	if (other.GetTag() == COLLIDER_TAG::PlayerBow) {
		Damage(20);

		// ヒットストップ
		TimeScale::GetIns().HitStop(3);

		if (!IsDead()) { hitReact = true; }
	}
}

void IceGeira::SubUpdate(void)
{
	// 無敵猶予フレーム処理
	if (invincibleFrame > 0 && GetTimeScale() > 0.0f) { invincibleFrame--; }

	// 生きている間は高さを固定
	if (!isDie) { trans.pos.y = 1500; }

	// 死亡判定
	if (IsDead()) {

		// 死亡したとき墜落させるために重力付与
		if (!isDie) { GetPhysicsBody()->SetGravityEnabled(true); }

		OnDeath();
		isDie = true;
	}

	// デバッグ用
	if (CheckHitKey(KEY_INPUT_0)) {
		Damage(200);
	}
}

void IceGeira::SubDraw(void)
{
	DrawFormatString(500, 0, 0xffffff, "IceGeiraHP %d", hp);
}

#pragma region ダメージ関数

void IceGeira::Damage(int damage) {
	// 死亡後・無敵中は受け付けない
	if (IsDead() || invincibleFrame > 0) { return; }

	hp = (std::max)(0, hp - damage);

	invincibleFrame = INVINCIBLE_FRAME_MAX;
}

#pragma endregion

void IceGeira::OnDeath(void)
{
	if (isFlyEnd) {
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameClear);
	}
}

void IceGeira::LookDir(const Vector3& dir)
{
}

void IceGeira::ShotIcePillar(void)
{
}

void IceGeira::SpawnTornado(void)
{
}

void IceGeira::OnMoveArrive(void)
{
}

void IceGeira::OnRoarEnd(void)
{
}

void IceGeira::OnWarpEnd(void)
{
}
