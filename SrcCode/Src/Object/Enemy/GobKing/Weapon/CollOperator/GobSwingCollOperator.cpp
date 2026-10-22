#include "GobSwingCollOperator.h"

#include "../../../../Common/Collider/SphereCollider.h"

GobSwingCollOperator::GobSwingCollOperator(
	float COLL_RADIUS,
	const Vector3& COLL_LOCAL_POS,

	const Transform& gobKingTrans
) :
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	gobKingTrans(gobKingTrans),

	attackColl(new SphereCollider(COLLIDER_TAG::GobKingSwing, COLL_RADIUS))
{
}

void GobSwingCollOperator::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetDynamicFlg(true);

	// 重力を無効にする
	SetGravityFlg(false);

	// 当たり判定による押し出しを無効にする
	SetPushFlg(false);

#pragma endregion

	attackColl->SetJudgeFlg(false);

	// コライダーを追加
	AddCollider(attackColl);
}

void GobSwingCollOperator::SubUpdate(void)
{
	// 判定が発生しているときだけ処理する
	if (!attackColl->GetJudgeFlg()) { return; }

	// 追従し続ける
	trans.pos = gobKingTrans.pos + gobKingTrans.VTrans(COLL_LOCAL_POS);
}

void GobSwingCollOperator::On(void)
{
	attackColl->SetJudgeFlg(true);

	// ゴブリンキングの座標、ゴブリンキングの角度、攻撃判定の相対座標をつかい
	// 判定の座標を割り出す
	trans.pos = gobKingTrans.pos + gobKingTrans.VTrans(COLL_LOCAL_POS);
}

void GobSwingCollOperator::Off(void)
{
	attackColl->SetJudgeFlg(false);
}