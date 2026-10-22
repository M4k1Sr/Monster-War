#include "GobKing.h"

#include "../../../Application/Application.h"

#include "../../../Utility/Utility.h"

#include "../../Common/Collider/CapsuleCollider.h"

#include "../../Common/Shader/DefaultShader.h"
#include "../../Common/Shader/RimLightShader.h"
#include "../../Common/Shader/WaterShader.h"

#include "../../../Scene/SceneManager.h"

#include "../GobKing/Weapon/CollOperator/GobSwingCollOperator.h"
//
#include "State/GobKingIdleState.h"
#include "State/GobKingMoveState.h"
#include "State/GobKingRoarState.h"
#include "State/GobKingSwingState.h"

void GobKing::Load(void)
{

#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を有効にする
	SetGravityFlg(true);

	// 当たり判定による押し出しを有効にする
	SetPushFlg(true);

	// 押し出しだしによる重みを設定
	SetPushWeight(50);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Enemy/GobKing");

	// モデルのスケール設定
	trans.scale = 5;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -90.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

	// 初期座標
	trans.pos = Vector3(VGet(0.0f, 0.0f, -2500.0f));

	// シェーダー登録
	CreateShader(new DefaultShader());

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
			Vector3::Yonly(-30.0f) * trans.scale,
			Vector3::Yonly(-60.0f) * trans.scale,
			30.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region 下位アクターの生成

	// 攻撃当たり判定管理クラス
	GobSwingCollOperator* swingCollOperator =
		new GobSwingCollOperator(100.0f, Vector3(0, -400, 300), trans);

	AddChildActor(swingCollOperator);

#pragma endregion


#pragma region 状態設定

	// 待機状態
	AddState(
		STATE::Idle,
		new GobKingIdleState(
			SEARCH_RANGE, ATTACK_RANGE,
			trans.pos,
			playerPos,
			[&]() { AnimePlay(ANIME_TYPE::Idle); }
		)
	);

	// おたけび状態
	AddState(
		STATE::Roar,
		new GobKingRoarState(
			HEAR_RANGE,
			trans.pos,
			playerPos,
			[&]() { AnimePlay(ANIME_TYPE::Roar); },
			std::bind(&GobKing::IsAnimeEnd, this),
			[&]() { 

				//再生したアニメが終了したら遷移
				if (IsAnimeEnd())
				{
					ChangeState(STATE::Idle);
				}
			},
			[&]() { 
				//再生したアニメが終了したら遷移
				if (IsAnimeEnd())
				{
					ChangeState(STATE::Move);
				}

			}
		)
	);

	// 移動状態
	AddState(
		STATE::Move,
		new GobKingMoveState(
			5.0f,SEARCH_RANGE,ATTACK_RANGE,
			std::bind(&GobKing::MoveAccel, this, std::placeholders::_1),
			ACCEL_MAX,
			trans.pos,
			playerPos,
			[&]() { AnimePlay(ANIME_TYPE::Walk); }
		)
	);

	// 攻撃状態
	AddState(
		STATE::Attack_Swing,
		new GobKingSwingState(
			0.5f,
			0.6f,
			ATTACK_RANGE,
			swingCollOperator,
			trans.pos,
			playerPos,
			[&]() {AnimePlay(ANIME_TYPE::Swing); },
			[&]() {AnimePlay(ANIME_TYPE::Taunt); },
			[&]() {return GetAnimeRatio(); },
			std::bind(&GobKing::IsAnimeEnd, this),
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	// 「待機状態」->「移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Move);
	// 「移動状態」->「待機状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Idle);

	// 「待機状態」->「おたけび状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Roar);

	// 「待機状態」->「攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Attack_Swing);
	// 「移動状態」->「攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Attack_Swing);

#pragma endregion
}

void GobKing::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (ownTag != COLLIDER_TAG::Enemy) { return; }

	if (other.GetTag() == COLLIDER_TAG::PlayerAttackSlash) { Damage(50); }

	if (other.GetTag() == COLLIDER_TAG::PlayerRollingSlash) { Damage(100); }
}

void GobKing::SubUpdate(void)
{
	static char type = 0;
	static bool prev = false, now = false;

	prev = now;
	now = CheckHitKey(KEY_INPUT_SPACE) == 1;

	if (!prev && now) {

		if (++type > 1) { type = 0; }

		switch (type) {
		case 0: { CreateShader(new DefaultShader()); break; }
		case 1: { CreateShader(new RimLightShader()); break; }
		}
	}
}

void GobKing::OnDeath(void)
{
	SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameClear);
}
