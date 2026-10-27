#include "FollowActionCamera.h"

#include "../../../Utility/Utility.h"

#include "../../../Application/Application.h"

#include "../../Input/InputManager.h"
#include "../../TimeScale/TimeScale.h"

FollowActionCamera::FollowActionCamera(
	const Vector3* targetPos,
	const Vector3& cameraOffset,
	const Vector3& lookAtOffset,
	float ROT_POWER,
	float LOCK_DIST_BASE,
	float LOCK_DIST_RATE,
	float LOCK_DIST_MIN,
	float LOCK_DIST_MAX,
	float LOCK_HEIGHT,
	float LOCK_HEIGHT_RATE,
	float LOCK_LOOK_RATE,
	float LOCK_LOOK_HEIGHT,
	const Vector3& angle,
	float fov
) :
	CameraBase(Vector3(), Vector3(), fov),

	mode(CAMERA_MODE::FREE_CAMERA),

	targetPos(targetPos),

	cameraOffset(cameraOffset),

	lookAtOffset(lookAtOffset),
	lookAtPos(),

	ROT_POWER(ROT_POWER),

	LOCK_DIST_BASE(LOCK_DIST_BASE),
	LOCK_DIST_RATE(LOCK_DIST_RATE),

	LOCK_DIST_MAX(LOCK_DIST_MAX),
	LOCK_DIST_MIN(LOCK_DIST_MIN),

	LOCK_HEIGHT(LOCK_HEIGHT),
	LOCK_HEIGHT_RATE(LOCK_HEIGHT_RATE),

	LOCK_LOOK_RATE(LOCK_LOOK_RATE),

	controlAngle(angle),

	lockOnTarget(nullptr)
{
	// 現在の追従対象の座標と角度情報から自身(カメラ)の座標を算出する
	pos = *targetPos + cameraOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(controlAngle.x,controlAngle.y) }));
	lookAtPos = *targetPos + lookAtOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(controlAngle.x, controlAngle.y) }));

	// 角度
	this->angle = CalcCameraAngle(pos, lookAtPos);

	// カメラ回転が有効だったらマウスを固定しておく
	if (ROT_POWER != 0.0f) { Input::GetIns().SetMouseFixed(true); }
}

void FollowActionCamera::SetLockOnTarget(const Vector3* target)
{
	// ロックオン(対象者がいない場合)
	if (target == nullptr) {

		// ロックオン中だった場合のみ、通常モードへ戻す
		if (mode == CAMERA_MODE::LOCK_ON_CAMERA) {

			// 今のカメラの向きを通常モードの操作角度に引き継ぐ（解除時のカメラ飛び防止）
			controlAngle.x = std::clamp(angle.x, Deg2Rad(-85.0f), Deg2Rad(85.0f));
			controlAngle.y = static_cast<float>(RadIn2PI(angle.y));
		
		}

		lockOnTarget = nullptr;
		mode = CAMERA_MODE::FREE_CAMERA;
		return;
	}

	lockOnTarget = target;
	mode = CAMERA_MODE::LOCK_ON_CAMERA;
}

void FollowActionCamera::NormalUpdate(void)
{
	// 追従対象が設定されていなかったら処理をしない(安全)
	if (targetPos == nullptr) { return; }

	// モード別処理
	switch (mode) {
	case CAMERA_MODE::FREE_CAMERA: { FreeUpdate(); break; }			// フリーモード
	case CAMERA_MODE::LOCK_ON_CAMERA: { LockOnUpdate(); break; }	// ロックオンモード
	}
}

void FollowActionCamera::FreeUpdate(void)
{
	// 追従対象が設定されていなかったら処理をしない(安全)
	if (targetPos == nullptr) { return; }

	Vector3 rotInput = Vector3();
	if (RotationInput(rotInput)) {

		controlAngle += (rotInput * ROT_POWER) * TimeScale::Get();

		// 回転の数値制御
		if (controlAngle.y <= Deg2Rad(0.0f)) { controlAngle.y += Deg2Rad(360.0f); }
		if (controlAngle.y >= Deg2Rad(360.0f)) { controlAngle.y -= Deg2Rad(360.0f); }
		if (controlAngle.x < Deg2Rad(-85.0f)) { controlAngle.x = Deg2Rad(-85.0f); }
		if (controlAngle.x > Deg2Rad(85.0f)) { controlAngle.x = Deg2Rad(85.0f); }
	}

	// 現在の追従対象の座標と角度情報から自身(カメラ)の座標を算出する
	SmoothCameraMove(pos, *targetPos + cameraOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(controlAngle.x,controlAngle.y) })));
	SmoothCameraMove(lookAtPos, *targetPos + lookAtOffset.TransMat(MatrixAllMultXY({ Vector3::XYonly(controlAngle.x, controlAngle.y) })));

	// 角度
	angle = CalcCameraAngle(pos, lookAtPos);

}

void FollowActionCamera::LockOnUpdate(void)
{
	// 対象が無効になっていたら通常モードへ戻す(安全)
	if (lockOnTarget == nullptr) { SetLockOnTarget(nullptr); return; }

	// プレイヤーから敵のベクトル
	const Vector3 toTarget = *lockOnTarget - *targetPos;

	// 水平方向の距離と向き
	Vector3 flat = Vector3(toTarget.x, 0.0f, toTarget.z);
	const float dist = flat.Length();

	// 重なっていて向きが決まらない時は、今のカメラの向きを使う
	Vector3 forward = (dist > 0.001f) ?
		(flat / dist) :
		Vector3(sinf(angle.y), 0.0f, cosf(angle.y));

	// 敵が遠いほど引く
	const float camDist = std::clamp(
		LOCK_DIST_BASE + dist * LOCK_DIST_RATE,
		LOCK_DIST_MIN, LOCK_DIST_MAX
	);

	// カメラ位置 プレイヤーの「敵と反対側」＋高さ
	const Vector3 desiredPos =
		*targetPos
		- forward * camDist
		+ Vector3::Yonly(LOCK_HEIGHT + dist * LOCK_HEIGHT_RATE);

	// 注視点 プレイヤーと敵の間
	const Vector3 desiredLookAt =
		*targetPos
		+ toTarget * LOCK_LOOK_RATE
		+ Vector3::Yonly(LOCK_HEIGHT);

	SmoothCameraMove(pos, desiredPos);
	SmoothCameraMove(lookAtPos, desiredLookAt);

	angle = CalcCameraAngle(pos, lookAtPos);
}

void FollowActionCamera::SubRelease(void)
{
	// 生成時固定したマウスを戻す
	Input::GetIns().SetMouseFixed(false);
}
