#include "PlayerBackFlipState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

PlayerBackFlipState::PlayerBackFlipState(
	float JUST_START_TIME,
	float JUST_END_TIME,

	//PlayerBackFlipCollOperator& collOperator,

	const Vector3* const& lockOnTarget,
	
	std::function<void(void)> playAnimeBackFlip,
	std::function<float(void)> getAnimeRatio,

	std::function<void(void)> changeStateIdle

) :
	JUST_START_TIME(JUST_START_TIME),
	JUST_END_TIME(JUST_END_TIME),

	//collOperator(collOperator),

	playAnimeBackFlip(playAnimeBackFlip),
	getAnimeRatio(getAnimeRatio),

	changeStateIdle(changeStateIdle),

	lockOnTarget(lockOnTarget),
		
	isBackFlip(false),

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
	// ステップを「前隙」へ
	step = STEP::Startup;

	//// 当たり判定を消去
	//collOperator.Off();

	// 攻撃アニメーション再生
	playAnimeBackFlip();
}

void PlayerBackFlipState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case PlayerBackFlipState::STEP::Startup: {
		// 前隙

		// 回避判定発生開始
		if (JUST_START_TIME <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			//// 当たり判定を発生
			//collOperator.On();
		}

		break;
	}

	case PlayerBackFlipState::STEP::Active: {
		// 回避判定発生中

		// 当たり判定を追従

		// 回避判定発生終了
		if (animeRatio <= JUST_END_TIME) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			//// 当たり判定を消去
			//collOperator.Off();
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
	//// 当たり判定を消去
	//collOperator.Off();
}