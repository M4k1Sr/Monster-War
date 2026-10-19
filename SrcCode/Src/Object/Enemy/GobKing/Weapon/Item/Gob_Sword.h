#pragma once

#include <map>

#include "../../../../Common/ActorBase/ActorBase.h"

class Gob_Sword
	: public ActorBase
{
public:
	Gob_Sword(const Vector3& COLL_LOCAL_POS,
		const Transform& playerTrans);
	~Gob_Sword()override = default;

	// 読み込み処理
	void Load(void)override;

private:

	// 武器ローカル座標・角度
	Vector3 swordLocalPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 swordLocalRot = Vector3(0.0f, 0.0f, 0.0f);

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 攻撃の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	// 更新
	void SubUpdate(void)override;

	void SubDraw(void)override;

};

