#include "PlayerAttackSlashState.h"

#include "../../Common/Transform/Transform.h"

#include "../../../Manager/Input/InputManager.h"

#include "../Wepon/PlayerAttackSlashCollOperator.h"

PlayerAttackSlashState::PlayerAttackSlashState(
	float COLL_START_TIME,
	float COLL_END_TIME,
	float COMBO_ACCEPT_TIME,

	PlayerAttackSlashCollOperator& collOperator,

	std::function<void(void)> playAnimeAttackSlash_Down,
	std::function<void(void)> playAnimeAttackSlash_Up,
	std::function<void(void)> playAnimeAttackSlash_End,

	std::function<float(void)> getAnimeRatio,
	std::function<void(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle
) :
	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	COMBO_ACCEPT_TIME(COMBO_ACCEPT_TIME),

	collOperator(collOperator),

	playAnimeAttackSlash_Down(playAnimeAttackSlash_Down),
	playAnimeAttackSlash_Up(playAnimeAttackSlash_Up),
	playAnimeAttackSlash_End(playAnimeAttackSlash_End),

	getAnimeRatio(getAnimeRatio),
	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),

	step(),
	comboStep(),

	comboKeyInput(false)
{
}

void PlayerAttackSlashState::OwnStateConditionUpdate(void)
{
	// 自分の状態に遷移する条件関数(斬撃)
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackSlash).down) {
		OwnChangeState();
	}
}	

void PlayerAttackSlashState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 当たり判定を消去
	collOperator.Off();

	// コンボステップも同時に「斬り下ろし」へ
	comboStep = COMBO_STEP::Down;

	// 攻撃アニメーション再生
	playAnimeAttackSlash_Down();

	// コンボキー受付をリセット
	comboKeyInput = false;
}

void PlayerAttackSlashState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// コンボ入力の受付
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackSlash).down) {
		comboKeyInput = true;
	}

	// ステップ別更新
	switch (step) {

	case PlayerAttackSlashState::STEP::Startup: {
		// 前隙

		// 攻撃判定発生開始
		if (COLL_START_TIME <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			// 当たり判定を発生
			collOperator.On();
		}

		break;
	}

	case PlayerAttackSlashState::STEP::Active: {
		// 攻撃判定発生中

		// 当たり判定を追従

		// 攻撃判定発生終了
		if (COLL_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			// 当たり判定を消去
			collOperator.Off();
		}

		break;
	}

	case PlayerAttackSlashState::STEP::Recovery: {
		// 後隙

		// アニメーション再生終了で強制的に待機状態へ
		if (1.0f <= animeRatio) {

			// 待機状態へ遷移
			changeStateIdle();
		}

		if (!comboKeyInput) { break; }					// コンボキーが押されていない場合はコンボ受付しない
		if (animeRatio <= COMBO_ACCEPT_TIME) { break; }	// コンボ受付時間に達していない場合は受付ない

		StartComboNext();

		break;
	}

	}
}

void PlayerAttackSlashState::Exit(void)
{
	// 当たり判定を消去
	collOperator.Off();
}

void PlayerAttackSlashState::StartComboNext(void)
{
	comboKeyInput = false;
	//step = STEP::Startup;

	// コンボステップ
	switch (comboStep) {
	case COMBO_STEP::Down:
		comboStep = COMBO_STEP::Up;
		// 斬撃アニメーション発生済みなので、斬撃アニメーション関数は呼び出さない
		break;
	case COMBO_STEP::Up:
		comboStep = COMBO_STEP::End;
		playAnimeAttackSlash_Up();
		break;
	case COMBO_STEP::End:
		comboStep = COMBO_STEP::Down;
		playAnimeAttackSlash_End();
		break;
	}
}
