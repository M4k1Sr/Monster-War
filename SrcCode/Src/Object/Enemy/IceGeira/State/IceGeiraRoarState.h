#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

// おたけび状態：初回発見時と第2フェーズ移行時に使う
// 終わった後どこへ行くかは IceGeira 側が phase を見て決める
class IceGeiraRoarState : public CharacterStateBase
{
public:

	IceGeiraRoarState(
		int ROAR_FRAME,

		bool& isPhaseChange,

		std::function<void(void)> playAnimeRoar,

		std::function<void(void)> onRoarEnd
	);

	~IceGeiraRoarState()override = default;

	// 自分の状態に遷移させる条件関数（第2フェーズ移行の割り込み）
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// おたけびの長さ（専用アニメがないのでフレームで管理）
	const int ROAR_FRAME;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 第2フェーズ移行のリクエスト
	bool& isPhaseChange;

	// おたけびアニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeRoar;

	// おたけびが終わったときに呼ぶ関数ポインタ
	const std::function<void(void)> onRoarEnd;

#pragma endregion

	// 経過フレーム
	float roarTimer;
};
