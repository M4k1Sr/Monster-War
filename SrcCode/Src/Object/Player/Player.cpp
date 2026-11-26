#include "Player.h"


#include "../../Application/Application.h"

#include "../../Utility/Utility.h"

#include "../Common/AnimationController/AnimationController.h"

#include "../Common/Collider/CapsuleCollider.h"

#include "../Common/PhysicsBody/PhysicsBody.h"
#include "../Common/MotionController/GroundMotionController.h"
#include "../Common/MotionController/ParaseilMotionController.h"

#include "../Common/Shader/DefaultShader.h"
#include "../Common/Shader/RimLightShader.h"
#include "../Common/Shader/WaterShader.h"

#include "../../Manager/Input/InputManager.h"
#include "../../Manager/Camera/CurrentCamera.h"
#include "../../Manager/TimeScale/TimeScale.h"

#include "../../Scene/SceneManager.h"

#include "../Enemy/EnemyManager.h"

#include "Weapon/CollOperator/PlayerAttackSlashCollOperator.h"
#include "Weapon/CollOperator/PlayerRollingSlashCollOperator.h"
#include "Weapon/CollOperator/PlayerBackFlipCollOperator.h"
#include "Weapon/CollOperator/PlayerKickDownAttackCollOperator.h"

#include "Weapon/Item/King_Sword.h"
#include "Weapon/Item/Paraseil.h"
#include "Weapon/Item/Arrow.h"

#include "State/PlayerIdleState.h"
#include "State/PlayerMoveState.h"
#include "State/PlayerParaseilMoveState.h"
#include "State/PlayerJumpState.h"
#include "State/PlayerSkyRiseState.h"
#include "State/PlayerAttackSlashState.h"
#include "State/PlayerRollingSlashState.h"
#include "State/PlayerBowState.h"
#include "State/PlayerKickDownAttackState.h"
#include "State/PlayerBackFlipState.h"
#include "State/PlayerReactState.h"
#include "State/PlayerFlyingState.h"
#include "State/PlayerDieState.h"

Player::Player()
	:
	CharacterBase("Data/Parameter/Player/"),
	isOpenParaseil(false),
	isGravity(true),
	hitReact(false),
	isDie(false),
	isFlyEnd(false),
	isJustDodge(false),
	justPhase(JUST_PHASE::None),
	king_Sword(nullptr),
	paraseil(nullptr),
	isTired(false)
{
	hp = 1000;
	invincibleFrame = 60.0f;
	isRush = false;
	SetTimeGroup(TimeScale::GROUP::Player);
}

