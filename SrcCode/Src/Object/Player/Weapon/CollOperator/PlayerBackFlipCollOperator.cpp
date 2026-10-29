#include "PlayerBackFlipCollOperator.h"

#include "../../../Common/Collider/SphereCollider.h"

PlayerBackFlipCollOperator::PlayerBackFlipCollOperator(
	float COLL_RADIUS,
	const Vector3& COLL_LOCAL_POS,

	const Transform& playerTrans,

	bool& isJustDodge
) :
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerTrans(playerTrans),

	isJustDodge(isJustDodge),

	dodgeColl(new SphereCollider(COLLIDER_TAG::Playerdodge, COLL_RADIUS))
{
	isJustDodge = false;
}

void PlayerBackFlipCollOperator::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	dodgeColl->SetJudgeFlg(false);

	// コライダーを追加
	AddCollider(dodgeColl);

}

void PlayerBackFlipCollOperator::SubUpdate(void)
{
	// 判定が発生しているときだけ処理する
	if (!dodgeColl->GetJudgeFlg()) { return; }

	// 追従し続ける
	trans.pos = playerTrans.pos + playerTrans.VTrans(COLL_LOCAL_POS);
}

void PlayerBackFlipCollOperator::On(void)
{
	dodgeColl->SetJudgeFlg(true);

	// プレイヤーの座標、プレイヤーの角度、攻撃判定の相対座標を使って
	// 判定の座標を割り出す
	trans.pos = playerTrans.pos + playerTrans.VTrans(COLL_LOCAL_POS);
}

void PlayerBackFlipCollOperator::Off(void)
{
	dodgeColl->SetJudgeFlg(false);
}

void PlayerBackFlipCollOperator::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	// ゴブリンキングのスイング攻撃
	if (other.GetTag() == COLLIDER_TAG::GobKingSwing) {
		
		// ジャスト回避成功
		isJustDodge = true;
	}
}

bool& PlayerBackFlipCollOperator::IsJustDodge(void)
{
	bool ret = isJustDodge;
	isJustDodge = false;
	return ret;
}
