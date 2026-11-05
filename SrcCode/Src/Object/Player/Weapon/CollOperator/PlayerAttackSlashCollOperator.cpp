#include "PlayerAttackSlashCollOperator.h"

#include "../../../Common/Collider/SphereCollider.h"

PlayerAttackSlashCollOperator::PlayerAttackSlashCollOperator(
	float COLL_RADIUS,
	const Vector3& COLL_LOCAL_POS,

	bool& isRush,

	const Transform& playerTrans
) :
	COLL_LOCAL_POS(COLL_LOCAL_POS),

	playerTrans(playerTrans),

	isRush(isRush),

	attackColl(new SphereCollider(COLLIDER_TAG::PlayerAttackSlash, COLL_RADIUS)),
	rushColl(new SphereCollider(COLLIDER_TAG::PlayerRushSlash,COLL_RADIUS)),

	isHit(false)
{
}

void PlayerAttackSlashCollOperator::Load(void)
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
	rushColl->SetJudgeFlg(false);	

	isHit = false;

	// コライダーを追加
	AddCollider(attackColl);
	AddCollider(rushColl);

}

void PlayerAttackSlashCollOperator::SubUpdate(void)
{
	if (isHit) {
		if (isRush) {
			rushColl->SetJudgeFlg(true);
		}
		else {
			attackColl->SetJudgeFlg(true);
		}
		isHit = false;
	}
	else {
		rushColl->SetJudgeFlg(false);
		attackColl->SetJudgeFlg(false);
	}

	// 判定が発生しているときだけ処理する
	if (!attackColl->GetJudgeFlg()) { return; }
	if (!rushColl->GetJudgeFlg()) { return; }

	// 追従し続ける
	trans.pos = playerTrans.pos + playerTrans.VTrans(COLL_LOCAL_POS);
}

void PlayerAttackSlashCollOperator::On(void)
{
	// 当たり判定を記録
	isHit = true;

	// プレイヤーの座標、プレイヤーの角度、攻撃判定の相対座標をつかい
	// 判定の座標を割り出す
	trans.pos = playerTrans.pos + playerTrans.VTrans(COLL_LOCAL_POS);
}

void PlayerAttackSlashCollOperator::Off(void)
{
	isHit = false;
	attackColl->SetJudgeFlg(false);
	rushColl->SetJudgeFlg(false);
}