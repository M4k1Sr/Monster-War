#pragma once

#include "../../../Common/ActorBase/ActorBase.h"

class Arrow
	: public ActorBase
{
public:
	Arrow(const Transform& playerTrans);
	~Arrow()override = default;

	// 読み込み処理
	void Load(void)override;

private:

	// 矢ローカル座標・角度
	Vector3 arrowLocalPos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 arrowLocalRot = Vector3(0.0f, 0.0f, 0.0f);

	// プレイヤーのモデル制御情報の参照
	const Transform& playerTrans;

	// 更新
	void SubUpdate(void)override;

	void SubDraw(void)override;

};

