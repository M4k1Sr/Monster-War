#include "PlayerRollingSlashState.h"

#include "../../Common/Transform/Transform.h"

#include "../../../Manager/Input/InputManager.h"

#include "../Weapon/CollOperator/PlayerRollingSlashCollOperator.h"

PlayerRollingSlashState::PlayerRollingSlashState(
	float COLL_START_TIME,
	float COLL_END_TIME,

	PlayerRollingSlashCollOperator& collOperator,

	std::function<void(void)> playAnimeCharge,
	std::function<void(void)> playAnimeRollingSlash,

	std::function<float(void)> getAnimeRatio,
	std::function<void(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle
) :
	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),
	HOLD_FRAME(30),
	CHARGE_COMPLETE_FRAME(130),

	collOperator(collOperator),

	playAnimeCharge(playAnimeCharge),
	playAnimeRollingSlash(playAnimeRollingSlash),

	getAnimeRatio(getAnimeRatio),
	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),

	step(),

	chargeCompleteFlg(false)
{
}

void PlayerRollingSlashState::OwnStateConditionUpdate(void)
{
	// 自分の状態に遷移する条件関数(回転攻撃)
	if (Input::GetIns().IsHoldTrigger(KEY_TYPE::PlayerAttackSlash, HOLD_FRAME)) {
		OwnChangeState();
	}
}

void PlayerRollingSlashState::Enter(void)
{
	// ステップを「溜め」に
	step = STEP::Charge;

	// チャージ完了フラグ・チャージ入力フレームをリセット
	chargeCompleteFlg = false;
	chargeFrame = 0.0f;

	// チャージアニメーション再生
	playAnimeCharge();

	// 当たり判定を消去
	collOperator.Off();

}

void PlayerRollingSlashState::Update(void)
{

	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case PlayerRollingSlashState::STEP::Charge: {
		// 溜め中
		chargeFrame++;

		// チャージ完了判定
		if (!chargeCompleteFlg && chargeFrame >= CHARGE_COMPLETE_FRAME) {
			chargeCompleteFlg = true;
		}

		// 離した判定（毎フレーム見る）
		if (!Input::GetIns().GetInfo(KEY_TYPE::PlayerAttackSlash).now) {

			if (chargeCompleteFlg) {
				// 回転斬りのアニメに切り替え
				playAnimeRollingSlash();

				// ステップを「前隙」に
				step = STEP::Startup;

			}
			else {
				// チャージ不足なら待機状態へ遷移
				changeStateIdle();

			}

		}

		break;
	}

	case PlayerRollingSlashState::STEP::Startup: {
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

	case PlayerRollingSlashState::STEP::Active: {
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

	case PlayerRollingSlashState::STEP::Recovery: {
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

void PlayerRollingSlashState::Exit(void)
{
	// 当たり判定を消去
	collOperator.Off();
}
