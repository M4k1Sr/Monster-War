#include "PlayerBowState.h"

#include <cmath>
#include <algorithm>

#include "../../../Manager/Input/InputManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"
#include "../../../Manager/TimeScale/TimeScale.h"

#include "../../Common/MotionController/GroundMotionController.h"

PlayerBowState::PlayerBowState(
	GroundMotionController& motion,
	const bool& isGround,
	std::function<bool(float)> useStamina,

	std::function<Vector3(void)> getBowPos,
	std::function<void(const Vector3&)> setFaceDir,
	std::function<void(bool)> setGravityEnabled,
	std::function<void(const Vector3&)> addVelocity,
	std::function<Vector3(void)> getVelocity,
	std::function<bool(const Vector3&, const Vector3&, Vector3&)> raycast,
	std::function<void(const Vector3&, const Vector3&, float)> spawnArrow,

	std::function<void(void)> playAnimeBowStart,
	std::function<void(void)> playAnimeAiming,
	std::function<void(void)> playAnimeBowEnd,

	std::function<float(void)> getAnimeRatio,

	std::function<void(void)> changeStateIdle,
	std::function<void(void)> changeStateFall
) :
	motion(motion),
	isGround(isGround),
	useStamina(useStamina),

	getBowPos(getBowPos),
	setFaceDir(setFaceDir),
	setGravityEnabled(setGravityEnabled),
	addVelocity(addVelocity),
	getVelocity(getVelocity),
	raycast(raycast),
	spawnArrow(spawnArrow),

	playAnimeBowStart(playAnimeBowStart),
	playAnimeAiming(playAnimeAiming),
	playAnimeBowEnd(playAnimeBowEnd),

	getAnimeRatio(getAnimeRatio),

	changeStateIdle(changeStateIdle),
	changeStateFall(changeStateFall),

	step(STEP::BowStart),
	chargeFrame(0.0f),
	isAirSlow(false)
{
}

void PlayerBowState::OwnStateConditionUpdate(void)
{
	// 弓ボタンを押した瞬間に構える
	if (InputManager::GetIns().GetInfo(KEY_TYPE::PlayerBow).down) {
		OwnChangeState();
	}
}

void PlayerBowState::Enter(void)
{
	isAirSlow = false;

	// 構えたらその場で止まる（移動はできない）
	motion.SetHorizontalVelocity(Vector3());

	// パラセール等から来たとき重力が切れている場合があるので、一度通常に戻す
	setGravityEnabled(true);

	// TODO：FollowActionCamera に AIM モードを追加して切り替える
	// CurrentCamera::Get().SetAimMode(true);

	// 空中で構えて、スタミナが残っていればスロー開始
	if (!isGround && useStamina(AIR_SLOW_STAMINA_COST)) { StartAirSlow(); }

	StartDraw();
}

void PlayerBowState::Update(void)
{
	// 空中スローの管理（ゲージ消費・着地で解除・落下制御）
	UpdateAirSlow();

	const bool isHold = InputManager::GetIns().GetInfo(KEY_TYPE::PlayerBow).now;

	switch (step) {

	case STEP::BowStart: {
		// 構え中も照準方向へ向き続ける
		FaceToAim();
		chargeFrame++;

		// 引き切る前に離したら弱い矢
		if (!isHold) { Shoot(); break; }

		// 構えアニメが終わったらエイム中へ
		if (getAnimeRatio() >= 1.0f) {
			step = STEP::Aiming;
			playAnimeAiming();
		}
		break;
	}

	case STEP::Aiming: {
		FaceToAim();

		// 引き切りで止める（ここで「キラッ」とエフェクトや音を出すと分かりやすい）
		chargeFrame = (std::min)(chargeFrame + 1.0f, CHARGE_MAX);

		// 離したら発射
		if (!isHold) { Shoot(); }
		break;
	}

	case STEP::BowEnd: {
		// 発射アニメが終わったら
		if (getAnimeRatio() >= 1.0f) {

			// まだ押されていたら連射
			if (isHold) { StartDraw(); }
			// 離されていたら終了（地上ならIdle、空中なら落下・滑空へ）
			else if (isGround) { changeStateIdle(); }
			else { changeStateFall(); }
		}
		break;
	}
	}
}

void PlayerBowState::Exit(void)
{
	// 被弾・回避キャンセルなど、どこから抜けても必ずスローを解除する
	EndAirSlow();

	// CurrentCamera::Get().SetAimMode(false);
}

