#include "Player.h"

#include "../../Application/Application.h"

#include "../../Utility/Utility.h"

#include "../Common/AnimationController/AnimationController.h"

#include "../Common/Collider/CapsuleCollider.h"

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

#include "State/PlayerIdleState.h"
#include "State/PlayerMoveState.h"
#include "State/PlayerJumpState.h"
#include "State/PlayerAttackSlashState.h"
#include "State/PlayerRollingSlashState.h"
#include "State/PlayerKickDownAttackState.h"
#include "State/PlayerBackFlipState.h"
#include "State/PlayerReactState.h"
#include "State/PlayerDieState.h"

Player::Player() : CharacterBase("Data/Parameter/Player/")
{
	hp = 100;
	invincibleFrame = 60.0f;
}

void Player::Load(void)
{

#pragma region ƒIƒuƒWƒFƒNƒgÝ’è

	// “®“IƒIƒuƒWƒFƒNƒg‚Æ‚µ‚Ä‚Ìˆ—‚ð—LŒø‚É‚·‚é
	SetDynamicFlg(true);

	// d—Í‚ð—LŒø‚É‚·‚é
	SetGravityFlg(true);

	// “–‚½‚è”»’è‚É‚æ‚é‰Ÿ‚µo‚µ‚ð—LŒø‚É‚·‚é
	SetPushFlg(true);

	// ‰Ÿ‚µo‚µ‚¾‚µ‚É‚æ‚éd‚Ý‚ðÝ’è
	SetPushWeight(50);

#pragma endregion


#pragma region ƒ‚ƒfƒ‹Ý’è

	// ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý
	trans.LoadModel("Player/Player");

	// ƒ‚ƒfƒ‹‚ÌƒXƒP[ƒ‹Ý’è
	trans.scale = 1;

	// ƒ‚ƒfƒ‹‚Ì’†S“_‚ÌƒYƒŒ‚Ì•â³
	trans.centerDiff = Vector3(0.0f, -90.0f, 0.0f) * trans.scale;

	// ƒ‚ƒfƒ‹‚ÌŠp“x‚ÌƒYƒŒ‚Ì•â³
	trans.SetLocalRotation(Quaternion::FromRotationY(Deg2Rad(180.0f)));

	//// ƒVƒF[ƒ_[“o˜^
	//CreateShader(new DefaultShader());

#pragma endregion


#pragma region ƒAƒjƒ[ƒVƒ‡ƒ““Ç‚Ýž‚Ý

	// ƒAƒjƒ[ƒVƒ‡ƒ“ƒRƒ“ƒgƒ[ƒ‰[‚Ì¶¬
	CreateAnimationController();

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì“Ç‚Ýž‚Ý
	AddInFbxAnimation((int)ANIME_TYPE::Max, ANIME_SPEED_TABLE, ANIME_LOOP_TABLE);

	anime->SetPositionCorrectionFrame("mixamorig:Hips");
	anime->SetPositionCorrection(true);

#pragma endregion


#pragma region ƒRƒ‰ƒCƒ_[‚Ì¶¬

	AddCollider(
		new CapsuleCollider(
			COLLIDER_TAG::Player,
			Vector3::Yonly(0.0f) * trans.scale,
			Vector3::Yonly(-60.0f) * trans.scale,
			30.0f * trans.scale.MaxElementF()
		)
	);

#pragma endregion


#pragma region ‰ºˆÊƒAƒNƒ^[‚Ì¶¬(CollOperator)

	// UŒ‚“–‚½‚è”»’èŠÇ—ƒNƒ‰ƒX
	PlayerAttackSlashCollOperator* attackSlashCollOperator =
		new PlayerAttackSlashCollOperator(100.0f, Vector3(0, 0, 100), trans);

	AddChildActor(attackSlashCollOperator);

	// ‰ñ“]UŒ‚“–‚½‚è”»’èŠÇ—ƒNƒ‰ƒX
	PlayerRollingSlashCollOperator* rollingSlashCollOperator =
		new PlayerRollingSlashCollOperator(100.0f, Vector3(0, 0, 100), trans);

	AddChildActor(rollingSlashCollOperator);

	// ‰ñ”ð“–‚½‚è”»’èŠÇ—ƒNƒ‰ƒX
	PlayerBackFlipCollOperator* backFlipCollOperator =
		new PlayerBackFlipCollOperator(100.0f, Vector3(0, 0, 0), trans,isJustDodge);

	AddChildActor(backFlipCollOperator);

#pragma endregion

#pragma region ‰ºˆÊƒAƒNƒ^[‚Ì¶¬(•Ší)

	// •ŠíƒNƒ‰ƒX
	King_Sword* king_Sword = 
		new King_Sword(-20.0f, trans);

	AddChildActor(king_Sword);

#pragma endregion


#pragma region ó‘ÔÝ’è

	// ‘Ò‹@ó‘Ô
	AddState(
		STATE::Idle,
		new PlayerIdleState([&]() { AnimePlay(ANIME_TYPE::Idle); })
	);

	// ˆÚ“®ó‘Ô
	AddState(
		STATE::Move,
		new PlayerMoveState(
			10.0f, 1.5f, 300,
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			ACCEL_MAX,
			[&]() { AnimePlay(ANIME_TYPE::Walk); },
			[&]() { AnimePlay(ANIME_TYPE::Run); },
			[&]() { AnimePlay(ANIME_TYPE::FastRun); }
		)
	);

	// ƒWƒƒƒ“ƒvó‘Ô
	// Œã‰ñ‚µ
	AddState(
		STATE::Jump,
		new PlayerJumpState(
			20.0f, velocity.y, isGround,
			std::bind(&Player::MoveAccel, this, std::placeholders::_1),
			[&]() { AnimePlay(ANIME_TYPE::JumpStart); },
			[&]() { AnimePlay(ANIME_TYPE::Jumping); },
			[&]() { AnimePlay(ANIME_TYPE::JumpEnd); },
			std::bind(&Player::IsAnimeEnd, this),
			[&]() { ChangeState(STATE::Idle); }
		)
	);

	// UŒ‚(ŽaŒ‚)ó‘Ô
	AddState(
		STATE::AttackSlash,
		new PlayerAttackSlashState(
			0.3f, 0.5f, 0.6f,
			*attackSlashCollOperator,
			[&]() {AnimePlay(ANIME_TYPE::Slash_Down); },
			[&]() {AnimePlay(ANIME_TYPE::Slash_Up); },
			[&]() {AnimePlay(ANIME_TYPE::Slash_End); },
			[&]() {return GetAnimeRatio(); },
			[&]() {return IsAnimeEnd(); },
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	// ‰ñ“]UŒ‚ó‘Ô
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

	// ƒpƒ‹ƒN[ƒ‹ó‘Ô
	AddState(
		STATE::BackFlip,
		new PlayerBackFlipState(
			0.05f, 0.3f,
			6.0f,12.0f, velocity, isGround,
			[&]() {return trans.VTrans(Vector3::Zonly(-1.0f)); },
			*backFlipCollOperator,
			lockOnTarget,
			[&]() {AnimePlay(ANIME_TYPE::BackFlip); },
			[&]() {return GetAnimeRatio(); },
			[&]() {ChangeState(STATE::Idle); }
		)
	);

	//// ƒŠƒAƒNƒVƒ‡ƒ“ó‘Ô
	//AddState(
	//	STATE::React,
	//	new PlayerReactState(
	//		hitReact,
	//		[&]() {AnimePlay(ANIME_TYPE::React); },
	//		std::bind(&Player::IsAnimeEnd, this),
	//		[&]() {ChangeState(STATE::Idle); }
	//	)
	//);

	//// Ž€–S(‚Á”ò‚Ñ)ó‘Ô
	//AddState(
	//	STATE::Flying,
	//	new PlayerDieState(
	//		isDie,
	//		isFlyEnd,
	//		[&]() {AnimePlay(ANIME_TYPE::Flying); },
	//		std::bind(&Player::IsAnimeEnd, this)
	//	)
	//);

	// u‘Ò‹@ó‘Ôv->uˆÚ“®ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Idle, STATE::Move);
	// u‰ñ“]UŒ‚ó‘Ôv->uˆÚ“®ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::AttackRollingSlash, STATE::Move);

	// uˆÚ“®ó‘Ôv->u‘Ò‹@ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Move, STATE::Idle);

	// u‘Ò‹@ó‘Ôv->uƒWƒƒƒ“ƒvó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Idle, STATE::Jump);
	// uˆÚ“®ó‘Ôv->uƒWƒƒƒ“ƒvó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Move, STATE::Jump);
	// u‰ñ“]UŒ‚ó‘Ôv->uƒWƒƒƒ“ƒvó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::AttackRollingSlash, STATE::Jump);

	// u‘Ò‹@ó‘Ôv->uUŒ‚iŽaŒ‚jó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Idle, STATE::AttackSlash);
	// uˆÚ“®ó‘Ôv->uUŒ‚iŽaŒ‚jó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Move, STATE::AttackSlash);
	// u‘Ò‹@ó‘Ôv->u‰ñ“]UŒ‚ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Idle, STATE::AttackRollingSlash);
	// uˆÚ“®ó‘Ôv->u‰ñ“]UŒ‚ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Move, STATE::AttackRollingSlash);
	// uUŒ‚iŽaŒ‚jó‘Ôv->u‰ñ“]UŒ‚ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::AttackSlash, STATE::AttackRollingSlash);

	// u‘Ò‹@ó‘Ôv->u‰ñ”ðó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Idle, STATE::BackFlip);
	// uˆÚ“®ó‘Ôv->u‰ñ”ðó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	RegisterStateTransition(STATE::Move, STATE::BackFlip);


	//// u‘Ò‹@ó‘Ôv->uƒqƒbƒgƒŠƒAƒNƒVƒ‡ƒ“ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	//RegisterStateTransition(STATE::Idle, STATE::React);
	//// uˆÚ“®ó‘Ôv->uƒqƒbƒgƒŠƒAƒNƒVƒ‡ƒ“ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	//RegisterStateTransition(STATE::Move, STATE::React);
	//// uUŒ‚iŽaŒ‚jó‘Ôv->uƒqƒbƒgƒŠƒAƒNƒVƒ‡ƒ“ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	//RegisterStateTransition(STATE::AttackSlash, STATE::React);
	//// u‰ñ“]UŒ‚ó‘Ôv->uƒqƒbƒgƒŠƒAƒNƒVƒ‡ƒ“ó‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	//RegisterStateTransition(STATE::AttackRollingSlash, STATE::React);

	//// u‘Ò‹@ó‘Ôv->uŽ€–Só‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	//RegisterStateTransition(STATE::Idle, STATE::Flying);
	//// uˆÚ“®ó‘Ôv->uŽ€–Só‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	//RegisterStateTransition(STATE::Move, STATE::Flying);
	//// uUŒ‚iŽaŒ‚jó‘Ôv->uŽ€–Só‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	//RegisterStateTransition(STATE::AttackSlash, STATE::Flying);
	//// u‰ñ“]UŒ‚ó‘Ôv->uŽ€–Só‘Ôv‚ÌŽ©“®‘JˆÚ“o˜^
	//RegisterStateTransition(STATE::AttackRollingSlash, STATE::Flying);

#pragma endregion

	

}

void Player::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (ownTag != COLLIDER_TAG::Enemy) { return; }

	// –³“G’†‚Í–³Ž‹
	if (invincibleFrame > 0) { return; }

	// ƒSƒuƒŠƒ“ƒLƒ“ƒO‚ÌƒXƒCƒ“ƒOUŒ‚
	if (other.GetTag() == COLLIDER_TAG::GobKingSwing) {
		Damage(50);

		// Ž€–S‚µ‚Ä‚¢‚½‚ç•s—v
		if (!IsDead()) { hitReact = true; }
	}

}

void Player::SubUpdate(void)
{
	if (invincibleFrame > 0 && TimeScale::Get() > 0.0f) { invincibleFrame--; }
	
	if (InputManager::GetIns().GetInfo(KEY_TYPE::LockOn).now) {

		// ‘ÎÛ‚ª–¢Œˆ’è‚È‚çAˆê”Ô‹ß‚¢“G‚ð’T‚·
		if (lockOnTarget == nullptr && enemyManager != nullptr) {
			lockOnTarget = enemyManager->GetNearestEnemyPos(trans.pos, 2000);
		}
	}
	else {
		lockOnTarget = nullptr;
	}

	// ƒJƒƒ‰‚Ö”½‰finullptr‚È‚ç’Êíƒ‚[ƒh‚É–ß‚éj
	CurrentCamera::Get().SetLockOnTarget(lockOnTarget);

	//static char type = 0;
	//static bool prev = false, now = false;

	//prev = now;
	//now = CheckHitKey(KEY_INPUT_SPACE) == 1;

	//if (!prev && now) {

	//	if (++type > 1) { type = 0; }

	//	switch (type){
	//	case 0: { CreateShader(new DefaultShader()); break; }
	//	case 1: { CreateShader(new RimLightShader()); break; }
	//	}
	//}

		// Ž€–S”»’è
	if (IsDead()) { OnDeath(); isDie = true; }

}

void Player::Damage(int damage) {

	printfDx("player::hp:%d  IsDead:%d\n", hp, (int)IsDead());

	// Ž€–SŒãE–³“G’†‚ÍŽó‚¯•t‚¯‚È‚¢
	if (IsDead() || invincibleFrame > 0) { return; }

	hp = (std::max)(0, hp - damage);

	invincibleFrame = 60;

	if (IsDead()) { OnDeath(); }
}

void Player::OnDeath(void)
{
	SceneManager::GetIns().ChangeSceneFade(SCENE_ID::GameOver);
}
