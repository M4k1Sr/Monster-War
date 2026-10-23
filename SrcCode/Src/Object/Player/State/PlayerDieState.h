#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../Common/Vector3.h"

class PlayerDieState : public CharacterStateBase
{
public:

	PlayerDieState(
		bool& isDie,
		bool& isFlyEnd,
		std::function<void(void)> playAnimeReact,

		std::function<bool(void)> isAnimeEnd
		);

	~PlayerDieState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

	// 遷移終了後1度行う処理
	void Exit(void)override;

private:

#pragma region 定数
#pragma endregion

	// 死亡アニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeDie;

	// アニメーションが終了したかどうかを取得する関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

	// 吹っ飛び終わったか
	bool& isFlyEnd;

	// Hpがなくなったか
	bool& isDie;

};