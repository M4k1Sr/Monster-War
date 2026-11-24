#include "Arrow.h"

#include "../../../Common/Collider/LineCollider.h"

#include "../../../../Utility/Utility.h"

Arrow::Arrow(
	float MAX_SPEED,
	float MIN_SPEED,
	int LIFE_FRAME,
	float ARROW_GRAVITY
) :
	MAX_SPEED(MAX_SPEED),
	MIN_SPEED(MIN_SPEED),
	LIFE_FRAME(LIFE_FRAME),
	ARROW_GRAVITY(ARROW_GRAVITY),

	moveVec(),
	power(0.0f),
	lifeCounter(0),
	isActive(false)
{
}

void Arrow::Load(void)
{
#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetColliderMobility(COLLIDER_MOBILITY::Movable);

	// 押し出しは無効（刺さる・すり抜けるだけ）
	SetContactResponseEnabled(false);

	// ※ActorBase側の重力は使わず、SubUpdateで自前の重力をかける
	//   （今のActorBaseで重力を切る関数を呼んでおく）

	// 世界と一緒にスローさせたい場合はコメントを外す
	// （AddChildActorでPlayerグループが引き継がれるので、何もしなければスロー中も等速で飛ぶ）
	// SetTimeGroup(TimeScale::GROUP::Other);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Weapon/Arrow");

	// モデルのスケール設定
	trans.scale = 3.0f;

	// モデルの中心点のズレの補正
	trans.centerDiff = Vector3(0.0f, 0.0f, 0.0f) * trans.scale;

	// モデルの角度のズレの補正
	trans.SetLocalRotation(Quaternion::FromRotationX(Deg2Rad(-90.0f)));

#pragma endregion


#pragma region コライダーの生成

	// すり抜け対策：矢先から「最高速度ぶん後ろ」まで伸びる線分
	// → 1フレームで進む距離を必ずカバーするので、速くても敵を貫通しない
	AddCollider(
		new LineCollider(
			COLLIDER_TAG::PlayerBow,
			Vector3(0.0f, 0.0f, ARROW_TIP),
			Vector3(0.0f, 0.0f, -MAX_SPEED)
		)
	);

#pragma endregion

	// 発射されるまでは待機（非表示・判定なし）
	Deactivate();
}

void Arrow::Fire(const Vector3& pos, const Vector3& dir, float power)
{
	this->power = power;

	// 引き絞り値で速度を決める
	const float speed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * power;
	moveVec = dir * speed;

	trans.pos = pos;
	LookToMoveVec();

	lifeCounter = LIFE_FRAME;
	isActive = true;

	SetIsDraw(true);
	SetJudgeFlg(true);
}

void Arrow::SubUpdate(void)
{
	if (!isActive) { return; }

	const float scale = GetTimeScale();

	// 自前の重力（弱い矢ほど遅いので、相対的に早く落ちて見える）
	moveVec.y += ARROW_GRAVITY * scale;

	// 移動
	trans.pos += moveVec * scale;

	// 放物線に合わせて矢先を下げる
	LookToMoveVec();

	// 時間切れ
	if (scale > 0.0f && --lifeCounter <= 0) { Deactivate(); }
}

void Arrow::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (!isActive) { return; }

	// 敵かステージに当たったら消す
	// （ダメージは敵側の OnCollision で PlayerArrow タグを見て処理する）
	// TODO：自分のタグに合わせて調整
	if (other.GetTag() == COLLIDER_TAG::Enemy ||
		other.GetTag() == COLLIDER_TAG::Stage) {

		// ヒットエフェクト・SEはここで
		Deactivate();
	}
}

void Arrow::LookToMoveVec(void)
{
	if (moveVec.LengthSq() <= 0.0001f) { return; }

	Vector3 dir = moveVec;
	dir.Normalize();

	// +Z を dir に向ける回転
	const MATRIX rotMat = MGetRotVec2(VGet(0.0f, 0.0f, 1.0f), dir.ToVECTOR());
	trans.rotation = Quaternion::FromMatrix(rotMat);
}

void Arrow::Deactivate(void)
{
	isActive = false;
	moveVec = Vector3();

	SetIsDraw(false);
	SetJudgeFlg(false);
}
