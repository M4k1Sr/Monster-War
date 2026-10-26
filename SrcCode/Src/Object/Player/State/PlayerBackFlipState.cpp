//#include "PlayerBackFlipState.h"
//
//#include "../../../Common/Vector3.h"
//
//#include "../../../Manager/Input/InputManager.h"
//#include "../../../Manager/Camera/CurrentCamera.h"
//
//PlayerBackFlipState::PlayerBackFlipState(
//	float ACCEL_MAX, float DASH_SPEED_RATE, short DASH_STAMINA_MAX,
//
//	std::function<void(const Vector3&)> moveAccel,
//
//	float& accelMax,
//
//	std::function<void(void)> playAnimeWalk,
//	std::function<void(void)> playAnimeRun,
//	std::function<void(void)> playAnimeFastRun
//) :
//	ACCEL_MAX(ACCEL_MAX), DASH_SPEED_RATE(DASH_SPEED_RATE), DASH_STAMINA_MAX(DASH_STAMINA_MAX),
//
//	moveAccel(moveAccel),
//
//	accelMax(accelMax),
//
//	playAnimeWalk(playAnimeWalk),
//	playAnimeRun(playAnimeRun),
//	playAnimeFastRun(playAnimeFastRun),
//
//	isFastDash(false),
//	dashStamina(DASH_STAMINA_MAX),
//	isTired(false)
//{
//}
//
//void PlayerBackFlipState::OwnStateConditionUpdate(void)
//{
//	// ロックオンをしている間に、ジャンプ入力があれば遷移
//	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerJump).now){
//			OwnChangeState();
//		}
//}
//
//void PlayerBackFlipState::Enter(void)
//{
//	// 非ダッシュ
//	if (Input::GetIns().GetInfo(KEY_TYPE::PlayerFastDash).now) { playAnimeFastRun(); }
//	// ダッシュ
//	else { playAnimeRun(); }
//}
//
//void PlayerBackFlipState::Update(void)
//{
//	// 移動方向入力を取得
//	Vector3 inputVec = InputVec();
//
//	// 全力ダッシュフラグを立てる(移動中じゃなければ、全力ダッシュフラグは変えない)
//	isFastDash = (isTired) ? false : Input::GetIns().GetInfo(KEY_TYPE::PlayerFastDash).now && inputVec != 0.0f;
//
//	// 移動量の最大値を更新する
//	accelMax = (isFastDash) ? ACCEL_MAX * DASH_SPEED_RATE : ACCEL_MAX * inputVec.Length();
//
//	// 最終的に入力があれば加速度に加算する
//	if (inputVec != 0.0f) {
//
//		// 移動方向をカメラで回転させる
//		inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));
//
//		// 移動
//		moveAccel(inputVec);
//
//		// 移動状態の更新(スタミナにより状態変化)
//		UpdateMove();
//	}
//}
//
//void PlayerBackFlipState::Exit(void)
//{
//	accelMax = ACCEL_MAX;
//}
//
//void PlayerBackFlipState::AlwaysUpdate(void)
//{
//	// 全力ダッシュしていないときはスタミナを回復させる
//	if (!isFastDash) {
//		if (++dashStamina > DASH_STAMINA_MAX) {
//			dashStamina = DASH_STAMINA_MAX;
//
//			// 息切れ回復
//			isTired = false;
//		}
//	}
//}
//
//Vector3 PlayerBackFlipState::InputVec(void) const
//{
//	// 返却用一時変数
//	Vector3 vec = Vector3();
//
//	// コントローラーの入力を取得
//	vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();
//
//	// 入力があれば遷移
//		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
//		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
//		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
//		if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
//		vec.Normalize();
//	}
//
//	return vec;
//}
//
//void PlayerBackFlipState::UpdateMove(void)
//{
//}
