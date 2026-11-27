#include "IceGeiraMoveState.h"

#include <cmath>

#include "../../../../Common/Vector3.h"

IceGeiraMoveState::IceGeiraMoveState(
	float MOVE_SPEED,
	float ARRIVE_RANGE,
	Vector3& pos,
	const Vector3& moveTarget,
	const MOVE_PATTERN& movePattern,
	std::function<void(const Vector3&)> lookDir,
	std::function<void(void)> playAnimeSwim,
	std::function<void(void)> onArrive,
	std::function<float(void)> getTimeScale
) :
	MOVE_SPEED(MOVE_SPEED),
	ARRIVE_RANGE(ARRIVE_RANGE),
	pos(pos),
	moveTarget(moveTarget),
	movePattern(movePattern),
	lookDir(lookDir),
	playAnimeSwim(playAnimeSwim),
	onArrive(onArrive),
	getTimeScale(getTimeScale)
{
}

void IceGeiraMoveState::Enter(void)
{
	playAnimeSwim();
}

void IceGeiraMoveState::Update(void)
{
	// 縦移動か横移動か確認
	if (movePattern == MOVE_PATTERN::Horizontal) {
		UpdateHorizontal();
	}
	else {
		UpdateVertical();
	}

	// ターゲット位置に到着したら必ずonArrive();
	if (IsArrive()) {
		onArrive();
	}
}

void IceGeiraMoveState::Exit(void)
{
}

void IceGeiraMoveState::UpdateHorizontal(void)
{

	// 目的地のターゲット位置（高さは無視して水平だけ）
	Vector3 dir = moveTarget - pos;
	dir.y = 0.0f;

	// ほぼ重なってるときは正規化で0割りになるので何もしない
	float lenSq = dir.x * dir.x + dir.z * dir.z;
	if (lenSq < 0.0001f)
	{
		return;
	}

	const float len = std::sqrt(lenSq);

	// 正規化（単位ベクトルにする）
	dir = dir / len;

	// 1フレームの移動量
	const float step = MOVE_SPEED * getTimeScale();

	// ターゲット位置まで移動
	if (len > step)
	{
		// まだ遠い：進行方向に移動量分だけ進む
		pos += dir * step;
	}
	else
	{
		// 次のフレームで追い越す距離：ピッタリ合わせる（スナップ）
		pos.x = moveTarget.x;
		pos.z = moveTarget.z;
	}

	// 進行方向を向く
	lookDir(dir);
	
}

void IceGeiraMoveState::UpdateVertical(void)
{
	// こちらもターゲット位置まで移動
	Vector3 dir = moveTarget - pos;
	dir.x = 0.0f;
	dir.z = 0.0f;

	// ほぼ重なってるときは正規化で0割りになるので何もしない
	float lenSq = dir.y * dir.y;
	if (lenSq < 0.0001f)
	{
		onArrive();
		return;
	}

	const float len = std::sqrt(lenSq);

	// 正規化（単位ベクトルにする）
	dir = dir / len;

	// 1フレームの移動量
	const float step = MOVE_SPEED * getTimeScale();

	// ターゲット位置まで移動
	if (len > step)
	{
		// まだ遠い：進行方向に移動量分だけ進む
		pos += dir * step;
	}
	else
	{
		// 次のフレームで追い越す距離：ピッタリ合わせる（スナップ）
		pos.y = moveTarget.y;
	}

}

bool IceGeiraMoveState::IsArrive(void) const
{
	const Vector3 diff = moveTarget - pos;

	switch (movePattern)
	{
		case MOVE_PATTERN::Horizontal:
		{
			// 高さ無視 横の判定
			const float distSq = diff.x * diff.x + diff.z * diff.z;
			return distSq <= ARRIVE_RANGE * ARRIVE_RANGE;
		}

		case MOVE_PATTERN::Vertical:
		{
			// 垂直判定
			return std::abs(diff.y) <= ARRIVE_RANGE;
		}
	}

	return false;
}