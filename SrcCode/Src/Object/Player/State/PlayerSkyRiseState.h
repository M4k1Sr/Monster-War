#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Transform;

struct Vector3;
class ParaseilMotionController;

class PlayerSkyRiseState : public CharacterStateBase
{
public:

	PlayerSkyRiseState(
		float UP_POWER,
		const float MAX_SKYRISE_HEIGHT,

		const bool& isCrouchHold,

		ParaseilMotionController& motion,

		const bool& isGround,

		const Transform& playerTrans,

		std::function<void(void)> PlayAnimeCrouch,
		std::function<void(void)> playAnimeHang,

		std::function<bool(void)> isAnimeEnd,

		std::function<void(bool)> setParaseilDraw,

		std::function<void(bool)> setGravityEnabled,

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

	// 最大上昇値
	const float MAX_SKYRISE_HEIGHT;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// しゃがみホールドされているか検知
	const bool& isCrouchHold;

	// 移動制御（上下方向の速度を設定する）
	ParaseilMotionController& motion;

	// 地面と設置しているか
	const bool& isGround;

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
	const std::function<void(bool)> setGravityEnabled;

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
	};

	// 現在のスキルステップ
	STEP step;

	// チャージ入力フレーム数
	float chargeFrame;

	// チャージ完了フラグ
	bool chargeCompleteFlg;

	// 上昇値
	float riseStartY;



};