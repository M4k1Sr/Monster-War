#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

// リアクション状態：弱点にダメージを受けたときのひるみ
// 終わったら直前の状態(prevState)に戻す
class IceGeiraReactState : public CharacterStateBase
{
public:

	IceGeiraReactState(
		bool& hitReact,
		const bool& isDie,

		std::function<void(void)> playAnimeDamage,
		std::function<bool(void)> isAnimeEnd,

		std::function<void(void)> returnPrevState
	);

	~IceGeiraReactState()override = default;

	// 自分の状態に遷移させる条件関数（被弾の割り込み）
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 受け取る参照変数・関数

	// ダメージを受けたか
	bool& hitReact;

	// 死亡したか
	const bool& isDie;

	// ダメージアニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeDamage;

	// アニメーションが終わったかを返す関数ポインタ
	const std::function<bool(void)> isAnimeEnd;

	// 直前の状態に戻す関数ポインタ
	const std::function<void(void)> returnPrevState;

#pragma endregion
};
