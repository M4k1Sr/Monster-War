#pragma once

#include <map>

#include "../../../Common/ActorBase/ActorBase.h"

class King_Sword
	: public ActorBase
{
public:
	King_Sword(const Vector3& COLL_LOCAL_POS,
		const Transform& playerTrans);
	~King_Sword()override = default;

	// 読み込み処理
	void Load(void)override;

private:

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 攻撃の判定を発生させる座標（プレイヤー座標からの相対座標）
	const Vector3 COLL_LOCAL_POS;

	// 更新
	void SubUpdate(void)override;
};