void PlayerBowState::StartDraw(void)
{
	step = STEP::BowStart;
	chargeFrame = 0.0f;
	playAnimeBowStart();
}

void PlayerBowState::Shoot(void)
{
	// 威力：引き具合で MIN_POWER 〜 1.0
	const float ratio = (std::min)(chargeFrame / CHARGE_MAX, 1.0f);
	const float power = MIN_POWER + (1.0f - MIN_POWER) * ratio;

	// 矢の生成
	const Vector3 dir = CalcAimDir();
	spawnArrow(getBowPos(), dir, power);

	// 反動：照準と逆方向（XZ）へ少し下がる。強い矢ほど大きく
	// 減速は GroundMotionController の Drag に任せる
	Vector3 back = Vector3(-dir.x, 0.0f, -dir.z);
	if (back.LengthSq() > 0.0001f) {
		back.Normalize();
		motion.SetHorizontalVelocity(back * (RECOIL_POWER * power));
	}

	step = STEP::BowEnd;
	playAnimeBowEnd();
}

Vector3 PlayerBowState::GetCameraForward(void) const
{
	// CameraBase::CalcCameraAngle の逆変換（angle.x = ピッチ, angle.y = ヨー）
	const Vector3& angle = CurrentCamera::Get().GetAngle();
	const float cp = cosf(angle.x);

	return Vector3(
		sinf(angle.y) * cp,
		-sinf(angle.x),
		cosf(angle.y) * cp
	);
}

Vector3 PlayerBowState::CalcAimDir(void) const
{
	const Vector3 camPos = CurrentCamera::Get().GetPos();
	const Vector3 camFwd = GetCameraForward();

	// 画面中央から飛ばしたレイの先を aimPoint にする
	const Vector3 rayEnd = camPos + camFwd * AIM_RAY_LENGTH;
	Vector3 aimPoint = rayEnd;

	Vector3 hitPos;
	if (raycast && raycast(camPos, rayEnd, hitPos)) { aimPoint = hitPos; }

	// 発射位置 → aimPoint の方向（肩越しカメラの視差を消す）
	Vector3 dir = aimPoint - getBowPos();
	if (dir.LengthSq() <= 0.0001f) { return camFwd; }

	dir.Normalize();
	return dir;
}

void PlayerBowState::FaceToAim(void)
{
	// 体はXZだけ回す（上下は背骨ボーンで対応）
	Vector3 fwd = GetCameraForward();
	fwd.y = 0.0f;
	if (fwd.LengthSq() <= 0.0001f) { return; }

	fwd.Normalize();
	setFaceDir(fwd);
}

void PlayerBowState::UpdateAirSlow(void)
{
	if (!isAirSlow) { return; }

	// 通常重力の代わりに弱い重力をかける
	addVelocity(Vector3(0.0f, AIR_GRAVITY, 0.0f));

	// 落下速度に上限をつけて「ふわっと」滞空させる
	const Vector3 v = getVelocity();
	if (v.y < AIR_FALL_MAX) { addVelocity(Vector3(0.0f, AIR_FALL_MAX - v.y, 0.0f)); }

	// スタミナ消費。足りなくなったら解除
	if (!useStamina(AIR_SLOW_STAMINA_COST)) { EndAirSlow(); return; }

	// 着地したら解除
	if (isGround) { EndAirSlow(); }
}

void PlayerBowState::StartAirSlow(void)
{
	isAirSlow = true;

	// 世界（敵・その他）だけ遅くする。プレイヤーとカメラは等速のまま
	TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Enemy, AIR_SLOW_SCALE, HOLD_LONG, 0);
	TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Other, AIR_SLOW_SCALE, HOLD_LONG, 0);

	// 通常重力を切る（UpdateAirSlowで弱い重力を自前でかける）
	setGravityEnabled(false);
}

void PlayerBowState::EndAirSlow(void)
{
	if (!isAirSlow) { return; }
	isAirSlow = false;

	// イージングして通常速度へ戻す
	TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Enemy, 1.0f, 0, AIR_SLOW_EASE);
	TimeScale::GetIns().SetGroupScale(TimeScale::GROUP::Other, 1.0f, 0, AIR_SLOW_EASE);

	// 通常重力に戻す
	setGravityEnabled(true);
}
