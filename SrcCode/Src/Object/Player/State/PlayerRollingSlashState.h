#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../Common/Vector3.h"

struct Transform;

class PlayerRollingSlashCollOperator;

class PlayerRollingSlashState : public CharacterStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="COLL_START_TIME"></param>
	/// <param name="COLL_END_TIME"></param>
	/// <param name="collOperator"></param>
	/// <param name="playAnimeCharge"></param>
	/// <param name="playAnimeRollingSlash"></param>
	/// <param name="getAnimeRatio"></param>
	/// <param name="isAnimeEnd"></param>
	/// <param name="changeStateIdle"></param>
	PlayerRollingSlashState(
		float COLL_START_TIME,
		float COLL_END_TIME,

		PlayerRollingSlashCollOperator& collOperator,

		std::function<void(void)> playAnimeCharge,
		std::function<void(void)> playAnimeRollingSlash,

		std::function<float(void)> getAnimeRatio,
		std::function<void(void)> isAnimeEnd,

		std::function<void(void)> changeStateIdle
	);

	~PlayerRollingSlashState()override = default;

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

	// 攻撃ステップ
	enum class STEP
	{
		// チャージ
		Charge,

		// 前隙(回転攻撃開始)
		Startup,

		// 攻撃判定発生中
		Active,

		// 後隙
		Recovery
	};

	// 攻撃の判定を発生させ始めるアニメーション再生割合
	const float COLL_START_TIME;
	// 攻撃の判定を発生させ終わるアニメーション再生割合
	const float COLL_END_TIME;

	// 回転攻撃入力に必要なフレーム数
	const int HOLD_FRAME;
	// 回転攻撃のチャージ完了に必要なフレーム数
	const int CHARGE_COMPLETE_FRAME;
	
#pragma endregion

#pragma region 受け取る参照変数・関数

	// 攻撃の当たり判定管理クラスの参照
	PlayerRollingSlashCollOperator& collOperator;

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeCharge;
	const std::function<void(void)> playAnimeRollingSlash;

	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> getAnimeRatio;

	// アニメーションが終了したかどうかを取得する関数のポインタ
	const std::function<void(void)> isAnimeEnd;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> changeStateIdle;

#pragma endregion

	// 攻撃ステップ
	STEP step;

	// チャージ入力フレーム数
	float chargeFrame;

	// チャージ完了フラグ
	bool chargeCompleteFlg;
};