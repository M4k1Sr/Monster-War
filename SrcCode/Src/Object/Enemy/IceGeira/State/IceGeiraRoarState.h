#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

class IceGeiraRoarState : public CharacterStateBase
{
public:

	IceGeiraRoarState(
		bool& isPhaseChange,

		std::function<void(void)> playAnimeRoar,
		std::function<bool(void)> isAnimeEnd,
		std::function<void(void)> changeStateMove
	);

	~IceGeiraRoarState()override = default;

	// 自分の状態に遷移させる条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数
#pragma endregion

#pragma region 受け取る参照変数・関数

	// 第2フェーズ移行のリクエスト
	bool& isPhaseChange;

	// おたけびアニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeRoar;

	// アニメーション終了取得関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

	// 移動状態に遷移する関数のポインタ
	const std::function<void(void)> changeStateMove;

#pragma endregion
};
