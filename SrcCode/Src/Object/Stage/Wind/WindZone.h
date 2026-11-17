#pragma once
#include "../../../Object/Common/ActorBase/ActorBase.h"

struct WindZoneParam {
	Vector3 startPos;
	Vector3 endPos;
	Vector3 windDir;
	float radius;
	bool isParaseilOnWind;
};

class WindZone
	: public ActorBase
{
public:
	WindZone(const WindZoneParam& param);
	~WindZone()override = default;

	// 読み込み処理
	void Load(void)override;

private:

	WindZoneParam param;

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
