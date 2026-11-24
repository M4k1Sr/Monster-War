#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class Arrow
	: public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="MAX_SPEED">引き切ったときの速度</param>
	/// <param name="MIN_SPEED">弱い矢の速度</param>
	/// <param name="LIFE_FRAME">何にも当たらなかったときに消えるまでのフレーム</param>
	/// <param name="ARROW_GRAVITY">1フレームごとの落下加速度（マイナス値）</param>
	Arrow(float MAX_SPEED, float MIN_SPEED, int LIFE_FRAME, float ARROW_GRAVITY);
	~Arrow()override = default;

	// 読み込み処理
	void Load(void)override;

	/// <summary>
	/// 発射（Player の spawnArrow から呼ばれる）
	/// </summary>
	/// <param name="pos">発射位置</param>
	/// <param name="dir">向き（正規化済み）</param>
	/// <param name="power">引き絞り値 0.0〜1.0</param>
	void Fire(const Vector3& pos, const Vector3& dir, float power,float dist);

	// 飛んでいるか（プールで空いてる矢を探す用）
	bool IsActive(void)const { return isActive; }

	// 威力（敵側でダメージ計算に使う場合用）
	float GetPower(void)const { return power; }

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const CollisionResult& result)override;

private:

#pragma region 定数

	// 1発ごとに変わらない値
	const float MAX_SPEED;
	const float MIN_SPEED;
	const int   LIFE_FRAME;
	const float ARROW_GRAVITY;

	// 矢先の位置（モデル原点からの距離）
	const float ARROW_TIP = 40.0f;

#pragma endregion

	// 撃つたびに変わる値
	Vector3 moveVec;     // 速度ベクトル（向き × 速度）
	float   power;       // 引き絞り値
	int     lifeCounter; // 残りフレーム
	bool    isActive;    // 飛んでいるか

	// 矢の向きを moveVec に合わせる
	void LookToMoveVec(void);

	// 当たった・時間切れで待機状態に戻す
	void Deactivate(void);

	// 更新
	void SubUpdate(void)override;
};
