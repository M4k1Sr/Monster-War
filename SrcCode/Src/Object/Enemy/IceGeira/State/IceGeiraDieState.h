#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

// 死亡状態：重力を付けて墜落させる
class IceGeiraDieState : public CharacterStateBase
{
public:

	IceGeiraDieState(
		const bool& isDie,
		bool& isFlyEnd,

		const bool& isGround,

		std::function<void(bool)> setGravityEnabled,

		std::function<void(void)> playAnimeDeath,
		std::function<bool(void)> isAnimeEnd
	);

	~IceGeiraDieState()override = default;

	// 自分の状態に遷移させる条件関数（HP0の割り込み）
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 受け取る参照変数・関数

	// 死亡したか
	const bool& isDie;

	// 墜落し終わったか
	bool& isFlyEnd;

	// 地面に着いたか
	const bool& isGround;

	// 重力の有効/無効を切り替える関数ポインタ
	const std::function<void(bool)> setGravityEnabled;

	// 死亡アニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeDeath;

	// アニメーションが終わったかを返す関数ポインタ
	const std::function<bool(void)> isAnimeEnd;

#pragma endregion
};
