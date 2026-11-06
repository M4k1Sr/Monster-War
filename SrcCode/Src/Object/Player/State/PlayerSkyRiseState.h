#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

struct Transform;

struct Vector3;

class PlayerSkyRiseState : public CharacterStateBase
{
public:

	PlayerSkyRiseState(
		const bool& isCrouchHold,

		const bool& isGround,

		const Transform& playerTrans,

		std::function<void(void)> PlayAnimeCrouch,
		std::function<void(void)> PlayAnimeHover,

		std::function<bool(void)> isAnimeEnd,

		std::function<void(void)> changeStateIdle
	);

	~PlayerSkyRiseState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;

private:

#pragma region 定数

	// スカイライズ入力に必要なフレーム数
	const int HOLD_FRAME;
	// スカイライズのチャージ完了に必要なフレーム数
	const int CHARGE_COMPLETE_FRAME;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// しゃがみホールドされているか検知
	const bool& isCrouchHold;

	// 地面と設置しているか
	const bool& isGround;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// しゃがみアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeCrouch;
	// ホバーアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeHover;

	// アニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> changeStateIdle;

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

	// ホバー更新処理
	void HoverMove(void);
};