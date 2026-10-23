#include <random>

#include "GobKingThrowState.h"

#include "../../../Common/Transform/Transform.h"

#include "../Weapon/CollOperator/GobThrowCollOperator.h"

static std::mt19937& Rng(void)
{
	static std::mt19937 rng{ std::random_device{}() };
	return rng;
}

GobKingThrowState::GobKingThrowState(
	float COLL_START_TIME,
	float COLL_END_TIME,
	float ATTACK_RANGE,

	GobThrowCollOperator* collOperator,

	const Vector3& gobPos,
	const Vector3*& playerPos,

	std::function<void(void)> playAnimeThrow,
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

	playAnimeThrow(playAnimeThrow),
	playAnimeTaunt(playAnimeTaunt),

	getAnimeRatio(getAnimeRatio),
	isAnimeEnd(isAnimeEnd),

	changeStateIdle(changeStateIdle),

	step()
{
}

void GobKingThrowState::OwnStateConditionUpdate(void)
{
	// 近くに岩があったら岩投擲状態へ
	// 今は岩がないので0キーで
	if (CheckHitKey(KEY_INPUT_0)) {
		OwnChangeState();
	}
}

void GobKingThrowState::Enter(void)
{
	// ステップを「前隙」へ
	step = STEP::Startup;

	// 当たり判定を消去
	collOperator->Off();

	// 攻撃アニメーション再生
	playAnimeThrow();

}

void GobKingThrowState::Update(void)
{
	// アニメーションの再生割合を取得
	const float animeRatio = getAnimeRatio();

	// ステップ別更新
	switch (step) {

	case GobKingThrowState::STEP::Startup: {
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

	case GobKingThrowState::STEP::Active: {
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

	case GobKingThrowState::STEP::Recovery: {
		// 後隙

		if (1.0f <= animeRatio) {

			// 今は確率で威嚇行動
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

void GobKingThrowState::Exit(void)
{
	// 当たり判定を消去
	collOperator->Off();
}

bool GobKingThrowState::IsInSwingRange(void) const
{
	if (playerPos == nullptr) { return false; }

	// 高さ(y値)は無視
	const Vector3 diff = *playerPos - gobPos;
	const float distSq = diff.x * diff.x + diff.z * diff.z;

	return distSq < ATTACK_RANGE * ATTACK_RANGE;
}

bool GobKingThrowState::IsTauntRoll(void) const
{
	// 二分の一の確率で威嚇行動
	std::uniform_int_distribution<int> dist(0, 99);
	return dist(Rng()) < 50;
}