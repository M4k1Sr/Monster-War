#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class Paraseil : public ActorBase
{
public:

	Paraseil(const Transform& playerTrans);
	~Paraseil()override = default;

	// 読み込み処理
	void Load(void)override;

private:

	// 翼ローカル座標・角度
	Vector3 paraseilLocalPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 paraseilLocalRot = Vector3(0.0f, 0.0f, 0.0f);

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 更新
	void SubUpdate(void)override;

	void SubDraw(void)override;

};