void Player::Load(void)
{

#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetColliderMobility(COLLIDER_MOBILITY::Movable);

	// 物理挙動の有効化
	EnablePhysics();

	// 重力を設定する
	GetPhysicsBody()->SetGravityEnabled(isGravity);

	// キャラクター用の物理挙動に設定する
	GetPhysicsBody()->SetMode(PhysicsBody::MotionMode::Character);

	// 当たり判定による押し出しを有効にする
	SetContactResponseEnabled(true);

	// 物理衝突時の重さを設定
	GetPhysicsBody()->SetMass(20.0f);

	// 着地時に跳ね返らないようにする（0=反発なし、1=完全に跳ね返る）
	GetPhysicsBody()->SetRestitution(0.0f);

	// 接触面の摩擦（0=氷のように滑る）。ここを下げると滑りやすくなる
	GetPhysicsBody()->SetStaticFriction(0.65f);
	GetPhysicsBody()->SetDynamicFriction(0.5f);

	// 移動・ジャンプ・攻撃などで共有する移動制御を生成
	GroundMotionController* groundMotionController = new GroundMotionController();
	// パラセールなどで共有する移動制御を生成
	ParaseilMotionController* paraseilMotionController = new ParaseilMotionController();

	AddMotionController((int)MOTION::Ground, groundMotionController);
	AddMotionController((int)MOTION::Paraseil, paraseilMotionController);

	// モデル制御情報を設定
	groundMotionController->SetTransform(&trans);
	// 接地判定の参照先を設定
	groundMotionController->SetGroundedSource(&isGround);
	// 接地時の加速度を設定
	groundMotionController->SetGroundAcceleration(3.0f);
	// 空中時の加速度を設定
	groundMotionController->SetAirAcceleration(3.0f);
	// 慣性操作の強さを設定
	groundMotionController->SetMomentumSteering(0.2f);
	// 慣性操作が効く速度の閾値を設定
	groundMotionController->SetMomentumThreshold(40.0f);
	// 地面での減速（大きいほどすぐ止まる、小さいほど滑る）
	groundMotionController->SetGroundDrag(0.65f);
	// 空中での減速（大きいほど空中ですぐ止まる）
	groundMotionController->SetAirDrag(0.08f);

	// モデル制御情報を設定
	paraseilMotionController->SetTransform(&trans);
	// パラセール展開状態の参照先を設定
	paraseilMotionController->SetParaseiledSource(&isOpenParaseil);
	// 滑空中の最大速度
	paraseilMotionController->SetMoveSpeed(15.0f);
	// 滑空中の加速度
	paraseilMotionController->SetAirAcceleration(0.9f);
	// 慣性操作の強さ
	paraseilMotionController->SetMomentumSteering(0.35f);
	// 慣性操作が効く速度の閾値
	paraseilMotionController->SetMomentumThreshold(25.0f);
	// 空中での減速
	paraseilMotionController->SetAirDrag(0.35f);
	// 滑空中の重力
	paraseilMotionController->SetGlideGravity(0.75f);
	// 落下速度の上限
	paraseilMotionController->SetMaxFallSpeed(1.5f);
	// 体の向きを変える速さ
	paraseilMotionController->SetTurnRate(Deg2Rad(0.30f));

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Player/Player");

	// モデルのスケール設定
	trans.scale = 1;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, -99.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

#pragma endregion


#pragma region アニメーション読み込み

	// アニメーションコントローラーの生成
	CreateAnimationController();

	// アニメーションの読み込み
	AddInFbxAnimation((int)ANIME_TYPE::Max, ANIME_SPEED_TABLE, ANIME_LOOP_TABLE);

	anime->SetPositionCorrectionFrame("mixamorig:Hips");
	anime->SetPositionCorrection(true);

#pragma endregion


#pragma region コライダーの生成

	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Player,
			Vector3::Yonly(0.0f) * trans.scale,
			Vector3::Yonly(-60.0f) * trans.scale,
			30.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region 下位アクターの生成(CollOperator)

	// 攻撃当たり判定管理クラス
	PlayerAttackSlashCollOperator* attackSlashCollOperator =
		new PlayerAttackSlashCollOperator(100.0f, Vector3(0, 0, 100), isRush, trans);

	AddChildActor(attackSlashCollOperator);

	// 回転攻撃当たり判定管理クラス
	PlayerRollingSlashCollOperator* rollingSlashCollOperator =
		new PlayerRollingSlashCollOperator(100.0f, Vector3(0, 0, 100), trans);

	AddChildActor(rollingSlashCollOperator);

	// 回避当たり判定管理クラス
	PlayerBackFlipCollOperator* backFlipCollOperator =
		new PlayerBackFlipCollOperator(100.0f, Vector3(0, 0, 0), trans, isJustDodge);

	AddChildActor(backFlipCollOperator);

#pragma endregion

#pragma region 下位アクターの生成(武器)

	// 武器クラス
	king_Sword = new King_Sword(trans);

	AddChildActor(king_Sword);

	// パラセールクラス
	paraseil = new Paraseil(trans);

	AddChildActor(paraseil);

	// 矢クラス
	for (int i = 0; i < ARROW_MAX; i++) {

		// 最高速度, 最低速度, 消えるまでのフレーム, 重力
		arrows[i] = new Arrow(80.0f, 30.0f, 360, -0.9f, 180);

		AddChildActor(arrows[i]);
	}

	// 矢を持つフレーム
	BOW_HAND_FRAME = MV1SearchFrame(trans.model, "mixamorig:LeftHand");

#pragma endregion


#pragma region 状態設定

	// 待機状態
	AddState(
		STATE::Idle,
		new PlayerIdleState(
			isTired,
			[&]() { AnimePlay(ANIME_TYPE::Idle); },
			[&]() { AnimePlay(ANIME_TYPE::Tired); }
		)
	);

	// 移動状態
	AddState(
		STATE::Move,
		new PlayerMoveState(
			10.0f, 1.0f, 1.5f, 300,
			isTired,
			*groundMotionController,
			[&](float cost) { return UseStamina(cost); },
			[&]() { AnimePlay(ANIME_TYPE::Walk); },
			[&]() { AnimePlay(ANIME_TYPE::Run); },
			[&]() { AnimePlay(ANIME_TYPE::FastRun); }
		)
	);

	// パラセール移動状態
	AddState(
		STATE::ParaseilMove,
		new PlayerParaseilMoveState(
			10.0f,
			300.0f,
			*paraseilMotionController,
			footHeight, isGround, isOpenParaseil,
			[&]() { AnimePlay(ANIME_TYPE::Hang); },
			[&]() { AnimePlay(ANIME_TYPE::JumpEnd); },
			[&]() { return IsAnimeEnd(); },
			[&](bool draw) { paraseil->SetIsDraw(draw); },
			[&]() { ChangeState(STATE::Idle); },
			[&]() { ChangeState(STATE::Move); },
			[this]() { ChangeMotionController((int)MOTION::Paraseil); },
			[this]() { ChangeMotionController((int)MOTION::Ground); },
			[this](bool flg) { isGravity = flg; GetPhysicsBody()->SetGravityEnabled(flg); }
		)
	);

	// ジャンプ状態
	AddState(
		STATE::Jump,
		new PlayerJumpState(
			20.0f, *groundMotionController,
			[&]() { AnimePlay(ANIME_TYPE::Jump); },
			[&]() { AnimePlay(ANIME_TYPE::Jumping); },
			[&]() { AnimePlay(ANIME_TYPE::JumpEnd); },
			std::bind(&Player::IsAnimeEnd, this),
			[&]() { ChangeState(STATE::Idle); },
			[&]() { ChangeState(STATE::Move); }

		)
	);

	// スカイライズ状態
	AddState(
		STATE::SkyRise,
		new PlayerSkyRiseState(
			20.0f,
			2000.0f,
			isCrouchHold,
			*paraseilMotionController,
			isGround,
			trans,
			[&]() { AnimePlay(ANIME_TYPE::Crouch); },
			[&]() { AnimePlay(ANIME_TYPE::Hang); },
			std::bind(&Player::IsAnimeEnd, this),
			[&](bool draw) { paraseil->SetIsDraw(draw); },
			[this](bool flg) { isGravity = flg; GetPhysicsBody()->SetGravityEnabled(flg); },
			[&]() { ChangeState(STATE::Idle); },
			[&]() { ChangeState(STATE::ParaseilMove); }
		)
	);

	// 攻撃(斬撃)状態
	AddState(
		STATE::AttackSlash,
		new PlayerAttackSlashState(
			0.3f, 0.5f, 0.6f,
			trans.pos,
			lockOnTarget,
			*groundMotionController,
			isRush,
			*attackSlashCollOperator,
			[&]() {AnimePlay(ANIME_TYPE::Slash_Down); },
			[&]() {AnimePlay(ANIME_TYPE::Slash_Up); },
			[&]() {AnimePlay(ANIME_TYPE::Slash_End); },
			[&]() {return GetAnimeRatio(); },
			[&]() {return IsAnimeEnd(); },
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	// 回転攻撃状態
	AddState(
		STATE::AttackRollingSlash,
		new PlayerRollingSlashState(
			0.3f, 0.5f,
			*rollingSlashCollOperator,
			[&]() {AnimePlay(ANIME_TYPE::Charge); },
			[&]() {AnimePlay(ANIME_TYPE::RollingSlash); },
			[&]() {return GetAnimeRatio(); },
			[&]() {return IsAnimeEnd(); },
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	AddState(
		STATE::Bow,
		new PlayerBowState(
			*groundMotionController,
			isGround,
			[&](float cost) { return UseStamina(cost); },
			bowCharge,
			chargeRatio,
			[&]() { return Vector3(MV1GetFramePosition(trans.model, BOW_HAND_FRAME)); },
			[&](const Vector3& dir) { trans.rotation = Quaternion::FromRotationY(atan2f(dir.x, dir.z)); },
			[this](bool flg) { isGravity = flg; GetPhysicsBody()->SetGravityEnabled(flg); },
			[&](const Vector3& v) { GetPhysicsBody()->AddVelocity(v); },
			[&]() { return GetPhysicsBody()->GetVelocity(); },
			[&](const Vector3& s, const Vector3& e, Vector3& hit) { return AimRaycast(s, e, hit); },
			[&](const Vector3& pos, const Vector3& dir, float power, float dist) { SpawnArrow(pos, dir, power, dist); },
			[&]() { AnimePlay(ANIME_TYPE::BowStart); },
			[&]() { AnimePlay(ANIME_TYPE::BowAiming); },
			[&]() { AnimePlay(ANIME_TYPE::BowEnd); },
			[&]() { return GetAnimeRatio(); },
			[&]() { ChangeState(STATE::Idle); },
			[&]() { ChangeState(STATE::ParaseilMove); }   // 空中で撃ち終わったら滑空へ（要動作確認）
		)
	);

	// パルクール状態
	AddState(
		STATE::BackFlip,
		new PlayerBackFlipState(
			0.05f, 0.3f,
			6.0f, 12.0f, *groundMotionController, isGround,
			[&]() {return trans.VTrans(Vector3::Zonly(-1.0f)); },
			*backFlipCollOperator,
			lockOnTarget,
			[&]() {AnimePlay(ANIME_TYPE::BackFlip); },
			[&]() {return GetAnimeRatio(); },
			[&]() {ChangeState(STATE::Idle); },
			[&]() {ChangeState(STATE::AttackSlash); }
		)
	);

	//// リアクション状態
	//AddState(
	//	STATE::React,
	//	new PlayerReactState(
	//		hitReact,
	//		[&]() {AnimePlay(ANIME_TYPE::React); },
	//		std::bind(&Player::IsAnimeEnd, this),
	//		[&]() {ChangeState(STATE::Idle); }
	//	)
	//);

	// 吹っ飛び状態
	AddState(
		STATE::Flying,
		new PlayerFlyingState(
			isImpact,
			isFlyEnd,
			[&]() { return trans.rotation.Rotate(Vector3(0.0f, 0.0f, 1.0f)); },   // 前方向
			[&](const Vector3& v) {GetPhysicsBody()->AddVelocity(v); },
			[&]() { AnimePlay(ANIME_TYPE::Flying); },
			[&]() { AnimePlay(ANIME_TYPE::GetUp); },
			[&]() { return IsAnimeEnd(); },
			[&]() {ChangeState(STATE::Idle); }
		)
	);


	//// 死亡状態
	//AddState(
	//	STATE::Flying,
	//	new PlayerDieState(
	//		isDie,
	//		isFlyEnd,
	//		[&]() {AnimePlay(ANIME_TYPE::Flying); },
	//		std::bind(&Player::IsAnimeEnd, this)
	//	)
	//);

	// 「待機状態」->「移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Move);
	// 「回転攻撃状態」->「移動状態」の自動遷移登録
	RegisterStateTransition(STATE::AttackRollingSlash, STATE::Move);

	// 「移動状態」->「待機状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Idle);

	// 「待機状態」->「ジャンプ状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Jump);
	// 「移動状態」->「ジャンプ状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Jump);
	// 「回転攻撃状態」->「ジャンプ状態」の自動遷移登録
	RegisterStateTransition(STATE::AttackRollingSlash, STATE::Jump);

	// 「待機状態」->「スカイライズ状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::SkyRise);

	// 「待機状態」->「パラセール移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::ParaseilMove);
	// 「移動状態」->「パラセール移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::ParaseilMove);
	// 「ジャンプ状態」->「パラセール移動状態」の自動遷移登録
	RegisterStateTransition(STATE::Jump, STATE::ParaseilMove);
	// 「スカイライズ状態」->「パラセール移動状態」の自動遷移登録
	RegisterStateTransition(STATE::SkyRise, STATE::ParaseilMove);

	// 「待機状態」->「攻撃（斬撃）状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::AttackSlash);
	// 「移動状態」->「攻撃（斬撃）状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::AttackSlash);

	// 「待機状態」->「回転攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::AttackRollingSlash);
	// 「移動状態」->「回転攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::AttackRollingSlash);
	// 「攻撃（斬撃）状態」->「回転攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::AttackSlash, STATE::AttackRollingSlash);

	// 「待機状態」->「弓矢攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Bow);
	// 「移動状態」->「弓矢攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Bow);
	// 「ジャンプ状態」->「弓矢攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::Jump, STATE::Bow);
	// 「パラセール移動状態」->「弓矢攻撃状態」の自動遷移登録
	RegisterStateTransition(STATE::ParaseilMove, STATE::Bow);

	// 「弓矢攻撃状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Bow, STATE::BackFlip);
	// 「弓矢攻撃状態」->「吹っ飛び状態」の自動遷移登録
	RegisterStateTransition(STATE::Bow, STATE::Flying);
	// 「待機状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::BackFlip);
	// 「移動状態」->「回避状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::BackFlip);

	// 「待機状態」->「吹っ飛び状態」の自動遷移登録
	RegisterStateTransition(STATE::Idle, STATE::Flying);
	// 「移動状態」->「吹っ飛び状態」の自動遷移登録
	RegisterStateTransition(STATE::Move, STATE::Flying);
	// 「攻撃(斬撃)状態」->「吹っ飛び状態」の自動遷移登録
	RegisterStateTransition(STATE::AttackSlash, STATE::Flying);
	// 「回転攻撃状態」->「吹っ飛び状態」の自動遷移登録
	RegisterStateTransition(STATE::AttackRollingSlash, STATE::Flying);
	// 「パラセール移動状態」->「吹っ飛び状態」の自動遷移登録
	RegisterStateTransition(STATE::ParaseilMove, STATE::Flying);
	// 「スカイライズ状態」->「吹っ飛び状態」の自動遷移登録
	RegisterStateTransition(STATE::SkyRise, STATE::Flying);


	//// 「待機状態」->「ヒットリアクション状態」の自動遷移登録
	//RegisterStateTransition(STATE::Idle, STATE::React);
	//// 「移動状態」->「ヒットリアクション状態」の自動遷移登録
	//RegisterStateTransition(STATE::Move, STATE::React);
	//// 「攻撃（斬撃）状態」->「ヒットリアクション状態」の自動遷移登録
	//RegisterStateTransition(STATE::AttackSlash, STATE::React);
	//// 「回転攻撃状態」->「ヒットリアクション状態」の自動遷移登録
	//RegisterStateTransition(STATE::AttackRollingSlash, STATE::React);

	//// 「待機状態」->「死亡状態」の自動遷移登録
	//RegisterStateTransition(STATE::Idle, STATE::Flying);
	//// 「移動状態」->「死亡状態」の自動遷移登録
	//RegisterStateTransition(STATE::Move, STATE::Flying);
	//// 「攻撃（斬撃）状態」->「死亡状態」の自動遷移登録
	//RegisterStateTransition(STATE::AttackSlash, STATE::Flying);
	//// 「回転攻撃状態」->「死亡状態」の自動遷移登録
	//RegisterStateTransition(STATE::AttackRollingSlash, STATE::Flying);

#pragma endregion

}

void Player::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (ownTag != COLLIDER_TAG::Player) { return; }

	if (IsInvincible()) { return; }

	// 無敵中は無視
	if (invincibleFrame > 0) { return; }

	// ゴブリンキングのスイング攻撃
	if (other.GetTag() == COLLIDER_TAG::GobKingSwing) {
		Damage(50);

		// 死亡していたら不要
		if (!IsDead()) { hitReact = true; }
	}

	// プロペラの当たり判定
	if (other.GetTag() == COLLIDER_TAG::WindMill) {
		isImpact = true;
		Damage(10);
	}
}

void Player::SpawnArrow(const Vector3& pos, const Vector3& dir, float power, float straightDist)
{
	// 空いている矢を優先して使う
	for (Arrow* a : arrows) {
		if (!a->IsActive()) { a->Fire(pos, dir, power, straightDist); return; }
	}

	// 全部飛んでいたら古い矢を使い回す
	arrows[oldestArrow]->Fire(pos, dir, power, straightDist);
	oldestArrow = (oldestArrow + 1) % ARROW_MAX;
}

bool Player::AimRaycast(const Vector3& start, const Vector3& end, Vector3& hitPos) const
{
	if (enemyManager == nullptr) { return false; }

	// 一番近い敵
	const Vector3* enemy = enemyManager->GetNearestEnemyPos(trans.pos, AIM_ENEMY_SEARCH);
	if (enemy == nullptr) { return false; }

	// レイの向き
	Vector3 dir = end - start;
	const float len = dir.Length();
	if (len <= 0.0001f) { return false; }
	dir = dir / len;

	// 敵の中心（座標が足元なので高さを足す）
	const Vector3 center = *enemy + Vector3::Yonly(AIM_ENEMY_HEIGHT);

	// レイの向きに沿った敵までの距離（敵の体の大きさに関係なく求まる）
	const float dist = (center - start).Dot(dir);

	// 近すぎる点は使わない（敵が後ろ側のときもここで弾かれる）
	if (dist < AIM_MIN_DIST) { return false; }

	// レイの上で一番敵に近い点
	const Vector3 closest = start + dir * dist;

	// レイが敵から離れすぎていたら、敵を狙っていないとみなす
	if ((center - closest).LengthSq() > AIM_ENEMY_ASSIST * AIM_ENEMY_ASSIST) { return false; }

	hitPos = closest;
	return true;
}

void Player::DrawReticle(float chargeRatio)
{
	// エイム中だけ表示
	if (!CurrentCamera::Get().IsAim()) { return; }

	const float cx = (float)Application::SCREEN_SIZE_X_HALF;
	const float cy = (float)Application::SCREEN_SIZE_Y_HALF;

	// 引き絞るほど線が中心に寄る
	const float GAP_MAX = 24.0f;   // 引き始めの隙間
	const float GAP_MIN = 6.0f;    // 引き切りの隙間
	const float LEN = 10.0f;       // 線の長さ
	const float gap = GAP_MAX - (GAP_MAX - GAP_MIN) * chargeRatio;

	// 引き切ったら色を変える
	const unsigned int color = (chargeRatio >= 1.0f) ? GetColor(255, 210, 80) : GetColor(255, 255, 255);

	// 中心の点
	DrawCircleAA(cx, cy, 2.0f, 16, color, TRUE);

	// 下・右上・左上の3本（120°ずつ。画面座標はYが下向き）
	const float DIRS[3][2] = {
		{  0.0f,    1.0f },   // 下
		{  0.866f, -0.5f },   // 右上
		{ -0.866f, -0.5f },   // 左上
	};

	for (const auto& d : DIRS) {
		DrawLineAA(
			cx + d[0] * gap, cy + d[1] * gap,
			cx + d[0] * (gap + LEN), cy + d[1] * (gap + LEN),
			color, 2.0f
		);
	}
}

bool Player::UseStamina(float cost)
{
	if (stamina < cost) { return false; }

	stamina -= cost;

	// 使ったら少しの間回復しない
	staminaRegenDelay = STAMINA_REGEN_DELAY;

	// 0になったら息切れ
	if (stamina <= 0.0f) { stamina = 0.0f; isTired = true; }

	return true;
}

void Player::UpdateStamina(void)
{
	// 消費直後は回復しない
	if (staminaRegenDelay > 0) { staminaRegenDelay--; return; }

	// 空中では回復しない
	if (!isGround) { return; }

	stamina = (std::min)(stamina + 1.0f, STAMINA_MAX);

	// 全回復で息切れ解除
	if (stamina >= STAMINA_MAX) { isTired = false; }
}

void Player::SubUpdate(void)
{
	// スタミナ回復
	UpdateStamina();

	// 真下へのレイ計算
	if (groundRay) {
		groundInfo = groundRay(trans.pos, GROUND_RAY_LENGTH);
		footHeight = groundInfo.hit ? (std::max)(0.0f, groundInfo.distance - FOOT_OFFSET) : 99999.0f;
	}

	// ジャスト回避
	UpdateJustDodge();

	//#pragma region ジャスト回避
	//
	//	// ジャスト回避が成功したとき
	//	if (isJustDodge) {
	//		isJustDodge = false;
	//		if (!isSlowSuccess) { isSlowSuccess = true; JustDodgeSuccess(); }
	//	}
	//
	//	if (isRushWait) {
	//		// プレイヤーグループなのでスローしない→実フレームで数える
	//		rushWaitCounter--;
	//
	//		if (InputManager::GetIns().GetInfo(KEY_TYPE::PlayerAttackSlash).down) {
	//			// ラッシュ発動（今は仮：通常の斬撃へ）
	//			JustDodgeEnd();
	//			ChangeState(STATE::AttackSlash);
	//		}
	//		else if (rushWaitCounter <= 0) {
	//			// 猶予切れ。敵は TimeScale 側がイージングで戻すので倍率はいじらない
	//			isRushWait = false;
	//			isSlowSuccess = false;
	//		}
	//	}
	//
	//#pragma endregion


	if (invincibleFrame > 0 && GetTimeScale() > 0.0f) {
		invincibleFrame--;
	}

	if (InputManager::GetIns().GetInfo(KEY_TYPE::LockOn).now) {

		// 対象が未決定なら、一番近い敵を探す
		if (lockOnTarget == nullptr && enemyManager != nullptr) {
			lockOnTarget = enemyManager->GetNearestEnemyPos(trans.pos, 2000);
		}
	}
	else {
		lockOnTarget = nullptr;
	}

	// カメラへ反映（nullptrなら通常モードに戻る）
	CurrentCamera::Get().SetLockOnTarget(lockOnTarget);

	// 死亡判定
	if (IsDead()) { OnDeath(); }

}

void Player::SubDraw(void)
{
	// レティクル
	DrawReticle(chargeRatio);

	if (App::GetIns().IsDrawDebug()) {
		const unsigned int color = groundInfo.hit ? 0x00ff00 : 0xff0000;
		const Vector3 to = groundInfo.hit ? groundInfo.point : trans.pos + Vector3::Yonly(-GROUND_RAY_LENGTH);
		DrawLine3D(trans.pos.ToVECTOR(), to.ToVECTOR(), color);
		if (groundInfo.hit) { DrawSphere3D(groundInfo.point.ToVECTOR(), 8.0f, 8, color, color, true); }
		DrawFormatString(300, 20, 0xffffff, "footHeight %.1f", footHeight);

		// 物理挙動のデバッグ表示
		if (PhysicsBody* body = GetPhysicsBody()) {
			const Vector3 v = body->GetVelocity();
			const float speedXZ = std::sqrt(v.x * v.x + v.z * v.z);

			//DrawFormatString(300, 40, 0xffffff, "速度XZ %.2f  縦速度 %.2f", speedXZ, v.y);
			//DrawFormatString(300, 60, 0xffffff, "接地 %s  タイムスケール %.2f", isGround ? "ON" : "OFF", GetTimeScale());

			// 速度ベクトルを線で表示（見やすいように10倍）
			const Vector3 velEnd = trans.pos + v * 10.0f;
			DrawLine3D(trans.pos.ToVECTOR(), velEnd.ToVECTOR(), 0x00ffff);
		}
	}

	//DrawFormatString(300, 0, 0xffffff, "PlayerHP %d", hp);

	//DrawFormatString(800, 150, 0xffffff, "posX:%.3f posY:%.3f posZ:%.3f", trans.pos.x, trans.pos.y, trans.pos.z);
//	const float global = TimeScale::Get();
//	const float gPlayer = TimeScale::GetTimeScale(TimeScale::GROUP::Player);
//	const float gEnemy = TimeScale::GetTimeScale(TimeScale::GROUP::Enemy);
//
//	DrawFormatString(0, 150, 0xffffff, "global:%.3f player:%.3f enemy:%.3f", global, gPlayer, gEnemy);
//	DrawFormatString(0, 170, 0xffffff, "state:%d phase:%d ratio:%.3f", GetState(), (int)justPhase, GetAnimeRatio());
//  DrawFormatString(0, 190, 0xffffff, "invincible:%.1f isGround:%d", (float)invincibleFrame, (int)isGround);
//
//	// 時間が完全に止まっている警告
//	if (gPlayer <= 0.0001f) {
//		DrawFormatString(0, 210, 0xff0000, "!!! PLAYER TIME STOPPED !!!");
//	}
//	if (global <= 0.0001f) {
//		DrawFormatString(0, 230, 0xff0000, "!!! GLOBAL TIME STOPPED !!!");
//	}

}

void Player::Damage(int damage) {

	//printfDx("player::hp:%d  IsDead:%d\n", hp, (int)IsDead());

	// 死亡後・無敵中は受け付けない
	if (IsDead() || invincibleFrame > 0) { return; }

	hp = (std::max)(0, hp - damage);

	invincibleFrame = 60;

	if (IsDead() || IsInvincible()) { return; }

	if (IsDead()) { OnDeath(); }
}

void Player::OnDeath(void)
{
	if (isDie) { return; }
	isDie = true;

	EndJustDodgeSlow();

	SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameOver);
}

bool Player::IsInvincible(void) const
{
	// 被弾後の通常の無敵
	if (invincibleFrame > 0) { return true; }

	// ジャスト回避の演出中(スロー開始からラッシュ終了まで)
	if (justPhase != JUST_PHASE::None) { return true; }

	// バク転の回避動作中
	if (GetState() == (int)STATE::BackFlip &&
		GetAnimeRatio() <= BACKFLIP_INVINCIBLE_END) {
		return true;
	}

	return false;
}

void Player::UpdateJustDodge(void)
{
	// ジャスト成立の検知(CollOperatorが立てたフラグ)
	if (isJustDodge) {
		isJustDodge = false;
		if (justPhase == JUST_PHASE::None) {
			StartJustDodgeSlow();
			TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Player, PLAYER_ACCEL, HOLD_LONG, 0);
		}
	}

	switch (justPhase) {

	case JUST_PHASE::EnemySlow:
		// 降り始めでプレイヤーもスロー
		if (GetAnimeRatio() >= FALL_START) {
			TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Player, PLAYER_SLOW, HOLD_LONG, 0);
			justPhase = JUST_PHASE::AllSlow;
		}
		break;

	case JUST_PHASE::AllSlow:
		if (InputManager::GetIns().GetInfo(KEY_TYPE::PlayerAttackSlash).down) {
			// プレイヤーだけ通常に戻してラッシュへ
			isRush = true;
			TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Player, PLAYER_ACCEL, HOLD_LONG, 0);
			ChangeState(STATE::AttackSlash);   // 将来はラッシュ専用ステート
			justPhase = JUST_PHASE::Rush;
		}
		else if (isGround && GetAnimeRatio() >= LAND_RATIO) {
			// 地面についたら全体のスローを解除する。
			EndJustDodgeSlow();
		}
		break;

	case JUST_PHASE::Rush:
		// ラッシュ(攻撃ステート)が終わって待機系に戻ったら解除
		if (GetState() != (int)STATE::AttackSlash) { EndJustDodgeSlow(); }
		break;
	}
}

void Player::StartJustDodgeSlow(void)
{
	// フェーズを「敵だけスロー」へ
	justPhase = JUST_PHASE::EnemySlow;

	// 敵グループだけスロー。解除は EndJustDodge で自分で行うので hold は十分長くする
	TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Enemy, ENEMY_SLOW_SCALE, HOLD_LONG, 0);

}

void Player::EndJustDodgeSlow(void)
{
	justPhase = JUST_PHASE::None;
	// 両グループを通常へ戻す（hold 0、ease 0 で即時）
	TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Enemy, 1.0f, 0, 0);
	TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Player, 1.0f, 0, 0);

	// ラッシュ攻撃しない
	isRush = false;

	if (invincibleFrame < JUST_END_GRACE_FRAME) { invincibleFrame = JUST_END_GRACE_FRAME; }
}
