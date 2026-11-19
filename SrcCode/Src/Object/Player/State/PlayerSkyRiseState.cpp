#include "PlayerSkyRiseState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../Common/ActorBase/ActorBase.h"
#include "../../Common/MotionController/ParaseilMotionController.h"

PlayerSkyRiseState::PlayerSkyRiseState(
	float UP_POWER,
	float MAX_SKYRISE_HEIGHT,

	const bool& isCrouchHold,

	ParaseilMotionController& motion,

	const bool& isGround,

	const Transform& playerTrans,

	std::function<void(void)> playAnimeCrouch,
	std::function<void(void)> playAnimeHang,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(bool)> setParaseilDraw,

	std::function<void(bool)> setGravityEnabled,

	std::function<void(void)> changeStateIdle,
	std::function<void(void)> changeStateParaseilMove
) :
	UP_POWER(UP_POWER),
	MAX_SKYRISE_HEIGHT(MAX_SKYRISE_HEIGHT),

	HOLD_FRAME(60),
	CHARGE_COMPLETE_FRAME(120),

	isCrouchHold(isCrouchHold),
	motion(motion),
	isGround(isGround),
	playerTrans(playerTrans),

	playAnimeCrouch(playAnimeCrouch),
	playAnimeHang(playAnimeHang),
	isAnimeEnd(isAnimeEnd),
	setParaseilDraw(setParaseilDraw),
	setGravityEnabled(setGravityEnabled),

	changeStateIdle(changeStateIdle),
	changeStateParaseilMove(changeStateParaseilMove),

	step(STEP::Crouch),
	chargeCompleteFlg(false),
	chargeFrame(0.0f),
	riseStartY(0.0f)
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

				// 開始位置を開始位置に渡す
				riseStartY = playerTrans.pos.y;

				// 重力付与×
				setGravityEnabled(false);

				// 上昇値加算
				motion.SetVerticalVelocity(UP_POWER);
				motion.ApplyImpulse(2.0f);

				// 描画判定をtrue
				setParaseilDraw(true);

				// ホバーアニメーションに切り替え
				playAnimeHang();

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

		// 移動値
		const bool moveTop = playerTrans.pos.y - riseStartY >= MAX_SKYRISE_HEIGHT;
		// ボタンが押されたか
		const bool pressButton = Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down;


		// 発動位置から最大上昇値まで上がったら止めてパラセールへ
		if (moveTop || pressButton) {

			// 上昇を止める
			// ふわっとさせたいので少し上昇させる
			motion.SetVerticalVelocity(UP_POWER * 0.5f);

			// パラセール移動状態へ（重力は Exit で戻り、ParaseilMove の Enter で切れる）
			changeStateParaseilMove();
		}
		break;
	}

	}
}

void PlayerSkyRiseState::Exit(void)
{
	setParaseilDraw(false);

	// どの抜け方でも重力を必ず戻す
	setGravityEnabled(true);

}
