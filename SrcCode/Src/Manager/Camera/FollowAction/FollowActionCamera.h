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
		LOCK_ON_CAMERA,

		// エイムモード
		AIM_CAMERA
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
		const Vector3& cameraOffset = Vector3::Zonly(-250),
		const Vector3& lookAtOffset = Vector3(),
		float ROT_POWER = 3.0f * (DX_PI_F / 180.0f),
		float LOCK_DIST_BASE = 300.0f,
		float LOCK_DIST_RATE = 0.5f,
		float LOCK_DIST_MIN = 200.0f,
		float LOCK_DIST_MAX = 350.0f,
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

	// エイムモードの切り替え（true = 構える、false = 解除）
	void SetAimMode(bool flg) override;

	// エイム中かどうか
	bool IsAim(void) const override { return mode == CAMERA_MODE::AIM_CAMERA; }

private:

#pragma region ロックオン用定数

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

#pragma endregion

#pragma region エイム用定数

	// カメラ位置（追従対象からの相対座標：右肩の後ろ）
	const Vector3 AIM_CAMERA_OFFSET = Vector3(0.0f, -40.0f, -150.0f);

	// 注視点（カメラと同じ横ずれで前方を見る → 画面中央が照準の向きになる）
	const Vector3 AIM_LOOK_OFFSET = Vector3(30.0f, 25.0f, 100.0f);

	// エイム中の視野角（ズーム）
	const float AIM_FOV = 40.0f * (DX_PI_F / 180.0f);

	// 視野角の補間率（1フレームあたり）
	const float FOV_LERP_RATE = 0.2f;

	// エイム中の回転感度の倍率（細かく狙えるように下げる）
	const float AIM_ROT_RATE = 0.7f;

	// エイム中の上下の角度制限
	const float AIM_PITCH_LIMIT = 70.0f * (DX_PI_F / 180.0f);

	// 切り替え時にスムーズに寄るフレーム数（それ以降はブレないよう即時追従）
	const int AIM_BLEND_FRAME = 10;

#pragma endregion

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

	// 通常時の視野角
	const float NORMAL_FOV;

	// 操作角度
	Vector3 controlAngle;

	// ロックオン用
	const Vector3* lockOnTarget;

	// エイム切り替え後の経過フレーム
	int aimBlendCounter;

	// 更新
	void NormalUpdate(void)override;

	// モード別の更新
	void FreeUpdate(void);
	void LockOnUpdate(void);
	void AimUpdate(void);

	// 視野角を目標値へ近づける
	void UpdateFov(float targetFov);

	// 今のカメラの向きを操作角度に引き継ぐ（モード切替時のカメラ飛び防止）
	void InheritAngle(void);

	// 終了
	void SubRelease(void)override; };