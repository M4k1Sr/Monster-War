#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class Wings : public ActorBase
{
public:

	Wings(const Transform& playerTrans);
	~Wings()override = default;

	// 読み込み処理
	void Load(void)override;

private:

	// 翼ローカル座標・角度
	Vector3 wingsLocalPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 wingsLocalRot = Vector3(0.0f, 0.0f, 0.0f);

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 更新
	void SubUpdate(void)override;

	void SubDraw(void)override;

};

