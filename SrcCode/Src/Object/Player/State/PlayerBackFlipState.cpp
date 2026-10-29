#include "PlayerBackFlipState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"

#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../../Manager/TimeScale/TimeScale.h"

#include "../Weapon/CollOperator/PlayerBackFlipCollOperator.h"

PlayerBackFlipState::PlayerBackFlipState(
	float JUST_START_TIME,
	float JUST_END_TIME,

	float BACKFLIP_POWER,
	float UP_POWER,

	Vector3& velocity,

	const bool& isGround,

	std::function<Vector3(void)> getVec,

	PlayerBackFlipCollOperator& collOperator,

	const Vector3* const& lockOnTarget,

	std::function<void(void)> playAnimeBackFlip,
	std::function<float(void)> getAnimeRatio,

	std::function<void(void)> changeStateIdle

) :
	JUST_START_TIME(JUST_START_TIME),
	JUST_END_TIME(JUST_END_TIME),

	BACKFLIP_POWER(BACKFLIP_POWER),
	UP_POWER(UP_POWER),

	velocity(velocity),

	isGround(isGround),

	getVec(getVec),

	collOperator(collOperator),

	playAnimeBackFlip(playAnimeBackFlip),
	getAnimeRatio(getAnimeRatio),

	changeStateIdle(changeStateIdle),

	lockOnTarget(lockOnTarget),

	isBackFlip(false),

	isAirborne(false),
		
	isJumped(false),

	isSlow(false),

	step()

{
}

void PlayerBackFlipState::OwnStateConditionUpdate(void)
{
	// ターゲット対象がいなければreturn
	if (lockOnTarget == nullptr) { return; }

	// ロックオンをしている間に、ジャンプ入力があれば遷移
	if (InputManager::GetIns().GetInfo(KEY_TYPE::PlayerJump).now) {
		OwnChangeState();
	}
}

void PlayerBackFlipState::Enter(void)
{
	// スロー判定初期化
	isSlow = false;

	// ジャンプ判定初期化
	isJumped = false;

	// ステップを「前隙」へ
	step = STEP::Startup;

	// 当たり判定を消去
	collOperator.Off();

	// 攻撃アニメーション再生
	playAnimeBackFlip();

}

void PlayerBackFlipState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ジャスト回避成功
	if (collOperator.IsJustDodge() && !isSlow) {
		// 0.2倍速を60フレーム、切り替えは即時(お試し)
		TimeScale::GetIns().TemporaryScale(0.18f, 180);
		isSlow = true;
	}

	// 後退する区間
	const bool inMoveRange = (animeRatio >= MOVE_START && animeRatio <= MOVE_END);
	if (inMoveRange) {

		// 区間内の進行度
		const float t = (animeRatio - MOVE_START) / (MOVE_END - MOVE_START);
	
		// 後ろ方向へ水平移動
		const float speed = BACKFLIP_POWER * sinf(t * DX_PI_F);

		const Vector3 back = getVec();
		velocity.x = back.x * speed;
		velocity.z = back.z * speed;

	}

	// スロー処理
	if (animeRatio > MOVE_END && isSlow) {
		TimeScale::GetIns().ChangeDefaultTimeScale();
		isSlow = false;
	}

	// ステップ別更新
	switch (step) {

	case PlayerBackFlipState::STEP::Startup: {
		// 前隙

		// 回避判定発生開始
		if (JUST_START_TIME <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			// 当たり判定を発生
			collOperator.On();
		}

		break;
	}

	case PlayerBackFlipState::STEP::Active: {
		// 回避判定発生中

		// 当たり判定を追従

		// 回避判定発生終了
		if (JUST_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			// 当たり判定を消去
			collOperator.Off();
		}

		break;
	}

	case PlayerBackFlipState::STEP::Recovery: {
		// 後隙

		// アニメーション再生終了で強制的に待機状態へ
		if (1.0f <= animeRatio) {

			// 待機状態へ遷移
			changeStateIdle();
		}

		break;
	}

	}
}

void PlayerBackFlipState::Exit(void)
{	
	// 当たり判定を消去
	collOperator.Off();
	// スローが残っていれば元に戻す
	if (isSlow) {
		TimeScale::GetIns().ChangeDefaultTimeScale();
		isSlow = false;
	}

}
