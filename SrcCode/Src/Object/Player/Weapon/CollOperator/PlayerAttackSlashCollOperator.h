#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class SphereCollider;

class PlayerAttackSlashCollOperator : public ActorBase
{
public:
	PlayerAttackSlashCollOperator(
		float COLL_RADIUS,
		const Vector3& COLL_LOCAL_POS,

		bool& isRush,

		const Transform& playerTrans
	);
	~PlayerAttackSlashCollOperator()override = default;

	void Load(void)override;

	// 判定発生
	void On(void);
	// 判定消去
	void Off(void);

private:

	// 攻撃の判定を発生させる座標
	const Vector3 COLL_LOCAL_POS;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 生成するコライダー
	// 通常攻撃
	SphereCollider* attackColl;
	// ラッシュ攻撃
	SphereCollider* rushColl;

	// 当たり判定を記録する変数
	bool isHit;

	// ラッシュ中か
	bool& isRush;

	// 更新処理
	void SubUpdate(void)override;
};