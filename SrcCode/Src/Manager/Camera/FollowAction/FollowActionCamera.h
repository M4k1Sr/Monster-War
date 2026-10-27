#pragma once

#include "../CameraBase.h"

class FollowActionCamera : public CameraBase
{
public:

	// ロックオンモード
	enum class CAMERA_MODE
	{
		// フリーモード
		FREE_CAMERA,

		// ロックオンモード
		LOCK_ON_CAMERA
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="targetPos">追従対象</param>
	/// <param name="cameraOffset">カメラ座標を割り出す追従対象からの相対座標</param>
	/// <param name="lookAtOffset">注視点を割り出す追従対象からの相対座標</param>
	/// <param name="ROT_POWER">通常モードの回転量</param>
	/// <param name="LOCK_DIST_BASE">カメラ距離の基本値</param>
	/// <param name="LOCK_DIST_RATE">敵との距離に掛ける係数</param>
	/// <param name="LOCK_DIST_MIN">カメラ距離の下限</param>
	/// <param name="LOCK_DIST_MAX">カメラ距離の上限</param>
	/// <param name="LOCK_HEIGHT">カメラの高さ（プレイヤー基準）</param>
	/// <param name="LOCK_HEIGHT_RATE">敵が遠いほど足す高さの係数</param>
	/// <param name="LOCK_LOOK_RATE">注視点をプレイヤーと敵の何割の位置に置くか</param>
	/// <param name="LOCK_LOOK_HEIGHT">注視点の高さの補正</param>
	/// <param name="angle">初期角度</param>
	/// <param name="fov">視野角</param>
	FollowActionCamera(
		const Vector3* targetPos,
		const Vector3& cameraOffset = Vector3::Zonly(-400),
		const Vector3& lookAtOffset = Vector3(),
		float ROT_POWER = 3.0f * (DX_PI_F / 180.0f),
		float LOCK_DIST_BASE = 350.0f,
		float LOCK_DIST_RATE = 0.5f,
		float LOCK_DIST_MIN = 200.0f,
		float LOCK_DIST_MAX = 300.0f,
		float LOCK_HEIGHT = -20.0f,
		float LOCK_HEIGHT_RATE = 0.01f,
		float LOCK_LOOK_RATE = 0.3f,
		float LOCK_LOOK_HEIGHT = 10.0f,
		const Vector3& angle = Vector3(),
		float fov = 55.0f * (DX_PI_F / 180.0f)
	);

	// デストラクタ
	~FollowActionCamera()override = default;

	// 追従対象を途中で変更する
	void TargetChange(const Vector3* targetPos) {
		if (targetPos == nullptr) { return; }
		this->targetPos = targetPos;
	}

	// ロックオン対象を設定（nullptr = 通常モードへ戻す）
	void SetLockOnTarget(const Vector3* target) override;

	// ロックオン中かどうか
	bool IsLockOn(void) const override { return mode == CAMERA_MODE::LOCK_ON_CAMERA; }

private:

	// カメラ
	CAMERA_MODE mode;

	// 追従対象
	const Vector3* targetPos;

	// カメラ座標を割り出す追従対象からの相対座標
	const Vector3 cameraOffset;

	// 注視点を割り出す追従対象からの相対座標
	const Vector3 lookAtOffset;
	// 注視点
	Vector3 lookAtPos;

	// 回転量
	const float ROT_POWER;

	// 敵との距離に足す基本のカメラ距離
	const float LOCK_DIST_BASE;

	// 敵との距離に掛ける係数
	const float LOCK_DIST_RATE;

	// カメラ距離の限界値
	const float LOCK_DIST_MIN;
	const float LOCK_DIST_MAX;

	// カメラの高さと、敵が遠いほど足す高さの係数
	const float LOCK_HEIGHT;
	const float LOCK_HEIGHT_RATE;

	// 注視点をプレイヤーと敵の何割の位置に置くか
	const float LOCK_LOOK_RATE;

	// 操作角度
	Vector3 controlAngle;

	// ロックオン用
	const Vector3* lockOnTarget;

	// 更新
	void NormalUpdate(void)override;

	// モード別の更新
	void FreeUpdate(void);
	void LockOnUpdate(void);

	// 終了
	void SubRelease(void)override;
};