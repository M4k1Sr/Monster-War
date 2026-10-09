#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../Common/Vector3.h"

struct Transform;

class PlayerAttackSlashCollOperator;

class PlayerAttackSlashState : public CharacterStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="COLL_START_TIME"></param>
	/// <param name="COLL_END_TIME"></param>
	/// <param name="collOperator"></param>
	/// <param name="playAnimeAttackSlash_Down"></param>
	/// <param name="playAnimeAttackSlash_Up"></param>
	/// <param name="playAnimeAttackSlash_End"></param>
	/// <param name="getAnimeRatio"></param>
	/// <param name="isAnimeEnd"></param>
	/// <param name="changeStateIdle"></param>
	PlayerAttackSlashState(
		float COLL_START_TIME,
		float COLL_END_TIME,
		float COMBO_ACCEPT_TIME,

		PlayerAttackSlashCollOperator& collOperator,

		std::function<void(void)> playAnimeAttackSlash_Down,
		std::function<void(void)> playAnimeAttackSlash_Up,
		std::function<void(void)> playAnimeAttackSlash_End,

		std::function<float(void)> getAnimeRatio,
		std::function<void(void)> isAnimeEnd,

		std::function<void(void)> changeStateIdle
	);

	~PlayerAttackSlashState()override = default;

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
		// 前隙
		Startup,

		// 攻撃判定発生中
		Active,

		// 後隙
		Recovery
	};

	// コンボステップ
	enum class COMBO_STEP
	{
		// 斬り下ろし
		Down,
		// 斬り上げ
		Up,
		// 最終斬り
		End
	};

	// 攻撃の判定を発生させ始めるアニメーション再生割合
	const float COLL_START_TIME;
	// 攻撃の判定を発生させ終わるアニメーション再生割合
	const float COLL_END_TIME;

	// コンボ受付時間
	const float COMBO_ACCEPT_TIME;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 攻撃の当たり判定管理クラスの参照
	PlayerAttackSlashCollOperator& collOperator;

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeAttackSlash_Down;
	const std::function<void(void)> playAnimeAttackSlash_Up;
	const std::function<void(void)> playAnimeAttackSlash_End;

	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> getAnimeRatio;

	// アニメーションが終了したかどうかを取得する関数のポインタ
	const std::function<void(void)> isAnimeEnd;

	// 待機状態に戻る関数のポインタ
	const std::function<void(void)> changeStateIdle;

#pragma endregion

	// 攻撃ステップ
	STEP step;

	// コンボステップ
	COMBO_STEP comboStep;

	// コンボキー受付
	bool comboKeyInput;

	void StartComboNext(void);
};