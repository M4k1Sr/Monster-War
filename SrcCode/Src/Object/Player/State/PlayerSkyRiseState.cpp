#include "PlayerSkyRiseState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../Common/ActorBase/ActorBase.h"

PlayerSkyRiseState::PlayerSkyRiseState(
	float UP_POWER,

	const bool& isCrouchHold,

	float& velocityY,

	bool& isGravity,

	const bool& isGround,

	std::function<void(const Vector3&)> moveAccel,

	const Transform& playerTrans,

	std::function<void(void)> playAnimeCrouch,
	std::function<void(void)> playAnimeHang,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(bool)> setParaseilDraw,

	std::function<void(void)> setGravityFlg,

	std::function<void(void)> changeStateIdle,
	std::function<void(void)> changeStateParaseilMove
) :
	UP_POWER(UP_POWER),

	HOLD_FRAME(60),
	CHARGE_COMPLETE_FRAME(120),

	isCrouchHold(isCrouchHold),

	velocityY(velocityY),

	isGravity(isGravity),

	isGround(isGround),

	moveAccel(moveAccel),

	playerTrans(playerTrans),

	playAnimeCrouch(playAnimeCrouch),
	playAnimeHang(playAnimeHang),

	isAnimeEnd(isAnimeEnd),

	setParaseilDraw(setParaseilDraw),

	setGravityFlg(setGravityFlg),

	changeStateIdle(changeStateIdle),
	changeStateParaseilMove(changeStateParaseilMove),

	step(STEP::Crouch),

	chargeCompleteFlg(false),

	chargeFrame(0.0f)
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

				// 重力付与×
				isGravity = false;
				setGravityFlg();

				// 上昇値加算
				velocityY = UP_POWER;

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

		// 上昇マックス
		if (playerTrans.pos.y > 1000.0f) {

			// 重力付与〇
			isGravity = true;
			setGravityFlg();

			// 次のステップへ
			step = STEP::Stop;
		}

		break;
	}

	case PlayerSkyRiseState::STEP::Stop: {
		// 上昇停止
		velocityY = 0.0f;

		// アニメーション終了
		if (velocityY == 0.0f) {

			//// 待機状態に戻り終了
			//changeStateIdle();
		}

		break;
	}

	}
}

void PlayerSkyRiseState::Exit(void)
{
	// 重力を必ず付与
	if (!isGravity) {
		isGravity = true;
		setGravityFlg();
	}
}
