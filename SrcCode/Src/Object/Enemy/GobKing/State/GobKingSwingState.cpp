#include <random>

#include "GobKingSwingState.h"

#include "../../../Common/Transform/Transform.h"

#include "../Weapon/CollOperator/GobSwingCollOperator.h"

static std::mt19937& Rng(void)
{
	static std::mt19937 rng{ std::random_device{}() };
	return rng;
}

GobKingSwingState::GobKingSwingState(
	float COLL_START_TIME,
	float COLL_END_TIME,
	float ATTACK_RANGE,

	GobSwingCollOperator* collOperator,

	const Vector3& gobPos,
	const Vector3*& playerPos,

	std::function<void(void)> playAnimeSwing,
	std::function<void(void)> playAnimeTaunt,

	std::function<float(void)> getAnimeRatio,
	std::function<bool(void)> isAnimeEnd,

	std::function<void(void)> changeStateIdle
) :
	COLL_START_TIME(COLL_START_TIME),
	COLL_END_TIME(COLL_END_TIME),

	ATTACK_RANGE(ATTACK_RANGE),

	gobPos(gobPos),

	playerPos(playerPos),

	collOperator(collOperator),

	playAnimeSwing(playAnimeSwing),
	playAnimeTaunt(playAnimeTaunt),

	getAnimeRatio(getAnimeRatio),
	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),

	step()
{
}

void GobKingSwingState::OwnStateConditionUpdate(void)
{
	// 攻撃範囲に入ったら攻撃状態へ
	if (IsInSwingRange()) {
		OwnChangeState();
	}
}

void GobKingSwingState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 当たり判定を消去
	collOperator->Off();

	// 攻撃アニメーション再生
	playAnimeSwing();

}

void GobKingSwingState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case GobKingSwingState::STEP::Startup: {
		// 前隙

		// 攻撃判定発生開始
		if (COLL_START_TIME <= animeRatio) {

			// ステップを「攻撃判定発生中」へ
			step = STEP::Active;

			// 当たり判定を発生
			collOperator->On();
		}

		break;
	}

	case GobKingSwingState::STEP::Active: {
		// 攻撃判定発生中

		// 当たり判定を追従

		// 攻撃判定発生終了
		if (COLL_END_TIME <= animeRatio) {

			// ステップを「後隙」へ
			step = STEP::Recovery;

			// 当たり判定を消去
			collOperator->Off();
		}

		break;
	}

	case GobKingSwingState::STEP::Recovery: {
		// 後隙

		if (1.0f <= animeRatio) {

			// 当てた時だけ煽る
			if (IsTauntRoll()) {
				step = STEP::Taunt;
				playAnimeTaunt();
			}
			else {
				changeStateIdle();
			}
		}
		break;
	}

	case STEP::Taunt: {
		// 威嚇
		
		// 威嚇が終わったら待機へ（一度だけ）
		if (isAnimeEnd()) { changeStateIdle(); }
		break;
	}

	}
}

void GobKingSwingState::Exit(void)
{
	// 当たり判定を消去
	collOperator->Off();
}

bool GobKingSwingState::IsInSwingRange(void) const
{
	if (playerPos == nullptr) { return false; }

	// 高さ(y値)は無視
	const Vector3 diff = *playerPos - gobPos;
	const float distSq = diff.x * diff.x + diff.z * diff.z;

	return distSq < ATTACK_RANGE * ATTACK_RANGE;
}

bool GobKingSwingState::IsTauntRoll(void) const
{
	// 二分の一の確率で威嚇行動
	std::uniform_int_distribution<int> dist(0, 99);
	return dist(Rng()) < 50;
}