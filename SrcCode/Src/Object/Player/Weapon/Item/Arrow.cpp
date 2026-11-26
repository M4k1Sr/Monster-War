#include "Arrow.h"

#include "../../../Common/Collider/LineCollider.h"

#include "../../../../Utility/Utility.h"

Arrow::Arrow(
	float MAX_SPEED,
	float MIN_SPEED,
	int LIFE_FRAME,
	float ARROW_GRAVITY,
	int DEAD_FRAME
) :
	MAX_SPEED(MAX_SPEED),
	MIN_SPEED(MIN_SPEED),
	LIFE_FRAME(LIFE_FRAME),
	ARROW_GRAVITY(ARROW_GRAVITY),
	DEAD_FRAME(DEAD_FRAME),

	moveVec(),
	power(0.0f),
	lifeCounter(0),
	deadCounter(0),
	isActive(false),
	isHit(false)
{
}

void Arrow::Load(void)
{

#pragma region オブジェクト設定

	// 動的オブジェクトとしての処理を有効にする
	SetColliderMobility(COLLIDER_MOBILITY::Movable);

	// 押し出しは無効
	SetContactResponseEnabled(false);

#pragma endregion


#pragma region モデル設定

	// モデルの読み込み
	trans.LoadModel("Weapon/Arrow");

	// モデルのスケール設定
	trans.scale = 2.0f;

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

void Arrow::Fire(const Vector3& pos, const Vector3& dir, float power,float dist)
{
	this->power = power;

	// 狙った点までの距離（ここまでは重力をかけない）
	straightRemain = STRAIGHT_DIST;

	// 引き絞り値で速度を決める
	const float speed = MIN_SPEED + (MAX_SPEED - MIN_SPEED) * power;
	moveVec = dir * speed;

	trans.pos = pos;
	LookToMoveVec();

	lifeCounter = LIFE_FRAME;
	isActive = true;

	isHit = false;
	deadCounter = 0;

	SetIsDraw(true);
	SetJudgeFlg(true);
}

void Arrow::SubUpdate(void)
{
	if (!isActive) { return; }

	const float scale = GetTimeScale();

	// 刺さっている間は動かさず、時間が来たら消す
	if (isHit) {
		if (scale > 0.0f && ++deadCounter >= DEAD_FRAME) { Deactivate(); }
		return;
	}

	// レティクルで狙った点に着くまでは重力をかけない
	if (straightRemain <= 0.0f) { moveVec.y += ARROW_GRAVITY * scale; }

	// 移動
	const Vector3 move = moveVec * scale;
	trans.pos += move;

	// 進んだ分だけ残りの距離を減らす
	straightRemain -= move.Length();

	// 放物線に合わせて矢先を下げる
	LookToMoveVec();

	// 時間切れ
	if (scale > 0.0f && --lifeCounter <= 0) { Deactivate(); }

	if (isHit) {
		deadCounter++;
	}
}

void Arrow::OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)
{
	if (!isActive || isHit) { return; }

	// 敵かステージに当たったら消す
	// （ダメージは敵側の OnCollision で PlayerArrow タグを見て処理する）
	// TODO：自分のタグに合わせて調整
	if (other.GetTag() == COLLIDER_TAG::Enemy ||
		other.GetTag() == COLLIDER_TAG::Stage) {

		isHit = true;
		deadCounter = 0;

		// 当たり判定は切る（同じ敵に何度もダメージが入らないように）
		SetJudgeFlg(false);

		// ヒットエフェクト・SEはここで
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
	isHit = false;
	deadCounter = 0;
	moveVec = Vector3();

	SetIsDraw(false);
	SetJudgeFlg(false);
}
