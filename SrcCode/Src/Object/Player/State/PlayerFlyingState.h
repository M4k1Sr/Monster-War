#pragma once

#include "../../Common/CharacterBase/CharacterStateBase.h"

#include "../../../Common/Vector3.h"

class PlayerFlyingState : public CharacterStateBase
{
public:

	PlayerFlyingState(
		bool& isImpact,
		bool& isFlyEnd,

		std::function<Vector3(void)> getForward,

		std::function<void(const Vector3&)> setAddVelocity,

		std::function<void(void)> playAnimeFlying,
		std::function<void(void)> playAnimeGetUp,

		std::function<bool(void)> isAnimeEnd
	);

	~PlayerFlyingState()override = default;

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

	// 吹っ飛ぶ高さ
	const float VELOCTY_Y = 30.0f;

#pragma endregion

	// 吹っ飛ぶほどの衝撃を受けたか
	bool& isImpact;

	// 吹っ飛び終わったか
	bool& isFlyEnd;

	// 前方の向き
	const std::function<Vector3(void)> getForward;

	// 吹っ飛ぶ量
	std::function<void(const Vector3&)> setAddVelocity;

	// ふっとびアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeFlying;
	// 起き上がりアニメーションの再生関数のポインタ
	const std::function<void(void)> playAnimeGetUp;

	// アニメーションが終了したかどうかを取得する関数のポインタ
	const std::function<bool(void)> isAnimeEnd;

};