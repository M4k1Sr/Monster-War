#include "PlayerParaseilMoveState.h"

#include "../../../Common/Vector3.h"

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

#include "../../Common/ActorBase/ActorBase.h"
#include "../../Common/MotionController/ParaseilMotionController.h"

PlayerParaseilMoveState::PlayerParaseilMoveState(
	const float ACCEL_MAX,
	const float PARASEIL_OPEN_HEIGHT,

	ParaseilMotionController& motion,

	const float& footHeight,

	const bool& isGround,

	bool& isOpenParaseil,

	std::function<void(void)> playAnimeHang,
	std::function<void(void)> playAnimeJumpEnd,

	std::function<bool(void)> isAnimeEnd,

	std::function<void(bool)> setParaseilDraw,

	std::function<void(void)> changeStateIdle,

	std::function<void(void)> activateParaseilMotion,
	std::function<void(void)> activateGroundMotion,

	std::function<void(bool)> setGravityEnabled
) :
	ACCEL_MAX(ACCEL_MAX),
	PARASEIL_OPEN_HEIGHT(PARASEIL_OPEN_HEIGHT),

	motion(motion),

	footHeight(footHeight),

	isGround(isGround),

	isOpenParaseil(isOpenParaseil),

	playAnimeHang(playAnimeHang),
	playAnimeJumpEnd(playAnimeJumpEnd),

	isAnimeEnd(isAnimeEnd),

	setParaseilDraw(setParaseilDraw),

	changeStateIdle(changeStateIdle),

	activateParaseilMotion(activateParaseilMotion),
	activateGroundMotion(activateGroundMotion),

	setGravityEnabled(setGravityEnabled),

	isJustEntered(false),

	step(STEP::Glide)

{
}

void PlayerParaseilMoveState::OwnStateConditionUpdate(void)
{
	// 条件
	// 空中にいるとき
	// 足元から地面の高さが定数の距離離れているとき
	// (ジャンプ後すぐ開けないように＆バグ防止)
	// パラセールボタン(ジャンプボタンと一緒)が押されたときに遷移
	if (isGround) { return; }
	if (footHeight < PARASEIL_OPEN_HEIGHT) { return; }
	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down) { OwnChangeState(); }
}

void PlayerParaseilMoveState::Enter(void)
{
	step = STEP::Glide;
	isJustEntered = true;
	isOpenParaseil = true;

	// 移動制御をパラセールに切り替え、物理の重力を切る
	activateParaseilMotion();

	// 落下を止めて、向きを初期化
	motion.OnOpen();

	setParaseilDraw(true);
	playAnimeHang();
}

void PlayerParaseilMoveState::Update(void)
{

	switch (step) {

	case PlayerParaseilMoveState::STEP::Glide: {
		// ジャンプ開始

		// 移動方向入力を取得
		Vector3 inputVec = InputVec();

		// 移動量の最大値を更新する
		motion.SetMoveSpeed(ACCEL_MAX * inputVec.Length());
		motion.SetBackSpeed(ACCEL_MAX * 0.7f * inputVec.Length());

		// 最終的に入力があれば加速度に加算する
		if (inputVec != 0.0f) {

			// 移動方向をカメラで回転させる
			inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

			// 移動
			motion.ApplyInput(inputVec);

		}

		// Update の Glide 内：閉じる処理を差し替え
		if (isJustEntered) {
			// 開いたフレームの入力は「開く」で使い切ってる
			isJustEntered = false;
		}
		else if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).down) {
			changeStateIdle();
			break;
		}

		// 着地
		if (isGround) {

			// パラセールを描画しない
			setParaseilDraw(false);

			// 着地のアニメーションを再生
			playAnimeJumpEnd();

			// グラウンドモーションにする
			activateGroundMotion();

			// パラセールを閉じる
			isOpenParaseil = false;

			// 次のステップへ
			step = STEP::JumpEnd;
		}

		break;
	}

	case PlayerParaseilMoveState::STEP::JumpEnd: {
		// 滞空

		// アニメーション終了
		if (isAnimeEnd()) {

			// 待機状態に戻り終了
			changeStateIdle();
		}

		break;
	}

	}

}

void PlayerParaseilMoveState::Exit(void)
{
	motion.SetMoveSpeed(ACCEL_MAX);
	isOpenParaseil = false;
	setParaseilDraw(false);

	activateGroundMotion();
}

void PlayerParaseilMoveState::AlwaysUpdate(void)
{
}

Vector3 PlayerParaseilMoveState::InputVec(void) const
{
	// 返却用一時変数
	Vector3 vec = Vector3();

	// コントローラーの入力を取得
	vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();

	// 倒し量によって移動量変化してる
	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
		vec.Normalize();
	}

	return vec;
}


