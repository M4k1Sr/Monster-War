#pragma once

#include "../../../Common/CharacterBase/CharacterStateBase.h"

struct Vector3;

// ワープ状態：ワープホールに入る → 消える → プレイヤーから一定距離の位置に出現
class IceGeiraWarpState : public CharacterStateBase
{
public:

	IceGeiraWarpState(
		float WARP_DISTANCE,
		float WARP_HEIGHT,
		int VANISH_FRAME,

		Vector3& pos,
		const Vector3* const& playerPos,

		std::function<void(bool)> setVisible,

		std::function<void(void)> playAnimeSwim,

		std::function<void(void)> onWarpEnd
	);

	~IceGeiraWarpState()override = default;

	// 状態遷移後1度行う初期処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

#pragma region 定数

	// ワープの段階
	enum class STEP
	{
		// ワープホールに入る
		Enter,

		// 消えている
		Vanish,

		// ワープホールから出る
		Exit,
	};

	// プレイヤーから離れた位置に出現する距離
	const float WARP_DISTANCE;

	// 出現する高さ
	const float WARP_HEIGHT;

	// 消えている時間
	const int VANISH_FRAME;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 自身の座標
	Vector3& pos;

	// プレイヤーの座標
	const Vector3* const& playerPos;

	// 表示/非表示を切り替える関数ポインタ
	const std::function<void(bool)> setVisible;

	// 泳ぐ(飛行)アニメーションの再生関数ポインタ
	const std::function<void(void)> playAnimeSwim;

	// ワープが終わったときに呼ぶ関数ポインタ
	const std::function<void(void)> onWarpEnd;

#pragma endregion

	// 現在の段階
	STEP step;

	// 段階ごとの経過フレーム
	float stepTimer;

	// 出現位置を決める
	Vector3 CalcWarpPos(void)const;

	// 各段階の更新
	void UpdateEnter(void);
	void UpdateVanish(void);
	void UpdateExit(void);
};
