#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Transform;

struct Vector3;

class PlayerSkyRiseState : public CharacterStateBase
{
public:

	PlayerSkyRiseState(
		float UP_POWER,

		const bool& isCrouchHold,

		float& velocityY,

		bool& isGravity,

		const bool& isGround,

		std::function<void(const Vector3&)> moveAccel,

		const Transform& playerTrans,

		std::function<void(void)> PlayAnimeCrouch,
		std::function<void(void)> playAnimeHang,

		std::function<bool(void)> isAnimeEnd,

		std::function<void(bool)> setParaseilDraw,

		std::function<void(void)> setGravityFlg,

		std::function<void(void)> changeStateIdle,
		std::function<void(void)> changeStateParaseilMove
	);

	~PlayerSkyRiseState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// スカイライズ入力に必要なフレーム数
	const int HOLD_FRAME;
	// スカイライズのチャージ完了に必要なフレーム数
	const int CHARGE_COMPLETE_FRAME;

	// 上昇力
	float UP_POWER;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// しゃがみホールドされているか検知
	const bool& isCrouchHold;

	// Y軸加速度
	float& velocityY;

	// 重力のかかり
	bool& isGravity;

	// 地面と設置しているか
	const bool& isGround;

	// 移動関数のポインタ
	const std::function<void(const Vector3&)> moveAccel;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// しゃがみアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeCrouch;
	// ハングアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeHang;

	// アニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

	// パラセールの描画設定関数のポインタ
	std::function<void(bool)> setParaseilDraw;

	// 重力の設定関数のポインタ
	const std::function<void(void)> setGravityFlg;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> changeStateIdle;
	// パラセール移動状態に行く関数のポインタ
	const std::function<void(void)> changeStateParaseilMove;

#pragma endregion

	// スキルステップ
	enum class STEP {

		// しゃがみ開始
		Crouch,

		// 上昇
		Rise,

		// 停止
		Stop,
	};

	// 現在のスキルステップ
	STEP step;

	// チャージ入力フレーム数
	float chargeFrame;

	// チャージ完了フラグ
	bool chargeCompleteFlg;

	// 上昇更新処理
	void SkyRiseMove(void);
};