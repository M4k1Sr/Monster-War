#include "PlayerSkyRiseState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

PlayerSkyRiseState::PlayerSkyRiseState(
	const bool& isCrouchHold,

	const bool& isGround,

	const Transform& playerTrans,

	std::function<void(void)> playAnimeCrouch,
	std::function<void(void)> playAnimeHover,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle
) :
	HOLD_FRAME(60),
	CHARGE_COMPLETE_FRAME(180),
	isCrouchHold(isCrouchHold),

	isGround(isGround),

	playerTrans(playerTrans),

	playAnimeCrouch(playAnimeCrouch),
	playAnimeHover(playAnimeHover),

	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),

	step(STEP::Crouch)
{
}

void PlayerSkyRiseState::OwnStateConditionUpdate(void)
{
	// ジャンプボタンがホールドされている
	// ジャンプ後にしゃがみ状態になる
	// 自分の状態に遷移する条件関数(スカイライズ)※Rキー発動
	// 原作は予期しないところでスキル発動という不満があったので別のキーに割り当て(私も思ってた)
	if (Input::GetIns().IsHoldTrigger(KEY_TYPE::PlayerSkyRise, HOLD_FRAME)) {
		OwnChangeState();
	}
}

void PlayerSkyRiseState::Enter(void)
{
	// ステップをしゃがみに
	step = STEP::Crouch;

	// チャージ完了フラグ・チャージ入力フレームをリセット
	chargeCompleteFlg = false;
	chargeFrame = 0.0f;
	
	playAnimeCrouch();

}

void PlayerSkyRiseState::Update(void)
{

	// ステップ別更新
	switch (step) {

	case PlayerSkyRiseState::STEP::Crouch: {
		// 溜め中
		chargeFrame++;

		// チャージ完了判定
		if (!chargeCompleteFlg && chargeFrame >= CHARGE_COMPLETE_FRAME) {
			chargeCompleteFlg = true;
		}

		// 離した判定（毎フレーム見る）
		if (!Input::GetIns().GetInfo(KEY_TYPE::PlayerSkyRise).now) {

			if (chargeCompleteFlg) {
				// ステップを上昇へ
				step = STEP::Rise;

			}
			else {
				// チャージ不足なら待機状態へ遷移
				changeStateIdle();
			}
		}
	}
		break;

	case PlayerSkyRiseState::STEP::Rise: {
		// 上昇

		// ホバーアニメーションに切り替え
		playAnimeHover();

		HoverMove();

		// 上昇マックス
		if (isGround) {

			// 次のステップへ
			step = STEP::Stop;
		}

		break;
	}

	case PlayerSkyRiseState::STEP::Stop: {
		// 上昇停止

		// アニメーション終了
		if (isAnimeEnd()) {

			// 待機状態に戻り終了
			changeStateIdle();
		}

		break;
	}

	}
}

void PlayerSkyRiseState::HoverMove(void)
{

}
