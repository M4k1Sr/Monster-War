#pragma once

#include "../Common/ActorBase/ActorBase.h"

// プロペラの構造体
struct WindMillParam {
	Vector3 pos;
	Vector3 angleDeg;
	float spinSpeed;
	float windHeight;
	float windRadius;
};

class WindMill
	: public ActorBase
{
public:
	WindMill(const WindMillParam& param);
	~WindMill()override = default;

	// 読み込み処理
	void Load(void)override;

private:

	WindMillParam param;

	// もととなるモデル
	int originModel;

	// モデルが必要な台数
	int modelCount;

	// 羽の回転角
	float spinAngle;
	Quaternion baseQuaternion;


	// 更新
	void SubUpdate(void)override;

	// 解放
	void SubRelease(void)override;
};
