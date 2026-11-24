#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../Common/Vector3.h"

class GroundMotionController;

class PlayerBowState : public CharacterStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="motion">移動制御（水平方向：構えたら止まる・反動）</param>
	/// <param name="isGround">接地判定</param>
	/// <param name="useStamina">スタミナを消費する関数（足りなければ消費せずfalse）</param>
	/// <param name="getBowPos">矢の発射位置を取得する関数</param>
	/// <param name="setFaceDir">プレイヤーを指定方向へ向ける関数</param>
	/// <param name="setGravityEnabled">重力の有効/無効を切り替える関数</param>
	/// <param name="addVelocity">速度を加算する関数（PhysicsBody）</param>
	/// <param name="getVelocity">現在の速度を取得する関数（PhysicsBody）</param>
	/// <param name="raycast">ステージとのレイ判定（当たったらtrue、hitPosに座標）。nullptr可</param>
	/// <param name="spawnArrow">矢を生成する関数（位置, 向き, 威力0.0〜1.0, まっすぐ飛ぶ距離）</param>
	/// <param name="playAnimeBowStart">構えアニメ再生</param>
	/// <param name="playAnimeAiming">エイム中アニメ再生（ループ）</param>
	/// <param name="playAnimeBowEnd">発射アニメ再生</param>
	/// <param name="getAnimeRatio">アニメ再生割合取得</param>
	/// <param name="changeStateIdle">地上で終了したときの遷移</param>
	/// <param name="changeStateFall">空中で終了したときの遷移</param>
	PlayerBowState(
		GroundMotionController& motion,
		const bool& isGround,
		std::function<bool(float)> useStamina,

		float& chargeFrame,
		float& chargeRatio,

		std::function<Vector3(void)> getBowPos,
		std::function<void(const Vector3&)> setFaceDir,
		std::function<void(bool)> setGravityEnabled,
		std::function<void(const Vector3&)> addVelocity,
		std::function<Vector3(void)> getVelocity,
		std::function<bool(const Vector3&, const Vector3&, Vector3&)> raycast,
		std::function<void(const Vector3&, const Vector3&, float, float)> spawnArrow,

		std::function<void(void)> playAnimeBowStart,
		std::function<void(void)> playAnimeAiming,
		std::function<void(void)> playAnimeBowEnd,

		std::function<float(void)> getAnimeRatio,

		std::function<void(void)> changeStateIdle,
		std::function<void(void)> changeStateFall
	);

	~PlayerBowState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// 弓ステップ
	enum class STEP
	{
		// 構える（引き始め）
		BowStart,

		// エイム中（引き絞り）
		Aiming,

		// 撃ち終わり
		BowEnd
	};

	// 引き切りまでのフレーム数
	const float CHARGE_MAX = 40.0f;
	// 弱い矢の最低威力（0.0〜1.0）
	const float MIN_POWER = 0.3f;

	// 反動の初速
	const float RECOIL_POWER = 4.0f;

	// 照準レイの最大距離
	const float AIM_RAY_LENGTH = 10000.0f;

	// 敵に当たらなかったときに狙う距離（ここを過ぎたら矢が落ち始める）
	const float AIM_DEFAULT_DIST = 1500.0f;

	// 空中スロー時の世界の速度
	const float AIR_SLOW_SCALE = 0.005f;
	// 空中スローのイージングフレーム
	const int AIR_SLOW_EASE = 10;
	// 滞空中の重力（通常重力を切って、代わりに毎フレームこれを足す）
	const float AIR_GRAVITY = -0.50f;
	// 滞空中の落下速度上限
	const float AIR_FALL_MAX = -1.0f;

	// 空中スロー中に1フレームで減るスタミナ量
	const float AIR_SLOW_STAMINA_COST = 1.0f;

	// スロー保持フレーム（解除は自前で行うので十分長く）
	const int HOLD_LONG = 9999;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 移動制御（水平方向の速度を設定する）
	GroundMotionController& motion;
	// 接地判定
	const bool& isGround;
	// スタミナ消費
	const std::function<bool(float)> useStamina;

	// チャージフレーム数
	float& chargeFrame;
	// 引き絞り具合の書き込み先（0.0〜1.0）
	float& chargeRatio;

	// 矢の発射位置
	const std::function<Vector3(void)> getBowPos;
	// 向き変更
	const std::function<void(const Vector3&)> setFaceDir;
	// 重力の有効/無効
	const std::function<void(bool)> setGravityEnabled;
	// 速度加算
	const std::function<void(const Vector3&)> addVelocity;
	// 速度取得
	const std::function<Vector3(void)> getVelocity;
	// ステージとのレイ判定
	const std::function<bool(const Vector3&, const Vector3&, Vector3&)> raycast;
	// 矢の生成
	const std::function<void(const Vector3&, const Vector3&, float, float)> spawnArrow;

	// 弓矢を撃つ一連のアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeBowStart;
	const std::function<void(void)> playAnimeAiming;
	const std::function<void(void)> playAnimeBowEnd;

	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> getAnimeRatio;

	// 終了時の遷移
	const std::function<void(void)> changeStateIdle;
	const std::function<void(void)> changeStateFall;

#pragma endregion

	// 弓ステップ
	STEP step;

	// 空中スロー中か
	bool isAirSlow;

	// 構え始め（連射時もここから）
	void StartDraw(void);

	// 矢を放つ
	void Shoot(void);

	// 矢が飛んでいく方向（画面中央レイ → aimPoint → 発射位置からの方向）
	// outDist に発射位置から狙った点までの距離を入れる
	Vector3 CalcAimDir(float& outDist) const;

	// カメラの前方向
	Vector3 GetCameraForward(void) const;

	// 照準方向へ向く（XZのみ）
	void FaceToAim(void);

	// 空中スロー
	void UpdateAirSlow(void);
	void StartAirSlow(void);
	void EndAirSlow(void);
};
