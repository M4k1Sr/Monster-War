#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class SphereCollider;

class PlayerBackFlipCollOperator : public ActorBase
{
public:
	PlayerBackFlipCollOperator(
		float COLL_RADIUS,
		const Vector3& COLL_LOCAL_POS,

		const Transform& playerTrans,
		bool& isJustDodge
	);
	~PlayerBackFlipCollOperator()override = default;

	void Load(void)override;

	// 判定発生
	void On(void);
	// 判定消去
	void Off(void);

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

private:

	// 回避の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 生成するコライダー
	SphereCollider* dodgeColl;

	// ジャスト回避したか
	bool& isJustDodge;

	// 更新処理
	void SubUpdate(void)override;
};